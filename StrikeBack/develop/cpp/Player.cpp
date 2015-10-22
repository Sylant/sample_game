#include "../../lib/gplib.h"
#include "movableObject.h"
#include "player.h"
#include "shot.h"
#include "bomb.h"
#include "tool.h"
#include "stage.h"
#include "Enemy.h"

namespace game
{
	using namespace std;
	using namespace gplib;
	using namespace ci_ext;
	using namespace input;

	//player��Y���W�����߂āA����Ă���
	extern float p_posY = (float)system::WINH - (float)(85 + 82 / 4);

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

	Player::Player(const string& objectName, int tips) :
		MovableObject(
			DrawObjf(objectName, Vec3f::one(), Vec3f::one(), "player", Vec3f::zero(), Vec3f(85.f, 63.f, 0.f), Color(0xFFFFFFFF), 0))
	{
		//�摜�ǂݍ���
		graph::Draw_LoadObject("player", "res/gra/Player/player.png");
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
		
		bullet_ = 2;             //���U���̔��˒e��
		init_moving_speed = 3.f; //�ړ����x�����l
		moving_speed = 3.f;      //�ړ����x
		Max_hp = 400.f;          //��������
		Max_mp = 400.f;          //��������
		hp_ = 0.f;               //����������
		mp_ = 0.f;               //����������
		mp_count = 0;            //�����x�𐧌䂷�邽�߂̃J�E���g��
		hp_times = 0.008f;       //�ő傈���ɂ�肈�����Ք{��
		mp_times = 0.02f;        //�ő傍���ɂ�肍�����Ք{��
		mp_healing = 0.006f;     //�ő傍���ɂ�肍�������񕜔{��
		shadow_count = 0;        //�e�̃J�E���g��
		result_ = true;          //�Q�[���I�[�o�[���鎞�ɐ���
		def_show = false;        //���ꂽ��A��ʕω��̔���
		hp_draw_top = 630;       //�����`��̏�A���A���A�E�̒l��ݒ�
		hp_draw_bottom = 615;
		hp_draw_left = 440;
		hp_draw_right = 840;
		mp_draw_top = 630;       //�����`��̏�A���A���A�E�̒l��ݒ�
		mp_draw_bottom = 615;
		mp_draw_left = 20;
		mp_draw_right = 420;
		tips_ = tips;            //�B���Z�R�}���h�@�l�n��
	}

	//�v���C���[�ړ�
	void Player::p_move(int key)
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

		//��ʍU��
		if (CheckPush(input::KEY_MOUSE_LBTN))
		{
			for (int i = 0;i < bullet_ + tips_;++i)
			{
				insertAsChild(new p_shot1("p_bullet", x(), y() - (float)player_h / 2.f,
					(float)mousePos_.x, (float)mousePos_.y, (float)i, 0.f));
			}
			//�B���Z�̘A�˂Ə����A�˂̂r�d����ʂ�����
			if (bullet_ + tips_ > bullet_)
				se::DSound_Play("p_bullet2");
			else
				se::DSound_Play("p_bullet");
		}

		//�����x�ړ�
		//�����������񕜂�
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
					//���x�������l�ɖ߂�
					if (mp_count > 15)
					{
						mp_count = 0;
						moving_speed = init_moving_speed;
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

		//�U�˒e�@hp�����炷����p����
		if (hp_draw_change > (float)hp_draw_left + Max_hp * hp_times)
		{
			if (CheckPush(input::KEY_MOUSE_RBTN))
			{
				for (int i = 0;i < 6;++i)
				{
					insertAsChild(new p_shot1("p_bullet", x(), y() - (float)player_h / 2.f,
						(float)mousePos_.x, (float)mousePos_.y, 0.f, i * 10.f - 30.f));
				}
				//�Q�[���N���A������A�������m�񂽂߂������炷�̂𐧌䂷��
				if (b->diedshow() == false) hp_ += Max_hp * hp_times;
				se::DSound_Play("scatter");
			}
		}
		//�ˌ��ł��Ȃ��Ȃ鎞��
		else if (hp_draw_change <= (float)hp_draw_left + Max_hp * hp_times && CheckPush(KEY_MOUSE_RBTN))
			se::DSound_Play("shell");
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
	//hp�̒l��Ԃ�
	float Player::playerHp()
	{
		return hp_draw_change;
	}
	//�����`��̍ő�l���`�悷��ő傈����߂�l��
	int Player::playerHpRight()
	{
		return hp_draw_right;
	}

	//��������
	void Player::p_receiving_atk(float hp)
	{
		//�G�l�~�[�̃v���W�F�N�g���擾����
		auto b = getEnemyPrt();
		
		if (hp_draw_change > (float)hp_draw_left && hp_draw_change <= (float)hp_draw_right)
		{
			//�Q�[���N���A�ƃQ�[���I�[�o�[����Ŕ������Ȃ����߂����Ő��䂷��
			if(b->diedshow() == false) hp_ += hp;
		}
		if (hp_draw_change <= hp_draw_left)
		{
			hp_draw_change = (float)hp_draw_left;
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
		hp_draw_change = (float)hp_draw_right - (float)(hp_draw_right - hp_draw_left) / Max_hp * hp_;
		//�g�𒴂��Ȃ��悤�ɐ���
		if(hp_draw_change <= (float)hp_draw_left) hp_draw_change = (float)hp_draw_left;
		if (hp_draw_change > (float)hp_draw_right) hp_draw_change = (float)hp_draw_right;
		//�U�[�`��
		graph::Draw_Box(hp_draw_left, hp_draw_top, (int)hp_draw_change, hp_draw_bottom, 0.05f,
			ARGB(255, 255, 255, 0), ARGB(255, 255, 255, 0), 1, 1);
		//�g�`��
		graph::Draw_Box(hp_draw_left, hp_draw_top, hp_draw_right, hp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 0), ARGB(255, 255, 0, 0), 1, 0);
	}
	//mp�̕`��
	void Player::p_drawMp()
	{
		//�ω����邍��
		mp_draw_change = (float)mp_draw_right - (float)(mp_draw_right - mp_draw_left) / Max_mp * mp_;
		//�g�𒴂��Ȃ��悤�ɐ���
		if (mp_draw_change <= (float)mp_draw_left) mp_draw_change = (float)mp_draw_left;
		if (mp_draw_change > (float)mp_draw_right) mp_draw_change = (float)mp_draw_right;
		//�U�[�`��
		graph::Draw_Box( mp_draw_left, mp_draw_top, (int)mp_draw_change, mp_draw_bottom, 0.05f,
			ARGB(255, 0, 255, 255), ARGB(255, 0, 255, 255), 1, 1);
		//�g�`��
		graph::Draw_Box( mp_draw_left, mp_draw_top, mp_draw_right, mp_draw_bottom, 0.05f,
			ARGB(255, 255, 0, 0), ARGB(255, 255, 0, 0), 1, 0);
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
			graph::Draw_Box(0, 0, system::WINW, system::WINH, 0.45f, ARGB(50, 255, 0, 0), ARGB(50, 255, 0, 0), 1, 1);
			def_show = false;
		}
	}
	//�����x��10�ȉ��ǂ������`�F�b�N����
	bool alpha_(MovableObject& move) { return move.color().a() <= 10; }
	//�����x�ړ����鎞�ɉe�𐶐�
	void Player::p_shadow()
	{
		//��2�t���[���ŉe�𐶐�����
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
	}
}