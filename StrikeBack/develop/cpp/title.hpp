#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "effect.h"
#include "Stage.h"
#include "player.h"

namespace game
{
	class Title :public ci_ext::Object
	{
	private:
		RECT option[2];    //�I�����ݒu
		int alpha_[2];     //�I�����Ă��Ȃ����ɓ����x�ω��֐�
		int bgm_alpha_;    //BGM��\������A�C�R���̓����x����
		int tips_;         //�B���Z�R�}���h�@�l�n���̂��ߐݒu����
		int key_[10];      //�������L�[�����ԂŒl��n��
		int compare_[10];  //�L�[�̒l��Δ䂷�邽�ߐݒ�
		int start_w;       //�`�悷��摜�̃T�C�Y���擾
		int start_h;
		int exit_w;
		int exit_h;
		bool bgmShow_;     //BGM�̍Đ��𐧌䂷��
		bool seShow_;      //SE�̍Đ��𐧌䂷��
		bool decide_[2];      //�I���������߂邩�ǂ���

	public:

		Title(const std::string& objectName) :
			Object(objectName)
		{
			//�G�t�F�N�g
			effect::Create(0, 0, effect::EFFECTTYPE::FADEINWHITE);
			//�摜�ǂݍ���
			gplib::graph::Draw_LoadObject("title_back", "res/gra/Title/back.png", 0xFFFFFFFF);
			gplib::graph::Draw_LoadObject("title_logo", "res/gra/Title/logo.png", 0xFFFFFFFF);
			gplib::graph::Draw_LoadObject("start", "res/gra/Title/start.png",0xFFFFFFFF);
			gplib::graph::Draw_LoadObject("exit", "res/gra/Title/exit.png", 0xFFFFFFFF);
			gplib::graph::Draw_LoadObject("bgm_icon", "res/gra/Title/bgm_icon.png", 0xFFFFFFFF);
			//�T�E���h�ǂݍ���
			gplib::bgm::DShow_LoadFile("title_bgm", "res/bgm/bgm_01.wav");
			gplib::se::DSound_LoadFile("button", "res/se/se_01.wav");
			gplib::se::DSound_LoadFile("tips", "res/se/se_02.wav");

			//start�{�^���摜�̃T�C�Y���擾
			start_w = gplib::graph::Draw_GetImageWidth("start");
			start_h = gplib::graph::Draw_GetImageHeight("start");

			//exit�{�^���摜�̃T�C�Y���擾
			exit_w = gplib::graph::Draw_GetImageWidth("exit");
			exit_h = gplib::graph::Draw_GetImageHeight("exit");

			//��̑I�����ʒu������������
			//left,top,right,bottom
			//�摜�T�C�Y�ɂ��ݒ�
			option[0] = { gplib::system::WINW / 3 - start_w / 2, gplib::system::WINH / 2 + 170 - start_h / 2,
				gplib::system::WINW / 3 + start_w / 2, gplib::system::WINH / 2 + 170 + start_h / 2 };
			option[1] = { gplib::system::WINW - gplib::system::WINW / 3 - exit_w / 2, gplib::system::WINH / 2 + 170 - exit_h / 2,
				gplib::system::WINW - gplib::system::WINW / 3 + exit_w / 2, gplib::system::WINH / 2 + 170 + exit_h / 2 };
			
			//�����x�̏����l�ݒ�
			alpha_[0] = 255;
			alpha_[1] = 50;
			
			//�d�˂Ȃ��悤�ɑΔ䂷��l�́{�P�ŏ�����
			for (int i = 0;i < 10;++i)
				compare_[i] = i + 1;
			for (int i = 0;i < 10;++i)
				key_[i] = 0;
			//�Đ����ǂ����m�F����
			bgmShow_ = true;
			//�Đ����ǂ����m�F����
			seShow_ = true;
			//�I���������߂邩�ǂ����m�F����
			decide_[0] = true;
			decide_[1] = false;
			//�A�C�R�������x������
			bgm_alpha_ = 255;
		}
		//�f�X�g���N�^
		Title::~Title()
		{

		}

