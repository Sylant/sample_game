#include "../../lib/gplib.h"
#include "movableObject.h"
#include "player.h"
#include "shot.h"
#include "bomb.h"
#include "tool.h"
#include "stage.h"
#include "Enemy.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;
	using namespace input;

	//playerのY座標を決めて、取っておき
	extern float p_posY = (float)system::WINH - (float)(85 + 82 / 4);

    //ステージのオブジェクトを取得
	shared_ptr<Stage> Player::getStagePrt()
	{
		auto object = getObjectFromRoot({ "stage" });
		shared_ptr<game::Stage> stage = ci_ext::weak_to_shared<game::Stage>(object);
		//ステージの値を返し
		return stage;
	}
	//エネミーのオブジェクトを取得
	shared_ptr<Enemy> Player::getEnemyPrt()
	{
		auto object = getObjectFromRoot({ "boss" });
		shared_ptr<game::Enemy> boss = ci_ext::weak_to_shared<game::Enemy>(object);
		//エネミーの値を返し
		return boss;
	}

	Player::Player(const string& objectName, int tips) :
		MovableObject(
			DrawObjf(objectName, Vec3f::one(), Vec3f::one(), "player", Vec3f::zero(), Vec3f(85.f, 63.f, 0.f), Color(0xFFFFFFFF), 0))
	{
		//画像読み込み
		graph::Draw_LoadObject("player", "res/gra/Player/player.png");
		//画像のサイズを取得
		player_w = graph::Draw_GetImageWidth("player") / 6;
		player_h = graph::Draw_GetImageHeight("player");
		//プレイヤー初期位置
		setPos((float)system::WINW / 2, p_posY, 0.3f);
		//アニメ関数　/最大枚数　/アニメスピード
		charabase::SetAnim(anim_, 6, 0.15f);
		//プレイヤー本体は攻め判定なし、受け判定だけ
		SetRect(&defenseRect, 0, 0, player_w, player_h);                                                  //受け判定範囲
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());     //受け判定初期化
		
		bullet_ = 2;             //左攻撃の発射弾数
		init_moving_speed = 3.f; //移動速度初期値
		moving_speed = 3.f;      //移動速度
		Max_hp = 400.f;          //初期ｈｐ
		Max_mp = 400.f;          //初期ｍｐ
		hp_ = 0.f;               //損したｈｐ
		mp_ = 0.f;               //損したｍｐ
		mp_count = 0;            //加速度を制御するためのカウント数
		hp_times = 0.008f;       //最大ｈｐによりｈｐ消耗倍数
		mp_times = 0.02f;        //最大ｍｐによりｍｐ消耗倍数
		mp_healing = 0.006f;     //最大ｍｐによりｍｐ自動回復倍数
		shadow_count = 0;        //影のカウント数
		result_ = true;          //ゲームオーバーする時に制御
		def_show = false;        //やられたら、画面変化の判定
		hp_draw_top = 630;       //ｈｐ描画の上、下、左、右の値を設定
		hp_draw_bottom = 615;
		hp_draw_left = 440;
		hp_draw_right = 840;
		mp_draw_top = 630;       //ｍｐ描画の上、下、左、右の値を設定
		mp_draw_bottom = 615;
		mp_draw_left = 20;
		mp_draw_right = 420;
		tips_ = tips;            //隠し技コマンド　値渡し
	}

	//プレイヤー移動
	void Player::p_move(int key)
	{
		switch (key)
		{
		case moveDir::NEUTRAL:
			velocity_.x(0.f);
			break;
		case moveDir::LEFT:
			velocity_.x(-moving_speed);
			break;
		case moveDir::RIGHT:
			velocity_.x(moving_speed);
			break;
		}
	}
	//アニメ
	void Player::p_anim()
	{
		//アニメフレーム
		int t[] = { 0, 1, 2, 3, 4, 5, 6 };
		int t2[] = { 5, 4, 3, 2, 1, 0 };

		if (charabase::StepAnim(anim_))
		{
			if (key == moveDir::NEUTRAL || key == moveDir::LEFT)
			{
				setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
			}
			else if (key == moveDir::RIGHT)
			{
				setSrc(Vec3f((float)t2[anim_.no], 0.f, 0.f));
			}
		}
	}
	//プレイヤースキル
	void Player::p_skill()
	{
		//エネミーのプロジェクトを取得する
		auto b = getEnemyPrt();

		//一般攻撃
		if (CheckPush(input::KEY_MOUSE_LBTN))
		{
			for (int i = 0;i < bullet_ + tips_;++i)
			{
				insertAsChild(new p_shot1("p_bullet", x(), y() - (float)player_h / 2.f,
					(float)mousePos_.x, (float)mousePos_.y, (float)i, 0.f));
			}
			//隠し技の連射と初期連射のＳＥも区別させる
			if (bullet_ + tips_ > bullet_)
				se::DSound_Play("p_bullet2");
			else
				se::DSound_Play("p_bullet");
		}

		//加速度移動
		//ｍｐを自動回復に
		if (mp_draw_change < (float)mp_draw_right)
			mp_ -= Max_mp * mp_healing;

		//気力を制御する
		if (CheckPress(KEY_BTN1) && (CheckPress(KEY_BTN0) || CheckPress(KEY_BTN2)))
		{
			if (mp_draw_change <= (float)mp_draw_right)
			{
				if (mp_draw_change - Max_mp * mp_times >= (float)mp_draw_left)
				{
					//スピードを倍に
					moving_speed = moving_speed * 1.2f;
					//毎回ｍｐ消耗する
					mp_ += Max_mp * mp_times;
					//影を表す
					p_shadow();
					//カウント数で制御
					++mp_count;
					se::DSound_Play("telesport");
					//速度を初期値に戻る
					if (mp_count > 15)
					{
						mp_count = 0;
						moving_speed = init_moving_speed;
					}
				}
				else if (mp_draw_change - Max_mp * mp_times < (float)mp_draw_left)
				{
					moving_speed = init_moving_speed;
				}
			}
			else if (mp_draw_change > (float)mp_draw_right)
			{
				mp_draw_change = (float)mp_draw_right;
			}
		}
		else
			moving_speed = init_moving_speed;

		//散射弾　hpを減らす副作用あり
		if (hp_draw_change > (float)hp_draw_left + Max_hp * hp_times)
		{
			if (CheckPush(input::KEY_MOUSE_RBTN))
			{
				for (int i = 0;i < 6;++i)
				{
					insertAsChild(new p_shot1("p_bullet", x(), y() - (float)player_h / 2.f,
						(float)mousePos_.x, (float)mousePos_.y, 0.f, i * 10.f - 30.f));
				}
				//ゲームクリアしたら、ｈｐを確報ためｈｐ減らすのを制御する
				if (b->diedshow() == false) hp_ += Max_hp * hp_times;
				se::DSound_Play("scatter");
			}
		}
		//射撃できなくなる時に
		else if (hp_draw_change <= (float)hp_draw_left + Max_hp * hp_times && CheckPush(KEY_MOUSE_RBTN))
			se::DSound_Play("shell");
	}
	//マウス座標を取得
	void Player::p_getmousepos()
	{
		mousePos_ = input::GetMousePosition();
	}
	//キーによる方向
	void Player::p_key()
	{
		//キーにより方向変更
		key = 0;
		if (CheckPress(KEY_BTN0))
		{
			key += moveDir::LEFT;
		}
		else if (CheckPress(KEY_BTN2))
		{
			key += moveDir::RIGHT;
		}
		else key += moveDir::NEUTRAL;
	}
	//画像左右反転
	//現在の画像に使わなくてもいい
	void Player::p_backrun()
	{
//		if (key == moveDir::LEFT && scale().x() != -1)
//		{
//			setScale(Vec3f(scale().x()*-1, scale().y(), scale().z()));
//		}
//		if (key == moveDir::RIGHT && scale().x() != 1)
//		{
//			setScale(Vec3f(scale().x()*-1, scale().y(), scale().z()));
//		}
	}

	//Ｍａｘｈｐ値を返す
	float Player::playerMaxHp()
	{
		return Max_hp;
	}
	//hpの値を返す
	float Player::playerHp()
	{
		return hp_draw_change;
	}
	//ｈｐ描画の最大値＝描画する最大ｈｐを戻り値に
	int Player::playerHpRight()
	{
		return hp_draw_right;
	}

	//ｈｐ制御
	void Player::p_receiving_atk(float hp)
	{
		//エネミーのプロジェクトを取得する
		auto b = getEnemyPrt();
		
		if (hp_draw_change > (float)hp_draw_left && hp_draw_change <= (float)hp_draw_right)
		{
			//ゲームクリアとゲームオーバーを先後で発生しないためｈｐで制御する
			if(b->diedshow() == false) hp_ += hp;
		}
		if (hp_draw_change <= hp_draw_left)
		{
			hp_draw_change = (float)hp_draw_left;
			//一回だけ呼ぶことに制御
			if (result_)
			{
				p_result();
				result_ = false;
			}
			//プレイヤーオブジェクト停止する
			pause();
		}
	}

	//ステージへ関数を呼ぶ
	void Player::p_result()
	{
		auto s = getStagePrt();
		s->result(2);
	}

	//受け判定更新
	void Player::p_setdefense()
	{
		SetRect(&defenseRect, (int)(ix() - player_w / 2), (int)(iy() - player_h / 2), (int)(ix() + player_w / 2), (int)(iy() + player_h / 2));
	}
	//画面外出ないように
	void Player::p_out()
	{
		if (pos().x() + velocity_.x() < player_w / 2 || pos().x() + velocity_.x() > (system::WINW - player_w / 2))
			velocity_.x(0.f);
	}
	//hpの描画
	void Player::p_drawHp()
	{
		//変化するｈｐ
		hp_draw_change = (float)hp_draw_right - (float)(hp_draw_right - hp_draw_left) / Max_hp * hp_;
		//枠を超えないように制御
		if(hp_draw_change <= (float)hp_draw_left) hp_draw_change = (float)hp_draw_left;
		if (hp_draw_change > (float)hp_draw_right) hp_draw_change = (float)hp_draw_right;
		//填充描画
		graph::Draw_Box(hp_draw_left, hp_draw_top, (int)hp_draw_change, hp_draw_bottom, 0.05f,
			ARGB(255, 255, 255, 0), ARGB(255, 255, 255, 0), 1, 1);
		//枠描画
		graph::Draw_Box(hp_draw_left, hp_draw_top, hp_draw_right, hp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 0), ARGB(255, 255, 0, 0), 1, 0);
	}
	//mpの描画
	void Player::p_drawMp()
	{
		//変化するｍｐ
		mp_draw_change = (float)mp_draw_right - (float)(mp_draw_right - mp_draw_left) / Max_mp * mp_;
		//枠を超えないように制御
		if (mp_draw_change <= (float)mp_draw_left) mp_draw_change = (float)mp_draw_left;
		if (mp_draw_change > (float)mp_draw_right) mp_draw_change = (float)mp_draw_right;
		//填充描画
		graph::Draw_Box( mp_draw_left, mp_draw_top, (int)mp_draw_change, mp_draw_bottom, 0.05f,
			ARGB(255, 0, 255, 255), ARGB(255, 0, 255, 255), 1, 1);
		//枠描画
		graph::Draw_Box( mp_draw_left, mp_draw_top, mp_draw_right, mp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 0), ARGB(255, 255, 0, 0), 1, 0);
	}

	//やられた時呼ぶ
	void Player::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		def_show = true;
	}
	//攻撃を受けた時に画面赤くする
	void Player::p_defense_atkshow()
	{
		if (def_show)
		{
			graph::Draw_Box(0, 0, system::WINW, system::WINH, 0.45f, ARGB(50, 255, 0, 0), ARGB(50, 255, 0, 0), 1, 1);
			def_show = false;
		}
	}
	//透明度は10以下どうかをチェックする
	bool alpha_(MovableObject& move) { return move.color().a() <= 10; }
	//加速度移動する時に影を生成
	void Player::p_shadow()
	{
		//毎2フレームで影を生成する
		++shadow_count;
		if (shadow_count > 3)
		{
			shadow_count = 0;
			//優先度だけ変化させ、他のは初期値を使い
			shadow_.push_back(MovableObject(
				DrawObjf("shadow", Vec3f(pos().x(), pos().y(), 0.4f), scale(), resname(), src(), srcSize(), color(), 0)));
		}
	}
	//影の描画
	void Player::p_draw_shadow()
	{
		for (auto& s : shadow_)
		{
			s.render();
		}
	}
	//影の更新
	void Player::p_step_shadow()
	{
		//各々の影に対して、透明度を減らしていく
		for (auto& s : shadow_) {
			s.setColor(s.color().a() - 10, s.color().r(), s.color().g(), s.color().b());
		}
		//削除する
		auto it = remove_if(shadow_.begin(), shadow_.end(), alpha_);
		shadow_.erase(it, shadow_.end());
	}

	void Player::render()
	{
		MovableObject::render();
		p_drawHp();
		p_drawMp();
		p_defense_atkshow();
		p_draw_shadow();
	}

	void Player::update()
	{
		MovableObject::update();
		p_anim();
		p_backrun();
		p_getmousepos();
		p_key();
		p_skill();
		p_move(key);
		p_out();
		p_setdefense();
		p_step_shadow();
		p_receiving_atk(hp);
	}
}