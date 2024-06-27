//
//                           SimuLTE
//
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself,
// and cannot be removed from it.
//

#include "stack/phy/layer/LtePhyBase.h"
#include "common/LteCommon.h"
#include <fstream>

using namespace omnetpp;

short LtePhyBase::airFramePriority_ = 10;

LtePhyBase::LtePhyBase()
{
    channelModel_ = nullptr;
    keyGeneration = nullptr;
}

LtePhyBase::~LtePhyBase()
{
    //cancelAndDelete(keyGeneration);
}

void LtePhyBase::initialize(int stage)
{
    ChannelAccess::initialize(stage);

    if (stage == inet::INITSTAGE_LOCAL)
    {
        binder_ = getBinder();
        cellInfo_ = nullptr;
        // get gate ids
        upperGateIn_ = findGate("upperGateIn");
        upperGateOut_ = findGate("upperGateOut");
        radioInGate_ = findGate("radioIn");

        // Initialize and watch statistics
        numAirFrameReceived_ = numAirFrameNotReceived_ = 0;
        ueTxPower_ = par("ueTxPower");
        eNodeBtxPower_ = par("eNodeBTxPower");
        microTxPower_ = par("microTxPower");
        relayTxPower_ = par("relayTxPower");

        carrierFrequency_ = 2.1e+9;
        WATCH(numAirFrameReceived_);
        WATCH(numAirFrameNotReceived_);

        multicastD2DRange_ = par("multicastD2DRange");
        enableMulticastD2DRangeCheck_ = par("enableMulticastD2DRangeCheck");

        keyGeneration = new cMessage("keyGeneration");
        generateLongTermKeys();
    }
    else if (stage == inet::INITSTAGE_PHYSICAL_ENVIRONMENT)
    {
        initializeChannelModel();
    }
}

void LtePhyBase::generateLongTermKeys()
{
    MatrixXd Aprime = MatrixXd::Zero(64, 64);
    std::ifstream fin("A.bin", std::ios::binary);
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
            fin >> Aprime(i, j);
    }
    fin.close();
    VectorXd dprime = VectorXd::Zero(64);
    std::ifstream finv("d.bin",std::ios::binary);
    for (int j = 0; j < 64; j++)
        finv>>dprime(j);
    finv.close();
    VectorXd c = VectorXd::Random(64);

    secreteKey = dprime+c;
    publicKey = c.transpose()*Aprime;
    scheduleAt(NOW, keyGeneration);
}
void LtePhyBase::handleMessage(cMessage* msg)
{
    EV << " LtePhyBase::handleMessage - new message received" << endl;

    if (msg->isSelfMessage())
    {
        handleSelfMessage(msg);
    }
    // AirFrame
    else if (msg->getArrivalGate()->getId() == radioInGate_)
    {
        handleAirFrame(msg);
    }

    // message from stack
    else if (msg->getArrivalGate()->getId() == upperGateIn_)
    {
        handleUpperMessage(msg);
    }
    // unknown message
    else
    {
        EV << "Unknown message received." << endl;
        delete msg;
    }
}

void LtePhyBase::handleControlMsg(LteAirFrame *frame,
    UserControlInfo *userInfo)
{
    auto pkt = check_and_cast<inet::Packet *>(frame->decapsulate());
    delete frame;
    *(pkt->addTagIfAbsent<UserControlInfo>()) = *userInfo;
    delete userInfo;
    send(pkt, upperGateOut_);
    return;
}

LteAirFrame *LtePhyBase::createHandoverMessage()
{
    // broadcast airframe
    LteAirFrame *bdcAirFrame = new LteAirFrame("handoverFrame");
    UserControlInfo *cInfo = new UserControlInfo();
    cInfo->setIsBroadcast(true);
    cInfo->setIsCorruptible(false);
    cInfo->setSourceId(nodeId_);
    cInfo->setFrameType(HANDOVERPKT);
    cInfo->setTxPower(txPower_);
    bdcAirFrame->setControlInfo(cInfo);

    bdcAirFrame->setDuration(0);
    bdcAirFrame->setSchedulingPriority(airFramePriority_);
    // current position
    cInfo->setCoord(getRadioPosition());
    return bdcAirFrame;
}

void LtePhyBase::handleUpperMessage(cMessage* msg)
{
    EV << "LtePhy: message from stack" << endl;

    auto pkt = check_and_cast<inet::Packet *>(msg);
    auto lteInfo = pkt->removeTag<UserControlInfo>();

    LteAirFrame* frame = nullptr;

    if (lteInfo->getFrameType() == HARQPKT
        || lteInfo->getFrameType() == GRANTPKT
        || lteInfo->getFrameType() == RACPKT
        || lteInfo->getFrameType() == D2DMODESWITCHPKT)
    {
        frame = new LteAirFrame("harqFeedback-grant");
    }
    else
    {
        // create LteAirFrame and encapsulate the received packet
        frame = new LteAirFrame("airframe");
    }

    frame->encapsulate(check_and_cast<cPacket*>(msg));

    // initialize frame fields
    if (lteInfo->getFrameType() == D2DMODESWITCHPKT)
        frame->setSchedulingPriority(-1);
    else
        frame->setSchedulingPriority(airFramePriority_);
    frame->setDuration(TTI);

    // set current position
    lteInfo->setCoord(getRadioPosition());

    lteInfo->setTxPower(txPower_);
    frame->setControlInfo(lteInfo);

    EV << "LtePhy: " << nodeTypeToA(nodeType_) << " with id " << nodeId_
       << " sending message to the air channel. Dest=" << lteInfo->getDestId() << endl;
    sendUnicast(frame);
}

