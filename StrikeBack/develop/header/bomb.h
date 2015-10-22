#pragma once

#include "../../lib/gplib.h"
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/CharBase.h"
#include "movableObject.h"

namespace game
{
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
		bomb1(const std::string& objectName, float bomb1_x, float bomb1_y, float bomb1_s);

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
		bomb2(const std::string& objectName, float bomb2_x, float bomb2_y, float bomb2_s);

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
		bomb3(const std::string& objectName, float bomb3_x, float bomb3_y, float bomb3_s);

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
		bomb4(const std::string& objectName, float bomb4_x, float bomb4_y, float bomb4_s);

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
		bomb5(const std::string& objectName, float bomb5_x, float bomb5_y, float bomb5_s);

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
		bomb6(const std::string& objectName, float bomb6_x, float bomb6_y, float bomb6_s);

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
		bomb7(const std::string& objectName, float bomb7_x, float bomb7_y, float bomb7_s);

		void b7_anim();
		void render() override;
		void update() override;
	};
}