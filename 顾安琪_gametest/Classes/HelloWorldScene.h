#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	//////////////////////////////
	//转场对战界面  
	void onPushSceneMap(Ref* pSender);
	//转场房间界面  
	void onPushSceneRoom(Ref* pSender);
	//////////////////////////////

	//选择器回调
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	//手动实现“static create()”方法
    CREATE_FUNC(HelloWorld);

};
#endif // __HELLOWORLD_SCENE_H__
