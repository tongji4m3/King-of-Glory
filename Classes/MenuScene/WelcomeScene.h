#ifndef  _WELCOME_SCENE_H_
#define  _WELCOME_SCENE_H_

#include "cocos2d.h"

class WelcomeScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//转场选择模式场景  
	void WelcomeToMode(Ref* pSender);

	CREATE_FUNC(WelcomeScene);
};

#endif