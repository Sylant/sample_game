#include "../../lib/gplib.h"
#include "movableObject.h"
#include "tool.h"
#include "bomb.h"
#include "player.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;

	//プレイヤーのオブジェクトを取得
	shared_ptr<Player> Tool::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//プレイヤーの値を返し
		return player;
	}

	Tool::Tool(const string& objectName) :
		MovableObject(DrawObjf(
			objectName, Vec3f::zero(), Vec3f::one(), objectName, Vec3f::zero(), Vec3f::zero(), Color(0xFFFFFFFF), 0))
	{

	}
	//デストラクタ
	Tool::~Tool()
	{

	}
	void Tool::render()
	{
		MovableObject::render();
	}

	void Tool::update()
	{

	}
	
	//プレイヤーhp回復アテイム
	Tool1::Tool1(const string& objectName) :
		Tool(objectName)
	{
		//画像のサイズを取得
		tool1_w = graph::Draw_GetImageWidth("ammo");
		tool1_h = graph::Draw_GetImageHeight("ammo");
		//初期化
		resname_ = "ammo";
		setScale(Vec3f(1.f, 1.f, 0.f));
		setSrcSize(Vec3f((float)tool1_w, (float)tool1_h, 0.f));
		//攻め判定として設定
		SetRect(&offenseRect, 0, 0, tool1_w, tool1_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//X、Y座標
		posY_ = (float)system::WINH - (tool1_h * 2 + tool1_h / 7);
		posX_ = (float)math::GetRandom(tool1_w / 2, system::WINW);
		//初期位置
		setPos(Vec3f(posX_, posY_, 0.35f));

		p_hp = 0;          //プレイヤー損したｈｐ
		healing_ = 0.f;    //回復値初期化
		heal_using = 0.1f; //プレイヤーの最大ｈｐにより回復倍数
	}
	//デストラクタ
	Tool1::~Tool1()
	{

	}
	//判定更新
	void Tool1::t1_setoffense()
	{
		SetRect(&offenseRect, (int)(ix() - tool1_w / 2), (int)(iy() - tool1_h / 2), (int)(ix() + tool1_w / 2), (int)(iy() + tool1_h / 2));
	}
	//当たったら呼ぶ
	void Tool1::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//プレイヤーのオブジェクトを取得
		auto p = getPlayerPrt();

		//プレイヤーのｈｐ設定により回復値変化する
		healing_ = p->playerMaxHp() * heal_using;
		
		//hp描画は枠を超えないように
		if (p->playerHp() >= p->playerHpRight())
		{
			p->p_receiving_atk(0);
		}
		else if (p->playerHp() < p->playerHpRight())
		{
			//hpの描画位置の座標は逆なので、＋と-も逆にする
			p_hp = p->playerHpRight() - (int)p->playerHp();
			//損したｈｐは回復値より上回り->回復値で回復する
			if (p_hp >= (int)healing_)
				p->p_receiving_atk(-healing_);
			//損したｈｐは回復値より下回り->損したｈｐだけを回復する
			else if (p_hp < (int)healing_)
				p->p_receiving_atk(-(float)p_hp);
		}

		insertToParent(new bomb3("get_tool", pos().x(), pos().y() - (float)tool1_h, 1.5f));
		kill();
	}

	void Tool1::render()
	{
		MovableObject::render();
	}

	void Tool1::update()
	{
		MovableObject::update();
		t1_setoffense();
	}
}