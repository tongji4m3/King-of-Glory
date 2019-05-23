#include"MainScene.h"
#include"SimpleAudioEngine.h"

#include<string>

//播放音乐要用的
using namespace CocosDenshion;
USING_NS_CC;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

extern int id;//获取之前选的英雄类型  妲己，亚瑟，后羿

bool  MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//循环播放音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/back.mp3", true);

	//载入地图
	map_layer = Layer::create();
	this->addChild(map_layer);
	map = Sprite::create("map/map.png");
	map_layer->addChild(map);
	map_layer->setAnchorPoint(Vec2(0, 0));
	map->setAnchorPoint(Vec2(0, 0));

	//玩家
	my_hero = HeroSprite::create(id);
	addChild(my_hero);
	my_hero->setPosition(Vec2(960, 540));
	my_hero->setAnchorPoint(Vec2(0.5, 0.5));

	//添加防御塔
	addTower();

	//载入时间
	time_label = Label::create("00:00", "Arial", 50);
	time_label->setColor(Color3B(200, 150, 156));
	this->addChild(time_label);
	time_label->setPosition(Vec2(960, 960));

	//英雄死亡时的复活时间 死亡时才可见
	int time_resurrection = 10;
	std::string resurrection("Resurrection time ");
	resurrection += std::to_string(time_resurrection);
	time_resurrection_label = Label::create(resurrection, "Arial", 50);
	time_resurrection_label->setColor(Color3B(200, 150, 156));
	this->addChild(time_resurrection_label);
	time_resurrection_label->setPosition(Vec2(960, 540));
	time_resurrection_label->setVisible(false);

	//胜利场景 胜利时可见
	win = Sprite::create("win_or_lose/win.png");
	this->addChild(win);
	win->setPosition(Vec2(960, 540));
	win->setVisible(false);

	//失败场景 失败时可见
	fail = Sprite::create("win_or_lose/lose.png");
	this->addChild(fail);
	fail->setPosition(Vec2(960, 540));
	fail->setVisible(false);

	//单点触摸事件监听器
	auto m = EventListenerTouchOneByOne::create();
	//[=]按值传递捕获当前上下文的所有变量
	m->onTouchBegan = 
	[=](Touch* touch, Event* e)
	{
		//convertToNodeSpace 将Vec2转换为节点（本地）空间坐标
		auto touch_position = convertToNodeSpace(touch->getLocation());//鼠标点的位置
		auto my_hero_position = convertToNodeSpace(my_hero->getPosition());//英雄原来的位置
		//终止之前的操作
		my_hero->stopAllActions();
		map_layer->stopAllActions();

		/*
		英雄移动与图层的移动，保证操控的英雄始终在屏幕x轴的中心
		具体操作是英雄负责上下移动，图层负责左右移动
		MoveBy是从当前坐标点移动给定的坐标点这么多的距离
		可以通过speed控制移动速度
		参数：
			duration：动作持续时间。
			deltaPosition：偏移坐标。这里会给出一对儿(x, y)坐标值
		*/
		int speed = 800;
		my_hero->runAction(MoveBy::create(touch_position.distance(e->getCurrentTarget()->getPosition()) / speed, Vec2(0, touch_position.y - my_hero_position.y))); 
		map_layer->runAction(MoveBy::create(touch_position.distance(e->getCurrentTarget()->getPosition()) / speed, Vec2((my_hero_position.x - touch_position.x), 0)));
		
		return false;
	};

	map_layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m, my_hero);

	return true;
}

//添加防御塔
void MainScene::addTower()
{
	for (int i = 0; i < 4; i++)
	{
		towers.push_back(TowerSprite::create(i));
		map_layer->addChild(towers[i]);
		towers[i]->setZOrder(100);
	}
	towers[0]->setPosition(Vec2(1550, 650));
	towers[1]->setPosition(Vec2(2550, 650));
	towers[2]->setPosition(Vec2(3750, 650));
	towers[3]->setPosition(Vec2(4950, 650));

	//防御塔范围的一个圈
	auto blue_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(blue_tower_bg);
	blue_tower_bg->setPosition(Vec2(2550, 650));
	blue_tower_bg->setColor(Color3B(100, 100, 100));

	auto red_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(red_tower_bg);
	red_tower_bg->setPosition(Vec2(3750, 650));
	red_tower_bg->setColor(Color3B(100, 100, 100));

}

