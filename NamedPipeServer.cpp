#include "NamedPipeServer.h"

bool NamedPipeServer::SecureClientQueue::try_pop(std::shared_ptr<NamedPipe> &outClient)
{
	std::lock_guard<std::mutex> guard(_queueLock);
	if (!empty())
	{
		outClient = Base::front();
		Base::pop();
		return true;
	}
	return false;
}

void NamedPipeServer::SecureClientQueue::push(std::shared_ptr<NamedPipe> client)
{
	std::lock_guard<std::mutex> guard(_queueLock);
	Base::push(client);
}

void NamedPipeServer::SecureClientQueue::clear()
{
	std::lock_guard<std::mutex> guard(_queueLock);
	while (!Base::empty())
		Base::pop();
}

void NamedPipeServer::run(NamedPipeServer * pipeServer)
{
	std::unique_ptr<NamedPipe> server(new NamedPipe(pipeServer->_name, true));
	try
	{
		server->ConnectOrOpen();
	}
	catch (const std::exception& e)
	{
	}
	while (pipeServer->_active)
	{
		try
		{
			std::shared_ptr<NamedPipe> client(server->WaitForConnection(1000));
			if (client != NULL)
			{
				pipeServer->_clients.push(client);
			}
		}
		catch (const std::exception& e)
		{
		}
		catch (...)
		{
		}
	}
	server->Close(); // Don't rely on the destructor
}

void NamedPipeServer::Start()
{
	_active = true;
	startWorkers();
}

void NamedPipeServer::workerProc(NamedPipeServer * pipeServer)
{
	std::shared_ptr<NamedPipe> client = nullptr;
	while (pipeServer->_active)
	{
		client = nullptr;
		if (pipeServer->_clients.try_pop(client))
			pipeServer->handleClient(client);
//		Sleep(1);
	}
}

void NamedPipeServer::startWorkers()
{
	for (size_t i = 0; i < _thread_pool_size; ++i)
	{
		std::shared_ptr<std::thread> thread(new std::thread(&NamedPipeServer::workerProc, this));
		_threads.push_back(thread);
	}
	std::shared_ptr<std::thread> dispatcher(new std::thread(&NamedPipeServer::run, this));
	_threads.push_back(dispatcher);
}

void NamedPipeServer::JoinWorkers()
{
	size_t size = _threads.size();
	for (auto thread : _threads)
		thread->join();
	_threads.clear();
}

void NamedPipeServer::Stop()
{
	_active = false;
	JoinWorkers();
}

NamedPipeServer::~NamedPipeServer(void)
{
	Stop(); // Cleanup threead
	_clients.clear();
}
