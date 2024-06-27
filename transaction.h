/*
 * transaction.h
 *
 *  Created on: Aug 27, 2022
 *      Author: veins
 */

#ifndef STACK_BCMANAGER_TRANSACTION_H_
#define STACK_BCMANAGER_TRANSACTION_H_

#include <omnetpp.h>
#include "common/LteCommon.h"
#include "utils.h"

using namespace omnetpp;
enum TransactionType {
    INIT = 0,
    UPDATE = 1,
    REVOKE = 2,
    UNKNOWN = 3
};
class SIMULTE_API Transaction
{

protected:
    string hash;
    simtime_t timestamp;
    TransactionType type;
    MacNodeId source;
    MacNodeId destination;
    string pubKey;

    //https://blog.csdn.net/GuoXuan_CHN/article/details/80532258

public:
    Transaction();
    Transaction(TransactionType t, MacNodeId s, MacNodeId d, VectorXd pk);
    Transaction(string hash, simtime_t ts, MacNodeId s, MacNodeId d, TransactionType t, string key);
    ~Transaction();

    struct Compare {
        bool operator() (const Transaction t1, const Transaction t2) const {
            return t1.timestamp < t2.timestamp;
        }
    };
    bool operator==(const Transaction t) const {
        return ((timestamp == t.timestamp) && (pubKey == t.pubKey));
    }
    Transaction& operator=(const Transaction& other) {
        if (&other == this)
            return *this;
        hash = other.hash;
        timestamp = other.timestamp;
        source = other.source;
        destination = other.destination;
        type = other.type;
        pubKey = other.pubKey;
        return *this;
    }
    Transaction(const Transaction& other) {operator=(other);}
    virtual Transaction *dup() const {return new Transaction(*this);}
    void calHash();
    string getHash() const {return hash;}
    simtime_t getTS() const {return timestamp;}
    MacNodeId getSource() const{return source;}
    MacNodeId getDestination() const{return destination;}
    TransactionType getType() const{return type;}
    string getKey() const{return pubKey;}

};

#endif /* STACK_BCMANAGER_TRANSACTION_H_ */
