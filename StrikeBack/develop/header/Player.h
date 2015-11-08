#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/CharBase.h"
#include "movableObject.h"
#include <sstream>

namespace game
{
	class Stage;
	class Enemy;
	class Player :public MovableObject
	{
	protected:
		charabase::Anim anim_;
	private:
		enum moveDir { NEUTRAL, LEFT, RIGHT };
		
		int level_;
		int is_level;
		int lv_count_w;
		int lv_count_h;
		int num_1;
		int num_2;
		int bullet_;
		int bullets_;
		int tips_;
		int key;
		int shadow_count;
		int player_w;
		int player_h;
		int hp_draw_top;
		int hp_draw_bottom;
		int hp_draw_left;
		int hp_draw_right;
		int mp_draw_top;
		int mp_draw_bottom;
		int mp_draw_left;
		int mp_draw_right;
		int exp_draw_top;
		int exp_draw_bottom;
		int exp_draw_left;
		int exp_draw_right;
		int mp_count;
		float hp;
		float exp;
		float hp_;
		float Max_hp;
		float mp_;
		float Max_mp;
		float hp_times;
		float mp_times;
		float mp_healing;
		float hp_draw_change;
		float mp_draw_change;
		float Max_exp;
		float exp_;
		float exp_draw_change;
		float moving_speed;
		float init_moving_speed;
		float angle_;
		bool def_show;
		bool self_atk;
		bool result_;
		bool kill_2;
		bool kill_3;
		POINT mousePos_;

		std::vector<MovableObject> shadow_;
		std::shared_ptr<Stage> getStagePrt();
		std::shared_ptr<Enemy> getEnemyPrt();
	public:
		Player(const std::string& obejctName,const int& tips);
		~Player();

		//プレイヤー動作関数ポインタの宣言
		typedef void(Player::*move)();

		float playerHp();
		float playerMaxHp();
		void p_levelup(const int& level);
		void p_exp(const float& exp);
		void p_drawLevel();
		void p_receiving_atk(const float& hp);
		void p_defense_atkshow();
		void p_getmousepos();
		void p_move(const int& key);
		void p_drawHp();
		void p_result();
		void p_shadow();
		void p_draw_shadow();
		void p_step_shadow();
		void p_drawMp();
		void p_anim();
		void p_skill();
		void p_key();
		void p_backrun();
		void p_setdefense();
		void p_out();
		void render() override;
		void update() override;
		//やられ判定に攻撃を食らった
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};
}