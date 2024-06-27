/*
 * BlockchainManager.h
 *
 *  Created on: Aug 25, 2022
 *      Author: veins
 */

#ifndef STACK_BCMANAGER_BLOCKCHAINMANAGER_H_
#define STACK_BCMANAGER_BLOCKCHAINMANAGER_H_

#include "block.h"
#include "corenetwork/binder/LteBinder.h"
#include "stack/phy/layer/LtePhyBase.h"

#include<fstream>

class SIMULTE_API BlockchainManager : public omnetpp::cSimpleModule
{

  protected:

    // X2 identifier
    X2NodeId nodeId_;

    // reference to the gates
    omnetpp::cGate* x2Manager_[2];

    set<Transaction, Transaction::Compare> transPool;
    list<SIMULTE_API::Block> blockchain;
    //unsigned int currentNonce;
    SIMULTE_API::Block* currentBlock;

    cMessage* checkTxPool;

    LteBinder* binder_;
    LtePhyBase* phy;
    simsignal_t TXDelay, BKDelay, CDelay;

  public:
    BlockchainManager() {checkTxPool=nullptr;}
    virtual ~BlockchainManager() {

        //cancelAndDelete(checkTxPool);

        //debug
        /*std::fstream f("BC.txt", ios::out | ios::app);
        for (list<SIMULTE_API::Block>::iterator itrtr = blockchain.begin(); itrtr != blockchain.end(); itrtr++)
        {
            f << itrtr->getTS().ustr()<<endl;*/
            /*std::string prevHash = itrtr->getPrevHash();
            std::string curHash = itrtr->getHash();*/
            //list<Transaction> temp = itrtr->getList();
            //f << prevHash << "\t" << curHash<< endl<<"\t";
            /*for (list<Transaction>::iterator k = temp.begin(); k != temp.end(); k++)
            {
                f << k->getType()<<"\t";
            }
            f<<endl;*/
            //EV << "############################ " << prevHash << "\t" << curHash << endl;
            //f << prevHash << "\t" << curHash << endl;
        /*}
        f << endl << endl;
        f.close();*/

        transPool.clear();
        blockchain.clear();
    }

    const static short maxTransactionNum=5;
    const static short batchNum=1000;
    const static short target = 1;
    const static short miningStart = 1;
    virtual void initialize() override;
    virtual void handleMessage(omnetpp::cMessage *msg) override;

    void process(omnetpp::cPacket* pkt);
    void checkPoolForMining();
    void sendBlock(SIMULTE_API::Block b);
    void generateTransaction(Transaction tx);
    void sendTransaction(Transaction tx);
    void sendAck(unsigned int source);
    bool verifyBlock(SIMULTE_API::Block b);
};

#endif /* STACK_BCMANAGER_BLOCKCHAINMANAGER_H_ */
