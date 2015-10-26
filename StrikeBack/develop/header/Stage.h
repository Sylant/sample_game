#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "timer.h"

namespace game
{
	class Player;
	class Stage:public ci_ext::Object
	{
		std::weak_ptr<ci_ext::Object> timer_;
	private:
		int num_1;
		int num_2;
		int num_3;
		int tips_;
		int result_;
		int count_w;
		int count_h;
		int kill_count;
		int kill_total;
		int kill_num;
		int sel_alpha;
		int mes_alpha;
		int mes_count;
		int icon_alpha;
		int player_hp;
		float clear_scale;
		float over_scale;
		float mes_scale;
		float icon_scale;
		float count_scale;
		float kill_healing;
		float kill_heal_using;
		bool se_show;
		bool bgm_show;
	public:
		Stage(const std::string& objectName, const int& tips);
		~Stage();
		std::shared_ptr<game::Player> getPlayerPrt();

		int e_kill(const int& count_);
		void total();
		void message();
		void icon();
		void volume();
		void stage_bgm();
		void result(const int& result);
		void init() override;
		void render() override;
		void update() override;
	};
}