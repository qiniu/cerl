#define WIN32_LEAN_AND_MEAN

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../../DataFrame.h"
#include <async/Application.h>

int			g_nclients = 0, g_nclimax = 0;
int			g_nMsg = 0, g_totalMsg;
int			g_maxCount = 500000;
DWORD		g_start, g_stop;
bool		g_started = false;


class ClientConnect
{
public:
	boost::asio::ip::tcp::socket socket;
	TestData data;
	size_t size;

public:
	ClientConnect(boost::asio::io_service& service)
		: socket(service)
	{
	}

public:
	void start()
	{
		//printf("start read.\n");
		async_read(socket, boost::asio::buffer((char*)&data, sizeof(data))
			, boost::bind(&ClientConnect::handle_read, this
				, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	void handle_read(const boost::system::error_code& error, size_t bytes)
	{
		//printf("read over.\nstart write.\n");
		async_write(socket, boost::asio::buffer((char*)&data, sizeof(data))
			, boost::bind(&ClientConnect::handle_write, this
				, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	void handle_write(const boost::system::error_code& error, size_t bytes)
	{
		//printf("write over.\nstart read.\n");
		start();

		if(g_started)
		{
			++g_nMsg;
			if(g_nMsg >= g_maxCount)
			{
				g_stop = GetTickCount();
				g_totalMsg = g_nMsg;
				printf("measure stop: %u.\n", g_stop);

				double time = (double)(g_stop - g_start)/1000;

				printf("max msg: %d; total msg: %d; total time: %fs; msg per second: %f.\n", g_maxCount, g_totalMsg, time, g_totalMsg/time);

				exit(0);
			}
		}
	}
};

class TestServer
{
	boost::asio::ip::tcp::acceptor	m_acceptor;

public:
	TestServer(boost::asio::io_service& service)
		: m_acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8888))
	{
		start_accept();
	}

	void start_accept()
	{
		ClientConnect* cc = new ClientConnect(m_acceptor.io_service());
		printf("begin accept.\n");
		m_acceptor.async_accept(cc->socket,
			boost::bind(&TestServer::handle_accept, this, cc, boost::asio::placeholders::error));
	}

	void handle_accept(ClientConnect* connection, const boost::system::error_code& error)
	{
		if(!error)
		{
			printf("client connected: #%d.\n", g_nclients);
			connection->start();
			++g_nclients;
			if (g_nclients == g_nclimax)
			{
				printf("INFO: %d clients have been accepted.\n", g_nclimax);

				Sleep(16000);

				printf("INFO: begin test ...\n");
				g_start = GetTickCount();
				g_started = true;
				g_nMsg = 0;
				printf("measure start: %u. msg:%d\n", (UINT)g_start, g_nMsg);
				return;
			}
			start_accept();
		}
		else
		{
			printf("ERROR: Accepted failed!\n");
			exit(1);
		}
	}
};


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <ClientCounts>\n", argv[0]);
		return 1;
	}

	if (sscanf(argv[1], "%d", &g_nclimax) != 1)
	{
		printf("param error.\n");
		return 1;
	}

	try
	{
		boost::asio::io_service	iosvr;
		TestServer server(iosvr);
		iosvr.run();
	}
	catch(...)
	{
		printf("fatal error. exit.\n");
	}
	return 0;
}
