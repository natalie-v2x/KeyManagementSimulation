/*
 * utils.h
 *
 *  Created on: Aug 27, 2022
 *      Author: veins
 */

#ifndef STACK_BCMANAGER_UTILS_H_
#define STACK_BCMANAGER_UTILS_H_

#include "sha3.h"
#include <Eigen/Dense>

using namespace CryptoPP;
using namespace std;
using namespace Eigen;

string hashing(string msg);
string PK2String(VectorXd pk);


#endif /* STACK_BCMANAGER_UTILS_H_ */