void LtePhyBase::initializeChannelModel()
{
    channelModel_ = check_and_cast<LteChannelModel*>(getParentModule()->getSubmodule("channelModel"));
    channelModel_->setBand(binder_->getNumBands());
    channelModel_->setPhy(this);
    return;
}

void LtePhyBase::updateDisplayString()
{
    char buf[80] = "";
    if (numAirFrameReceived_ > 0)
        sprintf(buf + strlen(buf), "af_ok:%d ", numAirFrameReceived_);
    if (numAirFrameNotReceived_ > 0)
        sprintf(buf + strlen(buf), "af_no:%d ", numAirFrameNotReceived_);
    getDisplayString().setTagArg("t", 0, buf);
}

void LtePhyBase::sendBroadcast(LteAirFrame *airFrame)
{
    // delegate the ChannelControl to send the airframe
    sendToChannel(airFrame);
}

LteAmc *LtePhyBase::getAmcModule(MacNodeId id)
{
    LteAmc *amc = nullptr;
    OmnetId omid = binder_->getOmnetId(id);
    if (omid == 0)
        return nullptr;

    amc = check_and_cast<LteMacEnb *>(
        getSimulation()->getModule(omid)->getSubmodule("lteNic")->getSubmodule(
            "mac"))->getAmc();
    return amc;
}

void LtePhyBase::sendMulticast(LteAirFrame *frame)
{
    UserControlInfo *ci = check_and_cast<UserControlInfo *>(frame->getControlInfo());

    // get the group Id
    inet::int32 groupId = ci->getMulticastGroupId();
    if (groupId < 0)
        throw cRuntimeError("LtePhyBase::sendMulticast - Error. Group ID %d is not valid.", groupId);

    // send the frame to nodes belonging to the multicast group only
    std::map<int, OmnetId>::const_iterator nodeIt = binder_->getNodeIdListBegin();
    for (; nodeIt != binder_->getNodeIdListEnd(); ++nodeIt)
    {
        if (nodeIt->first != nodeId_ && binder_->isInMulticastGroup(nodeIt->first, groupId))
        {
            EV << NOW << " LtePhyBase::sendMulticast - node " << nodeIt->first << " is in the multicast group"<< endl;

            // get a pointer to receiving module
            cModule *receiver = getSimulation()->getModule(nodeIt->second);
            LtePhyBase * recvPhy;
            double dist;

            if( enableMulticastD2DRangeCheck_ )
            {
                recvPhy =  check_and_cast<LtePhyBase *>(receiver->getSubmodule("lteNic")->getSubmodule("phy"));
                dist = recvPhy->getRadioPosition().distance(getRadioPosition());

                if( dist > multicastD2DRange_ )
                {
                    EV << NOW << " LtePhyBase::sendMulticast - node too far (" << dist << " > " << multicastD2DRange_ << ". skipping transmission" << endl;
                    continue;
                }
            }

            EV << NOW << " LtePhyBase::sendMulticast - sending frame to node " << nodeIt->first << endl;

            sendDirect(frame->dup(), 0, frame->getDuration(), receiver, getReceiverGateIndex(receiver));
        }
    }

    // delete the original frame
    delete frame;
}

void LtePhyBase::sendUnicast(LteAirFrame *frame)
{
    UserControlInfo *ci = check_and_cast<UserControlInfo *>(
        frame->getControlInfo());
    // dest MacNodeId from control info
    MacNodeId dest = ci->getDestId();
    // destination node (UE, RELAY or ENODEB) omnet id
    try {
        binder_->getOmnetId(dest);
    } catch (std::out_of_range& e) {
        delete frame;
        return;         // make sure that nodes that left the simulation do not send
    }
    OmnetId destOmnetId = binder_->getOmnetId(dest);
    if (destOmnetId == 0){
        // destination node has left the simulation
        delete frame;
        return;
    }
    // get a pointer to receiving module
    cModule *receiver = getSimulation()->getModule(destOmnetId);
    // receiver's gate
    sendDirect(frame, 0, frame->getDuration(), receiver, getReceiverGateIndex(receiver));

    return;
}

int LtePhyBase::getReceiverGateIndex(const omnetpp::cModule *receiver) const
{
    int gate = receiver->findGate("radioIn");
    if (gate < 0) {
        gate = receiver->findGate("lteRadioIn");
        if (gate < 0) {
            throw cRuntimeError("receiver \"%s\" has no suitable radio input gate",
                                receiver->getFullPath().c_str());
        }
    }
    return gate;
}
