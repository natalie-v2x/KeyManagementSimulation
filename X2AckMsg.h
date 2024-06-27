/*
 * X2AckMsg.h
 *
 *  Created on: Mar 18, 2024
 *      Author: veins
 */

#ifndef STACK_BCMANAGER_X2ACKMSG_H_
#define STACK_BCMANAGER_X2ACKMSG_H_

#include "x2/packet/LteX2Message.h"

class SIMULTE_API X2AckMsg : public LteX2Message
{
public:

    X2AckMsg() :
        LteX2Message()
    {
        type_ = X2_ACK;
    }

    X2AckMsg(const X2AckMsg& other) : LteX2Message() { operator=(other); }

    X2AckMsg& operator=(const X2AckMsg& other)
    {
        if (&other == this)
            return *this;
        LteX2Message::operator=(other);
        return *this;
    }

    virtual X2AckMsg* dup() const { return new X2AckMsg(*this); }

    virtual ~X2AckMsg() { }
};

#endif /* STACK_BCMANAGER_X2ACKMSG_H_ */
