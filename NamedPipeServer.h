#pragma once

#include "NamedPipe.h"
#include <vector>
#include <memory>
#include <queue>
#include <thread>
#include <mutex>

/*!
* \brief
* This class contains the main functions to make an multithreaded NamedPipe server
*
* Inherit from it and override the handleClient(INamedPipe* client) function to make your own server
*/
class NamedPipeServer
{
protected:
	/*!
	 *  \brief Secure access to the queue of clients
	 */
	class SecureClientQueue : protected std::queue<std::shared_ptr<NamedPipe>>
	{
		/// Beautifier to access base implementation
		using Base = std::queue<std::shared_ptr<NamedPipe>>;
		/// Secures access to queue
		std::mutex _queueLock;
	public:
		/*!
		 *  Secure access to the front of the queue
		 *
		 *      @param [in,out] outClient The client removed from 
		 *          the queue if true is rreturned
		 *
		 *      @return True if the param is set to the removed client.
		 */
		bool try_pop(std::shared_ptr<NamedPipe> &outClient);

		/*!
		 *  Add a client to the queue
		 *
		 *      @param [in] client new cliennt to add
		 */
		void push(std::shared_ptr<NamedPipe> client);

		/*!
		 *  Empty the queue of clients.
		 */
		void clear();
	};

	///the server pipe name 
	std::string _name;
	///the number of threads handling client connections 
	size_t _thread_pool_size;
	SecureClientQueue _clients;
	///activity. change to false in order to make all threads stop
	bool volatile _active;
	///this vector stores all threads serving clients
	std::vector<std::shared_ptr<std::thread> > _threads;

	/*!
	* \brief
	* Starts the main loop (waits for new clients and puts them into queue)
	*/
	static void run(NamedPipeServer * pipeServer);
	/*!
	* \brief
	* Starts all worker theads
	*/
	void startWorkers();
	/*!
	* \brief
	* Pops the client from queue and passes it to handleClient()
	*/
	static void workerProc(NamedPipeServer * pipeServer);
	/*!
	* \brief
	* This function must be overriden. It is intended for handling data from clients;
	*/
	virtual void handleClient(std::shared_ptr<NamedPipe> client) = 0; //Override this to make your own protocol
public:
	/*!
	* \brief
	* Starts the server and all worker theads. You must create a new thread for this func.
	*/
	void Start();
	/*!
	* \brief
	* Stops all worker threads and main thread - run()
	*/
	void Stop();
	/*!
	* \brief
	* Joins all worker theads
	*/
	void JoinWorkers();
	/*!
	* \brief
	* Write brief comment for NamedPipeServer here.
	*
	* \param name
	* The name for server pipe
	*
	* \param thread_pool_size
	* the number of threads for handling clients connection
	*/
	NamedPipeServer(const std::string& name, size_t thread_pool_size)
		: _name(name)
		, _thread_pool_size(thread_pool_size)
	{}
	virtual ~NamedPipeServer();
};