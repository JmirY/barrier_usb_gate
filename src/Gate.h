// Gate.h
// JMIRY

#pragma once
#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#include "libusb.h"
#include "EventQueue.h"

#define PORT 24800
#define BUF_LEN 4096

class Gate
{
public:
    Gate();
    ~Gate();

    // initiate winsock2 & libusb
    int         init();

    // used in init()
    bool        isTarget(libusb_device*);

    // socket-side methods
    int         createSocket();
    void        fillAddrInfo();
    int         sendSocket(char*, int len);
    int         recvSocket(char*, int len);
    void        pollSocket();

    // establish connection with barrier socket
    // ServerSide : create socket and connect to barriers.exe
    // ClientSide : create listening socket and accept barrierc.exe
    virtual int connectSocket() { return 0; }

    // usb-side methods
    virtual int sendTransfer(unsigned char* buf, int bufSize) { return 0; }
    virtual int receiveTransfer(unsigned char* buf, int bufSize) { return 0; }
    void        pollUSB();
    void        flush();

    static BOOL WINAPI eventHandler(DWORD event);

protected:
    SOCKET                m_sock;
    struct sockaddr_in    m_addrInfo;
    libusb_context*       m_usbContext;
    libusb_device_handle* m_usbDevHandle;

private:
    // decide when to stop polling thread
    static bool           m_isEnd;
};