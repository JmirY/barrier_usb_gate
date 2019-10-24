// App.h
// JMIRY

#pragma once

#include "ServerSideGate.h"
#include "ClientSideGate.h"

class App
{  
public:
    enum eBarrierProcess
    {
        kIsServer,
        kIsClient,
        kNoBarrier,
    };

public:
    App() {}
    ~App() {}

    int run();

    // find barrier process and detect whether it's client or server
    eBarrierProcess detectProcess();
};