#ifndef __CHECK_RESULT_H__
#define __CHECK_RESULT_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"

using namespace CocosDenshion;

class CheckResult :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	SimpleAudioEngine* Sound = SimpleAudioEngine::getInstance();

	virtual bool init();

	CREATE_FUNC(CheckResult);
};

#endif