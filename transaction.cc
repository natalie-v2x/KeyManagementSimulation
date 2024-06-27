/*
 * transaction.cc
 *
 *  Created on: Aug 27, 2022
 *      Author: veins
 */

#include "transaction.h"

Transaction::Transaction(TransactionType t, MacNodeId s, MacNodeId d, VectorXd pk) {
    timestamp = simTime();
    type = t;
    source = s;
    destination = d;
    pubKey = PK2String(pk);
    calHash();
}

Transaction::Transaction(string h, simtime_t ts, MacNodeId s, MacNodeId d, TransactionType t, string key) {
    hash = h;
    timestamp = ts;
    source = s;
    destination = d;
    type = t;
    pubKey = key;
}

Transaction::~Transaction() {

}

void Transaction::calHash() {
    string ss = timestamp.ustr().append(to_string(source)).append(to_string(destination)).append(to_string(type)).append(pubKey);
    hash = hashing(ss);
}

