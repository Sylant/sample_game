#include "../../lib/gplib.h"
#include "movableObject.h"
#include "bomb.h"
#include "Player.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;

	bomb::bomb(const string& objectName) :
		MovableObject(DrawObjf(
		objectName, Vec3f::zero(), Vec3f::one(), objectName, Vec3f::zero(), Vec3f::zero(), Color(0xFFFFFFFF), 0))
	{

	}

	//プレイヤーのオブジェクトを取得
	shared_ptr<Player> bomb::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//プレイヤーの値を返し
		return player;
	}
	//デストラクタ
	bomb::~bomb()
	{

	}
	void bomb::render()
	{
		MovableObject::render();
	}

	void bomb::update()
	{

	}
	//エネミー一般攻撃爆発アニメ
	//bomb1_x , bomb1_y  アニメ位置
	//bomb1_s    画像拡大縮小の倍率
	bomb1::bomb1(const std::string& objectName, const float& bomb1_x, const float& bomb1_y, const float& bomb1_s) :
		bomb(objectName)
	{
		//画像のサイズを取得
		bomb1_w = graph::Draw_GetImageWidth("e_explosion") / 29;
		bomb1_h = graph::Draw_GetImageHeight("e_explosion");
		//初期化
		setScale(Vec3f(bomb1_s, bomb1_s, 0.f));
		setPos(Vec3f(bomb1_x, bomb1_y, 0.1f));
		setSrcSize(Vec3f((float)bomb1_w, (float)bomb1_h, 0.f));
		resname_ = "e_explosion";
		//アニメ
		//SetAnim（obejctname，繰り返しアニメ枚数，アニメスピード）
		charabase::SetAnim(anim_, 30, 0.4f);
		animMax_ = 29;
		//SE
		se::DSound_Play("e_explosion");
	}
	//デストラクタ
	bomb1::~bomb1()
	{

	}
	//アニメ
	void bomb1::b1_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}

	void bomb1::render()
	{
		MovableObject::render();
	}

	void bomb1::update()
	{
		MovableObject::update();
		b1_anim();
	}
	//ミニエネミー自爆アニメ
	//攻撃判定はここで取る
	//bomb2_x , bomb2_y  アニメ位置
	//bomb2_s    画像拡大縮小の倍率
	bomb2::bomb2(const std::string& objectName, const float& bomb2_x, const float& bomb2_y, const float& bomb2_s) :
		bomb(objectName)
	{
		//画像のサイズを取得
		bomb2_w = graph::Draw_GetImageWidth("bomb2") / 28;
		bomb2_h = graph::Draw_GetImageHeight("bomb2");
		//初期化
		setScale(Vec3f(bomb2_s, bomb2_s, 0.f));
		setPos(Vec3f(bomb2_x, bomb2_y, 0.1f));
		setSrcSize(Vec3f((float)bomb2_w, (float)bomb2_h, 0.f));
		resname_ = "bomb2";
		//アニメ
		charabase::SetAnim(anim_, 29, 0.45f);
		animMax_ = 28;
		//攻撃判定を一回だけに抑える
		atk_show = true;
		//受け判定なし
		SetRect(&offenseRect, 0, 0, bomb2_w, bomb2_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//SE
		se::DSound_Play("kill");
		//攻撃力初期化
		atk_ = 0.f;
		//プレイヤーの最大ｈｐにより攻撃倍数
		atk_times = 0.03f;
	}
	//アニメ
	void bomb2::b2_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	//デストラクタ
	bomb2::~bomb2()
	{

	}
	//攻め判定更新
	void bomb2::b2_setoffense()
	{
		//描画は２倍広大しているので元のサイズを使います
		SetRect(&offenseRect, (int)(ix() - bomb2_w), (int)(iy() - bomb2_h),
			(int)(ix() + bomb2_w), (int)(iy() + bomb2_h));
	}

	//攻撃判定
	void bomb2::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//プレイヤーのオブジェクトを取得
		auto p = getPlayerPrt();
		//プレイヤーのｈｐ設定により攻撃力変化する
		atk_ = p->playerMaxHp() * atk_times;
		//プレイヤーのｈｐ設定により攻撃力変化しない
//		atk_ = atk_ * atk_using;
		if (atk_show)
		{
			p->p_receiving_atk(atk_);
			atk_show = false;
		}
	}

	void bomb2::render()
	{
		MovableObject::render();
		//判定チェックRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void bomb2::update()
	{
		MovableObject::update();
		b2_anim();
		b2_setoffense();
	}
	//道具ゲットアニメ
	//bomb3_x , bomb3_y  アニメ位置
	//bomb3_s    画像拡大縮小の倍率
	bomb3::bomb3(const std::string& objectName, const float& bomb3_x, const float& bomb3_y, const float& bomb3_s) :
		bomb(objectName)
	{
		//画像のサイズを取得
		bomb3_w = graph::Draw_GetImageWidth("get_tool") / 6;
		bomb3_h = graph::Draw_GetImageHeight("get_tool");
		//初期化
		setScale(Vec3f(bomb3_s, bomb3_s, 0.f));
		setPos(Vec3f(bomb3_x, bomb3_y, 0.4f));
		setSrcSize(Vec3f((float)bomb3_w, (float)bomb3_h, 0.f));
		resname_ = "get_tool";
		//アニメ
		charabase::SetAnim(anim_, 7, 0.3f);
		animMax_ = 6;
		//SE
		se::DSound_Play("tool_get");
	}
	//デストラクタ
	bomb3::~bomb3()
	{

	}
	//アニメ
	void bomb3::b3_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}

	void bomb3::render()
	{
		MovableObject::render();
	}

	void bomb3::update()
	{
		MovableObject::update();
		b3_anim();
	}
	//地雷爆発アニメ
	//攻め判定はここで行い
	//bomb4_x , bomb4_y  アニメ位置
	//bomb4_s    画像拡大縮小の倍率
	bomb4::bomb4(const std::string& objectName, const float& bomb4_x, const float& bomb4_y, const float& bomb4_s) :
		bomb(objectName)
	{
		//画像のサイズを取得
		bomb4_w = graph::Draw_GetImageWidth("ld_explosion") / 15;
		bomb4_h = graph::Draw_GetImageHeight("ld_explosion");
		//初期化
		setScale(Vec3f(bomb4_s, bomb4_s, 0.f));
		setPos(Vec3f(bomb4_x, bomb4_y, 0.1f));
		setSrcSize(Vec3f((float)bomb4_w, (float)bomb4_h, 0.f));
		resname_ = "ld_explosion";
		//アニメ
		charabase::SetAnim(anim_, 16, 0.3f);
		animMax_ = 15;
		//攻撃判定を一回だけに抑える
		atk_show = true;
		//攻め判定あり
		SetRect(&offenseRect, 0, 0, bomb4_w, bomb4_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//SE
		se::DSound_Play("landmine");
		//攻撃力初期化
		atk_ = 0.f;
		//プレイヤーの最大ｈｐにより攻撃倍数
		atk_times = 0.03f;
	}
	//デストラクタ
	bomb4::~bomb4()
	{

	}
	//アニメ
	void bomb4::b4_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ,15 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	//攻撃判定更新
	void bomb4::b4_setoffense()
	{
		//判定範囲を抑えるために*0.8fに
		SetRect(&offenseRect, (int)(ix() - bomb4_w * 0.8f / 2), (int)(iy() - bomb4_h / 2),
			(int)(ix() + bomb4_w * 0.8f / 2), (int)(iy() + bomb4_h / 2));
	}
	//攻撃判定
	void bomb4::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//プレイヤーのオブジェクトを取得
		auto p = getPlayerPrt();
		//プレイヤーのｈｐ設定により攻撃力変化する
		atk_ = p->playerMaxHp() * atk_times;
		//プレイヤーのｈｐ設定により攻撃力変化しない
//		atk_ = atk_ * atk_using;
		if (atk_show)
		{
			p->p_receiving_atk(atk_);
			atk_show = false;
		}
	}

	void bomb4::render()
	{
		MovableObject::render();
	}

	void bomb4::update()
	{
		MovableObject::update();
		b4_anim();
		b4_setoffense();
	}
	//UFO攻撃アニメ
	//攻撃判定ここで行い
	//bomb5_x , bomb5_y  アニメ位置
	//bomb5_s    画像拡大縮小の倍率
	bomb5::bomb5(const std::string& objectName, const float& bomb5_x, const float& bomb5_y, const float& bomb5_s) :
		bomb(objectName)
	{
		//画像のサイズを取得
		bomb5_w = graph::Draw_GetImageWidth("ufo_explosion") / 7;
		bomb5_h = graph::Draw_GetImageHeight("ufo_explosion");
		//初期化
		setScale(Vec3f(bomb5_s, bomb5_s, 0.f));
		setPos(Vec3f(bomb5_x, bomb5_y, 0.1f));
		setSrcSize(Vec3f((float)bomb5_w, (float)bomb5_h, 0.f));
		resname_ = "ufo_explosion";
		//アニメ
		charabase::SetAnim(anim_, 8, 0.17f);
		animMax_ = 7;
		//攻め判定あり
		SetRect(&offenseRect, 0, 0, bomb5_w, bomb5_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//発射速度
		velocity_.y(6.f);
		//攻撃判定を一回だけに抑える
		atk_show = true;
		//SE
		se::DSound_Play("beam");
		//攻撃力初期化
		atk_ = 0.f;
		//プレイヤーの最大ｈｐにより攻撃倍数
		atk_times = 0.04f;
	}
	//アニメ
	void bomb5::b5_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	//デストラクタ
	bomb5::~bomb5()
	{

	}
	//攻め判定更新
	void bomb5::b5_setoffense()
	{
		SetRect(&offenseRect, (int)(ix() - bomb5_w / 2), (int)(iy() - bomb5_h / 4), (int)(ix() + bomb5_w / 2), (int)(iy() + bomb5_h / 2));
	}
	
	void bomb5::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//プレイヤーのオブジェクトを取得
		auto p = getPlayerPrt();
		//プレイヤーのｈｐ設定により攻撃力変化する
		atk_ = p->playerMaxHp() * atk_times;
		//プレイヤーのｈｐ設定により攻撃力変化しない
//		atk_ = atk_ * atk_using;
		if (atk_show)
		{
			p->p_receiving_atk(atk_);
			atk_show = false;
		}
	}

	void bomb5::render()
	{
		MovableObject::render();
		//判定チェックrect
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void bomb5::update()
	{
		MovableObject::update();
		b5_anim();
		b5_setoffense();
	}
	//一般爆発アニメ
	//bomb6_x , bomb6_y  アニメ位置
	//bomb6_s    画像拡大縮小の倍率
	bomb6::bomb6(const std::string& objectName, const float& bomb6_x, const float& bomb6_y, const float& bomb6_s) :
		bomb(objectName)
	{
		//画像のサイズを取得
		bomb6_w = graph::Draw_GetImageWidth("all_explosion") / 28;
		bomb6_h = graph::Draw_GetImageHeight("all_explosion");
		//初期化
		setScale(Vec3f(bomb6_s, bomb6_s, 0.f));
		setPos(Vec3f(bomb6_x, bomb6_y, 0.1f));
		setSrcSize(Vec3f((float)bomb6_w, (float)bomb6_h, 0.f));
		resname_ = "all_explosion";
		//アニメ
		charabase::SetAnim(anim_, 29, 0.4f);
		animMax_ = 28;
		//SE
		se::DSound_Play("all_explosion");
	}
	//デストラクタ
	bomb6::~bomb6()
	{

	}
	//アニメ
	void bomb6::b6_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}

	void bomb6::render()
	{
		MovableObject::render();
	}

	void bomb6::update()
	{
		MovableObject::update();
		b6_anim();
	}
	//ヘリコプター攻撃爆発アニメ
	bomb7::bomb7(const std::string& objectName, const float& bomb7_x, const float& bomb7_y, const float& bomb7_s) :
		bomb(objectName)
	{
		//画像のサイズを取得
		bomb7_w = graph::Draw_GetImageWidth("bullet_explosion") / 5;
		bomb7_h = graph::Draw_GetImageHeight("bullet_explosion");
		//初期化
		setScale(Vec3f(bomb7_s, bomb7_s, 0.f));
		setPos(Vec3f(bomb7_x, bomb7_y, 0.1f));
		setSrcSize(Vec3f((float)bomb7_w, (float)bomb7_h, 0.f));
		resname_ = "bullet_explosion";
		//アニメ
		charabase::SetAnim(anim_, 6, 0.4f);
		animMax_ = 5;
	}
	//デストラクタ
	bomb7::~bomb7()
	{

	}
	//アニメ
	void bomb7::b7_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	void bomb7::render()
	{
		MovableObject::render();
	}

	void bomb7::update()
	{
		MovableObject::update();
		b7_anim();
	}

	//ヘリコプター２投げ爆弾アニメ
	bomb8::bomb8(const std::string& objectName, const float& bomb8_x, const float& bomb8_y, const float& bomb8_s) :
		bomb(objectName)
	{
		//画像のサイズを取得
		bomb8_w = graph::Draw_GetImageWidth("tk_explosion") / 7;
		bomb8_h = graph::Draw_GetImageHeight("tk_explosion");
		//初期化
		setScale(Vec3f(bomb8_s, bomb8_s, 0.f));
		setPos(Vec3f(bomb8_x, bomb8_y, 0.1f));
		setSrcSize(Vec3f((float)bomb8_w, (float)bomb8_h, 0.f));
		resname_ = "tk_explosion";
		//アニメ
		charabase::SetAnim(anim_, 8, 0.4f);
		animMax_ = 7;
	}
	//デストラクタ
	bomb8::~bomb8()
	{

	}
	//アニメ
	void bomb8::b8_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8};

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no == 4)
		{
//			insertAsChild(new e_shot6_atk());
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	void bomb8::render()
	{
		MovableObject::render();
	}

	void bomb8::update()
	{
		MovableObject::update();
		b8_anim();
	}
}