#ifndef _ROOM_SCENE_H_
#define _ROOM_SCENE_H_

#include "cocos2d.h"

class RoomScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//转场对战界面  
	void RoomToGame(Ref* pSender);

	CREATE_FUNC(RoomScene);
};

#endif