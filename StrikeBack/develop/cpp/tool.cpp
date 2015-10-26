#include "../../lib/gplib.h"
#include "movableObject.h"
#include "tool.h"
#include "bomb.h"
#include "player.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;

	//�v���C���[�̃I�u�W�F�N�g���擾
	shared_ptr<Player> Tool::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//�v���C���[�̒l��Ԃ�
		return player;
	}

	Tool::Tool(const string& objectName) :
		MovableObject(DrawObjf(
			objectName, Vec3f::zero(), Vec3f::one(), objectName, Vec3f::zero(), Vec3f::zero(), Color(0xFFFFFFFF), 0))
	{

	}
	//�f�X�g���N�^
	Tool::~Tool()
	{

	}
	void Tool::render()
	{
		MovableObject::render();
	}

	void Tool::update()
	{

	}
	
	//�v���C���[hp�񕜃A�e�C��
	Tool1::Tool1(const string& objectName) :
		Tool(objectName)
	{
		//�摜�̃T�C�Y���擾
		tool1_w = graph::Draw_GetImageWidth("ammo");
		tool1_h = graph::Draw_GetImageHeight("ammo");
		//������
		resname_ = "ammo";
		setScale(Vec3f(1.f, 1.f, 0.f));
		setSrcSize(Vec3f((float)tool1_w, (float)tool1_h, 0.f));
		//�U�ߔ���Ƃ��Đݒ�
		SetRect(&offenseRect, 0, 0, tool1_w, tool1_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//X�AY���W
		posY_ = (float)system::WINH - (tool1_h * 2 + tool1_h / 7);
		posX_ = (float)math::GetRandom(tool1_w / 2, system::WINW);
		//�����ʒu
		setPos(Vec3f(posX_, posY_, 0.35f));

		p_hp = 0;          //�v���C���[����������
		healing_ = 0.f;    //�񕜒l������
		heal_using = 0.1f; //�v���C���[�̍ő傈���ɂ��񕜔{��
	}
	//�f�X�g���N�^
	Tool1::~Tool1()
	{

	}
	//����X�V
	void Tool1::t1_setoffense()
	{
		SetRect(&offenseRect, (int)(ix() - tool1_w / 2), (int)(iy() - tool1_h / 2), (int)(ix() + tool1_w / 2), (int)(iy() + tool1_h / 2));
	}
	//����������Ă�
	void Tool1::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//�v���C���[�̃I�u�W�F�N�g���擾
		auto p = getPlayerPrt();

		//�v���C���[�̂����ݒ�ɂ��񕜒l�ω�����
		healing_ = p->playerMaxHp() * heal_using;
		
		//hp�`��͘g�𒴂��Ȃ��悤��
		if (p->playerHp() >= p->playerHpRight())
		{
			p->p_receiving_atk(0);
		}
		else if (p->playerHp() < p->playerHpRight())
		{
			//hp�̕`��ʒu�̍��W�͋t�Ȃ̂ŁA�{��-���t�ɂ���
			p_hp = p->playerHpRight() - (int)p->playerHp();
			//�����������͉񕜒l������->�񕜒l�ŉ񕜂���
			if (p_hp >= (int)healing_)
				p->p_receiving_atk(-healing_);
			//�����������͉񕜒l��艺���->�����������������񕜂���
			else if (p_hp < (int)healing_)
				p->p_receiving_atk(-(float)p_hp);
		}

		insertToParent(new bomb3("get_tool", pos().x(), pos().y() - (float)tool1_h, 1.5f));
		kill();
	}

	void Tool1::render()
	{
		MovableObject::render();
	}

	void Tool1::update()
	{
		MovableObject::update();
		t1_setoffense();
	}
}