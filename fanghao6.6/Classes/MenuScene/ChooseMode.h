#ifndef _CHOOSE_MODE_H_
#define _CHOOSE_MODE_H_

#include "cocos2d.h"

class ChooseMode : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//转场选择英雄界面  
	void onPushSceneChooseHero(Ref* pSender);
	//转场创建房间界面  
	void onPushSceneCreateRoom(Ref* pSender);
	//转场加入房间界面
	void onPushSceneJoinRoom(Ref* pSender);

	CREATE_FUNC(ChooseMode);

};
#endif 
