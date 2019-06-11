#include"Client.h"

//---------------chat_client---------------------------
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
				//std::cout.write(_read_msg.body() + 1, _read_msg.body_length()) << std::endl;
				std::string temp(_read_msg.body(), _read_msg.body_length());
				_message_deque.push_back(temp);
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

//---------------Client---------------------------
void Client::init_client()
{
	try
	{
		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve("127.0.0.1", "10000");
		chat_client chat_client(io_context, endpoints);
		_chat_client = &chat_client;
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

void Client::send_message(std::string message)
{
	message = _head + message;
	Message msg;
	msg.body_length(message.size());
	std::memcpy(msg.body(), message.c_str(), msg.body_length());
	msg.encode_header();
	_chat_client->write(msg);
}

/*std::string Client::get_message()
{
	//如果没有信息，则以00代替
	std::string temp = "00";
	if (!_chat_client->_message_deque.empty())
	{
		temp = _chat_client->_message_deque.front();
		_chat_client->_message_deque.pop_front();
	}

	return temp;
}*/

std::vector<std::string> Client::get_messages()
{
	std::vector<std::string> messages(_chat_client->_message_deque);
	_chat_client->_message_deque.clear();

	return messages;
}
