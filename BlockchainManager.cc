/*
 * BlockchainManager.cc
 *
 *  Created on: Aug 25, 2022
 *      Author: veins
 */
#include "BlockchainManager.h"
#include "X2TXMsg.h"
#include "X2BKMsg.h"
#include "X2AckMsg.h"
#include "x2/packet/X2ControlInfo_m.h"
#include "inet/common/ProtocolTag_m.h"

using namespace omnetpp;
using namespace inet;

Define_Module(BlockchainManager);

void BlockchainManager::initialize()
{
    // get the node id
    nodeId_ = getAncestorPar("macCellId");

    // get reference to the gates
    x2Manager_[IN_GATE] = gate("x2ManagerIn");
    x2Manager_[OUT_GATE] = gate("x2ManagerOut");

    binder_=getBinder();
    phy=check_and_cast<LtePhyBase*>(getParentModule()->getSubmodule("phy"));

    checkTxPool = new cMessage("checkTransactionPool");
    //checkTxPool->setSchedulingPriority(1);
    scheduleAt(NOW+miningStart, checkTxPool);

    list<Transaction> init;
    SIMULTE_API::Block genesis("0", VectorXd::Zero(64), init);
    blockchain.push_back(genesis);
    //currentNonce = 0;
    currentBlock = nullptr;

    TXDelay = registerSignal("txDelay");
    BKDelay = registerSignal("bkDelay");
    CDelay = registerSignal("cDelay");

    // register to the X2 Manager
    /*auto x2Packet = new Packet("X2TXMsg");
    auto initMsg = makeShared<X2TXMsg>();
    auto ctrlInfo = x2Packet->addTagIfAbsent<X2ControlInfoTag>();
    ctrlInfo->setInit(true);
    x2Packet->insertAtFront(initMsg);
    send(x2Packet, x2Manager_[OUT_GATE]);*/

    auto x2Packet = new Packet("X2AckMsg");
    auto initMsg = makeShared<X2AckMsg>();
    auto ctrlInfo = x2Packet->addTagIfAbsent<X2ControlInfoTag>();
    ctrlInfo->setInit(true);
    x2Packet->insertAtFront(initMsg);
    send(x2Packet, x2Manager_[OUT_GATE]);

    auto BKx2Packet = new Packet("X2BKMsg");
    auto BKinitMsg = makeShared<X2BKMsg>();
    auto BKctrlInfo = BKx2Packet->addTagIfAbsent<X2ControlInfoTag>();
    BKctrlInfo->setInit(true);
    BKx2Packet->insertAtFront(BKinitMsg);
    send(BKx2Packet, x2Manager_[OUT_GATE]);
}

void BlockchainManager::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        checkPoolForMining();
        scheduleAt(NOW+TTI, checkTxPool);
        return;
    }
    else
    {
        cPacket* pkt = check_and_cast<cPacket*>(msg);
        cGate* incoming = pkt->getArrivalGate();
        if (incoming == x2Manager_[IN_GATE])
        {
            // incoming data from X2 Manager
            EV << "BlockchainManager::handleMessage - Received message from X2 manager" << endl;
            process(pkt);
        }
        else
            delete msg;
    }

}

void BlockchainManager::checkPoolForMining()
{
    if (currentBlock == nullptr)
    {
        if (transPool.size() >= maxTransactionNum)
        {
            list<Transaction> temp;
            set<Transaction, Transaction::Compare>::iterator itr = transPool.begin();
            for (int i = 0; i < maxTransactionNum; i++)
            {
                temp.push_back(*itr);
                itr++;
            }
            //SIMULTE_API::Block bk(blockchain.back().getHash(), phy->getPubKey(), temp);
            currentBlock = new SIMULTE_API::Block(blockchain.back().getHash(), phy->getPubKey(), temp);
        }
        else
            return;

    }
    for (int i = 0; i < batchNum; i++)
    {
        if (currentBlock->getHash().substr(0, target) == string(target, '0'))
            break;
        currentBlock->setNonce(currentBlock->getNonce() + 1);
        currentBlock->calHash();
    }
    if (currentBlock->getHash().substr(0, target)==string(target, '0'))
    {
        //blockchain.push_back(bk);
        //EV  << "************************ " << binder_->getMutex() << endl;
        if (binder_->getMutex() > 0)//cnt > 0)//mtx.try_lock())
        {
            //cnt -= 1;
            binder_->acquireMutex();
            sendBlock(*currentBlock);
            //currentNonce = 0;
            list<Transaction> temp = currentBlock->getList();
            for (list<Transaction>::iterator it = temp.begin(); it != temp.end(); it++)
                transPool.erase(*it);
        }
        //emit(BKDelay, NOW-bk.getTS());
    }
}

