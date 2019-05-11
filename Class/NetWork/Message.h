#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

class Message
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };

	Message()
		: _body_length(0)
	{
	}

	const char* data() const
	{
		return _data;
	}

	char* data()
	{
		return _data;
	}

	std::size_t length() const
	{
		return header_length + _body_length;
	}

	const char* body() const
	{
		return _data + header_length;
	}

	char* body()
	{
		return _data + header_length;
	}

	std::size_t body_length() const
	{
		return _body_length;
	}

	void body_length(std::size_t new_length)
	{
		_body_length = new_length;
		if (_body_length > max_body_length)
			_body_length = max_body_length;
	}

	bool decode_header()
	{
		using namespace std;
		char header[header_length + 1] = "";
		strncat_s(header, _data, header_length);
		_body_length = atoi(header);
		if (_body_length > max_body_length)
		{
			_body_length = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		using namespace std;
		char header[header_length + 1] = "";
		sprintf_s(header, "%4d", static_cast<int>(_body_length));
		memcpy(_data, header, header_length);
	}

private:
	char _data[header_length + max_body_length];
	std::size_t _body_length;
};

#endif // _MESSAGE_H_