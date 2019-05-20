#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MapScene.h"
#include "RoomScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// 初始化函数
bool HelloWorld::init()
{
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 2.添加一个“关闭”图标以退出进度。它是一个自动释放对象。
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//////////////////////
	//在视图的正中间竖向位置添加人机对战+开房间按钮  
	auto item2 = MenuItemImage::create("man_vs_man.png", "man_vs_man_selected.png", CC_CALLBACK_1(HelloWorld::onPushSceneRoom, this));
	auto item1 = MenuItemImage::create("man_vs_computer.png", "man_vs_computer_selected.png", CC_CALLBACK_1(HelloWorld::onPushSceneMap, this));

	auto menu_oper = Menu::create(item1, item2, NULL);
	menu_oper->alignItemsVerticallyWithPadding(50);

	this->addChild(menu_oper, 1);
	//////////////////////

    //3.create and initialize a label
    auto label = Label::createWithTTF("Test Go!", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height / 2));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // 添加主界面背景
    auto sprite = Sprite::create("loading_background.png");
    if (sprite == nullptr)
    {
        problemLoading("'loading_background.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

    return true;
}
/**
 * 进入玩家对战房间RoomScene的方法
 */
void HelloWorld::onPushSceneRoom(Ref* pSender) {
	auto scene = Scene::create();
	scene->addChild(RoomScene::create());
	Director::getInstance()->replaceScene(TransitionFlipX::create(.5, scene));
}

/**
 *进入MapScene的方法
*/
void HelloWorld::onPushSceneMap(Ref* pSender) {
	auto scene = Scene::create();
	scene->addChild(MapScene::create());
	Director::getInstance()->replaceScene(TransitionFlipX::create(.5, scene));
}

/**
 * 关闭视窗的方法
 */
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

}


