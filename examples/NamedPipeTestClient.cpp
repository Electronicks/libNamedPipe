#include "../NamedPipe.h"
#include <string>
#include <vector>
#include <iostream>
#include <thread>


void spamming_thread_1()
{
	std::vector<std::string> words;
	words.push_back(std::string("one "));
	words.push_back(std::string("two "));
	words.push_back(std::string("three "));
	words.push_back(std::string("four "));
	words.push_back(std::string("five "));
	words.push_back(std::string("six "));
	words.push_back(std::string("seven "));
	words.push_back(std::string("eight "));
	words.push_back(std::string("nine "));
	words.push_back(std::string("ten "));

	NamedPipe client("NamedPipeTester", false);
	try
	{
		client.ConnectOrOpen();
		for (size_t i = 0; i < words.size(); ++i)
		{
			std::cout << "sending " << words[i] << "\n";
			size_t size = words[i].size();
			client.WriteBytes(&size, sizeof(size));
			client.WriteBytes(words[i].data(), size);
			//Sleep(1);
		}
		client.Close();
	}
	catch (const std::runtime_error &/*e*/)
	{
		//std::cout<<"Exception: "<<e.what()<<"\n";
	}
}

void spamming_thread_2()
{
	std::vector<std::string> words;
	words.push_back(std::string("A "));
	words.push_back(std::string("B "));
	words.push_back(std::string("C "));
	words.push_back(std::string("D "));
	words.push_back(std::string("E "));
	words.push_back(std::string("F "));
	words.push_back(std::string("G "));
	words.push_back(std::string("H "));
	words.push_back(std::string("I "));
	words.push_back(std::string("J "));

	NamedPipe client("NamedPipeTester", false);
	try
	{
		client.ConnectOrOpen();
		for (size_t i = 0; i < words.size(); ++i)
		{
			std::cout << "sending " << words[i] << "\n";
			size_t size = words[i].size();
			client.WriteBytes(&size, sizeof(size));
			client.WriteBytes(words[i].data(), size);
		}
		client.Close();
	}
	catch (const std::runtime_error &e)
	{
		std::cout<<"Exception: "<<e.what()<<"\n";
	}
}

int main(int argc, char* argv[])
{
	try
	{
		for (int i = 0; i < 5; ++i)
		{
			std::thread* t = new std::thread(spamming_thread_1);
			std::thread* q = new std::thread(spamming_thread_2);
			t->join();
			q->join();
			delete t;
			delete q;
		}
	}
	catch (const std::exception &e)
	{
		std::cout << "Exception!:" << e.what() << "\n";
	}
	return 0;
}