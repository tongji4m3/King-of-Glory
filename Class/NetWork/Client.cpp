#include"Client.h"

void chat_client::write(const Message& msg)
{
	boost::asio::post(_io_context,
		[this, msg]()
		{
			bool write_in_progress = !_write_msgs.empty();
			_write_msgs.push_back(msg);
			if (!write_in_progress)
			{
				do_write();
			}
		});
}

void chat_client::close()
{
	boost::asio::post(_io_context, [this]() { _socket.close(); });
}

void chat_client::do_connect(const tcp::resolver::results_type& endpoints)
{
	boost::asio::async_connect(_socket, endpoints,
		[this](boost::system::error_code ec, tcp::endpoint)
		{
			if (!ec)
			{
				do_read_header();
			}
		});
}

void chat_client::do_read_header()
{
	boost::asio::async_read(_socket,
		boost::asio::buffer(_read_msg.data(), Message::header_length),
		[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && _read_msg.decode_header())
			{
				do_read_body();
			}
			else
			{
				_socket.close();
			}
		});
}

void chat_client::do_read_body()
{
	boost::asio::async_read(_socket,
		boost::asio::buffer(_read_msg.body(), _read_msg.body_length()),
		[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				std::cout.write(_read_msg.body()+1, _read_msg.body_length());
				std::cout << "\n";
				do_read_header();
			}
			else
			{
				_socket.close();
			}
		});
}

void chat_client::do_write()
{
	boost::asio::async_write(_socket,
		boost::asio::buffer(_write_msgs.front().data(), _write_msgs.front().length()),
		[this](boost::system::error_code ec, std::size_t /*length*/)
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
				_socket.close();
			}
		});
}

void Client::init_client()
{
	try
	{
		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve("127.0.0.1", "10000");
		_chat_client=new chat_client(io_context, endpoints);
		std::thread t([&io_context]() { io_context.run(); });

		while (true)
		{
			std::string s = "ABCDE";
			send_message(s);
		}

	/*	std::string line;
		while (std::getline(std::cin, line))
		{
			line = 'A' + line;
			Message msg;
			msg.body_length(line.size());
			std::memcpy(msg.body(), line.c_str(), msg.body_length());
			msg.encode_header();
			_chat_client->write(msg);
		}*/
		//_chat_client->close();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

void Client::send_message(std::string& message)
{
	message = 'A' + message;
	Message msg;
	msg.body_length(message.size());
	std::memcpy(msg.body(), message.c_str(), msg.body_length());
	msg.encode_header();
	_chat_client->write(msg);
}

//#include"Client.h"
//
//void chat_client::write(const Message& msg)
//{
//	boost::asio::post(_io_context,
//		[this, msg]()
//		{
//			bool write_in_progress = !_write_msgs.empty();
//			_write_msgs.push_back(msg);
//			if (!write_in_progress)
//			{
//				do_write();
//			}
//		});
//}
//
//void chat_client::close()
//{
//	boost::asio::post(_io_context, [this]() { _socket.close(); });
//}
//
//void chat_client::do_connect(const tcp::resolver::results_type& endpoints)
//{
//	boost::asio::async_connect(_socket, endpoints,
//		[this](boost::system::error_code ec, tcp::endpoint)
//		{
//			if (!ec)
//			{
//				do_read_header();
//			}
//		});
//}
//
//void chat_client::do_read_header()
//{
//	boost::asio::async_read(_socket,
//		boost::asio::buffer(_read_msg.data(), Message::header_length),
//		[this](boost::system::error_code ec, std::size_t /*length*/)
//		{
//			if (!ec && _read_msg.decode_header())
//			{
//				do_read_body();
//			}
//			else
//			{
//				_socket.close();
//			}
//		});
//}
//
//void chat_client::do_read_body()
//{
//	boost::asio::async_read(_socket,
//		boost::asio::buffer(_read_msg.body(), _read_msg.body_length()),
//		[this](boost::system::error_code ec, std::size_t /*length*/)
//		{
//			if (!ec)
//			{
//				std::cout.write(_read_msg.body() + 1, _read_msg.body_length());
//				std::cout << "\n";
//				do_read_header();
//			}
//			else
//			{
//				_socket.close();
//			}
//		});
//}
//
//void chat_client::do_write()
//{
//	boost::asio::async_write(_socket,
//		boost::asio::buffer(_write_msgs.front().data(), _write_msgs.front().length()),
//		[this](boost::system::error_code ec, std::size_t /*length*/)
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
//				_socket.close();
//			}
//		});
//}
//
//void Client::client()
//{
//	try
//	{
//		boost::asio::io_context io_context;
//		tcp::resolver resolver(io_context);
//		auto endpoints = resolver.resolve("127.0.0.1", "10000");
//		chat_client c(io_context, endpoints);
//		std::thread t([&io_context]() { io_context.run(); });
//		t.join();
//
//		/*char line[Message::max_body_length + 1];
//		while (std::cin.getline(line, Message::max_body_length + 1))
//		{
//			Message msg;
//			msg.body_length(std::strlen(line));
//			std::memcpy(msg.body(), line, msg.body_length());
//			msg.encode_header();
//			c.write(msg);
//		}*/
//
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//}
//
//void Client::write_message(std::string& message)
//{
//	message = 'A' + message;
//	Message msg;
//	msg.body_length(message.size());
//	std::memcpy(msg.body(), message.c_str(), msg.body_length());
//	msg.encode_header();
//	_chat_client.write(msg);
//}