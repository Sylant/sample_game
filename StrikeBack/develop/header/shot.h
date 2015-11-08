#pragma once

#include "../../lib/gplib.h"
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/CharBase.h"
#include "movableObject.h"

namespace game
{
	//‘O•ûéŒ¾
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
		~shot();

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
		p_shot1(const std::string& objectName, const float& posX, const float& posY,
			const float& targetX, const float& targetY, const float& cnt, const float& angle);
		~p_shot1();

		void ps_out();
		void ps_setoffense();
		void render() override;
		void update() override;

		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot1 : public shot
	{
	private:
		int shot1_w;
		int shot1_h;
		float atk_;
		float atk_times;
		float s1_speed;
		bool atk_show;
	public:
		e_shot1(const std::string& objectName,const float& posX,const float& posY ,const float& speed);
		~e_shot1();

		void s1_out();
		void s1_anim();
		void s1_setoffense();
		void render() override;
		void update() override;

		//‚â‚ç‚ê”»’è‚ÉUŒ‚‚ğH‚ç‚Á‚½
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
		float exp_;
		float posX_;
		float s2_speed;
		float s2_speed2;
		bool down_;
		bool atk_show;
		bool bomb2_show;
		enum s2_mode { NORMAL, MOVE, ATTACK, SUICIDE, DIED };
	public:
		e_shot2(const std::string& obejctName, const float& posX, const float& posY, const int& num);
		~e_shot2();

		void s1_move();
		void s1_anim();
		void s1_setdefense();
		void render() override;
		void update() override;

		//‚â‚ç‚ê”»’è‚ÉUŒ‚‚ğH‚ç‚Á‚½
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
		e_shot2_atk(const std::string& objectName ,const float& posX);
		~e_shot2_atk();

		void s2a_anim();
		void s2a_setoffense();
		void render() override;
		void update() override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
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
		float exp_;
	public:
		e_shot3(const std::string& obejctName);
		~e_shot3();

		void s3_anim();
		void s3_setoffense();
		void s3_setdefense();
		void render() override;
		void update() override;
        //UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
		//‚â‚ç‚ê”»’è‚ÉUŒ‚‚ğH‚ç‚Á‚½
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
		float exp_;
		float posX_;
		bool appear_show;
		bool atk_show;
		enum s4_mode { NORMAL, ATTACK };

	public:
		e_shot4(const std::string& obejctName);
		~e_shot4();

		void s4_move();
		void s4_anim();
		void s4_setdefense();
		void render() override;
		void update() override;
		//‚â‚ç‚ê”»’è‚ÉUŒ‚‚ğH‚ç‚Á‚½
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot4_atk : public shot
	{
	private:
		int s4_atk_w;
		int s4_atk_h;
	public:
		e_shot4_atk(const std::string& objectName, const float& posX, const float& posY);
		~e_shot4_atk();

		void s4a_anim();
		void render() override;
		void update() override;
	};

	class e_shot5 : public shot
	{
	private:
		int hp_;
		int frame_;
		int max_frame;
		int state_;
		int type_;
		int shot5_w;
		int shot5_h;
		float exp_;
		float posX_;
		float moving_speed;
		bool atk_show;
		enum s5_mode { NORMAL, ATTACK };
		enum s5_type { type1, type2, type3 };
	public:
		e_shot5(const std::string& obejctName , const int& num);
		~e_shot5();

		void s5_move();
		void s5_anim();
		void s5_setdefense();
		void render() override;
		void update() override;
		//‚â‚ç‚ê”»’è‚ÉUŒ‚‚ğH‚ç‚Á‚½
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
		e_shot5_atk(const std::string& objectName, const float& posX, const float& posY, const float& angle);
		~e_shot5_atk();

		void s5a_out();
		void s5a_setoffense();
		void render() override;
		void update() override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot5_throw : public shot
	{
	private:
		int hp_;
		int fire_;
		int s5_throw_w;
		int s5_throw_h;
		float exp_;
	public:
		e_shot5_throw(const std::string& obejctName, const float& posX, const float& posY, const float& scale);
		~e_shot5_throw();

		void s5t_move();
		void s5t_setdefense();
		void render() override;
		void update() override;
		//‚â‚ç‚ê”»’è‚ÉUŒ‚‚ğH‚ç‚Á‚½
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot5_atk2 : public shot
	{
	private:
		int angle_;
		int distance;
		int s5_atk2_w;
		int s5_atk2_h;
		float atk_;
		float atk_times;
		float targetX;
		float targetY;
		float atk_speed;
		bool atk_show;
		bool is_distance;
	public:
		e_shot5_atk2(const std::string& objectName, const float& p_posX, const float& p_posY, const float& angle);
		~e_shot5_atk2();

		void s5a2_move();
		void s5a2_out();
		void s5a2_setoffense();
		void render() override;
		void update() override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot5_throw2 : public shot
	{
	private:
		int hp_;
		int fire_;
		int fire_count;
		int del_count;
		int s5_throw2_w;
		int s5_throw2_h;
		float exp_;
		bool atk_show;
	public:
		e_shot5_throw2(const std::string& obejctName, const float& posX, const float& posY, const float& scale);
		~e_shot5_throw2();

		void s5t2_move();
		void s5t2_setdefense();
		void render() override;
		void update() override;
		//‚â‚ç‚ê”»’è‚ÉUŒ‚‚ğH‚ç‚Á‚½
		void defenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class e_shot5_atk3 : public shot
	{
	private:
		int s5_atk3_w;
		int s5_atk3_h;
		float atk_;
		float atk_times;
		float atk_speed;
		bool atk_show;
		bool is_distance;
	public:
		e_shot5_atk3(const std::string& objectName, const float& p_posX, const float& p_posY, const float& angle);
		~e_shot5_atk3();

		void s5a3_out();
		void s5a3_setoffense();
		void render() override;
		void update() override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};
}