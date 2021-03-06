#include "../../lib/gplib.h"
#include "movableObject.h"
#include "Enemy.h"
#include "bomb.h"
#include "shot.h"
#include "player.h"
#include "stage.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;

	extern float p_posY;  //プレイヤーからの値を受ける

	//ステージのオブジェクトを取得
	shared_ptr<Stage> Enemy::getStagePrt()
	{
		auto object = getObjectFromRoot({ "stage" });
		shared_ptr<game::Stage> stage = ci_ext::weak_to_shared<game::Stage>(object);
		//ステージの値を返し
		return stage;
	}

	Enemy::Enemy(const string& objectName) :
		MovableObject(
			DrawObjf(objectName, Vec3f::one(), Vec3f::one(), "boss", Vec3f::zero(), Vec3f(230.f, 185.f, 0.f), Color(0xFFFFFFFF), 0))
	{
		//画像のサイズを取得
		boss_w = graph::Draw_GetImageWidth("boss") / 11;
		boss_h = graph::Draw_GetImageHeight("boss") / 4;
		//初期化
		setPos(Vec3f((float)(system::WINW / 2), (float)boss_w / 2, 0.3f));
		resname_ = "boss";
		//アニメ
		charabase::SetAnim(anim_, 17, 0.1f);
		//本体攻撃判定なし
		SetRect(&defenseRect, 0, 0, boss_w, boss_h);
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());

		state_ = e_mode::NORMAL;           //状態初期化
		
		if (math::GetRandom(0, 1) == 0)    //初期左右向きランダムで決める
			dir_ = e_Dir::LEFT;
		else 
			dir_ = e_Dir::RIGHT;
		
		heal_ = 0.f;                       //ボスｈｐ回復数初期化
		frame_ = 0;                        //フレーム数 攻撃状態と一般状態の変化
		s1_probability = 400;              //ステージ内自動攻撃の最大確率
		init_max_probability = 200;        //初期攻撃モードに入る最大確率
		max_probability = 0;               //後期攻撃モードに入る最大確率初期化
		max_probability_times = 0.4f;      //最低確率を確報ため、最大確率の倍率数
		init_moving_speed = 1.f;           //初期移動速度
		moving_speed = 0.f;                //移動速度初期化
		hide_count = 0;                    //隠すカウント
		get_s2_count = 0;                  //ショット２カウント　現存数
		is_s2_count = 3;                   //ショット２カウント　最大数+3
		Max_hp = 1000.f;                   //最大ｈｐ
		hp_ = Max_hp;                      //現時点ｈｐ
		previous_hp = 0.f;                 //前フレームのｈｐ初期化
		result_ = true;                    //ゲームクリアする時に制御
		atk_show = false;                  //連続攻撃させない
		died_show = false;                 //クリア時に一部を制御する
		heal_check = false;                //現れた時の回復を一回のみに制御する
		alpha_ = 255;                      //透明度
		setColor(alpha_, 255, 255, 255);   //描画色を初期化
	}
	Enemy::~Enemy()
	{

	}
	//ボス移動
	void Enemy::e_move()
	{
		//hp変化により移動速度の変化
		moving_speed = (Max_hp - hp_) * 0.002f + init_moving_speed;

		//移動
		if (dir_ == e_Dir::RIGHT)
		{
			velocity_.x(-moving_speed);
		}
		else if (dir_ == e_Dir::LEFT)
		{
			velocity_.x(moving_speed);
		}
	}
	//状態により動作変化
	void Enemy::e_state()
	{
		//動作制御
		switch (state_)
		{
		case e_mode::NORMAL:
		{
			//hpの変化により確率変化
			if (hp_ == Max_hp)
			{ 
				//初期時値を渡し
				max_probability = init_max_probability;
				previous_hp = hp_;
			}
			else if (hp_ != Max_hp)
			{
				//ｈｐ変化しない->何もしない
				if (hp_ == previous_hp) {}
				//ｈｐは前より少なくなったら、攻撃率上昇
				else if (hp_ < previous_hp)
				{
					--max_probability;
				}
				//ｈｐは前より多くなったら、攻撃率下降
				else if (hp_ > previous_hp)
				{
					++max_probability;
				}
				//最大と最小確率数を制御する
				if (max_probability <= init_max_probability * max_probability_times)
					max_probability = (int)(init_max_probability * max_probability_times);
				else if (max_probability > init_max_probability)
					max_probability = init_max_probability;
				//現時点ｈｐの値を渡す
				previous_hp = hp_;
			}

			//確率で攻撃状態に入る
			if (math::GetRandom(0, max_probability) == 0)
			{
				state_ = e_mode::ATTACK;
				atk_show = true;
			}
			//確率で方向変更
			else if (math::GetRandom(0, (int)(max_probability + init_max_probability * max_probability_times)) == 0)
			{
				if (dir_ == e_Dir::RIGHT)
				{
					dir_ = e_Dir::LEFT;
				}
				else if (dir_ = e_Dir::LEFT)
				{
					dir_ = e_Dir::RIGHT;
				}
			}
		}
		    break;
		//攻撃
		case e_mode::ATTACK:
		{
			velocity_.x(0.f);
			
			if (atk_show)
			{
				//初期
				//ｈｐは９５％〜
				if (hp_ >= Max_hp * 0.95f)
				{
					e_scene4();
					e_scene3();
				}
				//ｈｐは８５％〜９５％
				else if (hp_ >= Max_hp * 0.85f && hp_< Max_hp * 0.95)
				{
					for (int i = 0;i < 2;++i)
						e_scene4();
					for (int i = 0;i < 2;++i)
						e_scene3();
				}
				//ｈｐは７０％〜８５％
				else if (hp_ >= Max_hp * 0.7f && hp_ < Max_hp * 0.85f)
				{
					for (int i = 0;i < 3;++i)
						e_scene3();
					e_scene2();
					e_scene5();
				}
				//ｈｐは６０％〜７０％
				else if (hp_ >= Max_hp * 0.6f && hp_ < Max_hp * 0.7f)
				{
					for (int i = 0;i < 3;++i)
						e_scene2();
					for (int i = 0;i < 2;++i)
						e_scene5();
					e_scene1(2);
					e_scene6();
				}
				//ｈｐは４５％〜６０％
				else if (hp_ >= Max_hp * 0.45f && hp_ < Max_hp * 0.6f)
				{
					for (int i = 0;i < 3;++i)
						e_scene3();
					e_scene1(3);
					e_scene2();
					e_scene6();
				}
				//ｈｐは３５％〜４５％
				else if (hp_ >= Max_hp * 0.35f && hp_ < Max_hp * 0.45f)
				{
					for (int i = 0;i < 3;++i)
						e_scene2();
					for (int i = 0;i < 2;++i)
						e_scene6();
					for (int i = 0;i < 2;++i)
						e_scene4();
					e_scene1(3);
				}
				//ｈｐは２５％〜３５％
				else if (hp_ >= Max_hp * 0.25f && hp_ < Max_hp * 0.35f)
				{
					for (int i = 0;i < 4;++i)
						e_scene3();
					e_scene1(5);
				}
				//ｈｐは１５％〜２５％
				else if (hp_ >= Max_hp * 0.25f && hp_ < Max_hp * 0.35f)
				{
					for (int i = 0;i < 2;++i)
					{
						e_scene2();
						e_scene5();
						e_scene6();
					}
					e_scene4();
				}
				//ｈｐは〜１５％
				else if (hp_ <= Max_hp * 0.15f)
				{
					for (int i = 0;i < 4;++i)
						e_scene3();
					e_scene1(4);
					e_scene4();
					e_scene5();
					e_scene6();
				}
				atk_show = false;
			}
			//数秒後一般状態に戻る
			//一秒＝６０フレーム
			if (++frame_ / 60 == math::GetRandom(2, 5))
			{
				state_ = e_mode::NORMAL;
				frame_ = 0;
			}
		}
		    break;
		//消滅
		case e_mode::DIED:
		{
			velocity_.x(0.f);
			velocity_.y(0.f);

			//一回だけ呼ぶことに制御
			if (result_)
			{
				e_result();
				result_ = false;
			}
		}
		    break;
		}
	}
	//ボス攻撃方法
	//ミニエネミー呼ぶ
	void Enemy::e_scene1(const int& s2_count)
	{
		//スキル2が設定数以上存在すると、呼び出しことできないに設定
		if (get_s2_count <= s2_count)
		{
			for (int i = 0;i < s2_count;++i)
			{
				insertAsChild(new e_shot2("mini", pos().x(), pos().y(), i));
				++get_s2_count;
			}
		}
	}
	//地雷呼ぶ
	void Enemy::e_scene2()
	{
		insertAsChild(new e_shot3("landmine"));
	}
	//ＵＦＯ呼ぶ
	void Enemy::e_scene3()
	{
		insertAsChild(new e_shot4("ufo"));
	}
	//ヘリコプタータイプ１呼ぶ
	void Enemy::e_scene4()
	{
		insertAsChild(new e_shot5("heli", 1));
	}
	//ヘリコプタータイプ２呼ぶ
	void Enemy::e_scene5()
	{
		insertAsChild(new e_shot5("heli", 2));
	}
	//ヘリコプタータイプ３呼ぶ
	void Enemy::e_scene6()
	{
		insertAsChild(new e_shot5("heli", 3));
	}

	//ステージへ関数を呼ぶ
	void Enemy::e_result()
	{
		auto s = getStagePrt();
		s->result(1);
	}

	//画面外でないように
	void Enemy::e_out()
	{
		//画面外出ないように
		if (pos().x() + velocity_.x() >= float(system::WINW - boss_w / 2)) dir_ = e_Dir::RIGHT;
		else if (pos().x() + velocity_.x() <= (float)boss_w / 2.f) dir_ = e_Dir::LEFT;
	}
	//左右反転
	void Enemy::e_backrun()
	{
		if (dir_ == e_Dir::LEFT && scale().x() != -1)
			setScale(Vec3f(scale().x()*-1, scale().y(), scale().z()));
		if (dir_ == e_Dir::RIGHT && scale().x() != 1)
			setScale(Vec3f(scale().x()*-1, scale().y(), scale().z()));
	}
	//状態によりアニメ変化
	void Enemy::e_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0 };
		int t2[] = { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 7, 8, 9, 10 };
		int t3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		if (charabase::StepAnim(anim_))
		{
			if (state_ == e_mode::NORMAL)
			{
				setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
			}
			//二つ攻撃アニメ確率で表現
			else if (state_ == e_mode::ATTACK)
			{
				if (hp_ >= Max_hp * 0.4f)
				{
					setSrc(Vec3f((float)t[anim_.no], 1.f, 0.f));
					se::DSound_Play("boss_atk");
				}
				else if (hp_ < Max_hp * 0.4f)
				{
					setSrc(Vec3f((float)t2[anim_.no], 2.f, 0.f));
					se::DSound_Play("boss_atk2");
				}
			}
			else if (state_ == e_mode::DIED)
			{
				setSrc(Vec3f((float)t3[anim_.no], 3.f, 0.f));
			}
		}
	}

	//受け判定はボスの脳だけに制御
	void Enemy::e_setdefense()
	{
		if (dir_ == e_Dir::LEFT)
			SetRect(&defenseRect, (int)(ix() - ((boss_w / 3) / 1.5)), (int)(iy() - (boss_h / 2.5)),
				(int)(ix() + ((boss_w / 3) / 2)), (int)(iy() - boss_h / 10));
		else if (dir_ == e_Dir::RIGHT)
			SetRect(&defenseRect, (int)(ix() - ((boss_w / 3) / 2)), (int)(iy() - (boss_h / 2.5)), 
				(int)(ix() + ((boss_w / 3) / 1.5)), (int)(iy() - boss_h / 10));
	}
	//自動攻撃 確率で爆弾を落下する
	void Enemy::e_speceatk()
	{
		//爆弾のサイズに合わせ爆発アニメ位置
		float posX_ = math::GetRandom(0.f + 29.f / 2.f, system::WINW - 29.f / 2.f);
		if (!died_show)
		{
			if (math::GetRandom(0, s1_probability) == 0)
			{
				insertAsChild(new e_shot1("e_missiles", posX_, 0.f + 44.f / 2.f, math::GetRandom(4.f, 8.f)));
			}
		}
	}

	//スキル２制御
	void Enemy::s2_count(const int& num)
	{
		num_ = num;
		if (num_ == 1)
		{
			num_ = 0;
			get_s2_count -= 1;
		}
	}
	
	//やられた状態を値でプレイヤーに渡し
	bool Enemy::diedshow()
	{
		return died_show;
	}

	//やられた時に呼ぶ
	void Enemy::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		if (hp_ > 0.f)
		{
			--hp_;
			if (hp_ <= 0.f)
			{
				for (int i = 0;i < 10;++i)
				{
					insertToParent(new bomb6("all_explode", math::GetRandom(pos().x() - (float)boss_w / 2.f, pos().x() + (float)boss_w / 2.f),
						math::GetRandom(pos().y() - (float)boss_h / 2.f, pos().y() + (float)boss_h / 2.f), math::GetRandom(1.5f, 3.f)));
				}
				state_ = e_mode::DIED;
				died_show = true;
			}
		}
	}
	//ｈｐによりボスの身を隠すか表すか
	void Enemy::e_hide_check()
	{
		if (hp_ <= Max_hp * 0.9f && hp_ > Max_hp * 0.8f)
			e_not_hide();
		else if (hp_ <= Max_hp * 0.75f && hp_ > Max_hp * 0.65f)
			e_hide_it();
		else if (hp_ <= Max_hp * 0.6f && hp_ > Max_hp * 0.5f)
			e_not_hide();
		else if (hp_ <= Max_hp * 0.45f && hp_ > Max_hp * 0.35f)
			e_hide_it();
		else if (hp_ <= Max_hp * 0.3f && hp_ > Max_hp * 0.2f)
			e_not_hide();
		else if (hp_ <= Max_hp * 0.15f)
			e_hide_it();
	}

	//身を表す
	void Enemy::e_hide_it()
	{
		alpha_ += 30;
		if (alpha_ > 255)
		{
			alpha_ = 255;
			hide_count = 0;
			if (heal_check)
			{
				heal_ = math::GetRandom(Max_hp * 0.025f, Max_hp * 0.05f);
				//現れた時にｈｐ回復する
				hp_ += heal_;
				if (hp_ > Max_hp) hp_ = Max_hp;
				for (int i = 0;i <= (int)heal_;++i)
				{
					insertToParent(new bomb10("heal", math::GetRandom(pos().x() - (float)boss_w / 2.f, pos().x() + (float)boss_w / 2.f),
						math::GetRandom(pos().y() - (float)boss_h / 2.f, pos().y() + (float)boss_h / 2.f), 1.5f, i));
				}
				heal_check = false;
			}
		}
		setColor(alpha_, 255, 255, 255);
	}

	//身を隠す　当たり判定存在する
	void Enemy::e_not_hide()
	{
		alpha_ -= 30;
		if (alpha_ < 0)
		{
			alpha_ = 0;
		}
		if (alpha_ == 0)
		{
			++hide_count;
			//６０ＦＰＳ１秒、隠す１０秒後、ｈｐ回復開始
			if (hide_count / 60 > 10)
			{
				//毎３００フレーム１点回復
				if (hide_count % 300 == 0)
				{
					++hp_;
				}
			}
			heal_check = true;
		}
		setColor(alpha_, 255, 255, 255);
	}

	void Enemy::render()
	{
		MovableObject::render();
        //判定チェックRECT
//		gplib::graph::Draw_CkRect(defenseRect, ARGB(255, 255, 0, 0));
	}

	void Enemy::update()
	{
		MovableObject::update();
		e_anim();
		e_backrun();
		e_move();
		e_out();
		e_state();
		e_hide_check();
		e_speceatk();
		e_setdefense();
		s2_count(num_);
	}
}