#pragma once

#include <queue>

class EventQueue
{
public:
    enum eEventType
    {
        kUSB,
        kSOCK,
    };

public:
    EventQueue() {}
    ~EventQueue() {}

    // allow polling methods to push event
    void pushEvent(eEventType event)
    {
        m_eventQueue.push(event);
    }

private:
    std::queue<eEventType> m_eventQueue;

    friend class Gate;
};

