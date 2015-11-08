#include "../../lib/gplib.h"
#include "movableObject.h"
#include "player.h"
#include "shot.h"
#include "bomb.h"
#include "tool.h"
#include "stage.h"
#include "Enemy.h"
#include <sstream>

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;
	using namespace input;

	//player��Y���W�����߂āA����Ă���
	extern float p_posY = (float)system::WINH - (float)(85 + 82 / 4);

	int is = 0;

    //�X�e�[�W�̃I�u�W�F�N�g���擾
	shared_ptr<Stage> Player::getStagePrt()
	{
		auto object = getObjectFromRoot({ "stage" });
		shared_ptr<game::Stage> stage = ci_ext::weak_to_shared<game::Stage>(object);
		//�X�e�[�W�̒l��Ԃ�
		return stage;
	}
	//�G�l�~�[�̃I�u�W�F�N�g���擾
	shared_ptr<Enemy> Player::getEnemyPrt()
	{
		auto object = getObjectFromRoot({ "boss" });
		shared_ptr<game::Enemy> boss = ci_ext::weak_to_shared<game::Enemy>(object);
		//�G�l�~�[�̒l��Ԃ�
		return boss;
	}

	Player::Player(const string& objectName,const int& tips) :
		MovableObject(
			DrawObjf(objectName, Vec3f::one(), Vec3f::one(), "player", Vec3f::zero(), Vec3f(85.f, 63.f, 0.f), Color(0xFFFFFFFF), 0))
	{
		//�摜�̃T�C�Y���擾
		player_w = graph::Draw_GetImageWidth("player") / 6;
		player_h = graph::Draw_GetImageHeight("player");
		//�v���C���[�����ʒu
		setPos((float)system::WINW / 2, p_posY, 0.3f);
		//�A�j���֐��@/�ő喇���@/�A�j���X�s�[�h
		charabase::SetAnim(anim_, 6, 0.15f);
		//�v���C���[�{�͍̂U�ߔ���Ȃ��A�󂯔��肾��
		SetRect(&defenseRect, 0, 0, player_w, player_h);                                                  //�󂯔���͈�
		OffsetRect(&defenseRect, ix() - halfsize<int>().x(), iy() - halfsize<int>().y());     //�󂯔��菉����
		
		num_1 = 0;                   //���x���J�E���g������
		num_2 = 0;
		level_ = 0;                  //level�֐�������
		is_level = 0;
		lv_count_w = 0;
		lv_count_h = 0;
		exp_draw_change = 0.f;       //������
		hp_draw_change = 0.f;
		mp_draw_change = 0.f;
		bullet_ = 1;                 //���U���̔��˒e��
		bullets_ = 3;                //�E�U���̔��˒e��
		angle_ = 0.f;                //�E�U���p�x�C��
		init_moving_speed = 3.f;     //�ړ����x�����l
		moving_speed = 3.f;          //�ړ����x
		Max_hp = 300.f;              //��������
		Max_mp = 100.f;              //��������
		Max_exp = 20.f;              //�������x���A�b�v�ɕK�v�Ȃd����
		hp_ = 0.f;                   //����������
		mp_ = 0.f;                   //����������
		exp_ = 0.f;                  //����ꂽ�d����
		mp_count = 0;                //�����x�𐧌䂷�邽�߂̃J�E���g��
		hp_times = 0.03f;            //�X�L���Q�ő傈���ɂ�肈�����Ք{��
		mp_times = 0.035f;           //�X�L���R�ő傍���ɂ�肍�����Ք{��
		mp_healing = 0.002f;         //�ő傍���ɂ�肍�������񕜔{��
		shadow_count = 0;            //�e�̃J�E���g��
		result_ = true;              //�Q�[���I�[�o�[���鎞�ɐ���
		def_show = false;            //���ꂽ��A��ʕω��̔���
		kill_2 = false;              //���x���ŃX�L���𐧌䂷��
		kill_3 = false;
		self_atk = false;            //�X�L���Q�̂������Ղɂ�̂r�d����
		hp_draw_top = 615;           //�����`��̏�A���A���A�E�̒l��ݒ�
		hp_draw_bottom = 635;
		hp_draw_left = 440;
		hp_draw_right = 840;
		mp_draw_top = 630;           //�����`��̏�A���A���A�E�̒l��ݒ�
		mp_draw_bottom = 640;
		mp_draw_left = 20;
		mp_draw_right = 420;
		exp_draw_top = 620;          //�����`��̏�A���A���A�E�̒l��ݒ�
		exp_draw_bottom = 625;
		exp_draw_left = 20;
		exp_draw_right = 420;
		tips_ = tips;                //�B���Z�R�}���h�@�l�n��
	}
	//�f�X�g���N�^
	Player::~Player()
	{

	}
	//�v���C���[�ړ�
	void Player::p_move(const int& key)
	{
		switch (key)
		{
		case moveDir::NEUTRAL:
			velocity_.x(0.f);
			break;
		case moveDir::LEFT:
			velocity_.x(-moving_speed);
			break;
		case moveDir::RIGHT:
			velocity_.x(moving_speed);
			break;
		}
	}
	//�A�j��
	void Player::p_anim()
	{
		//�A�j���t���[��
		int t[] = { 0, 1, 2, 3, 4, 5, 6 };
		int t2[] = { 5, 4, 3, 2, 1, 0 };

		if (charabase::StepAnim(anim_))
		{
			if (key == moveDir::NEUTRAL || key == moveDir::LEFT)
			{
				setSrc(Vec3f((float)t[anim_.no], 0.f, 0.f));
			}
			else if (key == moveDir::RIGHT)
			{
				setSrc(Vec3f((float)t2[anim_.no], 0.f, 0.f));
			}
		}
	}
	//�v���C���[�X�L��
	void Player::p_skill()
	{
		//�G�l�~�[�̃v���W�F�N�g���擾����
		auto b = getEnemyPrt();

		//�X�L���P�@��ʍU��
		if (CheckPush(input::KEY_MOUSE_LBTN))
		{
			for (int i = 0;i < bullet_ + tips_;++i)
			{
				insertAsChild(new p_shot1("p_bullet", x(), y() - (float)player_h / 2.f,
					(float)mousePos_.x, (float)mousePos_.y, (float)i, 0.f));
			}
			//�B���Z�̘A�˂Ə����A�˂̂r�d����ʂ�����
			if (bullet_ + tips_ >= 3)
				se::DSound_Play("p_bullet2");
			else
				se::DSound_Play("p_bullet");
		}

		//�X�L���R�@�����x�ړ�
		//�����������񕜂�
		if (kill_3)
		{
			if (mp_draw_change < (float)mp_draw_right)
				mp_ -= Max_mp * mp_healing;

			//�C�͂𐧌䂷��
			if (CheckPress(KEY_BTN1) && (CheckPress(KEY_BTN0) || CheckPress(KEY_BTN2)))
			{
				if (mp_draw_change <= (float)mp_draw_right)
				{
					if (mp_draw_change - Max_mp * mp_times >= (float)mp_draw_left)
					{
						//�X�s�[�h��{��
						moving_speed = moving_speed * 1.2f;
						//���񂍂����Ղ���
						mp_ += Max_mp * mp_times;
						//�e��\��
						p_shadow();
						//�J�E���g���Ő���
						++mp_count;
						se::DSound_Play("telesport");
						//�P�T�t���[����A���x�������l�ɖ߂�
						if (mp_count > 15)
						{
							mp_count = 0;
							moving_speed = init_moving_speed;      //�ӊO�ȃo�O��h�����߁A����ɑ��x�𐧌䂷��
						}
					}
					else if (mp_draw_change - Max_mp * mp_times < (float)mp_draw_left)
					{
						moving_speed = init_moving_speed;
					}
				}
				else if (mp_draw_change > (float)mp_draw_right)
				{
					mp_draw_change = (float)mp_draw_right;
				}
			}
			else
				moving_speed = init_moving_speed;
		}

		//�X�L���Q�@�U�˒e
		//hp�����炷����p����
		if (kill_2)
		{
			if (hp_draw_change > Max_hp * hp_times)
			{
				if (CheckPush(input::KEY_MOUSE_RBTN))
				{
					//�U�˒e���ɂ��p�x�C��
					if (bullets_ > 0 && bullets_ <= 3)
						angle_ = 10.f;
					else if (bullets_ > 3 && bullets_ <= 5)
						angle_ = 20.f;
					else if (bullets_ > 5 && bullets_ <= 7)
						angle_ = 30.f;
					else if (bullets_ > 8 && bullets_ <= 10)
						angle_ = 40.f;

					for (int i = 0;i < bullets_;++i)
						insertAsChild(new p_shot1("p_bullet", x(), y() - (float)player_h / 2.f,
							(float)mousePos_.x, (float)mousePos_.y, 0.f, i * 10.f - angle_));

					//�Q�[���N���A������A�������m�񂽂߂������炷�̂𐧌䂷��
					if (b->diedshow() == false)
					{
						p_receiving_atk(Max_hp * hp_times);
						self_atk = true;
					}
					se::DSound_Play("scatter");
				}
			}
			//�ˌ��ł��Ȃ��Ȃ鎞��
			else if (hp_draw_change <= Max_hp * hp_times && CheckPush(KEY_MOUSE_RBTN))
				se::DSound_Play("shell");
		}
	}
	//�}�E�X���W���擾
	void Player::p_getmousepos()
	{
		mousePos_ = input::GetMousePosition();
	}
	//�L�[�ɂ�����
	void Player::p_key()
	{
		//�L�[�ɂ������ύX
		key = 0;
		if (CheckPress(KEY_BTN0))
		{
			key += moveDir::LEFT;
		}
		else if (CheckPress(KEY_BTN2))
		{
			key += moveDir::RIGHT;
		}
		else key += moveDir::NEUTRAL;
	}
	//�摜���E���]
	//���݂̉摜�Ɏg��Ȃ��Ă�����
	void Player::p_backrun()
	{
//		if (key == moveDir::LEFT && scale().x() != -1)
//		{
//			setScale(Vec3f(scale().x()*-1, scale().y(), scale().z()));
//		}
//		if (key == moveDir::RIGHT && scale().x() != 1)
//		{
//			setScale(Vec3f(scale().x()*-1, scale().y(), scale().z()));
//		}
	}

	//�l���������l��Ԃ�
	float Player::playerMaxHp()
	{
		return Max_hp;
	}
	//�����̒l��Ԃ�
	float Player::playerHp()
	{
		return hp_draw_change;
	}

	//��������
	void Player::p_receiving_atk(const float& hp)
	{
		//�G�l�~�[�̃v���W�F�N�g���擾����
		auto b = getEnemyPrt();
		
		if (hp_draw_change > 0 && hp_draw_change <= Max_hp)
		{
			if (hp < 0)
			{
				//se
				se::DSound_Play("heal_se");
				for (int i = 0;i < 15;++i)
					insertToParent(new bomb10("heal", math::GetRandom(pos().x() - (float)player_w / 2.f, pos().x() + (float)player_w / 2.f),
						math::GetRandom(pos().y() - (float)player_h / 2.f, pos().y() + (float)player_h / 2.f), 1.f, i));
			}
			if (hp > 0)
			{
				if (!self_atk)
					se::DSound_Play("def");
				else
					self_atk = false;
			}
			//�Q�[���N���A�ƃQ�[���I�[�o�[����Ŕ������Ȃ����߂����Ő��䂷��
			if (b->diedshow() == false)
				hp_ += hp;
		}
		if (hp_ < 0) hp_ = 0;

		if (hp_draw_change <= 0)
		{
			hp_draw_change = 0;
			//��񂾂��ĂԂ��Ƃɐ���
			if (result_)
			{
				p_result();
				result_ = false;
			}
			//�v���C���[�I�u�W�F�N�g��~����
			pause();
		}
	}
	//exp
	void Player::p_exp(const float&exp)
	{
		//level99�܂łɃ��x���A�b�v�\
		if (num_2 == lv_count_w * 9 && num_1 == lv_count_w * 9)
		{
			exp_ = Max_exp;
		}
		//���Z�łd������{��
		else if (tips_ != 0)
			exp_ += exp * tips_;
		else
			exp_ += exp;

		if (exp_ >= Max_exp)
		{
			exp_ = exp_ - Max_exp;
			Max_exp = Max_exp * 1.2f;
			Max_hp = Max_hp + Max_hp * 0.03f;
			p_levelup(1);
		}
	}
	//���x���A�b�v
	void Player::p_levelup(const int& level)
	{
		level_ += level;
		p_receiving_atk(-Max_hp / 2.f);
		insertToParent(new bomb9("levelup", pos().x(), pos().y() + (float)(player_h / 4), 1.f));
		num_1 += lv_count_w;
		//���x���J�E���g
		if (num_1 > lv_count_w * 9)
		{
			num_1 = 0;
			num_2 += lv_count_w;
		}
		else if (num_2 > lv_count_w * 9)
		{
			num_1 = lv_count_w * 9;
			num_2 = lv_count_w * 9;
		}
		//�X�L�����
		if (level_ >= 3)
		{
			kill_2 = true;                         //�X�L���Q�U�ˉ�� 
		}
		if (level_ >= 5)
		{
			kill_3 = true;                         //�X�L���R�ړ����
		}
		//��ʍU���p���[�A�b�v
		if (level_ >= 3 && level_ < 9)
			bullet_ = 2;                           //��ʍU���e�����Q�� 
		else if (level_ >= 9 && level_ < 15)
			bullet_ = 3;                           //��ʍU���e�����R�� 
		else if (level_ >= 15)
			bullet_ = 5;                           //��ʍU���e�����T��
		//�U�˒e�p���[�A�b�v
		if (level_ >= 10 && level_ < 15)
			bullets_ = 5;                          //�U�˒e�����T��
		else if (level_ >= 15 && level_ < 20)
			bullets_ = 7;                          //�U�˒e�����V��
		else if (level_ >= 20)
			bullets_ = 9;                          //�U�˒e�����X��
		
		//�U�ˏ��Ղ������� 
		//�X�L���ړ����Ղ��������A�񕜑��x����
		if (level_ > 5) 
		{
			if (level_ > is_level)
			{
				if (hp_times < 0.01f)
					hp_times = 0.01f;
				else
					hp_times -= 0.0015f;
				if (mp_times < 0.02f)
					mp_times = 0.02f;
				else
					mp_times -= 0.0015f;
				if (mp_healing > 0.008f)
					mp_healing = 0.008f;
				else
					mp_healing += 0.0005f;
				is_level = level_;
			}
			else
				is_level = level_;
		}
	}

	//�X�e�[�W�֊֐����Ă�
	void Player::p_result()
	{
		auto s = getStagePrt();
		s->result(2);
	}

	//�󂯔���X�V
	void Player::p_setdefense()
	{
		SetRect(&defenseRect, (int)(ix() - player_w / 2), (int)(iy() - player_h / 2), (int)(ix() + player_w / 2), (int)(iy() + player_h / 2));
	}
	//��ʊO�o�Ȃ��悤��
	void Player::p_out()
	{
		if (pos().x() + velocity_.x() < player_w / 2 || pos().x() + velocity_.x() > (system::WINW - player_w / 2))
			velocity_.x(0.f);
	}
	//hp�̕`��
	void Player::p_drawHp()
	{
		//�ω����邈��
		hp_draw_change = Max_hp - (float)(hp_draw_right - hp_draw_left) / Max_hp * hp_;
		//�g�𒴂��Ȃ��悤�ɐ���
		if(hp_draw_change <= 0) hp_draw_change = 0;
		if (hp_draw_change > Max_hp) hp_draw_change = Max_hp;
		//�U�[�`��
		graph::Draw_Box(hp_draw_left, hp_draw_top, 
			hp_draw_left + (int)(((float)(hp_draw_right - hp_draw_left) / Max_hp) * hp_draw_change), hp_draw_bottom, 0.05f,
			ARGB(255, 255, 255, 0), ARGB(255, 255, 255, 0), 1, 1);
		//�g�`��
		graph::Draw_Box(hp_draw_left, hp_draw_top, hp_draw_right, hp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 0), ARGB(255, 255, 0, 0), 1, 0);
		//HP�`��
		//float�^��int�^��string�^�֕ω����邽�ߗp��
		ostringstream o_hp[2];
		string s_hp[2];
		o_hp[0] << (int)Max_hp;
		o_hp[1] << (int)hp_draw_change;
		s_hp[0].operator=(o_hp[0].str());
		s_hp[1].operator=(o_hp[1].str());
		font::Draw_FontTextNC(hp_draw_left + 2, hp_draw_top - 4, 0.045f, s_hp[1] + "/" + s_hp[0], ARGB(255, 255, 0, 0), 0);
		o_hp[0].clear();
		o_hp[1].clear();
	}
	//mp�̕`��
	//�����`�悪�s��Ȃ��̂ŁA�����`����ȒP�ȕ��@�ŕ`�悷��
	void Player::p_drawMp()
	{
		//�ω����邍��
		mp_draw_change = (float)mp_draw_right - (float)(mp_draw_right - mp_draw_left) / Max_mp * mp_;
		//�g�𒴂��Ȃ��悤�ɐ���
		if (mp_draw_change <= (float)mp_draw_left) mp_draw_change = (float)mp_draw_left;
		if (mp_draw_change > (float)mp_draw_right) mp_draw_change = (float)mp_draw_right;
		//�U�[�`��
		graph::Draw_Box( mp_draw_left, mp_draw_top, (int)mp_draw_change, mp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 255), ARGB(255, 255, 0, 255), 1, 1);
		//�g�`��
		graph::Draw_Box( mp_draw_left, mp_draw_top, mp_draw_right, mp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 255), ARGB(255, 255, 0, 255), 1, 0);
	}
	//lv�̕`��
	//�����`�悪�s��Ȃ��̂ŁA�����`����ȒP�ȕ��@�ŕ`�悷��
	void Player::p_drawLevel()
	{
		//�ω����邍��
		exp_draw_change = (float)(exp_draw_right - exp_draw_left) / Max_exp * exp_;
		//�g�𒴂��Ȃ��悤�ɐ���
		if (exp_draw_change <= (float)exp_draw_left) exp_draw_change = (float)exp_draw_left;
		if (exp_draw_change > (float)exp_draw_right) exp_draw_change = (float)exp_draw_right;
		//�U�[�`��
		graph::Draw_Box(exp_draw_left, exp_draw_top, (int)exp_draw_change, exp_draw_bottom, 0.05f,
			ARGB(255, 0, 255, 255), ARGB(255, 0, 255, 255), 1, 1);
		//�g�`��
		graph::Draw_Box(exp_draw_left, exp_draw_top, exp_draw_right, exp_draw_bottom, 0.05f,
			ARGB(255, 0, 255, 255), ARGB(255, 0, 255, 255), 1, 0);
		//�摜�T�C�Y�����
		int level_w = graph::Draw_GetImageWidth("level");
		int level_h = graph::Draw_GetImageHeight("level");
		lv_count_w = graph::Draw_GetImageWidth("level_count") / 10;
		lv_count_h = graph::Draw_GetImageHeight("level_count");
		//���x�������`��
		graph::Draw_Graphics(level_w / 2 + 20, system::WINH - level_h * 2, 0.5f, "level", 0, 0, level_w, level_h, 0.f);
		//���x���J�E���g�`��A�ʒu�ɍ��킹�邽�ߒ�������
		graph::Draw_Graphics(level_w / 2 + 10 + lv_count_w * 3, system::WINH - level_h * 2 - 1, 0.5f, "level_count", num_2, 0, lv_count_w, lv_count_h, 0.f);
		graph::Draw_Graphics(level_w / 2 + 10 + lv_count_w * 4, system::WINH - level_h * 2 - 1, 0.5f, "level_count", num_1, 0, lv_count_w, lv_count_h, 0.f);
	}

	//���ꂽ���Ă�
	void Player::defenseHit(std::weak_ptr<ci_ext::Object>&)
	{
		def_show = true;
	}
	//�U�����󂯂����ɉ�ʐԂ�����
	void Player::p_defense_atkshow()
	{
		if (def_show)
		{
			graph::Draw_Box(0, 0, system::WINW, system::WINH, 0.45f, ARGB(75, 255, 0, 0), ARGB(75, 255, 0, 0), 1, 1);
			def_show = false;
		}
	}
	//�����x��10�ȉ��ǂ������`�F�b�N����
	bool alpha_(MovableObject& move) { return move.color().a() <= 10; }
	//�����x�ړ����鎞�ɉe�𐶐�
	void Player::p_shadow()
	{
		//��4�t���[���ŉe�𐶐�����
		++shadow_count;
		if (shadow_count > 3)
		{
			shadow_count = 0;
			//�D��x�����ω������A���̂͏����l���g��
			shadow_.push_back(MovableObject(
				DrawObjf("shadow", Vec3f(pos().x(), pos().y(), 0.4f), scale(), resname(), src(), srcSize(), color(), 0)));
		}
	}
	//�e�̕`��
	void Player::p_draw_shadow()
	{
		for (auto& s : shadow_)
		{
			s.render();
		}
	}
	//�e�̍X�V
	void Player::p_step_shadow()
	{
		//�e�X�̉e�ɑ΂��āA�����x�����炵�Ă���
		for (auto& s : shadow_) {
			s.setColor(s.color().a() - 10, s.color().r(), s.color().g(), s.color().b());
		}
		//�폜����
		auto it = remove_if(shadow_.begin(), shadow_.end(), alpha_);
		shadow_.erase(it, shadow_.end());
	}

	void Player::render()
	{
		MovableObject::render();
		p_drawHp();
		p_drawMp();
		p_drawLevel();
		p_defense_atkshow();
		p_draw_shadow();
	}

	void Player::update()
	{
		MovableObject::update();
		p_anim();
		p_backrun();
		p_getmousepos();
		p_key();
		p_skill();
		p_move(key);
		p_out();
		p_setdefense();
		p_step_shadow();
		p_receiving_atk(hp);
		p_exp(exp);
	}
}