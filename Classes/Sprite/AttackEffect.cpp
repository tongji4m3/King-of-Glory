#include"AttackEffect.h"
#include<string>
#include<vector>

AttackEffect* AttackEffect::create(int id)
{
	AttackEffect* effect = new AttackEffect();
	effect->init(id);// 妲己 亚瑟 后羿 小兵 防御塔 的攻击特效
	effect->autorelease();
	return effect;
}
void AttackEffect::init(int id)
{
	//亚瑟攻击范围很小，就用妲己的攻击特效占位
	const std::vector<std::string> effects = 
	{
		"atk_way/daji_atk.png",
		"atk_way/daji_atk.png",
		"atk_way/houyi_atk.png",
		"atk_way/soldier_atk.png",
		"atk_way/tower_atk.png"
	};

	Sprite::initWithFile(effects[id]);
}


