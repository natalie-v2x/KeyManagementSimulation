/*
 * X2TXMsg.h
 *
 *  Created on: Aug 30, 2023
 *      Author: veins
 */

#ifndef STACK_BCMANAGER_X2TXMSG_H_
#define STACK_BCMANAGER_X2TXMSG_H_

#include "x2/packet/LteX2Message.h"
#include "transaction.h"

class SIMULTE_API X2TXMsg : public LteX2Message
{
protected:
    string hash;
    simtime_t timestamp;
    TransactionType type;
    string pubKey;

public:
    X2TXMsg():
        LteX2Message()
    {
        type_ = X2_TX_MSG;
    }
    X2TXMsg(const X2TXMsg& other):LteX2Message() {operator=(other);}
    X2TXMsg& operator=(const X2TXMsg& other)
    {
        if (&other == this)
            return *this;
        LteX2Message::operator =(other);
        hash = other.hash;
        timestamp = other.timestamp;
        type = other.type;
        pubKey = other.pubKey;
        return *this;
    }
    virtual X2TXMsg* dup() const {return new X2TXMsg(*this);}
    virtual ~X2TXMsg() {}

    void setHash(string h) {hash = h;}
    string getHash() const{return hash;}
    void setTS(simtime_t ts){timestamp=ts;}
    simtime_t getTS()const{return timestamp;}
    void setTXType(TransactionType t) {type =t;}
    TransactionType getTXType(){return type;}
    void setPubKey(string k) {pubKey = k;}
    string getPubKey() {return pubKey;}
};



#endif /* STACK_BCMANAGER_X2TXMSG_H_ */
