/*
	*Message类，用于client与server之间交换信息
	
*/

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

/*
	* @brief 一个Message对象是传输的一条数据
	*它由数据长度，发送对象，具体信息组成
	*例如"   5Aabcd"中，前面空格加5代表body长度为5，A代表发送人为A，abcd为发送的具体内容
*/

class Message
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };

	Message(): _body_length(0)
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

	/*
		* @return 返回总长度
	*/
	std::size_t length() const
	{
		return header_length + _body_length;
	}

	/*
		* @return 返回真正的数据的第一个字符
	*/
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

	/*
	* @brief 将数据的前四个字符转换为size_t类型并赋值给了_body_length
	*/
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

	/*
		* @brief 将_body_length弄到数据的前四个字符上面
		* 如果_body_length小于1000，那么前面将补上空格
	*/
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