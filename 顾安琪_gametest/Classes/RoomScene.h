#ifndef __ROOM_SCENE_H__
#define __ROOM_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"

using namespace CocosDenshion;

class RoomScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	SimpleAudioEngine* Sound = SimpleAudioEngine::getInstance();

	virtual bool init();

	void backToMainScene(cocos2d::Ref* pSender);

	//转场对战界面  
	void RoomToMap(Ref* pSender);

	CREATE_FUNC(RoomScene);
};

#endif