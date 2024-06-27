/*
 * KeyGen.h
 *
 *  Created on: Aug 31, 2023
 *      Author: veins
 */

#ifndef STACK_PHY_PACKET_KEYGEN_H_
#define STACK_PHY_PACKET_KEYGEN_H_

#include "stack/phy/packet/LteFeedbackPkt_m.h"
#include "stack/bcManager/transaction.h"

class SIMULTE_API KeyGen : public LteFeedbackPkt_Base
{
protected:
    string hash;
    simtime_t timestamp;
    MacNodeId source;
    MacNodeId destination;
    TransactionType type;
    string pubKey;
private:
    void copy(const KeyGen& other) {
        hash = other.hash;
        timestamp = other.timestamp;
        source = other.source;
        destination = other.destination;
        type = other.type;
        pubKey = other.pubKey;
    }
public:
    KeyGen():
        LteFeedbackPkt_Base()
    {

    }
    KeyGen(const KeyGen& other):
        LteFeedbackPkt_Base(other)
    {
        copy(other);
    }
    KeyGen& operator=(const KeyGen& other)
    {
        if (this == &other) return *this;
        LteFeedbackPkt_Base::operator =(other);
        copy(other);
        return *this;
    }
    virtual KeyGen *dup() const
    {
        return new KeyGen(*this);
    }

    void setHash(string h) {hash = h;}
    string getHash()const {return hash;}
    void setTimestamp(simtime_t ts){timestamp = ts;}
    simtime_t getTimestamp()const {return timestamp;}
    void setSource(MacNodeId s){source =s;}
    MacNodeId getSource()const {return source;}
    void setDestination(MacNodeId d) {destination = d;}
    MacNodeId getDestination()const{return destination;}
    void setType(TransactionType t) {type=t;}
    TransactionType getType() const{return type;}
    void setKey(string key){pubKey = key;}
    string getKey() const {return pubKey;}

};

#endif /* STACK_PHY_PACKET_KEYGEN_H_ */
