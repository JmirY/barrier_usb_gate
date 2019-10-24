// ServerSideGate.h
// JMIRY

#pragma once

#include "Gate.h"

#define EP_IN 137
#define EP_OUT 10

class ServerSideGate : public Gate
{
public:
    // override Gate class methods
    int connectSocket();
    int sendTransfer(unsigned char* buf, int bufSize);
    int receiveTransfer(unsigned char* buf, int bufSize);
};

