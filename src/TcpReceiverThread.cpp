/// @file
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstream.h>
#include <hltypes/hthread.h>

#include "PlatformSocket.h"
#include "sakit.h"
#include "SocketDelegate.h"
#include "TcpReceiverThread.h"

namespace sakit
{
	TcpReceiverThread::TcpReceiverThread(PlatformSocket* socket) : SocketThread(socket), maxBytes(0)
	{
		this->stream = new hstream();
	}

	TcpReceiverThread::~TcpReceiverThread()
	{
		delete this->stream;
	}

	void TcpReceiverThread::_updateProcess()
	{
		int remaining = this->maxBytes;
		while (this->running)
		{
			if (!this->socket->receive(this->stream, this->mutex, remaining))
			{
				this->mutex.lock();
				this->result = FAILED;
				this->mutex.unlock();
				return;
			}
			if (this->maxBytes > 0 && remaining == 0)
			{
				break;
			}
			hthread::sleep(sakit::getRetryTimeout() * 1000.0f);
		}
		this->mutex.lock();
		this->result = FINISHED;
		this->mutex.unlock();
	}

}