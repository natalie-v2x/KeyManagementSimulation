/*
 * utils.cc
 *
 *  Created on: Aug 27, 2022
 *      Author: veins
 */
#include "utils.h"

string hashing(string msg) {
    //https://www.crypto.com/wiki/SHA3
    string digest;
    SHA3_256 hash;
    hash.Update((const byte*)msg.data(), msg.size());
    digest.resize(hash.DigestSize());
    hash.Final((byte*)&digest[0]);
    return digest;
}
string PK2String(VectorXd pubKey) {
    string ss;
    for (int i = 0; i < 64; i++)
    {
        ss.append(to_string(pubKey(i)));
        if (i < 63)
            ss.append(" ");
    }
    return ss;
}
