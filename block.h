/*
 * block.h
 *
 *  Created on: Aug 27, 2022
 *      Author: veins
 */

#ifndef STACK_BCMANAGER_BLOCK_H_
#define STACK_BCMANAGER_BLOCK_H_

//https://davenash.com/2017/10/build-a-blockchain-with-c/
//https://hackernoon.com/learn-blockchains-by-building-one-117428612f46

#include "transaction.h"

class SIMULTE_API Block {
protected:
    string prevHash;
    string curHash;
    string merkleRoot;
    simtime_t timestamp;
    unsigned int nonce;
    string pubKey;
    list<Transaction> transactionList_;

public:
    Block(string prev, VectorXd pk, list<Transaction> list);
    Block(string prev, string cur, string merkleR, simtime_t ts, unsigned int n, string key, list<Transaction> list);
    ~Block();
    void calMerkleRoot();
    string getMerkleRoot()const {return merkleRoot;}
    void calHash();
    string getHash()const{return curHash;}
    string getPrevHash()const{return prevHash;}
    simtime_t getTS()const{return timestamp;}
    void setList(list<Transaction> list);
    list<Transaction> getList()const {return transactionList_;}
    void setNonce(unsigned int n){nonce = n;}
    unsigned int getNonce()const{return nonce;}
    string getPubKey()const{return pubKey;}
};



#endif /* STACK_BCMANAGER_BLOCK_H_ */
