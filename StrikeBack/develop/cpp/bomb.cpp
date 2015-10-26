#include "../../lib/gplib.h"
#include "movableObject.h"
#include "bomb.h"
#include "Player.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;

	bomb::bomb(const string& objectName) :
		MovableObject(DrawObjf(
		objectName, Vec3f::zero(), Vec3f::one(), objectName, Vec3f::zero(), Vec3f::zero(), Color(0xFFFFFFFF), 0))
	{

	}

	//�v���C���[�̃I�u�W�F�N�g���擾
	shared_ptr<Player> bomb::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//�v���C���[�̒l��Ԃ�
		return player;
	}
	//�f�X�g���N�^
	bomb::~bomb()
	{

	}
	void bomb::render()
	{
		MovableObject::render();
	}

	void bomb::update()
	{

	}
	//�G�l�~�[��ʍU�������A�j��
	//bomb1_x , bomb1_y  �A�j���ʒu
	//bomb1_s    �摜�g��k���̔{��
	bomb1::bomb1(const std::string& objectName, const float& bomb1_x, const float& bomb1_y, const float& bomb1_s) :
		bomb(objectName)
	{
		//�摜�̃T�C�Y���擾
		bomb1_w = graph::Draw_GetImageWidth("e_explosion") / 29;
		bomb1_h = graph::Draw_GetImageHeight("e_explosion");
		//������
		setScale(Vec3f(bomb1_s, bomb1_s, 0.f));
		setPos(Vec3f(bomb1_x, bomb1_y, 0.1f));
		setSrcSize(Vec3f((float)bomb1_w, (float)bomb1_h, 0.f));
		resname_ = "e_explosion";
		//�A�j��
		//SetAnim�iobejctname�C�J��Ԃ��A�j�������C�A�j���X�s�[�h�j
		charabase::SetAnim(anim_, 30, 0.4f);
		animMax_ = 29;
		//SE
		se::DSound_Play("e_explosion");
	}
	//�f�X�g���N�^
	bomb1::~bomb1()
	{

	}
	//�A�j��
	void bomb1::b1_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}

	void bomb1::render()
	{
		MovableObject::render();
	}

	void bomb1::update()
	{
		MovableObject::update();
		b1_anim();
	}
	//�~�j�G�l�~�[�����A�j��
	//�U������͂����Ŏ��
	//bomb2_x , bomb2_y  �A�j���ʒu
	//bomb2_s    �摜�g��k���̔{��
	bomb2::bomb2(const std::string& objectName, const float& bomb2_x, const float& bomb2_y, const float& bomb2_s) :
		bomb(objectName)
	{
		//�摜�̃T�C�Y���擾
		bomb2_w = graph::Draw_GetImageWidth("bomb2") / 28;
		bomb2_h = graph::Draw_GetImageHeight("bomb2");
		//������
		setScale(Vec3f(bomb2_s, bomb2_s, 0.f));
		setPos(Vec3f(bomb2_x, bomb2_y, 0.1f));
		setSrcSize(Vec3f((float)bomb2_w, (float)bomb2_h, 0.f));
		resname_ = "bomb2";
		//�A�j��
		charabase::SetAnim(anim_, 29, 0.45f);
		animMax_ = 28;
		//�U���������񂾂��ɗ}����
		atk_show = true;
		//�󂯔���Ȃ�
		SetRect(&offenseRect, 0, 0, bomb2_w, bomb2_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//SE
		se::DSound_Play("kill");
		//�U���͏�����
		atk_ = 0.f;
		//�v���C���[�̍ő傈���ɂ��U���{��
		atk_times = 0.03f;
	}
	//�A�j��
	void bomb2::b2_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	//�f�X�g���N�^
	bomb2::~bomb2()
	{

	}
	//�U�ߔ���X�V
	void bomb2::b2_setoffense()
	{
		//�`��͂Q�{�L�債�Ă���̂Ō��̃T�C�Y���g���܂�
		SetRect(&offenseRect, (int)(ix() - bomb2_w), (int)(iy() - bomb2_h),
			(int)(ix() + bomb2_w), (int)(iy() + bomb2_h));
	}

	//�U������
	void bomb2::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//�v���C���[�̃I�u�W�F�N�g���擾
		auto p = getPlayerPrt();
		//�v���C���[�̂����ݒ�ɂ��U���͕ω�����
		atk_ = p->playerMaxHp() * atk_times;
		//�v���C���[�̂����ݒ�ɂ��U���͕ω����Ȃ�
//		atk_ = atk_ * atk_using;
		if (atk_show)
		{
			p->p_receiving_atk(atk_);
			atk_show = false;
		}
	}

	void bomb2::render()
	{
		MovableObject::render();
		//����`�F�b�NRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void bomb2::update()
	{
		MovableObject::update();
		b2_anim();
		b2_setoffense();
	}
	//����Q�b�g�A�j��
	//bomb3_x , bomb3_y  �A�j���ʒu
	//bomb3_s    �摜�g��k���̔{��
	bomb3::bomb3(const std::string& objectName, const float& bomb3_x, const float& bomb3_y, const float& bomb3_s) :
		bomb(objectName)
	{
		//�摜�̃T�C�Y���擾
		bomb3_w = graph::Draw_GetImageWidth("get_tool") / 6;
		bomb3_h = graph::Draw_GetImageHeight("get_tool");
		//������
		setScale(Vec3f(bomb3_s, bomb3_s, 0.f));
		setPos(Vec3f(bomb3_x, bomb3_y, 0.4f));
		setSrcSize(Vec3f((float)bomb3_w, (float)bomb3_h, 0.f));
		resname_ = "get_tool";
		//�A�j��
		charabase::SetAnim(anim_, 7, 0.3f);
		animMax_ = 6;
		//SE
		se::DSound_Play("tool_get");
	}
	//�f�X�g���N�^
	bomb3::~bomb3()
	{

	}
	//�A�j��
	void bomb3::b3_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}

	void bomb3::render()
	{
		MovableObject::render();
	}

	void bomb3::update()
	{
		MovableObject::update();
		b3_anim();
	}
	//�n�������A�j��
	//�U�ߔ���͂����ōs��
	//bomb4_x , bomb4_y  �A�j���ʒu
	//bomb4_s    �摜�g��k���̔{��
	bomb4::bomb4(const std::string& objectName, const float& bomb4_x, const float& bomb4_y, const float& bomb4_s) :
		bomb(objectName)
	{
		//�摜�̃T�C�Y���擾
		bomb4_w = graph::Draw_GetImageWidth("ld_explosion") / 15;
		bomb4_h = graph::Draw_GetImageHeight("ld_explosion");
		//������
		setScale(Vec3f(bomb4_s, bomb4_s, 0.f));
		setPos(Vec3f(bomb4_x, bomb4_y, 0.1f));
		setSrcSize(Vec3f((float)bomb4_w, (float)bomb4_h, 0.f));
		resname_ = "ld_explosion";
		//�A�j��
		charabase::SetAnim(anim_, 16, 0.3f);
		animMax_ = 15;
		//�U���������񂾂��ɗ}����
		atk_show = true;
		//�U�ߔ��肠��
		SetRect(&offenseRect, 0, 0, bomb4_w, bomb4_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//SE
		se::DSound_Play("landmine");
		//�U���͏�����
		atk_ = 0.f;
		//�v���C���[�̍ő傈���ɂ��U���{��
		atk_times = 0.03f;
	}
	//�f�X�g���N�^
	bomb4::~bomb4()
	{

	}
	//�A�j��
	void bomb4::b4_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ,15 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	//�U������X�V
	void bomb4::b4_setoffense()
	{
		//����͈͂�}���邽�߂�*0.8f��
		SetRect(&offenseRect, (int)(ix() - bomb4_w * 0.8f / 2), (int)(iy() - bomb4_h / 2),
			(int)(ix() + bomb4_w * 0.8f / 2), (int)(iy() + bomb4_h / 2));
	}
	//�U������
	void bomb4::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//�v���C���[�̃I�u�W�F�N�g���擾
		auto p = getPlayerPrt();
		//�v���C���[�̂����ݒ�ɂ��U���͕ω�����
		atk_ = p->playerMaxHp() * atk_times;
		//�v���C���[�̂����ݒ�ɂ��U���͕ω����Ȃ�
//		atk_ = atk_ * atk_using;
		if (atk_show)
		{
			p->p_receiving_atk(atk_);
			atk_show = false;
		}
	}

	void bomb4::render()
	{
		MovableObject::render();
	}

	void bomb4::update()
	{
		MovableObject::update();
		b4_anim();
		b4_setoffense();
	}
	//UFO�U���A�j��
	//�U�����肱���ōs��
	//bomb5_x , bomb5_y  �A�j���ʒu
	//bomb5_s    �摜�g��k���̔{��
	bomb5::bomb5(const std::string& objectName, const float& bomb5_x, const float& bomb5_y, const float& bomb5_s) :
		bomb(objectName)
	{
		//�摜�̃T�C�Y���擾
		bomb5_w = graph::Draw_GetImageWidth("ufo_explosion") / 7;
		bomb5_h = graph::Draw_GetImageHeight("ufo_explosion");
		//������
		setScale(Vec3f(bomb5_s, bomb5_s, 0.f));
		setPos(Vec3f(bomb5_x, bomb5_y, 0.1f));
		setSrcSize(Vec3f((float)bomb5_w, (float)bomb5_h, 0.f));
		resname_ = "ufo_explosion";
		//�A�j��
		charabase::SetAnim(anim_, 8, 0.17f);
		animMax_ = 7;
		//�U�ߔ��肠��
		SetRect(&offenseRect, 0, 0, bomb5_w, bomb5_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//���ˑ��x
		velocity_.y(6.f);
		//�U���������񂾂��ɗ}����
		atk_show = true;
		//SE
		se::DSound_Play("beam");
		//�U���͏�����
		atk_ = 0.f;
		//�v���C���[�̍ő傈���ɂ��U���{��
		atk_times = 0.04f;
	}
	//�A�j��
	void bomb5::b5_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	//�f�X�g���N�^
	bomb5::~bomb5()
	{

	}
	//�U�ߔ���X�V
	void bomb5::b5_setoffense()
	{
		SetRect(&offenseRect, (int)(ix() - bomb5_w / 2), (int)(iy() - bomb5_h / 4), (int)(ix() + bomb5_w / 2), (int)(iy() + bomb5_h / 2));
	}
	
	void bomb5::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//�v���C���[�̃I�u�W�F�N�g���擾
		auto p = getPlayerPrt();
		//�v���C���[�̂����ݒ�ɂ��U���͕ω�����
		atk_ = p->playerMaxHp() * atk_times;
		//�v���C���[�̂����ݒ�ɂ��U���͕ω����Ȃ�
//		atk_ = atk_ * atk_using;
		if (atk_show)
		{
			p->p_receiving_atk(atk_);
			atk_show = false;
		}
	}

	void bomb5::render()
	{
		MovableObject::render();
		//����`�F�b�Nrect
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void bomb5::update()
	{
		MovableObject::update();
		b5_anim();
		b5_setoffense();
	}
	//��ʔ����A�j��
	//bomb6_x , bomb6_y  �A�j���ʒu
	//bomb6_s    �摜�g��k���̔{��
	bomb6::bomb6(const std::string& objectName, const float& bomb6_x, const float& bomb6_y, const float& bomb6_s) :
		bomb(objectName)
	{
		//�摜�̃T�C�Y���擾
		bomb6_w = graph::Draw_GetImageWidth("all_explosion") / 28;
		bomb6_h = graph::Draw_GetImageHeight("all_explosion");
		//������
		setScale(Vec3f(bomb6_s, bomb6_s, 0.f));
		setPos(Vec3f(bomb6_x, bomb6_y, 0.1f));
		setSrcSize(Vec3f((float)bomb6_w, (float)bomb6_h, 0.f));
		resname_ = "all_explosion";
		//�A�j��
		charabase::SetAnim(anim_, 29, 0.4f);
		animMax_ = 28;
		//SE
		se::DSound_Play("all_explosion");
	}
	//�f�X�g���N�^
	bomb6::~bomb6()
	{

	}
	//�A�j��
	void bomb6::b6_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}

	void bomb6::render()
	{
		MovableObject::render();
	}

	void bomb6::update()
	{
		MovableObject::update();
		b6_anim();
	}
	//�w���R�v�^�[�U�������A�j��
	bomb7::bomb7(const std::string& objectName, const float& bomb7_x, const float& bomb7_y, const float& bomb7_s) :
		bomb(objectName)
	{
		//�摜�̃T�C�Y���擾
		bomb7_w = graph::Draw_GetImageWidth("bullet_explosion") / 5;
		bomb7_h = graph::Draw_GetImageHeight("bullet_explosion");
		//������
		setScale(Vec3f(bomb7_s, bomb7_s, 0.f));
		setPos(Vec3f(bomb7_x, bomb7_y, 0.1f));
		setSrcSize(Vec3f((float)bomb7_w, (float)bomb7_h, 0.f));
		resname_ = "bullet_explosion";
		//�A�j��
		charabase::SetAnim(anim_, 6, 0.4f);
		animMax_ = 5;
	}
	//�f�X�g���N�^
	bomb7::~bomb7()
	{

	}
	//�A�j��
	void bomb7::b7_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	void bomb7::render()
	{
		MovableObject::render();
	}

	void bomb7::update()
	{
		MovableObject::update();
		b7_anim();
	}

	//�w���R�v�^�[�Q�������e�A�j��
	bomb8::bomb8(const std::string& objectName, const float& bomb8_x, const float& bomb8_y, const float& bomb8_s) :
		bomb(objectName)
	{
		//�摜�̃T�C�Y���擾
		bomb8_w = graph::Draw_GetImageWidth("tk_explosion") / 7;
		bomb8_h = graph::Draw_GetImageHeight("tk_explosion");
		//������
		setScale(Vec3f(bomb8_s, bomb8_s, 0.f));
		setPos(Vec3f(bomb8_x, bomb8_y, 0.1f));
		setSrcSize(Vec3f((float)bomb8_w, (float)bomb8_h, 0.f));
		resname_ = "tk_explosion";
		//�A�j��
		charabase::SetAnim(anim_, 8, 0.4f);
		animMax_ = 7;
	}
	//�f�X�g���N�^
	bomb8::~bomb8()
	{

	}
	//�A�j��
	void bomb8::b8_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8};

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no == 4)
		{
//			insertAsChild(new e_shot6_atk());
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	void bomb8::render()
	{
		MovableObject::render();
	}

	void bomb8::update()
	{
		MovableObject::update();
		b8_anim();
	}
}