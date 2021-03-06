/// @file
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a socket working over TCP.

#ifndef SAKIT_TCP_SOCKET_H
#define SAKIT_TCP_SOCKET_H

#include <hltypes/hstream.h>

#include "Connector.h"
#include "Host.h"
#include "sakitExport.h"
#include "Socket.h"
#include "State.h"

namespace sakit
{
	class ConnectorThread;
	class TcpReceiverThread;
	class TcpSocketDelegate;

	class sakitExport TcpSocket : public Socket, public Connector
	{
	public:
		TcpSocket(TcpSocketDelegate* socketDelegate);
		~TcpSocket();

		bool setNagleAlgorithmActive(bool value);

		void update(float timeDelta = 0.0f) override;

		/// @note Keep in mind that only all queued stream data is received at once.
		int receive(hstream* stream, int maxCount = 0);
		hstr receive(int maxCount = 0);
		bool startReceiveAsync(int maxCount = 0);

	protected:
		TcpSocketDelegate* tcpSocketDelegate;
		TcpReceiverThread* tcpReceiver;

		void _updateReceiving() override;

		void _activateConnection(Host remoteHost, unsigned short remotePort, Host localHost, unsigned short localPort) override;

	private:
		TcpSocket(const TcpSocket& other); // prevents copying

	};

}
#endif
