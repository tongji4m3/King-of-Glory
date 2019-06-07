#ifndef _CHOOSE_HERO_SCENE_
#define _CHOOSE_HERO_SCENE_

#include "cocos2d.h"

class ChooseHeroScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(ChooseHeroScene);

	void chooseHero1(Ref* pSender);
	void chooseHero2(Ref* pSender);
	void chooseHero3(Ref* pSender);
};

#endif // __choose_hero_SCENE_H__
