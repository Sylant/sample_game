#include "../../lib/gplib.h"
#include "movableObject.h"
#include "shot.h"
#include "Player.h"
#include "Enemy.h"
#include "bomb.h"
#include "effect.h"
#include "Stage.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;

	extern float p_posY;      //プレイヤーからの値を受ける

	shot::shot(const string& objectName) :
		MovableObject(DrawObjf(
			objectName, Vec3f::zero(), Vec3f::one(), objectName, Vec3f::zero(), Vec3f::zero(), Color(0xFFFFFFFF), 0))
	{

	}

	void shot::render()
	{
		MovableObject::render();
	}

	void shot::update()
	{

	}

	//プレイヤーのオブジェクトを取得
	shared_ptr<Player> shot::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//プレイヤーの値を返し
		return player;
	}
	//エネミーーのオブジェクトを取得
	shared_ptr<Enemy> shot::getBossPrt()
	{
		auto object = getObjectFromRoot({ "boss" });
		shared_ptr<game::Enemy> boss = ci_ext::weak_to_shared<game::Enemy>(object);
		//エネミーの値を返し
		return boss;
	}
	//ステージのオブジェクトを取得
	shared_ptr<Stage> shot::getStagePrt()
	{
		auto object = getObjectFromRoot({ "stage" });
		shared_ptr<game::Stage> stage_ = ci_ext::weak_to_shared<game::Stage>(object);
		//ステージの値を返し
		return stage_;
	}


	//プレイヤーショット　一般攻撃
	//posX,posY       初期発射位置
	//targetX,targetY マウス位置
	//cnt	          連射数
	//angle           角度
	p_shot1::p_shot1(const string& objectName, float posX, float posY, float targetX, float targetY, float cnt, float angle) :
		shot(objectName)
	{
		//画像読み込み
		graph::Draw_LoadObject("p_bullet", "res/gra/Player/bullet.png");
		//画像のサイズを取得
		p_bullet_w = graph::Draw_GetImageWidth("p_bullet");
		p_bullet_h = graph::Draw_GetImageHeight("p_bullet");
		//初期化
		setPos(posX, posY, 0.1f);
		setScale(Vec3f(0.7f, 0.7f, 0.f));
		setSrcSize(Vec3f((float)p_bullet_w, (float)p_bullet_h, 0.f));
		resname_ = "p_bullet";

		//移動速度
		shot_speed = 4.f + cnt;
		//発射角度
		degree_ = math::DegreeOfPoints2(pos().x(), pos().y(), targetX, targetY);
		//弾の速度
		velocity_.x(math::ROUND_X(-degree_ + angle, shot_speed, 0.f));
		velocity_.y(math::ROUND_Y(-degree_ + angle, shot_speed, 0.f));
		//当たり判定
		SetRect(&offenseRect, 0, 0, (int)((float)p_bullet_w * 0.7f), (int)((float)p_bullet_h * 0.7f));  //攻め判定範
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
	}
	//画面外なら消し
	void p_shot1::ps_out()
	{
		if (pos().x() > system::WINW || pos().x() < 0) kill();
		if (pos().y() < 0) kill();
		if (pos().y() > p_posY + 63 / 2 )
		{
			insertAsChild(new bomb7("bullet_exlode", pos().x(), pos().y(), 2.f));
			kill();
		}
	}
	//攻撃判定更新
	void p_shot1::ps_setoffense()
	{
		//角度により判定更新
		//画像のサイズを大きさ設定により変更
		if (degree_ < 45.f || degree_ > 135.f)
			SetRect(&offenseRect, (int)(ix() - (p_bullet_w / 2) * 0.7f), (int)(iy() - (p_bullet_h / 2) * 0.7f),
				(int)(ix() + (p_bullet_w / 2) * 0.7f), (int)(iy() + (p_bullet_h / 2) * 0.7f));
		else if ((degree_ > 45.f && degree_ < 135.f))
			SetRect(&offenseRect, (int)(ix() - (p_bullet_h / 2) * 0.7f), (int)(iy() - (p_bullet_w / 2) * 0.7f),
				(int)(ix() + (p_bullet_h / 2) * 0.7f), (int)(iy() + (p_bullet_w / 2) * 0.7f));
	}
	//当たった時呼ぶ
	void p_shot1::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		insertToParent(new bomb7("bullet_exlode", pos().x(), pos().y(), 2.f));
		kill();
	}

	void p_shot1::render()
	{
		MovableObject::render();
		//判定チェックRECT
//		gplib::graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void p_shot1::update()
	{
		MovableObject::update();
		ps_out();
		ps_setoffense();
	}

	//エネミーショット１　爆弾落下
	e_shot1::e_shot1(const std::string& objectName, float posX, float posY , float speed) :
		shot(objectName)
	{
		//画像読み込み
		graph::Draw_LoadObject("e_missiles", "res/gra/Enemy/e_missile.png");
		//画像のサイズを取得
		shot1_w = graph::Draw_GetImageWidth("e_missiles") / 4;
		shot1_h = graph::Draw_GetImageHeight("e_missiles");
		//初期化
		setPos(Vec3f(posX, posY, 0.25f));
		setScale(Vec3f(1.f, 1.f, 0.f));
		setSrcSize(Vec3f((float)shot1_w, (float)shot1_h, 0.f));
		resname_ = "e_missiles";
		//移動速度
		velocity_.y(speed);
		//アニメ
		charabase::SetAnim(anim_, 4, 0.1f);
		//攻め判定
		SetRect(&offenseRect, 0, 0, shot1_w, shot1_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
        //攻撃判定を一回だけに
		s1_show = true;
	}
	//攻撃判定更新
	void e_shot1::s1_setoffense()
	{
		SetRect(&offenseRect, ix() - shot1_w / 2, iy() - shot1_h / 2, ix() + shot1_w / 2, iy() + shot1_h / 2);
	}
	//地面当たったら呼ぶ
	void e_shot1::s1_out()
	{
		if (y() > p_posY + 63 / 2)
		{
			insertToParent(new bomb1("e_explode", pos().x(), pos().y() - 105.f / 1.15f, 1.8f));
			kill();
		}
	}
	//アニメ
	void e_shot1::s1_anim()
	{
		int t[] = { 0, 1, 2, 3, 4 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
	}
	//プレイヤーに当たったら呼ぶ
	void e_shot1::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		auto p = getPlayerPrt();
		if (s1_show)
		{
			p->p_receiving_atk(10);
			s1_show = false;
		}
	}

	void e_shot1::render()
	{
		MovableObject::render();
		//判定チェックRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void e_shot1::update()
	{
		MovableObject::update();
		s1_anim();
		s1_out();
		s1_setoffense();
	}

	//エネミーショット２ ミニエネミー
	e_shot2::e_shot2(const string& objectName, float posX, float posY, int num) :
		shot(objectName)
	{
		//画像読み込み
		graph::Draw_LoadObject("mini", "res/gra/Enemy/miniboss.png", 0x00000000);
		//画像のサイズを取得
		shot2_w = graph::Draw_GetImageWidth("mini") / 23;
		shot2_h = graph::Draw_GetImageHeight("mini") / 3;
		setScale(Vec3f(1.2f, 1.2f, 0.f));
		setSrcSize(Vec3f((float)shot2_w, (float)shot2_h, 0.f));
		
		resname_ = "mini";
		//アニメ設定
		charabase::SetAnim(anim_, 22, 0.15f);
		animMax_ = 6;
		//受け判定
		SetRect(&defenseRect, 0, 0, (int)((float)shot2_w * 1.2f), (int)((float)shot2_h * 1.2f));
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//順番に落下する
		posX_ = posX + (num * 35.f);
		//初期速度
		s2_speed = 1.f + (float)num / 3.f;
		//初期位置をセットする
		setPos(Vec3f(posX_, posY, 0.35f));
		//初期化
		state_ = s2_mode::NORMAL; //初期は一般状態
		
		s2_speed_ = 2.f;          //落下した移動するスピード
		atk_count = 3;            //攻撃カウント数
		down_ = true;             //落下判定
		atk_show = true;          //攻撃状態の判定
		bomb2_show = true;        //爆発アニメを一回だけに呼ぶ
		hp_ = 6;                  //hp
		num_ = num;               //呼びかけたショット２の数
		frame_ = 0;               //加算するフレーム数初期化
	}
	//ミニエネミー移動
	void e_shot2::s1_move()
	{
		//ボスのオブジェクトをとる
		auto b = getBossPrt();
		//プレイヤーのオブジェクトをとる
		auto p = getPlayerPrt();
		//状態変化によりモード変化
		switch (state_)
		{
		//一般モード
		//落下するだけ呼ぶ
		case s2_mode::NORMAL:
		{
			velocity_.y(s2_speed);
			if (y() > 300.f && down_)
			{
				state_ = s2_mode::MOVE;
				down_ = false;
			}
		}
		    break;
		//移動モード
		//プレイヤーの移動により移動する
		case s2_mode::MOVE:
		{
			velocity_.y(0.f);
			if (pos().x() > p->pos().x())
			{
				velocity_.x(-s2_speed_);
				//位置はプレイヤーより120.fの距離で停止して、攻撃する
				if (pos().x() + (float)((num_)* 30.f) < p->pos().x() + 120.f)
				{
					velocity_.x(0.f);
				}
			}
			else if (pos().x() < p->pos().x())
			{
				velocity_.x(s2_speed_);
				if (pos().x() - (float)((num_)* 30.f) > p->pos().x() - 120.f)
				{
					velocity_.x(0.f);
				}
			}
			//確率で攻撃モードに変化
			if (math::GetRandom(0, 100) == 0)
			{
				state_ = s2_mode::ATTACK;
			}
		}
		    break;
		//攻撃モード
		case s2_mode::ATTACK:
		{
			velocity_.x(0.f);

			if (atk_show && atk_count > 0)
			{
				insertAsChild(new e_shot2_atk("lightning", p->pos().x()));
				atk_show = false;
				//カウントで計数、攻撃は3回まで
				--atk_count;
			}

			//連続攻撃不可に設定、一度移動モードに戻る
			if (!atk_show && (++frame_ / 60 == math::GetRandom(2, 5)))
			{
				state_ = s2_mode::MOVE;
				atk_show = true;
				frame_ = 0;
			}
			//3回攻撃が終わると、再び落下する
			if (atk_show && atk_count == 0)
			{
				velocity_.x(0.f);
				velocity_.y(s2_speed_);
				//追撃モードに変化
				if (y() > p_posY - 10.f)
				{
					state_ = s2_mode::SUICIDE;
				}
			}
		}
		break;
		//追撃モード
		case s2_mode::SUICIDE:
		{
			velocity_.x(s2_speed_ * 1.5f);
			velocity_.y(0.f);
			//加速度に設定
			++frame_;
			if (frame_ < math::GetRandom(120, 180))
			{
				if (pos().x() > p->pos().x())
				{
					velocity_.x(-s2_speed_ * velocity_.x());
				}
				else if (pos().x() < p->pos().x())
				{
					velocity_.x(s2_speed_ * velocity_.x());
				}
			}
			else
			{
				//カウントは足りれば、消滅モードに
				state_ = s2_mode::DIED;
			}
		}
		break;
		//消滅モード
		//爆発前にhpは0以下になる時に消滅
		case s2_mode::DIED:
		{
			velocity_.x(0.f);
			velocity_.y(0.f);
			//消滅する時に存在する数を‐１
			b->s2_count(1);
			if (hp_ > 0)
			{
				//一回に制御しないとバグが発生する可能性がある
				if (bomb2_show)
				{
					insertToParent(new bomb2("bomb2", pos().x(), p_posY - 32.f / 1.3f, 2.f));
					bomb2_show = false;
				}
			}
		}
		    break;
		}
	}
	//アニメ
	void e_shot2::s1_anim()
	{
		//モード別けアニメ
		int t[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
		int t2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9 };
		int t3[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };
		int t4[] = { 0, 1, 2, 3, 4, 5, 6 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (state_ != s2_mode::NORMAL && state_ != s2_mode::ATTACK && state_ != s2_mode::DIED)
		{
			setSrc(Vec3f((float)t2[anim_.no], 0.f, 0.f));
		}
		else if (state_ == s2_mode::ATTACK)
		{
			setSrc(Vec3f((float)t3[anim_.no], 1.f, 0.f));
		}
		else if (state_ == s2_mode::DIED)
		{
			setSrc(Vec3f((float)t4[anim_.no], 2.f, 0.f));
			if (anim_.no >= animMax_)
			{
				if (hp_ <= 0) insertToParent(new bomb6("all_explode", pos().x(), pos().y(), math::GetRandom(1.f, 1.5f)));
				kill();
			}
		}
	}
	//受け状態更新
	void e_shot2::s1_setdefense()
	{
		//受け状態更新
		SetRect(&defenseRect, (int)(ix() - (float)shot2_w * 1.2f / 2.f), (int)(iy() - (float)shot2_h * 1.2f / 2.f), 
			(int)(ix() + (float)shot2_w * 1.2f / 2.f), (int)(iy() + (float)shot2_h * 1.2f / 2.f));
	}
	
	void e_shot2::render()
	{
		MovableObject::render();
        //判定チェックRECT
//		graph::Draw_CkRect(defenseRect, ARGB(255, 255, 0, 0));
	}
	//やられた時に呼ぶ
	void e_shot2::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		--hp_;
		if (hp_ < 0)
		{
			state_ = s2_mode::DIED;
			auto s = getStagePrt();
			s->e_kill(1);
		}
	}

	void e_shot2::update()
	{
		MovableObject::update();

		s1_anim();
		s1_move();
		s1_setdefense();
	}

	//ミニ攻撃　雷
	e_shot2_atk::e_shot2_atk(const string& objectName, float posX) :
		shot(objectName)
	{
		//画像読み込み
		graph::Draw_LoadObject("lightning", "res/gra/Enemy/lightning.png");
		//画像のサイズを取得
		s2_atk_w = graph::Draw_GetImageWidth("lightning") / 9;
		s2_atk_h = graph::Draw_GetImageHeight("lightning");
		//初期化
		setScale(Vec3f(1.2f, 1.42f, 0.f));
		setSrcSize(Vec3f((float)s2_atk_w, (float)s2_atk_h, 0.f));
		setPos(Vec3f(posX, p_posY - 100.f, 0.4f));
		resname_ = "lightning";
		//アニメ
		charabase::SetAnim(anim_, 10, 0.2f);
		animMax_ = 9;
		//受け判定なし
		SetRect(&offenseRect, 0, 0, (int)((float)s2_atk_w * 1.2f), (int)((float)s2_atk_h * 1.42f));
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//攻撃判定を一回だけに
		s2_atk_show = true;
		//SE
	    se::DSound_Play("light");
		//攻撃力初期化
		atk_ = 0.f;
		//プレイヤーの最大ｈｐにより攻撃倍数
		atk_times = 0.03f;

	}

	void e_shot2_atk::render()
	{
		MovableObject::render();
		//判定チェックRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}
	//攻めた時に呼ぶ
	void e_shot2_atk::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//プレイヤーのオブジェクトを取得
		auto p = getPlayerPrt();
		//プレイヤーのｈｐ設定により攻撃力変化する
		atk_ = p->playerMaxHp() * atk_times;
		//プレイヤーのｈｐ設定により攻撃力変化しない
//		atk_ = atk_ * atk_using;
		if (s2_atk_show)
		{
			p->p_receiving_atk(atk_);
			s2_atk_show = false;
		}
	}
	//アニメ
	void e_shot2_atk::s2a_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	//攻め判定更新
	void e_shot2_atk::s2a_setoffense()
	{
		//攻撃判定更新
		SetRect(&offenseRect, (int)(ix() - (float)s2_atk_w * 1.2f / 2.f), (int)(iy() - (float)s2_atk_h * 1.42f / 2.f), 
			(int)(ix() + (float)s2_atk_w * 1.2f / 2.f), (int)(iy() + (float)(s2_atk_h - 9) * 1.42f / 2.f));
	}

	void e_shot2_atk::update()
	{
		MovableObject::update();
		s2a_anim();
		s2a_setoffense();
	}
	//エネミーショット３　地雷
	e_shot3::e_shot3(const string& objectName) :
		shot(objectName)
	{
		//画像の読み込み
		graph::Draw_LoadObject("landmine", "res/gra/Enemy/landmine.png");
		//画像のサイズを取得
		shot3_w = graph::Draw_GetImageWidth("landmine") / 2;
		shot3_h = graph::Draw_GetImageHeight("landmine");
		//初期化
		setScale(Vec3f(1.0f, 1.0f, 0.f));
		setSrcSize(Vec3f((float)shot3_w, (float)shot3_h, 0.f));
		resname_ = "landmine";
		//アニメ
		charabase::SetAnim(anim_, 2, 0.1f);
		animMax_ = 1;
		//攻めと受け判定
		SetRect(&offenseRect, 0, 0, shot3_w, shot3_h);
		SetRect(&defenseRect, 0, 0, shot3_w, shot3_h);

		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//初期はランダムで出現
		posX_ = math::GetRandom(0.f + (float)shot3_h / 2.f, (float)system::WINW - (float)(shot3_w - 15) / 2.f);
		setPos(Vec3f(posX_, p_posY + (float)shot3_h, 0.2f));
		//hp
		hp_ = 3;
		//加算するフレーム数初期化
		frame_ = 0;
	}
	//アニメ
	void e_shot3::s3_anim()
	{
		int t[] = { 0,1 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		//カウント数で自動爆発に設定
		if (++frame_ / 60 == math::GetRandom(8, 12))
		{
			kill();
			insertToParent(new bomb4("bomb4", pos().x(), p_posY - (float)shot3_h / 2, 1.0f));
		}
	}
	//サイズより小さめに判定範囲を設定する
	//受け判定更新
	void e_shot3::s3_setdefense()
	{
		SetRect(&defenseRect, ix() - shot3_w - 5 / 2, iy() - shot3_h - 5 / 2, ix() + shot3_w - 5 / 2, iy() + shot3_h - 5 / 2);
	}
	//攻め判定更新
	void e_shot3::s3_setoffense()
	{
		SetRect(&offenseRect, ix() - shot3_w - 15 / 2, iy() - shot3_h - 5 / 2, ix() + shot3_w - 15 / 2, iy() + shot3_h - 5 / 2);
	}
	//判定発生したら、爆発アニメに
	void e_shot3::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		kill();
		insertToParent(new bomb4("bomb4", pos().x(), p_posY - 30.f / 2, 1.0f));
	}
	//やられた時に呼ぶ
	void e_shot3::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		--hp_;
		if (hp_ < 0)
		{
			insertToParent(new bomb6("bomb6", pos().x(), y() - 40.f, math::GetRandom(1.5f, 2.f)));
			auto s = getStagePrt();
			s->e_kill(1);
			kill();
		}
	}

	void e_shot3::render()
	{
		MovableObject::render();
		//判定チェックRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void e_shot3::update()
	{
		MovableObject::update();
		s3_anim();
		s3_setdefense();
		s3_setoffense();
	}
	//エネミーショット４　UFO
	e_shot4::e_shot4(const string& objectName) :
		shot(objectName)
	{
		//画像の読み込み
		graph::Draw_LoadObject("ufo", "res/gra/Enemy/miniufo.png");
		//画像のサイズを取得
		shot4_w = graph::Draw_GetImageWidth("ufo") / 12;
		shot4_h = graph::Draw_GetImageHeight("ufo");
		setScale(Vec3f(1.3f, 1.3f, 0.f));
		setSrcSize(Vec3f((float)shot4_w, (float)shot4_h, 0.f));
		resname_ = "ufo";
		//アニメ設定
		charabase::SetAnim(anim_, 12, 0.4f);
		animMax_ = 11;
		//受け判定範囲設定
		SetRect(&defenseRect, 0, 0, shot4_w, shot4_h);
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//初期は一般状態に
		state_ = s4_mode::NORMAL;
		//確率で出現
		posX_ = (float)math::GetRandom((int)((float)shot4_w * 1.3f / 2.f), system::WINW - (int)((float)shot4_w * 1.3f / 2.f));
		//初期位置
		setPos(Vec3f(posX_, 220.f, 0.2f));
		//初期化
		appear_show = false;              //現れるかどうか
		atk_show = true;                  //攻撃判定
		frame_ = 0;                       //消えるまでにフレーム数初期化
		alpha_ = 0;                       //初期は透明に
		setColor(alpha_, 255, 255, 255);  //ARGB
		hp_ = 6;                          //hp
	}
	//UFO移動
	void e_shot4::s4_move()
	{
		switch (state_)
		{
		case s4_mode::NORMAL:
		{
			alpha_ += 5;
			if (alpha_ >= 255)
			{
				alpha_ = 255;
				//透明度255足りたら、攻撃状態に
				state_ = s4_mode::ATTACK;
			}
		}
		    break;
		case s4_mode::ATTACK:
		{
			//攻撃一回だけ
			if (atk_show)
			{
				insertAsChild(new e_shot4_atk("ufo_atk", pos().x(), pos().y()));
				atk_show = false;
			}
			//フレーム数で次へ
			if (++frame_ / 60 == math::GetRandom(2, 4))
			{
				appear_show = true;
			}
			//透明へ
			if (appear_show)
			{
				alpha_ -= 5;
				if (alpha_ <= 0)
				{
					kill();
				}
			}
		}
		    break;
		}
	}
	//アニメ
	void e_shot4::s4_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
	}
	//受け判定更新
	void e_shot4::s4_setdefense()
	{
		SetRect(&defenseRect, (int)(ix() - (float)shot4_w * 1.3f / 2.f), (int)(iy() - (float)shot4_h * 1.3f / 2.f),
			(int)(ix() + (float)shot4_w * 1.3f / 2.f), (int)(iy() + (float)shot4_h * 1.3f / 2.f));
	}

	void e_shot4::render()
	{
		MovableObject::render();
        //判定チェックRECT
//		graph::Draw_CkRect(defenseRect, ARGB(255, 0, 0, 255));
	}
	//やられた時に呼ぶ
	void e_shot4::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		--hp_;
		if (hp_ < 0)
		{
			insertToParent(new bomb6("all_explode", pos().x(), pos().y(), math::GetRandom(2.f, 2.5f)));
			auto s = getStagePrt();
			s->e_kill(1);
			kill();
		}
	}

	void e_shot4::update()
	{
		MovableObject::update();
		//一度ARGBをセットする
		setColor(alpha_, 255, 255, 255);

		s4_anim();
		s4_move();
		s4_setdefense();
	}
	//UFO攻撃
	e_shot4_atk::e_shot4_atk(const string& objectName, float posX, float posY) :
		shot(objectName)
	{
		//画像の読み込み
		graph::Draw_LoadObject("ufo_atk", "res/gra/Enemy/miniufo_attack.png");
		//画像のサイズを取得
		s4_atk_w = graph::Draw_GetImageWidth("ufo_atk") / 9;
		s4_atk_h = graph::Draw_GetImageHeight("ufo_atk");
		//初期化
		setScale(Vec3f(1.3f, 1.3f, 0.f));
		setSrcSize(Vec3f((float)s4_atk_w, (float)s4_atk_h, 0.f));
		resname_ = "ufo_atk";
		//アニメ設定
		charabase::SetAnim(anim_, 9, 0.3f);
		animMax_ = 8;
		//初期位置
		setPos(Vec3f(posX, posY + 75.f, 0.25f));
	}
	//アニメ
	void e_shot4_atk::s4a_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		//7フレームのアニメに弾を発射する
		else if (anim_.no == 7)
		{
			insertToParent(new bomb5("bomb5", pos().x() - 1.f, pos().y() + 50.f, 1.3f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}

	void e_shot4_atk::render()
	{
		MovableObject::render();
	}

	void e_shot4_atk::update()
	{
		MovableObject::update();
		s4a_anim();
	}

	//エネミーショット５　ヘリコプター
	e_shot5::e_shot5(const string& objectName) :
		shot(objectName)
	{
		//画像の読み込み
		graph::Draw_LoadObject("heli", "res/gra/Enemy/helicopter.png");
		//画像のサイズを取得
		shot5_w = graph::Draw_GetImageWidth("heli") / 4;
		shot5_h = graph::Draw_GetImageHeight("heli");
		//初期化
		setScale(Vec3f(1.f, 1.f, 0.f));
		setSrcSize(Vec3f((float)shot5_w, (float)shot5_h, 0.f));
		resname_ = "heli";
		//アニメ設定
		charabase::SetAnim(anim_, 4, 0.8f);
		//本体は攻撃判定なし
		SetRect(&defenseRect, 0, 0, shot5_w, shot5_h);
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//確率で出現　初期位置は画面外
		posX_ = (float)math::GetRandom(shot5_w / 2, system::WINW - shot5_w / 2);
		setPos(Vec3f(posX_, -10.f, 0.2f));
		//初期モード
		state_ = s5_mode::NORMAL;
		//初期化
		atk_show = false;       //攻撃状態
		frame_ = 0;             //落下と上昇のフレーム数
		moving_speed = 0.6f;    //移動速度
		hp_ = 6;                //hp
	}
	//ヘリコプター移動
	void e_shot5::s5_move()
	{
		velocity_.y(moving_speed);

		switch (state_)
		{
		case s5_mode::NORMAL:
		{
			//設定位置に着いたら、攻撃モードに
			if (y() > 50.f)
			{
				state_ = s5_mode::ATTACK;
				atk_show = true;
			}
		}
		break;
		case s5_mode::ATTACK:
		{
			velocity_.y(0.f);
			//カウント数で攻撃をセットする
			frame_++;
			//60フレーム一回攻撃
			if (frame_ == 120 || frame_ == 180)
			{
				atk_show = true;
			}
			//攻撃3回　8発 x 3
			if (atk_show)
			{
				se::DSound_Play("e_bullet");
				for (int i = 0;i <= 6;++i)
				{
					insertAsChild(new e_shot5_atk("e_bullet", pos().x(), pos().y(), i * 15.f));
				}
				atk_show = false;
			}
			//420フレーム後、上昇する
			else if (frame_ >= 480)
			{
				velocity_.y(-1.f);
				if (y() < 0.f)
				{
					kill();
				}
			}
		}
		    break;
		}
	}
	//アニメ
	void e_shot5::s5_anim()
	{
		int t[] = { 0, 1, 2, 3, 4 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
	}
	//受け判定更新
	void e_shot5::s5_setdefense()
	{
		SetRect(&defenseRect, (int)(ix() - shot5_w / 2), (int)(iy() - shot5_h / 2),
			(int)(ix() + shot5_w / 2), (int)(iy() + shot5_h / 2));
	}

	void e_shot5::render()
	{
		MovableObject::render();
        //判定チェックRECT
//		graph::Draw_CkRect(defenseRect, ARGB(255, 0, 0, 255));
	}
	//やられた時に呼ぶ
	void e_shot5::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		--hp_;
		if (hp_ < 0 )
		{
			insertToParent(new bomb6("bomb6", pos().x(), pos().y(), math::GetRandom(2.f, 2.5f)));
			auto s = getStagePrt();
			s->e_kill(1);
			kill();
		}
	}

	void e_shot5::update()
	{
		MovableObject::update();

		s5_move();
		s5_anim();
		s5_setdefense();
	}
	//ヘリコプター攻撃
	e_shot5_atk::e_shot5_atk(const string& objectName, float posX, float posY, float angle) :
		shot(objectName)
	{
		//画像の読み込み
		graph::Draw_LoadObject("e_bullet", "res/gra/Enemy/bullet.png");
		//画像のサイズを取得
		s5_atk_w = graph::Draw_GetImageWidth("e_bullet");
		s5_atk_h = graph::Draw_GetImageHeight("e_bullet");
		//初期化
		setScale(Vec3f(1.f, 1.f, 0.f));
		setSrcSize(Vec3f((float)s5_atk_w, (float)s5_atk_h, 0.f));
		resname_ = "e_bullet";
		setPos(Vec3f(posX, posY + 30.f, 0.25f));
		//攻撃パタン変換できる

/*      //プレイヤーの位置に攻撃する
		//プレイヤーの位置に対する角度を求める
		targetX_ = math::DegreeOfPoints2(x(), y(), player_posX, player_posY);
		targetY_ = -math::DegreeOfPoints2(x(), y(), plyaer_posX, player_posY);
        //角度に対してのX,Yを求める
		velocity_.x(math::ROUND_X(targetX_, speed_, 0.f));
		velocity_.y(math::ROUND_Y(targetY_, speed_, 0.f));
*/
		//移動速度
		atk_speed = 3.f;
		//弾の移動修正
		velocity_.x(math::ROUND_X(angle + 45.f, atk_speed, 0.f));
		velocity_.y(math::ROUND_Y(angle + 45.f, atk_speed, 0.f));
		//攻撃判定
		SetRect(&offenseRect, 0, 0, s5_atk_w, s5_atk_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//攻撃力初期化
		atk_ = 0.f;
		//プレイヤーの最大ｈｐにより攻撃倍数
		atk_times = 0.02f;
		//攻撃判定を一回だけに
		atk_show = true;
	}

	void e_shot5_atk::s5a_out()
	{
		//地面にぶつかったら、爆発アニめへ
		if (y() > p_posY + 28.f)
		{
			insertToParent(new bomb7("bullet_exlode", pos().x(), pos().y(), 2.f));
			kill();
		}
		//画面外なら消し
		if (pos().x() > system::WINW - s5_atk_w / 2) kill();
		else if (pos().x() < 0 + s5_atk_w / 2) kill();
	}
	//攻め判定更新
	void e_shot5_atk::s5a_setoffense()
	{
		SetRect(&offenseRect, (int)(ix() - s5_atk_w / 2), (int)(iy() - s5_atk_h / 2), 
			(int)(ix() + s5_atk_w / 2), (int)(iy() + s5_atk_h / 2));
	}

	void e_shot5_atk::render()
	{
		MovableObject::render();
		//判定チェックRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 0, 0, 255));
	}
	//判定はtrueだったら 爆発アニメへ
	void e_shot5_atk::offenseHit(std::weak_ptr<ci_ext::Object>&)
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
			insertToParent(new bomb7("bomb7", pos().x(), pos().y(), 2.f));
			atk_show = false;
		}
		kill();
	}

	void e_shot5_atk::update()
	{
		MovableObject::update();
		s5a_out();
		s5a_setoffense();
	}
}