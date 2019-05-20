#include "MapScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace std;

Scene* MapScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MapScene::create();
	scene->addChild(layer);
	return scene;
}
//视角追随参数
int kTagTileMap;
//建立英雄精灵
CCSprite* hero1;
bool  MapScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
		
	//载入tiled地图
	CCParallaxNode* voidNode = CCParallaxNode::create();

	////////////////////////////////////////////////////////////

	CCTMXTiledMap* map= CCTMXTiledMap::create("map.tmx");
	addChild(map,0,kTagTileMap);

	hero1 = CCSprite::create("hero1.png"); 
	map->addChild(hero1, map->getChildrenCount());
	hero1->retain();
	int mapWidth = map->getMapSize().width * map->getTileSize().width;

	///////////////////////////
	/*
	//载入对象层
	CCTMXObjectGroup* objgroup = map->objectGroupNamed("Object1");

	auto objs = objgroup->getObjects();

	ValueMap dict;

	ValueVector::iterator iter;

	//加载玩家坐标

	Value objPointMap;
	// 遍历所有的点
	for each (objPointMap in objs)
	{
		ValueMap objPoint = objPointMap.asValueMap();
		float x = objPoint.at("x").asFloat();
		float y = objPoint.at("y").asFloat();
		String type = objPoint.at("type").asString();


		if (type._string == "playerBorn")
		{
			hero1->setPosition(x, y);
			//创建角色出生地  
		}
	}

	///////////////////////////////////
	*/
	hero1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	hero1->setAnchorPoint(Vec2(0.5, 0.5));
	//英雄移动
	CCActionInterval* move = CCMoveBy::create(2, ccpMult(ccp(300, 0), 1 / CC_CONTENT_SCALE_FACTOR()));
	//CCActionInterval * back = move-> reverse(); 
	CCFiniteTimeAction * seq = CCSequence::create(move, NULL);
	hero1->runAction(CCRepeatForever::create((CCActionInterval*)seq)); 
	scheduleUpdate(); 
	////////////////////////////////////////////////////////////

	//退出
	auto quit = MenuItemFont::create("quit", CC_CALLBACK_1(MapScene::backToMainScene, this));
	quit->setName("quit");

	//create menu, it's an autorelease object
	auto menu = Menu::create(quit, nullptr);
	menu->alignItemsVertically();
	this->addChild(menu);
	menu->setPositionY(visibleSize.height / 2 + 400);
	menu->setPositionX(visibleSize.width / 2 + 300);

	return true;

}

//返回HelloWorld主界面
void MapScene::backToMainScene(Ref * pSender) {
	auto scene = Scene::create();
	scene->addChild(HelloWorld::create());
	Director::getInstance()->replaceScene(TransitionFlipX::create(.5, scene));
}
//视角跟随主角
void MapScene::update(float delta)
{
	char mch[256];
	CCPoint herop = hero1 ->getPosition(); 
	sprintf(mch, " x ->% 0.2f---y-> % .2f ", herop.x, herop.y);
	CCTMXTiledMap* map = (CCTMXTiledMap*)getChildByTag(kTagTileMap);
	CCSize s = map ->getContentSize(); 
	int mapWidth = map ->getMapSize().width* map -> getTileSize().width; 
	float deltax = herop.x - mapWidth / 2 / CC_CONTENT_SCALE_FACTOR(); 
	float deltay = herop.y;
	CCSize winSize = CCDirector::sharedDirector() ->getWinSize(); 
	//视角追随
	map -> setPosition(ccp(-s.width / 2 + winSize.width / 2 - deltax, -30));
 }
