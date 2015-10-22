#include <memory>
#include <string>
#include <iostream>
#include "Stage.h"
#include "player.h"
#include "Enemy.h"
#include "shot.h"
#include "tool.h"
#include "collider.h"
#include "movableObject.h"
#include "easing.h"
#include "logo.hpp"

namespace game
{
	using namespace ci_ext;
	using namespace std;
	using namespace gplib;
	using namespace se;
	using namespace bgm;

	///////////////////////////////////////////////////////////////////////////
	//�摜�D��x�����W�ݒ�����@�������������ʂ̑O��
	//0.05f���x��  �Q�[���N���A�`��A�Q�[���I�[�o�[�`��A���b�Z�[�W�`��AHP�`��A�C�͕`��
	//0.1f���x�� �@���ׂĂ̔����A�j���A�v���C���[�U��
	//0.15f���x���@�Ȃ�
	//0.2f���x���@ �n���AUFO�A�w���R�v�^�[
	//0.25f���x���@�G�l�~�[���e�AUFO�U���A�w���R�v�^�[�U��
	//0.3f���x���@ �v���C���[�A�{�X
	//0.35f���x���@�~�j�G�l�~�[�A����
	//0.4f���x�� �@���A����Q�b�g�A�j���A�v���C���[�̉e
	//0.45f���x��  BGM�A�C�R��
	//0.5f���x���@ �X�e�[�W�w�i�`��A�n�ʕ`��A���E���`��
	//0.6f���x��   LOGO�A�I����
	//0.7f���x��   �^�C�g���w�i
	///////////////////////////////////////////////////////////////////////////
	//Key�̐ݒ�@     �L�[�{�[�h�ɑΉ�����
	//KEY_BTN0        -> A
	//KEY_BTN1        -> S
	//KEY_BTN2        -> D
	//KEY_B           -> B
	//KEY_SPACE       -> space
	//KEY_MOUSE_LBTN  -> ���N���b�N
	//KEY_MOUSE_RBTN  -> �E�N���b�N
	//////////////////////////////////////////////////////////////////////////

