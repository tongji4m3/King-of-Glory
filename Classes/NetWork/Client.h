/*
	*Client相关类
*/
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <cstdlib>
#include <deque>
#include<vector>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <string>
#include "Message.h"

using boost::asio::ip::tcp;

typedef std::deque<Message> Message_queue;

/*---------------chat_client---------------------------
*该类是client的底层类，负责不断的从服务器中读取信息，并且
*把信息压入deque里面，Client的get_message()就是读取该类
*的 _message_deque来获取信息的
*/
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

	//std::deque<std::string>     _message_deque;             //接受到的信息的集合
	std::vector<std::string>     _message_deque;             //接受到的信息的集合

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

/*---------------Client---------------------------
*通过额外加一条线程启动client，并且端口号：10000
*detach（）表示此线程和主线程分离
*初始化时要添加client的标识，以便发送消息时发到对面
*send_message（）与get_message()是该类的主要功能
*这样就能实现在任意时刻任意位置收发信息
*get_message()一次只读取一条，且读deque顶部数据
*/
class Client
{
public:
	Client(const std::string& head) :_head(head)
	{
		std::thread t(&Client::init_client, this);
		t.detach();
	}

	void init_client();

	void send_message(std::string message);

	//std::string get_message();

	std::vector<std::string> get_messages();

	chat_client* _chat_client;
	std::string _head;
};

#endif //_CLIENT_H_


