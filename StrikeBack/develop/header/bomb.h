#pragma once

#include "../../lib/gplib.h"
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/CharBase.h"
#include "movableObject.h"

namespace game
{
	//ƒGƒtƒFƒNƒg‚Æ‚µ‚Äg‚¢‚Ü‚·
	class Player;
	class bomb : public MovableObject
	{
	protected:
		charabase::Anim anim_;
		int animMax_;
		float atk_;
		float atk_times;
	public:
		bomb(const std::string& objectName);
		~bomb();

		std::shared_ptr<Player> getPlayerPrt();

		void render() override;
		void update() override;
	};
	class bomb1 : public bomb
	{
	private:
		int bomb1_w;
		int bomb1_h;
	public:
		bomb1(const std::string& objectName,const float& bomb1_x, const float& bomb1_y, const float& bomb1_s);
		~bomb1();

		void b1_anim();
		void render() override;
		void update() override;
	};

	class bomb2 : public bomb
	{
	private:
		bool atk_show;
		int bomb2_w;
		int bomb2_h;
	public:
		bomb2(const std::string& objectName, const float& bomb2_x, const float& bomb2_y, const float& bomb2_s);
		~bomb2();

		void b2_anim();
		void b2_setoffense();
		void render() override;
		void update() override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class bomb3 : public bomb
	{
	private:
		int bomb3_w;
		int bomb3_h;
	public:
		bomb3(const std::string& objectName, const float& bomb3_x, const float& bomb3_y, const float& bomb3_s);
		~bomb3();

		void b3_anim();
		void render() override;
		void update() override;
	};

	class bomb4 : public bomb
	{
	private:
		bool atk_show;
		int bomb4_w;
		int bomb4_h;
	public:
		bomb4(const std::string& objectName, const float& bomb4_x, const float& bomb4_y, const float& bomb4_s);
		~bomb4();

		void b4_anim();
		void b4_setoffense();
		void render() override;
		void update() override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class bomb5 : public bomb
	{
	private:
		bool atk_show;
		int bomb5_w;
		int bomb5_h;
	public:
		bomb5(const std::string& objectName, const float& bomb5_x, const float& bomb5_y, const float& bomb5_s);
		~bomb5();

		void b5_anim();
		void b5_setoffense();
		void render() override;
		void update() override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};

	class bomb6 : public bomb
	{
	private:
		int bomb6_w;
		int bomb6_h;
	public:
		bomb6(const std::string& objectName, const float& bomb6_x, const float& bomb6_y, const float& bomb6_s);
		~bomb6();

		void b6_anim();
		void render() override;
		void update() override;
	};

	class bomb7 : public bomb
	{
	private:
		int bomb7_w;
		int bomb7_h;
	public:
		bomb7(const std::string& objectName, const float& bomb7_x, const float& bomb7_y, const float& bomb7_s);
		~bomb7();

		void b7_anim();
		void render() override;
		void update() override;
	};

	class bomb8 : public bomb
	{
	private:
		int bomb8_w;
		int bomb8_h;
	public:
		bomb8(const std::string& objectName, const float& bomb8_x, const float& bomb8_y, const float& bomb8_s);
		~bomb8();

		void b8_anim();
		void render() override;
		void update() override;
	};

	class bomb9 : public bomb
	{
	private:
		int bomb9_w;
		int bomb9_h;
		int alpha_;
		float player_posY;
	public:
		bomb9(const std::string& objectName, const float& bomb9_x, const float& bomb9_y, const float& bomb9_s);
		~bomb9();

		void b9_move();
		void render() override;
		void update() override;
	};

	class bomb10 : public bomb
	{
	private:
		int bomb10_w;
		int bomb10_h;
		int alpha_;
		bool begin_;
	public:
		bomb10(const std::string& objectName, const float& bomb10_x, const float& bomb10_y, const float& bomb10_s,const int& count);
		~bomb10();

		void b10_move();
		void render() override;
		void update() override;
	};
}