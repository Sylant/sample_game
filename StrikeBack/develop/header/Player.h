#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/CharBase.h"
#include "movableObject.h"

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
		
		int bullet_;
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
		int mp_count;
		float hp;
		float hp_;
		float Max_hp;
		float mp_;
		float Max_mp;
		float hp_times;
		float mp_times;
		float mp_healing;
		float hp_draw_change;
		float mp_draw_change;
		float moving_speed;
		float init_moving_speed;
		bool def_show;
		bool result_;
		POINT mousePos_;

		std::vector<MovableObject> shadow_;
		std::shared_ptr<Stage> getStagePrt();
		std::shared_ptr<Enemy> getEnemyPrt();
	public:
		Player(const std::string& obejctName, int tips);

		//�v���C���[����֐��|�C���^�̐錾
		typedef void(Player::*move)();

		int playerHpRight();
		float playerHp();
		float playerMaxHp();
		void p_receiving_atk(float hp);
		void p_defense_atkshow();
		void p_getmousepos();
		void p_move(int key);
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
		//���ꔻ��ɍU����H�����
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};
}