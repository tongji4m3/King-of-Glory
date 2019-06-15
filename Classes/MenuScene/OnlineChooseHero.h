#ifndef _ONLINE_CHOOSE_HERO_
#define _ONLINE_CHOOSE_HERO_

#include "cocos2d.h"
#include"../NetWork/Client.h"

class OnlineChooseHero : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(Client* client);

	virtual bool init();

	CREATE_FUNC(OnlineChooseHero);

	void chooseHero1(Ref* pSender);
	void chooseHero2(Ref* pSender);
	void chooseHero3(Ref* pSender);
};

#endif 
