#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/CharBase.h"
#include "movableObject.h"

namespace game
{
	class Stage;
	class Enemy : public MovableObject
	{
	protected:
		charabase::Anim anim_;
	private:
		enum e_mode{ NORMAL, ATTACK , DIED };
		enum e_Dir{ LEFT, RIGHT };
		
		int num;
		int dir_;
		int state_;
		int frame_;
		int alpha_;
		int s2_count_;
		int shot_count;
		int get_s2_count;
		int boss_w;
		int boss_h;
		int s1_probability;
		int max_probability;
		int init_max_probability;
		float max_probability_times;
		float Max_hp;
		float hp_;
		float previous_hp;
		float moving_speed;
		float init_moving_speed;
		bool atk_show;
		bool hide_atk;
		bool died_show;
		bool result_;

		std::shared_ptr<Stage> getStagePrt();
	public:
		Enemy(const std::string& objectName);

		bool diedshow();
		void s2_count(int num);
		void e_speceatk();
		void e_anim();
		void e_move();
		void e_state();
		void e_out();
		void e_result();
		void e_hide_it();
		void e_backrun();
		void e_setdefense();
		void render() override;
		void update() override;
		//‚â‚ç‚ê”»’è‚ÉUŒ‚‚ğH‚ç‚Á‚½
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
//		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};
}