#include"Server.h"

//---------------chat_room---------------------
void chat_room::deliver(const Message& msg)
{
	if (_participants.size() == 2)
	{
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

//---------------chat_session---------------------
void chat_session::start()
{
	//收到连接之后，如果聊天室中人数小于2，将客户加入到room中
	//如果有了两个人，则判断已经连接
	if (_room.participants_size() < 2)
	{
		_room.join(shared_from_this());
		do_read_header();
	}
}

void chat_session::deliver(const Message& msg)
{
	_write_msg = msg;
	do_write();
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
		boost::asio::buffer(_write_msg.data(),
			_write_msg.length()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
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
void Server::init_server()
{
	try
	{
		boost::asio::io_context io_context;
		tcp::endpoint endpoint(tcp::v4(), std::atoi("10000"));
		chat_server server(io_context, endpoint);
		_chat_server = &server;

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}
