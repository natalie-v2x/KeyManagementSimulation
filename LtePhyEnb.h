//
//                           SimuLTE
//
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself,
// and cannot be removed from it.
//

#ifndef _LTE_AIRPHYENB_H_
#define _LTE_AIRPHYENB_H_

#include "stack/phy/layer/LtePhyBase.h"
#include <veins/modules/mobility/traci/TraCIScenarioManager.h>

class DasFilter;
class LteFeedbackPkt;

class SIMULTE_API LtePhyEnb : public LtePhyBase
{
    friend class DasFilter;

  protected:
    /** Broadcast messages interval (equal to updatePos interval for mobility) */
    double bdcUpdateInterval_;

    /** Self message to trigger broadcast message sending for handover purposes */
    omnetpp::cMessage *bdcStarter_;
    omnetpp::cMessage *revoker_;

    /**
     * Pointer to the DAS Filter: used to call das function
     * when receiving broadcasts and to retrieve physical
     * antenna properties on packet reception
     */
    DasFilter* das_;
    //Used for PisaPhy feedback generator
    LteFeedbackDoubleVector fb_;

    veins::TraCIScenarioManager* manager;

    virtual void initialize(int stage);

    virtual void handleSelfMessage(omnetpp::cMessage *msg);
    virtual void handleAirFrame(omnetpp::cMessage* msg);
    bool handleControlPkt(UserControlInfo* lteinfo, LteAirFrame* frame);
    void handleFeedbackPkt(UserControlInfo* lteinfo, LteAirFrame* frame);
    virtual void requestFeedback(UserControlInfo* lteinfo, LteAirFrame* frame, inet::Packet* pkt);
    void handleKeyInit(UserControlInfo* lteinfo, LteAirFrame* frame);
    /**
     * Getter for the Das Filter
     */
    DasFilter* getDasFilter();
    // Feedback computation for PisaPhy
    LteFeedbackComputation* getFeedbackComputationFromName(std::string name, ParameterMap& params);
    void initializeFeedbackComputation();

  public:
    const static short int REVOKE_CLOCK=5; // the time step for revocation
    //short int CNT_R; // the counter of revocation, 30%
    LtePhyEnb();
    virtual ~LtePhyEnb();

};

#endif  /* _LTE_AIRPHYENB_H_ */
