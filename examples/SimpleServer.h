#pragma once
#include "../NamedPipeServer.h"

class SimpleServer : public NamedPipeServer
{
protected:
	void handleClient(std::shared_ptr<NamedPipe> client);
public:
	SimpleServer(const std::string &name)
		: NamedPipeServer(name, 1)
	{};
	virtual ~SimpleServer() 
	{};
};