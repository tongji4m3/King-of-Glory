#include"Client.h"
#include"Server.h"
#include"Message.h"
#include<string>



int main()
{
	std::thread t2(&Server::server);
	t2.detach();


	Client client;
	//std::string s = "CBA";
	//client.send_message(s);

	while (true)
	{

	}
	return 0;
}
