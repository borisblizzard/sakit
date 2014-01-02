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
#include "ReceiverThread.h"

namespace sakit
{
	ReceiverThread::ReceiverThread(PlatformSocket* socket) : WorkerThread(&process, socket), maxBytes(INT_MAX)
	{
	}

	ReceiverThread::~ReceiverThread()
	{
	}

	void ReceiverThread::_updateProcess()
	{
		while (this->running)
		{
			if (!this->socket->receive(this->stream, this->mutex, this->maxBytes))
			{
				this->mutex.lock();
				this->state = FAILED;
				this->mutex.unlock();
				return;
			}
			if (this->maxBytes == 0)
			{
				break;
			}
			hthread::sleep(sakit::getRetryTimeout() * 1000.0f);
		}
		this->mutex.lock();
		this->state = FINISHED;
		this->mutex.unlock();
	}

	void ReceiverThread::process(hthread* thread)
	{
		((ReceiverThread*)thread)->_updateProcess();
	}

}
