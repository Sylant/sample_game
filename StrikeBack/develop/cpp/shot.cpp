#include "../../lib/gplib.h"
#include "movableObject.h"
#include "shot.h"
#include "Player.h"
#include "Enemy.h"
#include "bomb.h"
#include "effect.h"
#include "Stage.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;

	extern float p_posY;      //�v���C���[����̒l���󂯂�

	shot::shot(const string& objectName) :
		MovableObject(DrawObjf(
			objectName, Vec3f::zero(), Vec3f::one(), objectName, Vec3f::zero(), Vec3f::zero(), Color(0xFFFFFFFF), 0))
	{

	}

	void shot::render()
	{
		MovableObject::render();
	}

	void shot::update()
	{

	}

	//�v���C���[�̃I�u�W�F�N�g���擾
	shared_ptr<Player> shot::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//�v���C���[�̒l��Ԃ�
		return player;
	}
	//�G�l�~�[�[�̃I�u�W�F�N�g���擾
	shared_ptr<Enemy> shot::getBossPrt()
	{
		auto object = getObjectFromRoot({ "boss" });
		shared_ptr<game::Enemy> boss = ci_ext::weak_to_shared<game::Enemy>(object);
		//�G�l�~�[�̒l��Ԃ�
		return boss;
	}
	//�X�e�[�W�̃I�u�W�F�N�g���擾
	shared_ptr<Stage> shot::getStagePrt()
	{
		auto object = getObjectFromRoot({ "stage" });
		shared_ptr<game::Stage> stage_ = ci_ext::weak_to_shared<game::Stage>(object);
		//�X�e�[�W�̒l��Ԃ�
		return stage_;
	}


	//�v���C���[�V���b�g�@��ʍU��
	//posX,posY       �������ˈʒu
	//targetX,targetY �}�E�X�ʒu
	//cnt	          �A�ː�
	//angle           �p�x
	p_shot1::p_shot1(const string& objectName, float posX, float posY, float targetX, float targetY, float cnt, float angle) :
		shot(objectName)
	{
		//�摜�ǂݍ���
		graph::Draw_LoadObject("p_bullet", "res/gra/Player/bullet.png");
		//�摜�̃T�C�Y���擾
		p_bullet_w = graph::Draw_GetImageWidth("p_bullet");
		p_bullet_h = graph::Draw_GetImageHeight("p_bullet");
		//������
		setPos(posX, posY, 0.1f);
		setScale(Vec3f(0.7f, 0.7f, 0.f));
		setSrcSize(Vec3f((float)p_bullet_w, (float)p_bullet_h, 0.f));
		resname_ = "p_bullet";

		//�ړ����x
		shot_speed = 4.f + cnt;
		//���ˊp�x
		degree_ = math::DegreeOfPoints2(pos().x(), pos().y(), targetX, targetY);
		//�e�̑��x
		velocity_.x(math::ROUND_X(-degree_ + angle, shot_speed, 0.f));
		velocity_.y(math::ROUND_Y(-degree_ + angle, shot_speed, 0.f));
		//�����蔻��
		SetRect(&offenseRect, 0, 0, (int)((float)p_bullet_w * 0.7f), (int)((float)p_bullet_h * 0.7f));  //�U�ߔ����
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
	}
	//��ʊO�Ȃ����
	void p_shot1::ps_out()
	{
		if (pos().x() > system::WINW || pos().x() < 0) kill();
		if (pos().y() < 0) kill();
		if (pos().y() > p_posY + 63 / 2 )
		{
			insertAsChild(new bomb7("bullet_exlode", pos().x(), pos().y(), 2.f));
			kill();
		}
	}
	//�U������X�V
	void p_shot1::ps_setoffense()
	{
		//�p�x�ɂ�蔻��X�V
		//�摜�̃T�C�Y��傫���ݒ�ɂ��ύX
		if (degree_ < 45.f || degree_ > 135.f)
			SetRect(&offenseRect, (int)(ix() - (p_bullet_w / 2) * 0.7f), (int)(iy() - (p_bullet_h / 2) * 0.7f),
				(int)(ix() + (p_bullet_w / 2) * 0.7f), (int)(iy() + (p_bullet_h / 2) * 0.7f));
		else if ((degree_ > 45.f && degree_ < 135.f))
			SetRect(&offenseRect, (int)(ix() - (p_bullet_h / 2) * 0.7f), (int)(iy() - (p_bullet_w / 2) * 0.7f),
				(int)(ix() + (p_bullet_h / 2) * 0.7f), (int)(iy() + (p_bullet_w / 2) * 0.7f));
	}
	//�����������Ă�
	void p_shot1::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		insertToParent(new bomb7("bullet_exlode", pos().x(), pos().y(), 2.f));
		kill();
	}

	void p_shot1::render()
	{
		MovableObject::render();
		//����`�F�b�NRECT
//		gplib::graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void p_shot1::update()
	{
		MovableObject::update();
		ps_out();
		ps_setoffense();
	}

	//�G�l�~�[�V���b�g�P�@���e����
	e_shot1::e_shot1(const std::string& objectName, float posX, float posY , float speed) :
		shot(objectName)
	{
		//�摜�ǂݍ���
		graph::Draw_LoadObject("e_missiles", "res/gra/Enemy/e_missile.png");
		//�摜�̃T�C�Y���擾
		shot1_w = graph::Draw_GetImageWidth("e_missiles") / 4;
		shot1_h = graph::Draw_GetImageHeight("e_missiles");
		//������
		setPos(Vec3f(posX, posY, 0.25f));
		setScale(Vec3f(1.f, 1.f, 0.f));
		setSrcSize(Vec3f((float)shot1_w, (float)shot1_h, 0.f));
		resname_ = "e_missiles";
		//�ړ����x
		velocity_.y(speed);
		//�A�j��
		charabase::SetAnim(anim_, 4, 0.1f);
		//�U�ߔ���
		SetRect(&offenseRect, 0, 0, shot1_w, shot1_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
        //�U���������񂾂���
		s1_show = true;
	}
	//�U������X�V
	void e_shot1::s1_setoffense()
	{
		SetRect(&offenseRect, ix() - shot1_w / 2, iy() - shot1_h / 2, ix() + shot1_w / 2, iy() + shot1_h / 2);
	}
	//�n�ʓ���������Ă�
	void e_shot1::s1_out()
	{
		if (y() > p_posY + 63 / 2)
		{
			insertToParent(new bomb1("e_explode", pos().x(), pos().y() - 105.f / 1.15f, 1.8f));
			kill();
		}
	}
	//�A�j��
	void e_shot1::s1_anim()
	{
		int t[] = { 0, 1, 2, 3, 4 };

		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
	}
	//�v���C���[�ɓ���������Ă�
	void e_shot1::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		auto p = getPlayerPrt();
		if (s1_show)
		{
			p->p_receiving_atk(10);
			s1_show = false;
		}
	}

	void e_shot1::render()
	{
		MovableObject::render();
		//����`�F�b�NRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void e_shot1::update()
	{
		MovableObject::update();
		s1_anim();
		s1_out();
		s1_setoffense();
	}

	//�G�l�~�[�V���b�g�Q �~�j�G�l�~�[
	e_shot2::e_shot2(const string& objectName, float posX, float posY, int num) :
		shot(objectName)
	{
		//�摜�ǂݍ���
		graph::Draw_LoadObject("mini", "res/gra/Enemy/miniboss.png", 0x00000000);
		//�摜�̃T�C�Y���擾
		shot2_w = graph::Draw_GetImageWidth("mini") / 23;
		shot2_h = graph::Draw_GetImageHeight("mini") / 3;
		setScale(Vec3f(1.2f, 1.2f, 0.f));
		setSrcSize(Vec3f((float)shot2_w, (float)shot2_h, 0.f));
		
		resname_ = "mini";
		//�A�j���ݒ�
		charabase::SetAnim(anim_, 22, 0.15f);
		animMax_ = 6;
		//�󂯔���
		SetRect(&defenseRect, 0, 0, (int)((float)shot2_w * 1.2f), (int)((float)shot2_h * 1.2f));
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//���Ԃɗ�������
		posX_ = posX + (num * 35.f);
		//�������x
		s2_speed = 1.f + (float)num / 3.f;
		//�����ʒu���Z�b�g����
		setPos(Vec3f(posX_, posY, 0.35f));
		//������
		state_ = s2_mode::NORMAL; //�����͈�ʏ��
		
		s2_speed_ = 2.f;          //���������ړ�����X�s�[�h
		atk_count = 3;            //�U���J�E���g��
		down_ = true;             //��������
		atk_show = true;          //�U����Ԃ̔���
		bomb2_show = true;        //�����A�j������񂾂��ɌĂ�
		hp_ = 6;                  //hp
		num_ = num;               //�Ăт������V���b�g�Q�̐�
		frame_ = 0;               //���Z����t���[����������
	}
	//�~�j�G�l�~�[�ړ�
	void e_shot2::s1_move()
	{
		//�{�X�̃I�u�W�F�N�g���Ƃ�
		auto b = getBossPrt();
		//�v���C���[�̃I�u�W�F�N�g���Ƃ�
		auto p = getPlayerPrt();
		//��ԕω��ɂ�胂�[�h�ω�
		switch (state_)
		{
		//��ʃ��[�h
		//�������邾���Ă�
		case s2_mode::NORMAL:
		{
			velocity_.y(s2_speed);
			if (y() > 300.f && down_)
			{
				state_ = s2_mode::MOVE;
				down_ = false;
			}
		}
		    break;
		//�ړ����[�h
		//�v���C���[�̈ړ��ɂ��ړ�����
		case s2_mode::MOVE:
		{
			velocity_.y(0.f);
			if (pos().x() > p->pos().x())
			{
				velocity_.x(-s2_speed_);
				//�ʒu�̓v���C���[���120.f�̋����Œ�~���āA�U������
				if (pos().x() + (float)((num_)* 30.f) < p->pos().x() + 120.f)
				{
					velocity_.x(0.f);
				}
			}
			else if (pos().x() < p->pos().x())
			{
				velocity_.x(s2_speed_);
				if (pos().x() - (float)((num_)* 30.f) > p->pos().x() - 120.f)
				{
					velocity_.x(0.f);
				}
			}
			//�m���ōU�����[�h�ɕω�
			if (math::GetRandom(0, 100) == 0)
			{
				state_ = s2_mode::ATTACK;
			}
		}
		    break;
		//�U�����[�h
		case s2_mode::ATTACK:
		{
			velocity_.x(0.f);

			if (atk_show && atk_count > 0)
			{
				insertAsChild(new e_shot2_atk("lightning", p->pos().x()));
				atk_show = false;
				//�J�E���g�Ōv���A�U����3��܂�
				--atk_count;
			}

			//�A���U���s�ɐݒ�A��x�ړ����[�h�ɖ߂�
			if (!atk_show && (++frame_ / 60 == math::GetRandom(2, 5)))
			{
				state_ = s2_mode::MOVE;
				atk_show = true;
				frame_ = 0;
			}
			//3��U�����I���ƁA�Ăї�������
			if (atk_show && atk_count == 0)
			{
				velocity_.x(0.f);
				velocity_.y(s2_speed_);
				//�ǌ����[�h�ɕω�
				if (y() > p_posY - 10.f)
				{
					state_ = s2_mode::SUICIDE;
				}
			}
		}
		break;
		//�ǌ����[�h
		case s2_mode::SUICIDE:
		{
			velocity_.x(s2_speed_ * 1.5f);
			velocity_.y(0.f);
			//�����x�ɐݒ�
			++frame_;
			if (frame_ < math::GetRandom(120, 180))
			{
				if (pos().x() > p->pos().x())
				{
					velocity_.x(-s2_speed_ * velocity_.x());
				}
				else if (pos().x() < p->pos().x())
				{
					velocity_.x(s2_speed_ * velocity_.x());
				}
			}
			else
			{
				//�J�E���g�͑����΁A���Ń��[�h��
				state_ = s2_mode::DIED;
			}
		}
		break;
		//���Ń��[�h
		//�����O��hp��0�ȉ��ɂȂ鎞�ɏ���
		case s2_mode::DIED:
		{
			velocity_.x(0.f);
			velocity_.y(0.f);
			//���ł��鎞�ɑ��݂��鐔���]�P
			b->s2_count(1);
			if (hp_ > 0)
			{
				//���ɐ��䂵�Ȃ��ƃo�O����������\��������
				if (bomb2_show)
				{
					insertToParent(new bomb2("bomb2", pos().x(), p_posY - 32.f / 1.3f, 2.f));
					bomb2_show = false;
				}
			}
		}
		    break;
		}
	}
	//�A�j��
	void e_shot2::s1_anim()
	{
		//���[�h�ʂ��A�j��
		int t[] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
		int t2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9 };
		int t3[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };
		int t4[] = { 0, 1, 2, 3, 4, 5, 6 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (state_ != s2_mode::NORMAL && state_ != s2_mode::ATTACK && state_ != s2_mode::DIED)
		{
			setSrc(Vec3f((float)t2[anim_.no], 0.f, 0.f));
		}
		else if (state_ == s2_mode::ATTACK)
		{
			setSrc(Vec3f((float)t3[anim_.no], 1.f, 0.f));
		}
		else if (state_ == s2_mode::DIED)
		{
			setSrc(Vec3f((float)t4[anim_.no], 2.f, 0.f));
			if (anim_.no >= animMax_)
			{
				if (hp_ <= 0) insertToParent(new bomb6("all_explode", pos().x(), pos().y(), math::GetRandom(1.f, 1.5f)));
				kill();
			}
		}
	}
	//�󂯏�ԍX�V
	void e_shot2::s1_setdefense()
	{
		//�󂯏�ԍX�V
		SetRect(&defenseRect, (int)(ix() - (float)shot2_w * 1.2f / 2.f), (int)(iy() - (float)shot2_h * 1.2f / 2.f), 
			(int)(ix() + (float)shot2_w * 1.2f / 2.f), (int)(iy() + (float)shot2_h * 1.2f / 2.f));
	}
	
	void e_shot2::render()
	{
		MovableObject::render();
        //����`�F�b�NRECT
//		graph::Draw_CkRect(defenseRect, ARGB(255, 255, 0, 0));
	}
	//���ꂽ���ɌĂ�
	void e_shot2::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		--hp_;
		if (hp_ < 0)
		{
			state_ = s2_mode::DIED;
			auto s = getStagePrt();
			s->e_kill(1);
		}
	}

	void e_shot2::update()
	{
		MovableObject::update();

		s1_anim();
		s1_move();
		s1_setdefense();
	}

	//�~�j�U���@��
	e_shot2_atk::e_shot2_atk(const string& objectName, float posX) :
		shot(objectName)
	{
		//�摜�ǂݍ���
		graph::Draw_LoadObject("lightning", "res/gra/Enemy/lightning.png");
		//�摜�̃T�C�Y���擾
		s2_atk_w = graph::Draw_GetImageWidth("lightning") / 9;
		s2_atk_h = graph::Draw_GetImageHeight("lightning");
		//������
		setScale(Vec3f(1.2f, 1.42f, 0.f));
		setSrcSize(Vec3f((float)s2_atk_w, (float)s2_atk_h, 0.f));
		setPos(Vec3f(posX, p_posY - 100.f, 0.4f));
		resname_ = "lightning";
		//�A�j��
		charabase::SetAnim(anim_, 10, 0.2f);
		animMax_ = 9;
		//�󂯔���Ȃ�
		SetRect(&offenseRect, 0, 0, (int)((float)s2_atk_w * 1.2f), (int)((float)s2_atk_h * 1.42f));
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//�U���������񂾂���
		s2_atk_show = true;
		//SE
	    se::DSound_Play("light");
		//�U���͏�����
		atk_ = 0.f;
		//�v���C���[�̍ő傈���ɂ��U���{��
		atk_times = 0.03f;

	}

	void e_shot2_atk::render()
	{
		MovableObject::render();
		//����`�F�b�NRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}
	//�U�߂����ɌĂ�
	void e_shot2_atk::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		//�v���C���[�̃I�u�W�F�N�g���擾
		auto p = getPlayerPrt();
		//�v���C���[�̂����ݒ�ɂ��U���͕ω�����
		atk_ = p->playerMaxHp() * atk_times;
		//�v���C���[�̂����ݒ�ɂ��U���͕ω����Ȃ�
//		atk_ = atk_ * atk_using;
		if (s2_atk_show)
		{
			p->p_receiving_atk(atk_);
			s2_atk_show = false;
		}
	}
	//�A�j��
	void e_shot2_atk::s2a_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}
	//�U�ߔ���X�V
	void e_shot2_atk::s2a_setoffense()
	{
		//�U������X�V
		SetRect(&offenseRect, (int)(ix() - (float)s2_atk_w * 1.2f / 2.f), (int)(iy() - (float)s2_atk_h * 1.42f / 2.f), 
			(int)(ix() + (float)s2_atk_w * 1.2f / 2.f), (int)(iy() + (float)(s2_atk_h - 9) * 1.42f / 2.f));
	}

	void e_shot2_atk::update()
	{
		MovableObject::update();
		s2a_anim();
		s2a_setoffense();
	}
	//�G�l�~�[�V���b�g�R�@�n��
	e_shot3::e_shot3(const string& objectName) :
		shot(objectName)
	{
		//�摜�̓ǂݍ���
		graph::Draw_LoadObject("landmine", "res/gra/Enemy/landmine.png");
		//�摜�̃T�C�Y���擾
		shot3_w = graph::Draw_GetImageWidth("landmine") / 2;
		shot3_h = graph::Draw_GetImageHeight("landmine");
		//������
		setScale(Vec3f(1.0f, 1.0f, 0.f));
		setSrcSize(Vec3f((float)shot3_w, (float)shot3_h, 0.f));
		resname_ = "landmine";
		//�A�j��
		charabase::SetAnim(anim_, 2, 0.1f);
		animMax_ = 1;
		//�U�߂Ǝ󂯔���
		SetRect(&offenseRect, 0, 0, shot3_w, shot3_h);
		SetRect(&defenseRect, 0, 0, shot3_w, shot3_h);

		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//�����̓����_���ŏo��
		posX_ = math::GetRandom(0.f + (float)shot3_h / 2.f, (float)system::WINW - (float)(shot3_w - 15) / 2.f);
		setPos(Vec3f(posX_, p_posY + (float)shot3_h, 0.2f));
		//hp
		hp_ = 3;
		//���Z����t���[����������
		frame_ = 0;
	}
	//�A�j��
	void e_shot3::s3_anim()
	{
		int t[] = { 0,1 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		//�J�E���g���Ŏ��������ɐݒ�
		if (++frame_ / 60 == math::GetRandom(8, 12))
		{
			kill();
			insertToParent(new bomb4("bomb4", pos().x(), p_posY - (float)shot3_h / 2, 1.0f));
		}
	}
	//�T�C�Y��菬���߂ɔ���͈͂�ݒ肷��
	//�󂯔���X�V
	void e_shot3::s3_setdefense()
	{
		SetRect(&defenseRect, ix() - shot3_w - 5 / 2, iy() - shot3_h - 5 / 2, ix() + shot3_w - 5 / 2, iy() + shot3_h - 5 / 2);
	}
	//�U�ߔ���X�V
	void e_shot3::s3_setoffense()
	{
		SetRect(&offenseRect, ix() - shot3_w - 15 / 2, iy() - shot3_h - 5 / 2, ix() + shot3_w - 15 / 2, iy() + shot3_h - 5 / 2);
	}
	//���蔭��������A�����A�j����
	void e_shot3::offenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		kill();
		insertToParent(new bomb4("bomb4", pos().x(), p_posY - 30.f / 2, 1.0f));
	}
	//���ꂽ���ɌĂ�
	void e_shot3::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		--hp_;
		if (hp_ < 0)
		{
			insertToParent(new bomb6("bomb6", pos().x(), y() - 40.f, math::GetRandom(1.5f, 2.f)));
			auto s = getStagePrt();
			s->e_kill(1);
			kill();
		}
	}

	void e_shot3::render()
	{
		MovableObject::render();
		//����`�F�b�NRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 255, 0, 0));
	}

	void e_shot3::update()
	{
		MovableObject::update();
		s3_anim();
		s3_setdefense();
		s3_setoffense();
	}
	//�G�l�~�[�V���b�g�S�@UFO
	e_shot4::e_shot4(const string& objectName) :
		shot(objectName)
	{
		//�摜�̓ǂݍ���
		graph::Draw_LoadObject("ufo", "res/gra/Enemy/miniufo.png");
		//�摜�̃T�C�Y���擾
		shot4_w = graph::Draw_GetImageWidth("ufo") / 12;
		shot4_h = graph::Draw_GetImageHeight("ufo");
		setScale(Vec3f(1.3f, 1.3f, 0.f));
		setSrcSize(Vec3f((float)shot4_w, (float)shot4_h, 0.f));
		resname_ = "ufo";
		//�A�j���ݒ�
		charabase::SetAnim(anim_, 12, 0.4f);
		animMax_ = 11;
		//�󂯔���͈͐ݒ�
		SetRect(&defenseRect, 0, 0, shot4_w, shot4_h);
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//�����͈�ʏ�Ԃ�
		state_ = s4_mode::NORMAL;
		//�m���ŏo��
		posX_ = (float)math::GetRandom((int)((float)shot4_w * 1.3f / 2.f), system::WINW - (int)((float)shot4_w * 1.3f / 2.f));
		//�����ʒu
		setPos(Vec3f(posX_, 220.f, 0.2f));
		//������
		appear_show = false;              //����邩�ǂ���
		atk_show = true;                  //�U������
		frame_ = 0;                       //������܂łɃt���[����������
		alpha_ = 0;                       //�����͓�����
		setColor(alpha_, 255, 255, 255);  //ARGB
		hp_ = 6;                          //hp
	}
	//UFO�ړ�
	void e_shot4::s4_move()
	{
		switch (state_)
		{
		case s4_mode::NORMAL:
		{
			alpha_ += 5;
			if (alpha_ >= 255)
			{
				alpha_ = 255;
				//�����x255���肽��A�U����Ԃ�
				state_ = s4_mode::ATTACK;
			}
		}
		    break;
		case s4_mode::ATTACK:
		{
			//�U����񂾂�
			if (atk_show)
			{
				insertAsChild(new e_shot4_atk("ufo_atk", pos().x(), pos().y()));
				atk_show = false;
			}
			//�t���[�����Ŏ���
			if (++frame_ / 60 == math::GetRandom(2, 4))
			{
				appear_show = true;
			}
			//������
			if (appear_show)
			{
				alpha_ -= 5;
				if (alpha_ <= 0)
				{
					kill();
				}
			}
		}
		    break;
		}
	}
	//�A�j��
	void e_shot4::s4_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
	}
	//�󂯔���X�V
	void e_shot4::s4_setdefense()
	{
		SetRect(&defenseRect, (int)(ix() - (float)shot4_w * 1.3f / 2.f), (int)(iy() - (float)shot4_h * 1.3f / 2.f),
			(int)(ix() + (float)shot4_w * 1.3f / 2.f), (int)(iy() + (float)shot4_h * 1.3f / 2.f));
	}

	void e_shot4::render()
	{
		MovableObject::render();
        //����`�F�b�NRECT
//		graph::Draw_CkRect(defenseRect, ARGB(255, 0, 0, 255));
	}
	//���ꂽ���ɌĂ�
	void e_shot4::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		--hp_;
		if (hp_ < 0)
		{
			insertToParent(new bomb6("all_explode", pos().x(), pos().y(), math::GetRandom(2.f, 2.5f)));
			auto s = getStagePrt();
			s->e_kill(1);
			kill();
		}
	}

	void e_shot4::update()
	{
		MovableObject::update();
		//��xARGB���Z�b�g����
		setColor(alpha_, 255, 255, 255);

		s4_anim();
		s4_move();
		s4_setdefense();
	}
	//UFO�U��
	e_shot4_atk::e_shot4_atk(const string& objectName, float posX, float posY) :
		shot(objectName)
	{
		//�摜�̓ǂݍ���
		graph::Draw_LoadObject("ufo_atk", "res/gra/Enemy/miniufo_attack.png");
		//�摜�̃T�C�Y���擾
		s4_atk_w = graph::Draw_GetImageWidth("ufo_atk") / 9;
		s4_atk_h = graph::Draw_GetImageHeight("ufo_atk");
		//������
		setScale(Vec3f(1.3f, 1.3f, 0.f));
		setSrcSize(Vec3f((float)s4_atk_w, (float)s4_atk_h, 0.f));
		resname_ = "ufo_atk";
		//�A�j���ݒ�
		charabase::SetAnim(anim_, 9, 0.3f);
		animMax_ = 8;
		//�����ʒu
		setPos(Vec3f(posX, posY + 75.f, 0.25f));
	}
	//�A�j��
	void e_shot4_atk::s4a_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
		//7�t���[���̃A�j���ɒe�𔭎˂���
		else if (anim_.no == 7)
		{
			insertToParent(new bomb5("bomb5", pos().x() - 1.f, pos().y() + 50.f, 1.3f));
		}
		else if (anim_.no >= animMax_)
		{
			kill();
		}
	}

	void e_shot4_atk::render()
	{
		MovableObject::render();
	}

	void e_shot4_atk::update()
	{
		MovableObject::update();
		s4a_anim();
	}

	//�G�l�~�[�V���b�g�T�@�w���R�v�^�[
	e_shot5::e_shot5(const string& objectName) :
		shot(objectName)
	{
		//�摜�̓ǂݍ���
		graph::Draw_LoadObject("heli", "res/gra/Enemy/helicopter.png");
		//�摜�̃T�C�Y���擾
		shot5_w = graph::Draw_GetImageWidth("heli") / 4;
		shot5_h = graph::Draw_GetImageHeight("heli");
		//������
		setScale(Vec3f(1.f, 1.f, 0.f));
		setSrcSize(Vec3f((float)shot5_w, (float)shot5_h, 0.f));
		resname_ = "heli";
		//�A�j���ݒ�
		charabase::SetAnim(anim_, 4, 0.8f);
		//�{�͍̂U������Ȃ�
		SetRect(&defenseRect, 0, 0, shot5_w, shot5_h);
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//�m���ŏo���@�����ʒu�͉�ʊO
		posX_ = (float)math::GetRandom(shot5_w / 2, system::WINW - shot5_w / 2);
		setPos(Vec3f(posX_, -10.f, 0.2f));
		//�������[�h
		state_ = s5_mode::NORMAL;
		//������
		atk_show = false;       //�U�����
		frame_ = 0;             //�����Ə㏸�̃t���[����
		moving_speed = 0.6f;    //�ړ����x
		hp_ = 6;                //hp
	}
	//�w���R�v�^�[�ړ�
	void e_shot5::s5_move()
	{
		velocity_.y(moving_speed);

		switch (state_)
		{
		case s5_mode::NORMAL:
		{
			//�ݒ�ʒu�ɒ�������A�U�����[�h��
			if (y() > 50.f)
			{
				state_ = s5_mode::ATTACK;
				atk_show = true;
			}
		}
		break;
		case s5_mode::ATTACK:
		{
			velocity_.y(0.f);
			//�J�E���g���ōU�����Z�b�g����
			frame_++;
			//60�t���[�����U��
			if (frame_ == 120 || frame_ == 180)
			{
				atk_show = true;
			}
			//�U��3��@8�� x 3
			if (atk_show)
			{
				se::DSound_Play("e_bullet");
				for (int i = 0;i <= 6;++i)
				{
					insertAsChild(new e_shot5_atk("e_bullet", pos().x(), pos().y(), i * 15.f));
				}
				atk_show = false;
			}
			//420�t���[����A�㏸����
			else if (frame_ >= 480)
			{
				velocity_.y(-1.f);
				if (y() < 0.f)
				{
					kill();
				}
			}
		}
		    break;
		}
	}
	//�A�j��
	void e_shot5::s5_anim()
	{
		int t[] = { 0, 1, 2, 3, 4 };
		if (charabase::StepAnim(anim_))
		{
			setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
		}
	}
	//�󂯔���X�V
	void e_shot5::s5_setdefense()
	{
		SetRect(&defenseRect, (int)(ix() - shot5_w / 2), (int)(iy() - shot5_h / 2),
			(int)(ix() + shot5_w / 2), (int)(iy() + shot5_h / 2));
	}

	void e_shot5::render()
	{
		MovableObject::render();
        //����`�F�b�NRECT
//		graph::Draw_CkRect(defenseRect, ARGB(255, 0, 0, 255));
	}
	//���ꂽ���ɌĂ�
	void e_shot5::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		--hp_;
		if (hp_ < 0 )
		{
			insertToParent(new bomb6("bomb6", pos().x(), pos().y(), math::GetRandom(2.f, 2.5f)));
			auto s = getStagePrt();
			s->e_kill(1);
			kill();
		}
	}

	void e_shot5::update()
	{
		MovableObject::update();

		s5_move();
		s5_anim();
		s5_setdefense();
	}
	//�w���R�v�^�[�U��
	e_shot5_atk::e_shot5_atk(const string& objectName, float posX, float posY, float angle) :
		shot(objectName)
	{
		//�摜�̓ǂݍ���
		graph::Draw_LoadObject("e_bullet", "res/gra/Enemy/bullet.png");
		//�摜�̃T�C�Y���擾
		s5_atk_w = graph::Draw_GetImageWidth("e_bullet");
		s5_atk_h = graph::Draw_GetImageHeight("e_bullet");
		//������
		setScale(Vec3f(1.f, 1.f, 0.f));
		setSrcSize(Vec3f((float)s5_atk_w, (float)s5_atk_h, 0.f));
		resname_ = "e_bullet";
		setPos(Vec3f(posX, posY + 30.f, 0.25f));
		//�U���p�^���ϊ��ł���

/*      //�v���C���[�̈ʒu�ɍU������
		//�v���C���[�̈ʒu�ɑ΂���p�x�����߂�
		targetX_ = math::DegreeOfPoints2(x(), y(), player_posX, player_posY);
		targetY_ = -math::DegreeOfPoints2(x(), y(), plyaer_posX, player_posY);
        //�p�x�ɑ΂��Ă�X,Y�����߂�
		velocity_.x(math::ROUND_X(targetX_, speed_, 0.f));
		velocity_.y(math::ROUND_Y(targetY_, speed_, 0.f));
*/
		//�ړ����x
		atk_speed = 3.f;
		//�e�̈ړ��C��
		velocity_.x(math::ROUND_X(angle + 45.f, atk_speed, 0.f));
		velocity_.y(math::ROUND_Y(angle + 45.f, atk_speed, 0.f));
		//�U������
		SetRect(&offenseRect, 0, 0, s5_atk_w, s5_atk_h);
		OffsetRect(&offenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());
		//�U���͏�����
		atk_ = 0.f;
		//�v���C���[�̍ő傈���ɂ��U���{��
		atk_times = 0.02f;
		//�U���������񂾂���
		atk_show = true;
	}

	void e_shot5_atk::s5a_out()
	{
		//�n�ʂɂԂ�������A�����A�j�߂�
		if (y() > p_posY + 28.f)
		{
			insertToParent(new bomb7("bullet_exlode", pos().x(), pos().y(), 2.f));
			kill();
		}
		//��ʊO�Ȃ����
		if (pos().x() > system::WINW - s5_atk_w / 2) kill();
		else if (pos().x() < 0 + s5_atk_w / 2) kill();
	}
	//�U�ߔ���X�V
	void e_shot5_atk::s5a_setoffense()
	{
		SetRect(&offenseRect, (int)(ix() - s5_atk_w / 2), (int)(iy() - s5_atk_h / 2), 
			(int)(ix() + s5_atk_w / 2), (int)(iy() + s5_atk_h / 2));
	}

	void e_shot5_atk::render()
	{
		MovableObject::render();
		//����`�F�b�NRECT
//		graph::Draw_CkRect(offenseRect, ARGB(255, 0, 0, 255));
	}
	//�����true�������� �����A�j����
	void e_shot5_atk::offenseHit(std::weak_ptr<ci_ext::Object>&)
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
			insertToParent(new bomb7("bomb7", pos().x(), pos().y(), 2.f));
			atk_show = false;
		}
		kill();
	}

	void e_shot5_atk::update()
	{
		MovableObject::update();
		s5a_out();
		s5a_setoffense();
	}
}