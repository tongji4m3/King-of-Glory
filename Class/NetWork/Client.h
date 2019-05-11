//#ifndef _CLIENT_H_
//#define _CLIENT_H_
//
//#include <cstdlib>
//#include <deque>
//#include <iostream>
//#include <thread>
//#include <boost/asio.hpp>
//#include <string>
//#include "Message.h"
//
//using boost::asio::ip::tcp;
//
//typedef std::deque<Message> Message_queue;
//
//class chat_client
//{
//public:
//	chat_client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
//		: _io_context(io_context), _socket(io_context)
//	{
//		do_connect(endpoints);
//	}
//
//	void write(const Message& msg);
//
//	void close();
//
//private:
//	void do_connect(const tcp::resolver::results_type& endpoints);
//
//	void do_read_header();
//
//	void do_read_body();
//
//	void do_write();
//
//private:
//	boost::asio::io_context& _io_context;
//	tcp::socket _socket;
//	Message _read_msg;
//	Message_queue _write_msgs;
//};
//
//class Client
//{
//public:
//	static void client();
//
//private:
//	chat_client c;
//
//};
//
//#endif //_CLIENT_H_
//

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <string>
#include "Message.h"

using boost::asio::ip::tcp;

typedef std::deque<Message> Message_queue;

class chat_client
{
public:
	chat_client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
		: _io_context(io_context), _socket(io_context)
	{
		do_connect(endpoints);
	}

	void write(const Message& msg);

	void close();

private:
	void do_connect(const tcp::resolver::results_type& endpoints);

	void do_read_header();

	void do_read_body();

	void do_write();

private:
	boost::asio::io_context& _io_context;
	tcp::socket _socket;
	Message _read_msg;
	Message_queue _write_msgs;
};

class Client
{
public:
	Client()
	{
		std::thread t(&Client::init_client,this);
		t.detach();
	}

	void init_client();

	void send_message(std::string& message);

	std::string get_message();

private:
	chat_client *_chat_client;
	std::deque<std::string>     _message_deque;             //接受到的信息的集合
};

#endif //_CLIENT_H_


