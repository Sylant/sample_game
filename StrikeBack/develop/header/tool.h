#pragma once

#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/CharBase.h"
#include "movableObject.h"

namespace game
{
	class Player;
	class Tool : public MovableObject
	{
	protected:
		charabase::Anim anim_;
		int animMax_;
	public:
		Tool(const std::string& objectName);
		~Tool();

		std::shared_ptr<Player> getPlayerPrt();

		void render() override;
		void update() override;
	};

	class Tool1 : public Tool
	{
	private:
		float player_hp;
		float regen;
		float regen_times;
		float posX_;
		float posY_;
		int tool1_w;
		int tool1_h;
	public:
		Tool1(const std::string& objectName);
		~Tool1();

		void t1_setoffense();
		void render() override;
		void update() override;
		//UŒ‚”»’è‚É‚æ‚èUŒ‚‚ğ—^‚¦‚½
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};
}
