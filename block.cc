/*
 * block.cc
 *
 *  Created on: Aug 30, 2023
 *      Author: veins
 */

#include "block.h"
#include <stdio.h>
#include <time.h>

SIMULTE_API::Block::Block(string prev, VectorXd pk, list<Transaction> list) {
    srand(time(0));
    prevHash = prev;
    setList(list);
    calMerkleRoot();
    timestamp = simTime();
    nonce = rand();
    pubKey = PK2String(pk);
    calHash();
}
SIMULTE_API::Block::Block(string prev, string cur, string merkleR, simtime_t ts, unsigned int n, string key, list<Transaction> list) {
    prevHash = prev;
    curHash = cur;
    merkleRoot = merkleR;
    timestamp = ts;
    nonce = n;
    pubKey = key;
    setList(list);
}
SIMULTE_API::Block::~Block() {

}

void SIMULTE_API::Block::calMerkleRoot() {
    if (transactionList_.size() == 0)
    {
        merkleRoot = "";
        return;
    }
    list<string> cur;
    for(list<Transaction>::iterator iter = transactionList_.begin(); iter!=transactionList_.end();iter++) {
        cur.push_back((*iter).getHash());
    }
    while (cur.size() > 1) {
        list<string> temp;
        for (list<string>::iterator iterator = cur.begin(); iterator != cur.end();) {
            string str1 = *iterator;
            iterator++;
            string str2;
            if (iterator == cur.end())
                str2 = str1;
            else {
                str2 = *iterator;
                iterator++;
            }
            str1.append(str2);
            temp.push_back(hashing(str1));
        }
        cur.assign(temp.begin(), temp.end());
    }
    merkleRoot = cur.front();
}

void SIMULTE_API::Block::calHash() {
    string msg = timestamp.ustr();
    string msgNonce = to_string(nonce);
    msg.append(prevHash).append(merkleRoot).append(msgNonce).append(pubKey);
    curHash = hashing(msg);
}

void SIMULTE_API::Block::setList(list<Transaction> lst) {
    if (lst.size() == 0)
        return;
    for (list<Transaction>::iterator iterator = lst.begin(); iterator!=lst.end(); iterator++) {
        transactionList_.push_back(*iterator);
    }
    return;
}


