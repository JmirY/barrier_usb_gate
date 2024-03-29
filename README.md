# barrier_usb_gate
### This project is USB bridge of [debauchee/barrier](https://github.com/debauchee/barrier).  
### CLI based, only for Windows.  
---

## Requirements
__USB data transfer cable required__.  
Which enables communication between USB hosts.  
Cable also need to provide libusb as API.  
This one was tested with cable using Prolific PL2701 chipset.

## Depedencies
- libusb  
barrier_usb_gate needs [libusb](https://libusb.info/) to use USB transfer.  
Static library file (libusb-1.0.lib) is in (projRoot)/libusb/.  

## How to Build
1. Install Microsoft Visual Studio  
Program uses Windows API (winsock2).  
So it would be much convinient for you to use msvc as builder.  
Visual Studio 2019 is recommended.  
2. Open solution file (_barrier_usb_gate.sln_) then __BUILD__  
Build output directory will be seen at root directory.  

## How to Use
1. Run barrier server and client  
Client process must target localhost IP __127.0.0.1__  
2. Run barrier_usb_gate on both PC  
Process will set connection with barrier process.  
Server / Client process of barrier = barriers.exe / barrierc.exe.  
3. Press Ctrl+C to end process  
Log message will be printed if process ends properly. 