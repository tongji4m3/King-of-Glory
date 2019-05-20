#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__
#include "cocos2d.h"
#include"SimpleAudioEngine.h"

using namespace CocosDenshion;

class MapScene :public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	void MapScene::update(float delta);

	SimpleAudioEngine* Sound = SimpleAudioEngine::getInstance();

	virtual bool init();

	void backToMainScene(cocos2d::Ref* pSender);

	CREATE_FUNC(MapScene);
};

#endif