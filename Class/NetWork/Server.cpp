//#include"Server.h"
//
////---------------chat_session---------------------
//void chat_session::start()
//{
//	_room.join(shared_from_this());////收到连接之后首先将客户加入到room中
//	do_read_header();
//}
//
//void chat_session::deliver(const Message& msg)
//{
//	bool write_in_progress = !_write_msgs.empty();
//	_write_msgs.push_back(msg);//会将消息先放到write_msgs_中
//	if (!write_in_progress)
//	{
//		do_write();
//	}
//}
//
//void chat_session::do_read_header()
//{
//	auto self(shared_from_this());
//	boost::asio::async_read(_socket,
//		boost::asio::buffer(_read_msg.data(), Message::header_length),//从此人嘴里说的话
//		[this, self](boost::system::error_code ec, std::size_t /*length*/)
//		{
//			if (!ec && _read_msg.decode_header())
//			{
//				do_read_body();
//			}
//			else
//			{
//				_room.leave(shared_from_this());//此人离开了聊天室
//			}
//		});
//}
//
//void chat_session::do_read_body()
//{
//	auto self(shared_from_this());
//	boost::asio::async_read(_socket,
//		boost::asio::buffer(_read_msg.body(), _read_msg.body_length()),
//		[this, self](boost::system::error_code ec, std::size_t /*length*/)
//		{
//			if (!ec)
//			{
//				_room.deliver(_read_msg);
//				do_read_header();
//			}
//			else
//			{
//				_room.leave(shared_from_this());
//			}
//		});
//}
//
//void chat_session::do_write()
//{
//	auto self(shared_from_this());
//	boost::asio::async_write(_socket,
//		boost::asio::buffer(_write_msgs.front().data(),
//			_write_msgs.front().length()),
//		[this, self](boost::system::error_code ec, std::size_t /*length*/)
//		{
//			if (!ec)
//			{
//				_write_msgs.pop_front();
//				if (!_write_msgs.empty())
//				{
//					do_write();
//				}
//			}
//			else
//			{
//				_room.leave(shared_from_this());
//			}
//		});
//}
//
////---------------chat_server----------------------
//void chat_server::do_accept()
//{
//	_acceptor.async_accept(
//		[this](boost::system::error_code ec, tcp::socket socket)
//		{
//			if (!ec)
//			{
//				std::make_shared<chat_session>(std::move(socket), _room)->start();
//			}
//
//			do_accept();
//		});
//}
//
////---------------Server---------------------------
//void Server::server()
//{
//	try
//	{
//		boost::asio::io_context io_context;
//		tcp::endpoint endpoint(tcp::v4(), std::atoi("10000"));
//		chat_server server(io_context, endpoint);
//
//		io_context.run();
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//}

#include"Server.h"

//---------------chat_session---------------------
void chat_session::start()
{
	_room.join(shared_from_this());////收到连接之后首先将客户加入到room中
	do_read_header();
}

void chat_session::deliver(const Message& msg)
{
	bool write_in_progress = !_write_msgs.empty();
	_write_msgs.push_back(msg);//会将消息先放到write_msgs_中
	if (!write_in_progress)
	{
		do_write();
	}
}

void chat_session::do_read_header()
{
	auto self(shared_from_this());
	boost::asio::async_read(_socket,
		boost::asio::buffer(_read_msg.data(), Message::header_length),//从此人嘴里说的话
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && _read_msg.decode_header())
			{
				do_read_body();
			}
			else
			{
				_room.leave(shared_from_this());//此人离开了聊天室
			}
		});
}

void chat_session::do_read_body()
{
	auto self(shared_from_this());
	boost::asio::async_read(_socket,
		boost::asio::buffer(_read_msg.body(), _read_msg.body_length()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				_room.deliver(_read_msg);
				do_read_header();
			}
			else
			{
				_room.leave(shared_from_this());
			}
		});
}

void chat_session::do_write()
{
	auto self(shared_from_this());
	boost::asio::async_write(_socket,
		boost::asio::buffer(_write_msgs.front().data(),
			_write_msgs.front().length()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				_write_msgs.pop_front();
				if (!_write_msgs.empty())
				{
					do_write();
				}
			}
			else
			{
				_room.leave(shared_from_this());
			}
		});
}

//---------------chat_server----------------------
void chat_server::do_accept()
{
	_acceptor.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<chat_session>(std::move(socket), _room)->start();
			}

			do_accept();
		});
}

//---------------Server---------------------------
void Server::server()
{
	try
	{
		boost::asio::io_context io_context;
		tcp::endpoint endpoint(tcp::v4(), std::atoi("10000"));
		chat_server server(io_context, endpoint);

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}