#pragma once

#include "../../lib/gplib.h"
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/CharBase.h"
#include "movableObject.h"

namespace game
{
	//前方宣言
	class Stage;
	class Player;
	class Enemy;
	class shot : public MovableObject
	{
	protected:
		charabase::Anim anim_;
		int animMax_;
	public:
		shot(const std::string& objectName);

		void render() override;
		void update() override;

		std::shared_ptr<Player> getPlayerPrt();
		std::shared_ptr<Enemy> getBossPrt();
		std::shared_ptr<Stage> getStagePrt();
	};

	class p_shot1 : public shot
	{
	private:
		float shot_speed;
		int p_bullet_w;
		int p_bullet_h;
	public:
		p_shot1(const std::string& objectName, float posX, float posY, float targetX, float targetY, float cnt , float angle);

		void ps_out();
		void ps_setoffense();
		void render() override;
		void update() override;

		//攻撃判定により攻撃を与えた
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot1 : public shot
	{
	private:
		int shot1_w;
		int shot1_h;
		bool s1_show;
		float s1_speed;
	public:
		e_shot1(const std::string& objectName, float posX, float posY , float speed);

		void s1_out();
		void s1_anim();
		void s1_setoffense();
		void render() override;
		void update() override;

		//やられ判定に攻撃を食らった
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot2 : public shot
	{
	private:
		
		int frame_;
		int hp_;
		int num_;
		int state_;
		int shot2_w;
		int shot2_h;
		int atk_count;
		float posX_;
		float s2_speed;
		float s2_speed_;
		bool down_;
		bool atk_show;
		bool bomb2_show;
		enum s2_mode { NORMAL, MOVE, ATTACK, SUICIDE, DIED };
	public:
		e_shot2(const std::string& obejctName, float posX, float posY, int num);

		void s1_move();
		void s1_anim();
		void s1_setdefense();
		void render() override;
		void update() override;

		//やられ判定に攻撃を食らった
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot2_atk : public shot
	{
	private:
		int s2_atk_w;
		int s2_atk_h;
		float atk_;
		float atk_times;
		bool s2_atk_show;
	public:
		e_shot2_atk(const std::string& objectName , float posX);

		void s2a_anim();
		void s2a_setoffense();
		void render() override;
		void update() override;
		//攻撃判定により攻撃を与えた
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;

	};

	class e_shot3 : public shot
	{
	private:
		int hp_;
		int frame_;
		int shot3_w;
		int shot3_h;
		float posX_;
		float posY_;
		
	public:
		e_shot3(const std::string& obejctName);

		void s3_anim();
		void s3_setoffense();
		void s3_setdefense();
		void render() override;
		void update() override;
        //攻撃判定により攻撃を与えた
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
		//やられ判定に攻撃を食らった
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot4 : public shot
	{
	private:
		int shot4_w;
		int shot4_h;
		int alpha_;
		int frame_;
		int state_;
		int hp_;
		float posX_;
		bool appear_show;
		bool atk_show;
		enum s4_mode { NORMAL, ATTACK };

	public:
		e_shot4(const std::string& obejctName);

		void s4_move();
		void s4_anim();
		void s4_setdefense();
		void render() override;
		void update() override;
		//やられ判定に攻撃を食らった
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot4_atk : public shot
	{
	private:
		int s4_atk_w;
		int s4_atk_h;
	public:
		e_shot4_atk(const std::string& objectName, float posX, float posY);

		void s4a_anim();
		void render() override;
		void update() override;
	};

	class e_shot5 : public shot
	{
	private:
		int hp_;
		int frame_;
		int state_;
		int shot5_w;
		int shot5_h;
		float posX_;
		float moving_speed;
		bool atk_show;
		enum s5_mode { NORMAL, ATTACK };
	public:
		e_shot5(const std::string& obejctName);

		void s5_move();
		void s5_anim();
		void s5_setdefense();
		void render() override;
		void update() override;
		//やられ判定に攻撃を食らった
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot5_atk : public shot
	{
	private:
		int s5_atk_w;
		int s5_atk_h;
		float atk_;
		float atk_times;
		float targetX_;
		float targetY_;
		float atk_speed;
		bool atk_show;
	public:
		e_shot5_atk(const std::string& objectName, float posX, float posY, float angle);

		void s5a_out();
		void s5a_setoffense();
		void render() override;
		void update() override;
		//攻撃判定により攻撃を与えた
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};
}