		void render() override
		{
			//�w�i�摜�̃T�C�Y���擾
			int back_w = gplib::graph::Draw_GetImageWidth("title_back");
			int back_h = gplib::graph::Draw_GetImageHeight("title_back");
			//�w�i��`�悷��
			gplib::graph::Draw_GraphicsLeftTop(0, 0, 0.7f, "title_back", 0, 0, back_w, back_h, 0.f, nullptr,
				gplib::system::WINW / (float)back_w,
				gplib::system::WINH / (float)back_h);

			//logo�摜�̃T�C�Y���擾
			int logo_w = gplib::graph::Draw_GetImageWidth("title_logo");
			int logo_h = gplib::graph::Draw_GetImageHeight("title_logo");
			//logo��`�悷��
			gplib::graph::Draw_Graphics(gplib::system::WINW / 2, gplib::system::WINH / 2 - 180,
				0.6f, "title_logo", 0, 0, logo_w, logo_h, 0.f, 1.1f, 1.1f);

			//start�{�^����`�悷��
			gplib::graph::Draw_Graphics(gplib::system::WINW / 3, gplib::system::WINH / 2 + 170,
				0.6f, "start", 0, 0, start_w, start_h, 0.f, 1.f, 1.f, alpha_[0]);

			//exit�{�^����`�悷��
			gplib::graph::Draw_Graphics(gplib::system::WINW - gplib::system::WINW / 3, gplib::system::WINH / 2 + 170,
				0.6f, "exit", 0, 0, exit_w, exit_h, 0.f, 1.f, 1.f, alpha_[1]);
			
			//�`��`�F�b�N
//			for (int i = 0;i < 2;++i)
//				gplib::graph::Draw_CkRect(option[i], ARGB(255, 255, 0, 0));
			bgm_icon();
		}
		//�X�^�[�g�I�v�V����
		//�m�F���ăX�e�[�W��
		void start_option()
		{
			gplib::se::DSound_Play("button");
			effect::Create(0, 0, effect::EFFECTTYPE::FADEOUTWHITE);
			insertToParent(new Stage("stage", tips_));
			//�X�e�[�W�ֈړ����鎞�ɂ���~������
			bgmShow_ = false;
			kill();
		}
		//�G�O�W�b�g�I�v�V����
		//�m�F���ďI����
		void exit_option()
		{
			gplib::se::DSound_Play("button");
			effect::Create(0, 0, effect::EFFECTTYPE::FADEOUTBLACK);
			exit(0);
		}
		//�Đ�����
		void title_bgm()
		{
			if (bgmShow_)
				gplib::bgm::DShow_Play("title_bgm");
			else if (!bgmShow_)
				gplib::bgm::DShow_Pause("title_bgm");
		}

		//BGM�Đ����ǂ����m�F����A�C�R��
		//�A�C�R���T�C�Y�@144*144
		void bgm_icon()
		{
			if (bgmShow_)
			{
				gplib::graph::Draw_Graphics(gplib::system::WINW - 144 / 2, gplib::system::WINH - 144 / 3 ,
					0.45f, "bgm_icon", 0, 0, 144, 144, 0.f, 0.5f, 0.5f, bgm_alpha_);
				bgm_alpha_ -= 3;
				if (bgm_alpha_ < 0) bgm_alpha_ = 0;

			}
			else if (!bgmShow_)
			{
				gplib::graph::Draw_Graphics(gplib::system::WINW - 144 / 2, gplib::system::WINH - 144 / 3,
					0.45f, "bgm_icon", 144, 0, 144, 144, 0.f, 0.5f, 0.5f, bgm_alpha_);
				bgm_alpha_ -= 3;
				if (bgm_alpha_ < 0) bgm_alpha_ = 0;
			}
		}

