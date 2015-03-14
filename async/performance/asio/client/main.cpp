#define WIN32_LEAN_AND_MEAN

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../../DataFrame.h"
#include <async/Application.h>


bool		go = false;
char*		addr;


const TestData data =
{
	{ sizeof(NS_CERL::MailHeader), 0, VENUS_MAIL_MAGIC_CODE, 0x456, (NS_CERL::FID)1},
	NS_CERL_IO::vtI4,
   	123,
	NS_CERL_IO::vtNull 
};

class ClientConnect
{
public:
	boost::asio::ip::tcp::socket socket;
	char buf[256];

public:
	ClientConnect(boost::asio::io_service& service)
		: socket(service)
	{
	}

public:
	void start()
	{
		//printf("client wait...\n");
		while(!go)
			Sleep(1);

		//printf("start write.\n");
		async_write(socket, boost::asio::buffer((char*)(&data), sizeof(data))
			, boost::bind(&ClientConnect::handle_write, this
				, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	void handle_write(const boost::system::error_code& error, size_t bytes)
	{
		//printf("write over.\nstart read.\n");
		socket.async_read_some(boost::asio::buffer((char*)(buf), sizeof(buf))
			, boost::bind(&ClientConnect::handle_read, this
				, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	void handle_read(const boost::system::error_code& error, size_t bytes)
	{
		//printf("write over.\nstart read.\n");
		start();
	}
};

THREADRET WINAPI clientCallback(void* param)
{
	using namespace boost::asio;
	try
	{
		boost::asio::io_service	iosvr;
		ClientConnect conn(iosvr);

		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::address::from_string(addr), 8888);

		conn.socket.connect(endpoint);

		conn.start();

		iosvr.run();
		Sleep(1000000);
		printf("client exiting ..\n");
	}
	catch(...)
	{
		printf("fatal error. exit.\n");
		exit(1);
	}

	return 0;
}


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: %s <ConnectionCount> <IP>\n", argv[0]);
		return 1;
	}

	int ncli = 0;
	if (sscanf(argv[1], "%d", &ncli) != 1)
	{
		printf("param error.\n");
	}
	addr = argv[2];

	for (int i=0; i<ncli; ++i)
	{
		HTHREAD hThread = CreateThread(clientCallback, addr);
		CloseThread(hThread);
		printf("start client # %d\n", i);
		Sleep(100);
	}

	printf("ready...\n");
	Sleep(10000);
	go = true;
	printf("go!\n");
	Sleep(1000000);

	return 0;
}
