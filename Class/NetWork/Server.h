//
//#ifndef _SERVER_H_
//#define _SERVER_H_
//
//#include <cstdlib>
//#include <deque>
//#include <iostream>
//#include <list>
//#include <memory>
//#include<vector>
//#include <utility>
//#include <boost/asio.hpp>
//#include "Message.h"
//
//
//using boost::asio::ip::tcp;
//
//
//typedef std::deque<Message> Message_queue;
//
////---------------chat_participant----------------
//class chat_participant
//{
//public:
//	virtual ~chat_participant() {}
//	virtual void deliver(const Message& msg) = 0;
//};
//
//
//typedef std::shared_ptr<chat_participant> chat_participant_ptr;
//
////---------------chat_room-----------------------
//class chat_room
//{
//public:
//	void join(chat_participant_ptr participant)
//	{
//		_participants.push_back(participant);
//	}
//
//	void leave(chat_participant_ptr participant)
//	{
//		_participants.pop_back();
//	}
//
//	void deliver(const Message& msg)
//	{
//		//第一个进来的肯定是A
//		if (*msg.body() == 'A')
//		{
//			_participants[1]->deliver(msg);
//		}
//		else if(*msg.body() == 'B')
//		{
//			_participants[0]->deliver(msg);
//		}
//	}
//
//private:
//	std::vector<chat_participant_ptr> _participants;
//};
//
////---------------chat_session---------------------
//class chat_session
//	: public chat_participant, public std::enable_shared_from_this<chat_session>
//{
//public:
//	chat_session(tcp::socket socket, chat_room& room)
//		: _socket(std::move(socket)), _room(room)
//	{
//	}
//
//	void start();
//
//	void deliver(const Message& msg);
//
//private:
//	void do_read_header();
//
//	void do_read_body();
//
//	void do_write();
//
//	tcp::socket _socket;
//	chat_room& _room;
//	Message _read_msg;
//	Message_queue _write_msgs;
//};
//
////---------------chat_server----------------------
//class chat_server
//{
//public:
//	chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
//		: _acceptor(io_context, endpoint)
//	{
//		do_accept();
//	}
//
//private:
//	void do_accept();
//
//	tcp::acceptor _acceptor;
//	chat_room _room;
//};
//
////---------------Server---------------------------
//class Server
//{
//public:
//	//提供Server服务，端口号：10000
//	static void server();
//};
//
//#endif //_SERVER_H_

#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include<vector>
#include <utility>
#include <boost/asio.hpp>
#include "Message.h"


using boost::asio::ip::tcp;


typedef std::deque<Message> Message_queue;

//---------------chat_participant----------------
class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const Message& msg) = 0;
};


typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//---------------chat_room-----------------------
class chat_room
{
public:
	void join(chat_participant_ptr participant)
	{
		_participants.push_back(participant);
	}

	void leave(chat_participant_ptr participant)
	{
		_participants.pop_back();
	}

	void deliver(const Message& msg)
	{
		if (_participants.size() == 2)
		{
			//第一个进来的肯定是A
			if (*msg.body() == 'A')
			{
				_participants[1]->deliver(msg);
			}
			else if (*msg.body() == 'B')
			{
				_participants[0]->deliver(msg);
			}
		}
	}

private:
	std::vector<chat_participant_ptr> _participants;
};

//---------------chat_session---------------------
class chat_session
	: public chat_participant, public std::enable_shared_from_this<chat_session>
{
public:
	chat_session(tcp::socket socket, chat_room& room)
		: _socket(std::move(socket)), _room(room)
	{
	}

	void start();

	void deliver(const Message& msg);

private:
	void do_read_header();

	void do_read_body();

	void do_write();

	tcp::socket _socket;
	chat_room& _room;
	Message _read_msg;
	Message_queue _write_msgs;
};

//---------------chat_server----------------------
class chat_server
{
public:
	chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
		: _acceptor(io_context, endpoint)
	{
		do_accept();
	}

private:
	void do_accept();

	tcp::acceptor _acceptor;
	chat_room _room;
};

//---------------Server---------------------------
class Server
{
public:
	//提供Server服务，端口号：10000
	static void server();
};

#endif //_SERVER_H_