		//�L�[�ɂ��Đ�������s��
		void bgm_volume()
		{
			if (gplib::input::CheckPush(gplib::input::KEY_F1))
			{
				//�Đ�
				bgmShow_ = true;
				//�����x��߂�
				bgm_alpha_ = 255;
			}
			if (gplib::input::CheckPush(gplib::input::KEY_F2))
			{
				//��~
				bgmShow_ = false;
				//�����x��߂�
				bgm_alpha_ = 255;
			}
		}
		//�I��������
		void operations()
		{
			//�}�E�X
			POINT mousePos = gplib::input::GetMousePosition();
			for (int i = 0; i < 2; i++)
			{
				if (mousePos.x > option[i].left && mousePos.x < option[i].right)
				{
					if (mousePos.y > option[i].top && mousePos.y < option[i].bottom)
					{
						if (i == 0)
						{
							decide_[0] = true;
							decide_[1] = false;
						}
						else
						{
							decide_[0] = false;
							decide_[1] = true;
						}
					}
				}
			}
			//�L�[�|�[�g
			if (gplib::input::CheckPush(gplib::input::KEY_LEFT))
			{
				decide_[0] = true;
				decide_[1] = false;
			}
			else if (gplib::input::CheckPush(gplib::input::KEY_RIGHT))
			{
				decide_[0] = false;
				decide_[1] = true;
			}
			//���N���b�N�ƃX�y�[�X�L�[�Ō��߂�
			if (decide_[0])
			{
				alpha_[0] = 255;
				alpha_[1] = 50;
				if (gplib::input::CheckPush(gplib::input::KEY_MOUSE_LBTN) || gplib::input::CheckPush(gplib::input::KEY_SPACE))
					start_option();
			}
			else if (decide_[1])
			{
				alpha_[0] = 50;
				alpha_[1] = 255;
				if (gplib::input::CheckPush(gplib::input::KEY_MOUSE_LBTN) || gplib::input::CheckPush(gplib::input::KEY_SPACE))
					exit_option();
			}
		}

		//�B���Z �R�i�~�R�}���h ��㉺�����E���EBA
		//�v���C���[�̈�ʍU���͂Q�A��->�S�A�˂ɕω�
		//-----------------------------------------------------------------
		//�t���[�����䂷�邱�Ƃł��Ă��Ȃ��̂ŁA����������Ƃł��Ȃ��Ȃ邱�Ƃ͂���
		void tips()
		{
			if (gplib::input::CheckPush(gplib::input::KEY_UP))
				key_[0] = 1;
			if (key_[0] == compare_[0] && gplib::input::CheckPush(gplib::input::KEY_UP))
				key_[1] = 2;
			if (key_[1] == compare_[1] && gplib::input::CheckPush(gplib::input::KEY_DOWN))
				key_[2] = 3;
			if (key_[2] == compare_[2] && gplib::input::CheckPush(gplib::input::KEY_DOWN))
				key_[3] = 4;
			if (key_[3] == compare_[3] && gplib::input::CheckPush(gplib::input::KEY_LEFT))
				key_[4] = 5;
			if (key_[4] == compare_[4] && gplib::input::CheckPush(gplib::input::KEY_RIGHT))
				key_[5] = 6;
			if (key_[5] == compare_[5] && gplib::input::CheckPush(gplib::input::KEY_LEFT))
				key_[6] = 7;
			if (key_[6] == compare_[6] && gplib::input::CheckPush(gplib::input::KEY_RIGHT))
				key_[7] = 8;
			if (key_[7] == compare_[7] && gplib::input::CheckPush(gplib::input::KEY_B))
				key_[8] = 9;
			if (key_[8] == compare_[8] && gplib::input::CheckPush(gplib::input::KEY_BTN0))
				key_[9] = 10;
			if (key_[9] == compare_[9])
			{
				//�A�˒e�{�Q
				tips_ = 2;
				//���m�ɉ������特��炷
				if (seShow_)
				{
					seShow_ = false;
					gplib::se::DSound_Play("tips");
				}
			}
		}

		void update() override
		{
			operations();
			tips();
			title_bgm();
			bgm_volume();
		}
	};
}