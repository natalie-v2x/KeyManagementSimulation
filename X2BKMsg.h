/*
 * X2BKMsg.h
 *
 *  Created on: Aug 30, 2023
 *      Author: veins
 */

#ifndef STACK_BCMANAGER_X2BKMSG_H_
#define STACK_BCMANAGER_X2BKMSG_H_


#include "x2/packet/LteX2Message.h"
#include "transaction.h"

class SIMULTE_API X2BKMsg : public LteX2Message
{
protected:
    string prevHash;
    string curHash;
    string merkleRoot;
    simtime_t timestamp;
    unsigned int nonce;
    string pubKey;
    list<Transaction> transactionList_;

public:
    X2BKMsg():
        LteX2Message()
    {
        type_ = X2_BK_MSG;
    }
    X2BKMsg(const X2BKMsg& other): LteX2Message() {operator=(other);}
    X2BKMsg& operator=(const X2BKMsg& other)
    {
        if (&other == this)
            return *this;
        LteX2Message::operator =(other);
        prevHash = other.prevHash;
        curHash = other.curHash;
        merkleRoot = other.merkleRoot;
        timestamp = other.timestamp;
        nonce = other.nonce;
        pubKey = other.pubKey;
        setList(other.transactionList_);
        return *this;
    }
    virtual X2BKMsg* dup() const {return new X2BKMsg(*this);}
    virtual ~X2BKMsg() {}

    void setPrevHash(string h) {prevHash = h;}
    string getPrevHash() const {return prevHash;}
    void setCurHash(string h) {curHash = h;}
    string getCurHash() const {return curHash;}
    void setMerkleRoot(string r) {merkleRoot = r;}
    string getMerkleRoot() const {return merkleRoot;}
    void setTS(simtime_t ts) {timestamp=ts;}
    simtime_t getTS() const {return timestamp;}
    void setNonce (unsigned int n) {nonce = n;}
    unsigned int getNonce() const {return nonce;}
    void setPubKey(string key) {pubKey = key;}
    string getPubKey() const {return pubKey;}
    void setList(list<Transaction>list) {
        for (auto it = list.begin(); it != list.end(); it++)
            transactionList_.push_back(*(it->dup()));
    }
    list<Transaction> getList() {return transactionList_;}
};


#endif /* STACK_BCMANAGER_X2BKMSG_H_ */
