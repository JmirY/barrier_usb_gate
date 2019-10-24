// ClientSideGate.h
// JMIRY

#pragma once

#include "Gate.h"

#define EP_IN 139
#define EP_OUT 8

class ClientSideGate : public Gate
{
public:
    // override Gate class methods
    int connectSocket();
    int sendTransfer(unsigned char* buf, int bufSize);
    int receiveTransfer(unsigned char* buf, int bufSize);
};

