/*
-----------------------------------------------------------------------------
Filename:    Network.h
-----------------------------------------------------------------------------
*/

#ifndef _Network_H_
#define _Network_H_

#include <iostream>
#include <SDL/SDL_net.h>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Player.h"

class Network {
    
    public:   
		Network();
		virtual ~Network();
		void checkForClient();
		bool checkForServer();
		bool checkForPackets();
		bool sendPacket(gameUpdate update);
		void broadcastToClients(const char* host);
		gameUpdate* getGameUpdate();
		bool isConnectionOpen();
		bool isThisServer();
		void waitForClientConnection();
		char* intToIpAddr(long ipAddress);
		char* intToIpAddr(long ipAddr, bool networkByteOrder);
		int getMyIp();
		int getTheirIp();
		
	private:
		void readPacketToBuffer();
		bool connectionOpen;
		bool isServer;
		UDPsocket UdpSocket;		//socket for broadcasting and recieving UDP packets so that clients can find a server on the local network.
		SDLNet_SocketSet socketSet;	//socketSet for checking for new packets
		TCPsocket serverSocket; 	//server only socket where clients can request connections
		TCPsocket peerSocket; 		//socket for communicating once connection is established
		IPaddress myIp; 			//this computer's ip address
		IPaddress *remoteIP; 		//other computer's ip address
		int theirIp;				//Ip address read in from theirIp.txt, Server will broadcast to this Ip address as well if it exists
		
		char buffer[BUFFER_SIZE];	//buffer for gameUpdates to be copied into when packets are recieved
		char* conversion;			//pointer to last intToIpAddr conversion to avoid memory leaks
		
};

#endif 

