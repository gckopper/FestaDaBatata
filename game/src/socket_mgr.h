#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOSOUND
#define NOGDI
#define NORASTEROP
#define NODRAWTEXT
#define _WINGDI_
#define NOUSER 

#ifndef socket_manager_h
#define socket_manager_h

#define BUFFER_SIZE 512
#define MSG_BUFFER_SIZE (BUFFER_SIZE / sizeof(ServerMessage))+1

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "client_packer.h"

#define PORT 27015

#pragma comment (lib, "Ws2_32.lib")

int start_conn(unsigned char* const room, unsigned char* const ip);

int send_msg(unsigned char* const msg, int len);

int send_client_msg(ClientMessage* const msg);

int recv_msg(unsigned char* const buf, int len);

int recv_client_msg(ServerMessage* const array_of_messages);
#endif // !1