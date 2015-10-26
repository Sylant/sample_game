#include "../../lib/gplib.h"
#include "movableObject.h"
#include "Enemy.h"
#include "bomb.h"
#include "shot.h"
#include "player.h"
#include "stage.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;

	extern float p_posY;  //�v���C���[����̒l���󂯂�

	//�X�e�[�W�̃I�u�W�F�N�g���擾
	shared_ptr<Stage> Enemy::getStagePrt()
	{
		auto object = getObjectFromRoot({ "stage" });
		shared_ptr<game::Stage> stage = ci_ext::weak_to_shared<game::Stage>(object);
		//�X�e�[�W�̒l��Ԃ�
		return stage;
	}

	Enemy::Enemy(const string& objectName) :
		MovableObject(
			DrawObjf(objectName, Vec3f::one(), Vec3f::one(), "boss", Vec3f::zero(), Vec3f(230.f, 185.f, 0.f), Color(0xFFFFFFFF), 0))
	{
		//�摜�̃T�C�Y���擾
		boss_w = graph::Draw_GetImageWidth("boss") / 11;
		boss_h = graph::Draw_GetImageHeight("boss") / 4;
		//������
		setPos(Vec3f((float)(system::WINW / 2), (float)boss_w / 2, 0.3f));
		resname_ = "boss";
		//�A�j��
		charabase::SetAnim(anim_, 17, 0.1f);
		//�{�̍U������Ȃ�
		SetRect(&defenseRect, 0, 0, boss_w, boss_h);
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());

		state_ = e_mode::NORMAL;           //��ԏ�����
		
		if (math::GetRandom(0, 1) == 0)    //�������E���������_���Ō��߂�
			dir_ = e_Dir::LEFT;
		else 
			dir_ = e_Dir::RIGHT;
		
		frame_ = 0;                        //�t���[���� �U����Ԃƈ�ʏ�Ԃ̕ω�
		s1_probability = 400;              //�X�e�[�W�������U���̍ő�m��
		init_max_probability = 200;        //�����U�����[�h�ɓ���ő�m��
		max_probability =   0;             //�U�����[�h�ɓ���ő�m��
		max_probability_times = 0.4f;      //�Œ�m�����m�񂽂߂̍ő�m���̔{����
		init_moving_speed = 1.f;           //�����ړ����x
		moving_speed = 0.f;                //�ړ����x������
		get_s2_count = 0;                  //�V���b�g�Q�J�E���g�@������
		is_s2_count = 3;                   //�V���b�g�Q�J�E���g�@�ő吔+3
		shot_count = 3;                    //�V���b�g�͈��ŉ����Ă΂�邩���䂷��
		Max_hp = 300.f;                    //�ő傈��
		hp_ = 300.f;                       //�����_�̂���
		previous_hp = 0.f;                 //�O�t���[���̂���������
		result_ = true;                    //�Q�[���N���A���鎞�ɐ���
		atk_show = false;                  //�A���U�������Ȃ�
		died_show = false;                 //�N���A���Ɉꕔ�𐧌䂷��
		hide_atk = false;                  //�����B���A�����蔻��͂��̂܂ܑ��݂���
		alpha_ = 0;                        //�����x�O��
		setColor(alpha_, 255, 255, 255);   //�`��F��������
	}
	Enemy::~Enemy()
	{

	}
	//�{�X�ړ�
	void Enemy::e_move()
	{
		//hp�ω��ɂ��ړ����x�̕ω�
		moving_speed = (Max_hp - hp_) * 0.001f + init_moving_speed;

		//�ړ�
		if (dir_ == e_Dir::RIGHT)
		{
			velocity_.x(-moving_speed);
		}
		else if (dir_ == e_Dir::LEFT)
		{
			velocity_.x(moving_speed);
		}
	}
	//��Ԃɂ�蓮��ω�
	void Enemy::e_state()
	{
		//���쐧��
		switch (state_)
		{
		case e_mode::NORMAL:
		{
			//hp�̕ω��ɂ��m���ω�
			if (hp_ == Max_hp)
			{ 
				max_probability = init_max_probability;
				//�����_�����̒l��n��
				previous_hp = hp_;
			}
			else if (hp_ != Max_hp)
			{
				if (hp_ == previous_hp) {}
				else if (hp_ < previous_hp)
				{
					--max_probability;
				}
				else if (hp_ > previous_hp)
				{
					++max_probability;
				}
				//�m�������䂷��
				if (max_probability <= init_max_probability * max_probability_times)
					max_probability = (int)(init_max_probability * max_probability_times);
				else if (max_probability > init_max_probability)
					max_probability = init_max_probability;
				//�����_�����̒l��n��
				previous_hp = hp_;
			}

			//�m���ōU����Ԃɓ���
			if (math::GetRandom(0, max_probability) == 0)
			{
				state_ = e_mode::ATTACK;
				atk_show = true;
			}
			//�m���ŕ����ύX
			else if (math::GetRandom(0, (int)(max_probability + init_max_probability * max_probability_times)) == 0)
			{
				if (dir_ == e_Dir::RIGHT)
				{
					dir_ = e_Dir::LEFT;
				}
				else if (dir_ = e_Dir::LEFT)
				{
					dir_ = e_Dir::RIGHT;
				}
			}
		}
		    break;
		//�U��
		case e_mode::ATTACK:
		{
			velocity_.x(0.f);
			
			if (atk_show)
			{
				//�X�L��2��3 + 3�ȏ㑶�݂���΁A�Ăяo�����Ƃł��Ȃ��ɐݒ�
				if (get_s2_count <= is_s2_count)
				{
					for (int i = 0;i < shot_count;++i)
					{
						insertAsChild(new e_shot2("mini", pos().x(), pos().y(), i));
						++get_s2_count;
					}
				}
				//hp�̕ω��ɂ��O�̃p�^���𕪂��āA�U�����x�ω�����
				if (hp_ >= Max_hp * 0.7f)
				{
					insertAsChild(new e_shot3("landmine"));
					insertAsChild(new e_shot4("ufo"));
					insertAsChild(new e_shot5("heli"));
				}
				else if (hp_ >= Max_hp * 0.3f && hp_ < Max_hp * 0.7f)
				{
					for (int i = 0;i < shot_count - 2;++i)
						insertAsChild(new e_shot3("landmine"));
					for (int i = 0;i < shot_count;++i)
						insertAsChild(new e_shot4("ufo"));
					for (int i = 0;i < shot_count - 2;++i)
						insertAsChild(new e_shot5("heli"));
				}
				else if (hp_ < Max_hp * 0.3f)
				{
					for (int i = 0;i < shot_count - 1;++i)
						insertAsChild(new e_shot3("landmine"));
					for (int i = 0;i < shot_count + 1;++i)
						insertAsChild(new e_shot4("ufo"));
					for (int i = 0;i < shot_count - 1;++i)
						insertAsChild(new e_shot5("heli"));
				}
				atk_show = false;
			}
			//���b���ʏ�Ԃɖ߂�
			//��b���U�O�t���[��
			if (++frame_ / 60 == math::GetRandom(2, 5))
			{
				state_ = e_mode::NORMAL;
				frame_ = 0;
			}
		}
		    break;
		//����
		case e_mode::DIED:
		{
			velocity_.x(0.f);
			velocity_.y(0.f);

			//��񂾂��ĂԂ��Ƃɐ���
			if (result_)
			{
				e_result();
				result_ = false;
			}
		}
		    break;
		}
	}

	//�X�e�[�W�֊֐����Ă�
	void Enemy::e_result()
	{
		auto s = getStagePrt();
		s->result(1);
	}

	//��ʊO�łȂ��悤��
	void Enemy::e_out()
	{
		//��ʊO�o�Ȃ��悤��
		if (pos().x() + velocity_.x() >= float(system::WINW - boss_w / 2)) dir_ = e_Dir::RIGHT;
		else if (pos().x() + velocity_.x() <= (float)boss_w / 2.f) dir_ = e_Dir::LEFT;
	}
	//���E���]
	void Enemy::e_backrun()
	{
		if (dir_ == e_Dir::LEFT && scale().x() != -1)
			setScale(Vec3f(scale().x()*-1, scale().y(), scale().z()));
		if (dir_ == e_Dir::RIGHT && scale().x() != 1)
			setScale(Vec3f(scale().x()*-1, scale().y(), scale().z()));
	}
	//��Ԃɂ��A�j���ω�
	void Enemy::e_anim()
	{
		int t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0 };
		int t2[] = { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 7, 8, 9, 10 };
		int t3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		if (charabase::StepAnim(anim_))
		{
			if (state_ == e_mode::NORMAL)
			{
				setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
			}
			//��U���A�j���m���ŕ\��
			else if (state_ == e_mode::ATTACK)
			{
				if (hp_ >= Max_hp * 0.4f)
				{
					setSrc(Vec3f((float)t[anim_.no], 1.f, 0.f));
					se::DSound_Play("boss_atk");
				}
				else if (hp_ < Max_hp * 0.4f)
				{
					setSrc(Vec3f((float)t2[anim_.no], 2.f, 0.f));
					se::DSound_Play("boss_atk2");
				}
			}
			else if (state_ == e_mode::DIED)
			{
				setSrc(Vec3f((float)t3[anim_.no], 3.f, 0.f));
			}
		}
	}

	//�󂯔���̓{�X�̔]�����ɐ���
	void Enemy::e_setdefense()
	{
		if (dir_ == e_Dir::LEFT)
			SetRect(&defenseRect, (int)(ix() - ((boss_w / 3) / 1.5)), (int)(iy() - (boss_h / 2.5)),
				(int)(ix() + ((boss_w / 3) / 2)), (int)(iy() - boss_h / 10));
		else if (dir_ == e_Dir::RIGHT)
			SetRect(&defenseRect, (int)(ix() - ((boss_w / 3) / 2)), (int)(iy() - (boss_h / 2.5)), 
				(int)(ix() + ((boss_w / 3) / 1.5)), (int)(iy() - boss_h / 10));
	}
	//�����U�� �m���Ŕ��e�𗎉�����
	void Enemy::e_speceatk()
	{
		//���e�̃T�C�Y�ɍ��킹�����A�j���ʒu
		float posX_ = math::GetRandom(0.f + 29.f / 2.f, system::WINW - 29.f / 2.f);
		if (!died_show)
		{
			if (math::GetRandom(0, s1_probability) == 0)
			{
				insertAsChild(new e_shot1("e_missiles", posX_, 0.f + 44.f / 2.f, math::GetRandom(4.f, 8.f)));
			}
		}
	}

	//�X�L���Q����
	void Enemy::s2_count(const int& num)
	{
		num_ = num;
		if (num_ == 1)
		{
			num_ = 0;
			get_s2_count -= 1;
		}
	}
	
	//���ꂽ��Ԃ�l�Ńv���C���[�ɓn��
	bool Enemy::diedshow()
	{
		return died_show;
	}

	//���ꂽ���ɌĂ�
	void Enemy::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		if (hp_ > 0.f)
		{
			//���ꂽ��A���������炷
			hp_ -= Max_hp * 0.005f;
			if (hp_ <= 0.f)
			{
				for (int i = 0;i < 10;++i)
				{
					insertToParent(new bomb6("all_explode", math::GetRandom(pos().x() - (float)boss_w / 2.f, pos().x() + (float)boss_w / 2.f),
						math::GetRandom(pos().y() - (float)boss_h / 2.f, pos().y() + (float)boss_h / 2.f), math::GetRandom(1.5f, 3.f)));
				}
				state_ = e_mode::DIED;
				died_show = true;
			}
		}
	}
	//�����͐g���B���@�����蔻�葶�݂���
	void Enemy::e_hide_it()
	{
		if (Max_hp - hp_ >= Max_hp * 0.15f)
		{
			alpha_ += 30;
			if (alpha_ >= 255) alpha_ = 255;
		}

		setColor(alpha_, 255, 255, 255);
	}

	void Enemy::render()
	{
		MovableObject::render();
        //����`�F�b�NRECT
//		gplib::graph::Draw_CkRect(defenseRect, ARGB(255, 255, 0, 0));
	}

	void Enemy::update()
	{
		MovableObject::update();
		e_anim();
		e_backrun();
		e_move();
		e_out();
		e_state();
		e_hide_it();
		e_speceatk();
		e_setdefense();
		s2_count(num_);
	}
}