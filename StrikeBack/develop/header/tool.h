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

		std::shared_ptr<Player> getPlayerPrt();

		void render() override;
		void update() override;
	};

	class Tool1 : public Tool
	{
	private:
		float healing_;
		float heal_using;
		float posX_;
		float posY_;
		int tool1_w;
		int tool1_h;
		int p_hp;
	public:
		Tool1(const std::string& objectName);

		void t1_setoffense();
		void render() override;
		void update() override;
		//çUåÇîªíËÇ…ÇÊÇËçUåÇÇó^Ç¶ÇΩ
		void offenseHit(std::weak_ptr<ci_ext::Object>&) override;
	};
}