	Stage::Stage(const std::string& objectName, int tips)
		:
		Object(objectName)
	{
		//�B���Z�R�}���h�@�l�n��
		tips_ = tips;
		//�t�@�C���ǂݍ���
		//�摜
		graph::Draw_LoadObject("stage_back", "res/gra/Stage/back.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("ground", "res/gra/Stage/ground.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("over", "res/gra/Stage/over.png", 0x00000000);
		graph::Draw_LoadObject("clear", "res/gra/Stage/clear.png", 0x00000000);
		graph::Draw_LoadObject("message", "res/gra/Stage/message.png", 0x00000000);
		graph::Draw_LoadObject("count", "res/gra/Stage/count.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("kill", "res/gra/Stage/kill.png", 0xFFFFFFFF);
		//BGM�ASE
		DShow_LoadFile("stage_bgm", "res/bgm/bgm_02.wav");
		DSound_LoadFile("tool_get", "res/se/se_03.wav");
		DSound_LoadFile("p_bullet", "res/se/se_04.wav");
		DSound_LoadFile("p_bullet2", "res/se/se_04_2.wav");
		DSound_LoadFile("shell", "res/se/se_05.wav");
		DSound_LoadFile("scatter", "res/se/se_06.wav");
		DSound_LoadFile("telesport", "res/se/se_07.wav");
		DSound_LoadFile("light", "res/se/se_08.wav");
		DSound_LoadFile("e_bullet", "res/se/se_09.wav");
		DSound_LoadFile("beam", "res/se/se_10.wav");
		DSound_LoadFile("e_explode", "res/se/se_11.wav");
		DSound_LoadFile("kill", "res/se/se_12.wav");
		DSound_LoadFile("landmine", "res/se/se_13.wav");
		DSound_LoadFile("all_explode", "res/se/se_14.wav");
		DSound_LoadFile("boss_atk", "res/se/se_15.wav");
		DSound_LoadFile("boss_atk2", "res/se/se_16.wav");
		DSound_LoadFile("win", "res/se/se_17.wav");
		DSound_LoadFile("lose", "res/se/se_18.wav");
	}

	//�v���C���[�̃I�u�W�F�N�g���擾
	shared_ptr<Player> Stage::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//�v���C���[�̒l��Ԃ�
		return player;
	}

	//������
	void Stage::init()
	{
		//�o������
		insertAsChild(new Player("player", tips_));
		insertAsChild(new Enemy("boss"));

		//�G�l�~�[->�v���C���[�̓����蔻��
		insertAsChild(new Collider("����", "ammo", "player"));
		insertAsChild(new Collider("����", "bomb4", "player"));
		insertAsChild(new Collider("����", "bomb5", "player"));
		insertAsChild(new Collider("����", "landmine", "player"));
		insertAsChild(new Collider("����", "bomb2", "player"));
		insertAsChild(new Collider("����", "e_bullet", "player"));
		insertAsChild(new Collider("����", "lightning", "player"));
		insertAsChild(new Collider("����", "e_missiles", "player"));
		//�v���C���[�G�l�~�[�̓����蔻��
		insertAsChild(new Collider("����", "p_bullet", "boss"));
		insertAsChild(new Collider("����", "p_bullet", "mini"));
		insertAsChild(new Collider("����", "p_bullet", "landmine"));
		insertAsChild(new Collider("����", "p_bullet", "heli"));
		insertAsChild(new Collider("����", "p_bullet", "ufo"));

		sel_alpha = 0;          //�����x������
		mes_alpha = 0;
		mes_count = 0;          //���b�Z�[�W�J�E���g��������
		se_show = true;         //SE�Đ�����
		bgm_show = true;        //BGM�Đ�����
		icon_alpha = 255;       //�A�C�R�������x����
		kill_count = 0;         //���E����������
		kill_total = 0;
		kill_num = 50;          //���E���T�O�^��A�����񕜂���
		kill_healing = 0.f;     //�񕜒l������
		kill_heal_using = 0.5f; //�v���C���[�̍ő傈���ɂ��񕜔{��
		player_hp = 0;          //�v���C���[�����������l������
		count_w = 0;            //�摜�T�C�Y�ϐ�������
		count_h = 0;
		result_ = 0;            //���ʃp�^����������
		clear_scale = 1.3f;     //�`�悷��摜�̑傫��������
		over_scale = 1.3f;
		mes_scale = 0.9f;
		icon_scale = 0.5f;
		count_scale = 0.9f;
		num_1 = 0;              //���E���\���̏������A��
		num_2 = 0;              //�\��
		num_3 = 0;              //�S��
	}

	//��Ԃɂ��\������message��ω�
	void Stage::result(int result)
	{
		result_ = result;

		//gameclear���
		if (result_ == 1)
		{
			sel_alpha += 5;
			if (sel_alpha > 255) sel_alpha = 255;
			//gameclear�摜�̃T�C�Y���擾
			float clear_w = (float)gplib::graph::Draw_GetImageWidth("clear");
			float clear_h = (float)gplib::graph::Draw_GetImageHeight("clear");
			//gameclear��`�悷��
			gplib::graph::Draw_Graphics(gplib::system::WINW / 2, gplib::system::WINH / 2,
				0.05f, "clear", 0, 0, (int)clear_w, (int)clear_h, 0.f, clear_scale, clear_scale, sel_alpha);
			//�ʏ�BGM���~���AWIN��SE�𗬂�
			DShow_Stop("stage_bgm");
			if (se_show)
			{
				DSound_Play("win");
				se_show = false;
			}
			message();
		}
		//gameover���
		else if (result_ == 2)
		{
			sel_alpha += 5;
			if (sel_alpha > 255) sel_alpha = 255;
			//gameover�摜�̃T�C�Y���擾
			float over_w = (float)gplib::graph::Draw_GetImageWidth("over");
			float over_h = (float)gplib::graph::Draw_GetImageHeight("over");
			//gameover��`�悷��
			gplib::graph::Draw_Graphics(gplib::system::WINW / 2, gplib::system::WINH / 2,
				0.05f, "over", 0, 0, (int)over_w, (int)over_h, 0.f, over_scale, over_scale, sel_alpha);
			//�ʏ�BGM���~���ALOSE��SE�𗬂�
			DShow_Stop("stage_bgm");
			if (se_show)
			{
				DSound_Play("lose");
				se_show = false;
			}
			message();
		}
	}

	//message�`��
	void Stage::message()
	{
		//�J�E���g���Ń��b�Z�[�W�𐧌䂷��
		//�����B��悤�ɂ���
		++mes_count;
		if (mes_count > 60 && mes_count <= 120)
		{
			mes_alpha += 5;
			if (mes_alpha > 255) mes_alpha = 255;
		}
		else if (mes_count > 120)
		{
			mes_alpha -= 5;
			if (mes_alpha < 0)
			{
				mes_alpha = 0;
				mes_count = 0;
			}
		}

		//message�摜�̃T�C�Y���擾
		float message_w = (float)gplib::graph::Draw_GetImageWidth("message");
		float message_h = (float)gplib::graph::Draw_GetImageHeight("message");
		//message��`�悷��
		gplib::graph::Draw_Graphics(gplib::system::WINW / 2, gplib::system::WINH / 2 + 80,
			0.05f, "message", 0, 0, (int)message_w, (int)message_h, 0.f, mes_scale, mes_scale, mes_alpha);

		//�X�y�[�X�Ń^�C�g����
		if (input::CheckPush(input::KEY_SPACE))
		{
			insertToParent(new Logo("logo"));
			DSound_Play("button");
			kill();
		}
	}

	//BGM�Đ�����
	void Stage::stage_bgm()
	{
		if (bgm_show)
			DShow_Play("stage_bgm");
		if (!bgm_show)
			DShow_Pause("stage_bgm");
	}

	//�L�[�ɂ��Đ�������s��
	void Stage::volume()
	{
		if (input::CheckPush(input::KEY_F1))
		{
			//BGM�Đ�
			bgm_show = true;
			//�����x��߂�
			icon_alpha = 255;
		}
		if (input::CheckPush(input::KEY_F2))
		{
			//BGM��~
			bgm_show = false;
			//�����x��߂�
			icon_alpha = 255;
		}
	}

	//BGM�Đ����ǂ����m�F����A�C�R��
	//�A�C�R���T�C�Y�@144*144
	void Stage::icon()
	{
		if (bgm_show)
		{
			gplib::graph::Draw_Graphics(gplib::system::WINW - 144 / 2, gplib::system::WINH - 144 / 3,
				0.45f, "bgm_icon", 0, 0, 144, 144, 0.f, icon_scale, icon_scale, icon_alpha);
			icon_alpha -= 3;
			if (icon_alpha < 0) icon_alpha = 0;

		}
		else if (!bgm_show)
		{
			gplib::graph::Draw_Graphics(gplib::system::WINW - 144 / 2, gplib::system::WINH - 144 / 3,
				0.45f, "bgm_icon", 144, 0, 144, 144, 0.f, icon_scale, icon_scale, icon_alpha);
			icon_alpha -= 3;
			if (icon_alpha < 0) icon_alpha = 0;
		}
	}
	//int�œG��|�����̖߂�l��Ԃ�
	int Stage::e_kill(int count)
	{
		kill_count = count;
		return kill_count;
	}
	//���E���`��
	void Stage::total()
	{
		//���E��
		kill_total = kill_total + kill_count;
		
		//�v���C���[�̃I�u�W�F�N�g���擾
		auto p = getPlayerPrt();

		//�G��|������A�ʂ̕`�悩��{�P��
		if (kill_count > 0)
		{
			num_1 += count_w;
			if (num_1 > count_w * 9)
			{
				num_1 = 0;
				num_2 += count_w;
				num_3 = 0;
			}
			else if (num_2 > count_w * 9)
			{
				num_1 = 0;
				num_2 = 0;
				num_3 += count_w;
			}
			//�����ő吔�𒴂�����A�O�ɖ߂�
			else if (num_3 > count_w * 9)
			{
				num_1 = 0;
				num_2 = 0;
				num_3 = 0;
			}
			kill_count = 0;
		}
		//���v�T�O�̓G�����E����ƁA�v���C���[�̂����͑啝�ɉ񕜂���
		if (kill_total == kill_num)
		{
			//�v���C���[�̂����ݒ�ɂ��񕜒l�ω�����a

			//hp�`��͘g�𒴂��Ȃ��悤��
			if (p->playerHp() >= p->playerHpRight())
			{
				p->p_receiving_atk(0);
			}
			else if (p->playerHp() < p->playerHpRight())
			{
				//hp�̕`��ʒu�̍��W�͋t�Ȃ̂ŁA�{��-���t�ɂ���
				player_hp = p->playerHpRight() - (int)p->playerHp();
				//�����������͉񕜒l������->�񕜒l�ŉ񕜂���
				if (player_hp >= (int)kill_healing)
					p->p_receiving_atk(-kill_healing);
				//�����������͉񕜒l��艺���->�����������������񕜂���
				else if (player_hp < (int)kill_healing)
					p->p_receiving_atk(-(float)player_hp);
			}
			kill_total = 0;
		}
	}

	void Stage::render()
	{
		graph::Draw_Clear();
		//�w�i
		//�T�C�Y�����
		int back_w = gplib::graph::Draw_GetImageWidth("stage_back");
		int back_h = gplib::graph::Draw_GetImageHeight("stage_back");
		graph::Draw_Graphics(system::WINW / 2.f, system::WINH / 2.f, 0.5f, "stage_back", 0, 0,
			back_w, back_h, 0, (float)system::WINW / (float)back_w, (float)system::WINH / (float)back_h);
		//�n��
		int ground_w = gplib::graph::Draw_GetImageWidth("ground");
		int ground_h = gplib::graph::Draw_GetImageHeight("ground");
		int ground_length = system::WINW / ground_w;
		int ground_posY = system::WINH - ground_h;
		for (int i = 0; i < ground_length + 1; ++i)
			graph::Draw_GraphicsLeftTop((i*ground_w), ground_posY, 0.5f, "ground", 0, 0, ground_w, ground_h);
		//���E���`��
		int kill_w = graph::Draw_GetImageWidth("kill");
		int kill_h = graph::Draw_GetImageHeight("kill");
		count_w = graph::Draw_GetImageWidth("count") / 10;
		count_h = graph::Draw_GetImageHeight("count");

		graph::Draw_Graphics(system::WINW - kill_w / 1.6f, system::WINH - kill_h / 1.5f, 0.5f, "kill", 0, 0, kill_w, kill_h, 0.f);
		graph::Draw_Graphics(system::WINW - count_w * 2.f, system::WINH - count_h * 0.8f, 0.5f, "count", num_1, 0, count_w, count_h, 0.f, count_scale, count_scale);
		graph::Draw_Graphics(system::WINW - count_w * 3.f, system::WINH - count_h * 0.8f, 0.5f, "count", num_2, 0, count_w, count_h, 0.f, count_scale, count_scale);
		graph::Draw_Graphics(system::WINW - count_w * 4.f, system::WINH - count_h * 0.8f, 0.5f, "count", num_3, 0, count_w, count_h, 0.f, count_scale, count_scale);

		result(result_);
		icon();
	}

	void Stage::update()
	{
		//�A�e�C�����m���ŏo������
		if (math::GetRandom(0, 200) == 0)
		{
			insertAsChild(new Tool1("ammo"));
		}

		total();
		stage_bgm();
		volume();
		e_kill(kill_count);
	}
}