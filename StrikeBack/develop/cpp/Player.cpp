#include "../../lib/gplib.h"
#include "movableObject.h"
#include "player.h"
#include "shot.h"
#include "bomb.h"
#include "tool.h"
#include "stage.h"
#include "Enemy.h"
#include <sstream>

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;
	using namespace input;

	//playerのY座標を決めて、取っておき
	extern float p_posY = (float)system::WINH - (float)(85 + 82 / 4);

	int is = 0;

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

	Player::Player(const string& objectName,const int& tips) :
		MovableObject(
			DrawObjf(objectName, Vec3f::one(), Vec3f::one(), "player", Vec3f::zero(), Vec3f(85.f, 63.f, 0.f), Color(0xFFFFFFFF), 0))
	{
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
		
		num_1 = 0;                   //レベルカウント初期化
		num_2 = 0;
		level_ = 0;                  //level関数初期化
		is_level = 0;
		lv_count_w = 0;
		lv_count_h = 0;
		exp_draw_change = 0.f;       //初期化
		hp_draw_change = 0.f;
		mp_draw_change = 0.f;
		bullet_ = 1;                 //左攻撃の発射弾数
		bullets_ = 3;                //右攻撃の発射弾数
		angle_ = 0.f;                //右攻撃角度修正
		init_moving_speed = 3.f;     //移動速度初期値
		moving_speed = 3.f;          //移動速度
		Max_hp = 300.f;              //初期ｈｐ
		Max_mp = 100.f;              //初期ｍｐ
		Max_exp = 20.f;              //初期レベルアップに必要なＥｘｐ
		hp_ = 0.f;                   //損したｈｐ
		mp_ = 0.f;                   //損したｍｐ
		exp_ = 0.f;                  //得られたＥｘｐ
		mp_count = 0;                //加速度を制御するためのカウント数
		hp_times = 0.03f;            //スキル２最大ｈｐによりｈｐ消耗倍数
		mp_times = 0.035f;           //スキル３最大ｍｐによりｍｐ消耗倍数
		mp_healing = 0.002f;         //最大ｍｐによりｍｐ自動回復倍数
		shadow_count = 0;            //影のカウント数
		result_ = true;              //ゲームオーバーする時に制御
		def_show = false;            //やられたら、画面変化の判定
		kill_2 = false;              //レベルでスキルを制御する
		kill_3 = false;
		self_atk = false;            //スキル２のｈｐ消耗につれのＳＥ制御
		hp_draw_top = 615;           //ｈｐ描画の上、下、左、右の値を設定
		hp_draw_bottom = 635;
		hp_draw_left = 440;
		hp_draw_right = 840;
		mp_draw_top = 630;           //ｍｐ描画の上、下、左、右の値を設定
		mp_draw_bottom = 640;
		mp_draw_left = 20;
		mp_draw_right = 420;
		exp_draw_top = 620;          //ｌｖ描画の上、下、左、右の値を設定
		exp_draw_bottom = 625;
		exp_draw_left = 20;
		exp_draw_right = 420;
		tips_ = tips;                //隠し技コマンド　値渡し
	}
	//デストラクタ
	Player::~Player()
	{

	}
	//プレイヤー移動
	void Player::p_move(const int& key)
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

		//スキル１　一般攻撃
		if (CheckPush(input::KEY_MOUSE_LBTN))
		{
			for (int i = 0;i < bullet_ + tips_;++i)
			{
				insertAsChild(new p_shot1("p_bullet", x(), y() - (float)player_h / 2.f,
					(float)mousePos_.x, (float)mousePos_.y, (float)i, 0.f));
			}
			//隠し技の連射と初期連射のＳＥも区別させる
			if (bullet_ + tips_ >= 3)
				se::DSound_Play("p_bullet2");
			else
				se::DSound_Play("p_bullet");
		}

		//スキル３　加速度移動
		//ｍｐを自動回復に
		if (kill_3)
		{
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
						//１５フレーム後、速度を初期値に戻る
						if (mp_count > 15)
						{
							mp_count = 0;
							moving_speed = init_moving_speed;      //意外なバグを防ぐため、何回に速度を制御する
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
		}

		//スキル２　散射弾
		//hpを減らす副作用あり
		if (kill_2)
		{
			if (hp_draw_change > Max_hp * hp_times)
			{
				if (CheckPush(input::KEY_MOUSE_RBTN))
				{
					//散射弾数により角度修正
					if (bullets_ > 0 && bullets_ <= 3)
						angle_ = 10.f;
					else if (bullets_ > 3 && bullets_ <= 5)
						angle_ = 20.f;
					else if (bullets_ > 5 && bullets_ <= 7)
						angle_ = 30.f;
					else if (bullets_ > 8 && bullets_ <= 10)
						angle_ = 40.f;

					for (int i = 0;i < bullets_;++i)
						insertAsChild(new p_shot1("p_bullet", x(), y() - (float)player_h / 2.f,
							(float)mousePos_.x, (float)mousePos_.y, 0.f, i * 10.f - angle_));

					//ゲームクリアしたら、ｈｐを確報ためｈｐ減らすのを制御する
					if (b->diedshow() == false)
					{
						p_receiving_atk(Max_hp * hp_times);
						self_atk = true;
					}
					se::DSound_Play("scatter");
				}
			}
			//射撃できなくなる時に
			else if (hp_draw_change <= Max_hp * hp_times && CheckPush(KEY_MOUSE_RBTN))
				se::DSound_Play("shell");
		}
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
	//ｈｐの値を返す
	float Player::playerHp()
	{
		return hp_draw_change;
	}

	//ｈｐ制御
	void Player::p_receiving_atk(const float& hp)
	{
		//エネミーのプロジェクトを取得する
		auto b = getEnemyPrt();
		
		if (hp_draw_change > 0 && hp_draw_change <= Max_hp)
		{
			if (hp < 0)
			{
				//se
				se::DSound_Play("heal_se");
				for (int i = 0;i < 15;++i)
					insertToParent(new bomb10("heal", math::GetRandom(pos().x() - (float)player_w / 2.f, pos().x() + (float)player_w / 2.f),
						math::GetRandom(pos().y() - (float)player_h / 2.f, pos().y() + (float)player_h / 2.f), 1.f, i));
			}
			if (hp > 0)
			{
				if (!self_atk)
					se::DSound_Play("def");
				else
					self_atk = false;
			}
			//ゲームクリアとゲームオーバーを先後で発生しないためｈｐで制御する
			if (b->diedshow() == false)
				hp_ += hp;
		}
		if (hp_ < 0) hp_ = 0;

		if (hp_draw_change <= 0)
		{
			hp_draw_change = 0;
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
	//exp
	void Player::p_exp(const float&exp)
	{
		//level99までにレベルアップ可能
		if (num_2 == lv_count_w * 9 && num_1 == lv_count_w * 9)
		{
			exp_ = Max_exp;
		}
		//裏技でＥｘｐを倍に
		else if (tips_ != 0)
			exp_ += exp * tips_;
		else
			exp_ += exp;

		if (exp_ >= Max_exp)
		{
			exp_ = exp_ - Max_exp;
			Max_exp = Max_exp * 1.2f;
			Max_hp = Max_hp + Max_hp * 0.03f;
			p_levelup(1);
		}
	}
	//レベルアップ
	void Player::p_levelup(const int& level)
	{
		level_ += level;
		p_receiving_atk(-Max_hp / 2.f);
		insertToParent(new bomb9("levelup", pos().x(), pos().y() + (float)(player_h / 4), 1.f));
		num_1 += lv_count_w;
		//レベルカウント
		if (num_1 > lv_count_w * 9)
		{
			num_1 = 0;
			num_2 += lv_count_w;
		}
		else if (num_2 > lv_count_w * 9)
		{
			num_1 = lv_count_w * 9;
			num_2 = lv_count_w * 9;
		}
		//スキル解放
		if (level_ >= 3)
		{
			kill_2 = true;                         //スキル２散射解放 
		}
		if (level_ >= 5)
		{
			kill_3 = true;                         //スキル３移動解放
		}
		//一般攻撃パワーアップ
		if (level_ >= 3 && level_ < 9)
			bullet_ = 2;                           //一般攻撃弾数を２に 
		else if (level_ >= 9 && level_ < 15)
			bullet_ = 3;                           //一般攻撃弾数を３に 
		else if (level_ >= 15)
			bullet_ = 5;                           //一般攻撃弾数を５に
		//散射弾パワーアップ
		if (level_ >= 10 && level_ < 15)
			bullets_ = 5;                          //散射弾数を５に
		else if (level_ >= 15 && level_ < 20)
			bullets_ = 7;                          //散射弾数を７に
		else if (level_ >= 20)
			bullets_ = 9;                          //散射弾数を９に
		
		//散射消耗ｈｐ減少 
		//スキル移動消耗ｍｐ減少、回復速度増加
		if (level_ > 5) 
		{
			if (level_ > is_level)
			{
				if (hp_times < 0.01f)
					hp_times = 0.01f;
				else
					hp_times -= 0.0015f;
				if (mp_times < 0.02f)
					mp_times = 0.02f;
				else
					mp_times -= 0.0015f;
				if (mp_healing > 0.008f)
					mp_healing = 0.008f;
				else
					mp_healing += 0.0005f;
				is_level = level_;
			}
			else
				is_level = level_;
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
		hp_draw_change = Max_hp - (float)(hp_draw_right - hp_draw_left) / Max_hp * hp_;
		//枠を超えないように制御
		if(hp_draw_change <= 0) hp_draw_change = 0;
		if (hp_draw_change > Max_hp) hp_draw_change = Max_hp;
		//填充描画
		graph::Draw_Box(hp_draw_left, hp_draw_top, 
			hp_draw_left + (int)(((float)(hp_draw_right - hp_draw_left) / Max_hp) * hp_draw_change), hp_draw_bottom, 0.05f,
			ARGB(255, 255, 255, 0), ARGB(255, 255, 255, 0), 1, 1);
		//枠描画
		graph::Draw_Box(hp_draw_left, hp_draw_top, hp_draw_right, hp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 0), ARGB(255, 255, 0, 0), 1, 0);
		//HP描字
		//float型やint型をstring型へ変化するため用意
		ostringstream o_hp[2];
		string s_hp[2];
		o_hp[0] << (int)Max_hp;
		o_hp[1] << (int)hp_draw_change;
		s_hp[0].operator=(o_hp[0].str());
		s_hp[1].operator=(o_hp[1].str());
		font::Draw_FontTextNC(hp_draw_left + 2, hp_draw_top - 4, 0.045f, s_hp[1] + "/" + s_hp[0], ARGB(255, 255, 0, 0), 0);
		o_hp[0].clear();
		o_hp[1].clear();
	}
	//mpの描画
	//文字描画が行わないので、ｈｐ描画より簡単な方法で描画する
	void Player::p_drawMp()
	{
		//変化するｍｐ
		mp_draw_change = (float)mp_draw_right - (float)(mp_draw_right - mp_draw_left) / Max_mp * mp_;
		//枠を超えないように制御
		if (mp_draw_change <= (float)mp_draw_left) mp_draw_change = (float)mp_draw_left;
		if (mp_draw_change > (float)mp_draw_right) mp_draw_change = (float)mp_draw_right;
		//填充描画
		graph::Draw_Box( mp_draw_left, mp_draw_top, (int)mp_draw_change, mp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 255), ARGB(255, 255, 0, 255), 1, 1);
		//枠描画
		graph::Draw_Box( mp_draw_left, mp_draw_top, mp_draw_right, mp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 255), ARGB(255, 255, 0, 255), 1, 0);
	}
	//lvの描画
	//文字描画が行わないので、ｈｐ描画より簡単な方法で描画する
	void Player::p_drawLevel()
	{
		//変化するｍｐ
		exp_draw_change = (float)(exp_draw_right - exp_draw_left) / Max_exp * exp_;
		//枠を超えないように制御
		if (exp_draw_change <= (float)exp_draw_left) exp_draw_change = (float)exp_draw_left;
		if (exp_draw_change > (float)exp_draw_right) exp_draw_change = (float)exp_draw_right;
		//填充描画
		graph::Draw_Box(exp_draw_left, exp_draw_top, (int)exp_draw_change, exp_draw_bottom, 0.05f,
			ARGB(255, 0, 255, 255), ARGB(255, 0, 255, 255), 1, 1);
		//枠描画
		graph::Draw_Box(exp_draw_left, exp_draw_top, exp_draw_right, exp_draw_bottom, 0.05f,
			ARGB(255, 0, 255, 255), ARGB(255, 0, 255, 255), 1, 0);
		//画像サイズを取る
		int level_w = graph::Draw_GetImageWidth("level");
		int level_h = graph::Draw_GetImageHeight("level");
		lv_count_w = graph::Draw_GetImageWidth("level_count") / 10;
		lv_count_h = graph::Draw_GetImageHeight("level_count");
		//レベル文字描画
		graph::Draw_Graphics(level_w / 2 + 20, system::WINH - level_h * 2, 0.5f, "level", 0, 0, level_w, level_h, 0.f);
		//レベルカウント描画、位置に合わせるため調整する
		graph::Draw_Graphics(level_w / 2 + 10 + lv_count_w * 3, system::WINH - level_h * 2 - 1, 0.5f, "level_count", num_2, 0, lv_count_w, lv_count_h, 0.f);
		graph::Draw_Graphics(level_w / 2 + 10 + lv_count_w * 4, system::WINH - level_h * 2 - 1, 0.5f, "level_count", num_1, 0, lv_count_w, lv_count_h, 0.f);
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
			graph::Draw_Box(0, 0, system::WINW, system::WINH, 0.45f, ARGB(75, 255, 0, 0), ARGB(75, 255, 0, 0), 1, 1);
			def_show = false;
		}
	}
	//透明度は10以下どうかをチェックする
	bool alpha_(MovableObject& move) { return move.color().a() <= 10; }
	//加速度移動する時に影を生成
	void Player::p_shadow()
	{
		//毎4フレームで影を生成する
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
		p_drawLevel();
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
		p_exp(exp);
	}
}