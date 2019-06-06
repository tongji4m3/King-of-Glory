#include"ShopMenu.h"

USING_NS_CC;

bool  ShopMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//添加进入商店游戏按钮  
	auto item1 = MenuItemImage::create("shop/shop.png","shop/shop.png",CC_CALLBACK_1(ShopMenu::OpenShop, this));
	auto menu = Menu::create(item1, NULL);
	menu->alignItemsVerticallyWithPadding(50);
	//按钮放置在屏幕左中间
	item1->setPosition(Vec2(50, origin.y - 300));
	this->addChild(menu, 1);

	return true;
}

void ShopMenu::OpenShop(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto sprite = Sprite::create("shop/shopopened.png");
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sprite, 2);
}

