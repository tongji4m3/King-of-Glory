/*
	*几个Server相关类
*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include<vector>
#include <boost/asio.hpp>
#include "Message.h"

using boost::asio::ip::tcp;

/*---------------chat_participant----------------
*每一个客户端加入都相当于一个chat_participant，这只是基类，它要求了
*所以继承它的类都有deliver方法，相当于能想每个客户端传递信息
*/
class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const Message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

/*---------------chat_room-----------------------
*相当于聊天室，用vector存储了每个加入聊天室的客户端
*因为只做1v1，所以相当于只有两个客户端
*server只是负责把一个客户端的信息传递给另外一个
*/
class chat_room
{
public:
	void join(chat_participant_ptr participant)
	{
		_participants.push_back(participant);
		if (participants_size() == 2)
		{
			is_connected = true;
		}
	}

	void leave(chat_participant_ptr participant)
	{
		_participants.pop_back();
		is_connected = false;
	}

	size_t participants_size()
	{
		return _participants.size();
	}

	/*
	*第一个进入的在vector[0]，且它发送过来的信息带上了它的来源（A）
	*所以如果数据来自A，则传给B(即vector[1])，反之则传给A
	*传递信息调用的是deliver（）方法，即调用了子类chat_session的deliver（）方法
	*/
	void deliver(const Message& msg);

	bool is_connected = false;

private:
	std::vector<chat_participant_ptr> _participants;
};

/*---------------chat_session---------------------
*此类是chat_participant和enable_shared_from_this的子类
*说明他必须有传递信息的函数，也能够通过shared_from_this()复制自己
*他主要任务就是，如果来了一个客户端，则向聊天室里面加入这个客户端，并且
*不断的从这个客户端读取信息，读取到后，又通过聊天室向另一个客户端传递信息
*/
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
	Message _write_msg;
};

/*---------------chat_server----------------------
*此类负责一直等到客户端连接，如果有一个客户端连接，那么送他到chat_session类
*然后又继续等待其他客户端的连接
*/
class chat_server
{
public:
	chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
		: _acceptor(io_context, endpoint)
	{
		do_accept();
	}
	const chat_room& get_room()
	{
		return _room;
	}
private:
	void do_accept();

	tcp::acceptor _acceptor;
	chat_room _room;
};

/*---------------Server---------------------------
*通过额外加一条线程启动服务器，并且端口号：10000
*detach（）表示此线程和主线程分离
*/
class Server
{
public:
	Server()
	{
		std::thread t(&Server::init_server,this);
		t.detach();
	}

	void init_server();

	bool is_connected()
	{
		return _chat_server->get_room().is_connected;
	}

	//仅仅为了获得is_connected
	chat_server * _chat_server;
};

#endif //_SERVER_H_

