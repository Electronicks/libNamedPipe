#include "SimpleServer.h"
#include <iostream>

int main(int argc, char* argv[])
{
	SimpleServer* s = new SimpleServer("NamedPipeTester");
	s->Start();
    std::cout << "Hit ENTER to quit..." << std::endl;
	getchar();
	delete s; // Stops on delete
	return 0;
}