void BlockchainManager::sendBlock(SIMULTE_API::Block b)
{
    vector<EnbInfo*>* enbList = binder_->getEnbList();
    DestinationIdList destList;
    for (vector<EnbInfo*>::iterator itr = enbList->begin(); itr!=enbList->end(); itr++)
    {
        if ((*itr)->id != nodeId_)
            destList.push_back((*itr)->id);
    }

    auto pkt = new Packet("X2BKMsg");
    auto ctrlInfo = pkt->addTagIfAbsent<X2ControlInfoTag>();
    ctrlInfo->setSourceId(nodeId_);
    ctrlInfo->setDestIdList(destList);

    auto BK = makeShared<X2BKMsg>();
    BK->setPrevHash(b.getPrevHash());
    BK->setCurHash(b.getHash());
    BK->setMerkleRoot(b.getMerkleRoot());
    BK->setTS(b.getTS());
    BK->setNonce(b.getNonce());
    BK->setPubKey(b.getPubKey());
    BK->setList(b.getList());

    pkt->insertAtFront(BK);
    pkt->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&LteProtocol::x2ap);
    send(pkt, x2Manager_[OUT_GATE]);
}

void BlockchainManager::generateTransaction(Transaction tx)
{
    Enter_Method_Silent();
    transPool.insert(tx);
    //checkPoolForMining();
    //sendTransaction(tx);
}

/*void BlockchainManager::sendTransaction(Transaction tx)
{
    vector<EnbInfo*>* enbList = binder_->getEnbList();
    DestinationIdList destList;
    for (vector<EnbInfo*>::iterator itr = enbList->begin(); itr!=enbList->end(); itr++)
    {
        if ((*itr)->id != nodeId_)
            destList.push_back((*itr)->id);
    }
    auto pkt = new Packet("X2TXMsg");
    auto ctrlInfo = pkt->addTagIfAbsent<X2ControlInfoTag>();
    ctrlInfo->setSourceId(nodeId_);
    ctrlInfo->setDestIdList(destList);

    auto TX = makeShared<X2TXMsg>();
    TX->setHash(tx.getHash());
    TX->setTS(tx.getTS());
    TX->setSourceId(tx.getSource());
    TX->setDestinationId(tx.getDestination());
    TX->setTXType(tx.getType());
    TX->setPubKey(tx.getKey());
    pkt->insertAtFront(TX);
    pkt->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&LteProtocol::x2ap);
    send(pkt, x2Manager_[OUT_GATE]);
}*/

void BlockchainManager::process(cPacket* pkt)
{
    Packet* datagram = check_and_cast<Packet*>(pkt);
    auto x2msg = datagram->peekAtFront<LteX2Message>();
    datagram->removeTagIfPresent<X2ControlInfoTag>();

    /*if (x2msg->getType() == X2_TX_MSG) {
        auto TX = datagram->popAtFront<X2TXMsg>()->dup();
        string hash = TX->getHash();
        simtime_t ts = TX->getTS();
        MacNodeId src = TX->getSourceId();
        MacNodeId dst = TX->getDestinationId();
        TransactionType type = TX->getTXType();
        string key = TX->getPubKey();
        Transaction tx(hash, ts, src, dst, type, key);
        transPool.insert(tx);
        emit(TXDelay, NOW-ts);
    }
    else */
    if (x2msg->getType() == X2_BK_MSG) {
        auto BK = datagram->popAtFront<X2BKMsg>()->dup();
        string prevHash = BK->getPrevHash();
        string curHash = BK->getCurHash();
        string merkleRoot = BK->getMerkleRoot();
        simtime_t timestamp = BK->getTS();
        unsigned int nc = BK->getNonce();
        string minerKey = BK->getPubKey();
        list<Transaction> lst = BK->getList();
        SIMULTE_API::Block bk(prevHash, curHash, merkleRoot, timestamp, nc, minerKey, lst);
        if (verifyBlock(bk))
        {
            blockchain.push_back(bk);
            sendAck(BK->getSourceId());
            for (list<Transaction>::iterator it = lst.begin();it != lst.end(); it++) {
                if (timestamp > it->getTS())
                    timestamp = it->getTS();
            }
            //emit(BKDelay, NOW-timestamp);
            emit(CDelay, NOW-timestamp);
        }
    }
    else if (x2msg->getType() == X2_ACK) { // There are only two base stations in the simulation. Thus, we can omit checking an array of Acks.
        blockchain.push_back(*currentBlock);
        currentBlock = nullptr;
        //mtx.unlock();
        //cnt += 1;
        binder_->releaseMutex();
    }
    //delete x2msg;
    delete pkt;
}

void BlockchainManager::sendAck(unsigned int source)
{
    auto pkt = new Packet("X2AckMsg");
    auto ctrlInfo = pkt->addTagIfAbsent<X2ControlInfoTag>();
    ctrlInfo->setSourceId(nodeId_);
    DestinationIdList destList;
    MacNodeId src = (unsigned short)source;
    destList.push_back(src);
    ctrlInfo->setDestIdList(destList);

    auto Ack = makeShared<X2AckMsg>();
    Ack->setDestinationId(source);
    Ack->setSourceId(nodeId_);
    pkt->insertAtFront(Ack);
    pkt->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&LteProtocol::x2ap);
    send(pkt, x2Manager_[OUT_GATE]);
}

bool BlockchainManager::verifyBlock(SIMULTE_API::Block b)
{
    if (b.getHash().substr(0, target)==string(target, '0'))
    {
        /*list<Transaction> cur = b.getList();
        for (list<Transaction>::iterator it = cur.begin(); it != cur.end(); it++)
        {
            if (transPool.find(*it) != transPool.end())
            {
                currentNonce = 0;
                transPool.erase(*it);
            }
        }
        */
        return true;
    }
    return false;
}
