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
		RECT option[2];    //選択し設置
		int alpha_[2];     //選択していない時に透明度変化関数
		int bgm_alpha_;    //BGMを表示するアイコンの透明度制御
		int tips_;         //隠し技コマンド　値渡しのため設置する
		int key_[10];      //押したキーを順番で値を渡し
		int compare_[10];  //キーの値を対比するため設定
		int start_w;       //描画する画像のサイズを取得
		int start_h;
		int exit_w;
		int exit_h;
		bool bgmShow_;     //BGMの再生を制御する
		bool seShow_;      //SEの再生を制御する
		bool decide_[2];      //選択しを決めるかどうか

	public:

		Title(const std::string& objectName) :
			Object(objectName)
		{
			//エフェクト
			effect::Create(0, 0, effect::EFFECTTYPE::FADEINWHITE);
			//画像読み込み
			gplib::graph::Draw_LoadObject("title_back", "res/gra/Title/back.png", 0xFFFFFFFF);
			gplib::graph::Draw_LoadObject("title_logo", "res/gra/Title/logo.png", 0xFFFFFFFF);
			gplib::graph::Draw_LoadObject("start", "res/gra/Title/start.png",0xFFFFFFFF);
			gplib::graph::Draw_LoadObject("exit", "res/gra/Title/exit.png", 0xFFFFFFFF);
			gplib::graph::Draw_LoadObject("bgm_icon", "res/gra/Title/bgm_icon.png", 0xFFFFFFFF);
			//サウンド読み込み
			gplib::bgm::DShow_LoadFile("title_bgm", "res/bgm/bgm_01.wav");
			gplib::se::DSound_LoadFile("button", "res/se/se_01.wav");
			gplib::se::DSound_LoadFile("tips", "res/se/se_02.wav");

			//startボタン画像のサイズを取得
			start_w = gplib::graph::Draw_GetImageWidth("start");
			start_h = gplib::graph::Draw_GetImageHeight("start");

			//exitボタン画像のサイズを取得
			exit_w = gplib::graph::Draw_GetImageWidth("exit");
			exit_h = gplib::graph::Draw_GetImageHeight("exit");

			//二つの選択し位置を初期化する
			//left,top,right,bottom
			//画像サイズにより設定
			option[0] = { gplib::system::WINW / 3 - start_w / 2, gplib::system::WINH / 2 + 170 - start_h / 2,
				gplib::system::WINW / 3 + start_w / 2, gplib::system::WINH / 2 + 170 + start_h / 2 };
			option[1] = { gplib::system::WINW - gplib::system::WINW / 3 - exit_w / 2, gplib::system::WINH / 2 + 170 - exit_h / 2,
				gplib::system::WINW - gplib::system::WINW / 3 + exit_w / 2, gplib::system::WINH / 2 + 170 + exit_h / 2 };
			
			//透明度の初期値設定
			alpha_[0] = 255;
			alpha_[1] = 50;
			
			//重ねないように対比する値は＋１で初期化
			for (int i = 0;i < 10;++i)
				compare_[i] = i + 1;
			for (int i = 0;i < 10;++i)
				key_[i] = 0;
			//再生かどうか確認する
			bgmShow_ = true;
			//再生かどうか確認する
			seShow_ = true;
			//選択しを決めるかどうか確認する
			decide_[0] = true;
			decide_[1] = false;
			//アイコン透明度初期化
			bgm_alpha_ = 255;
		}
		//デストラクタ
		Title::~Title()
		{

		}

		void render() override
		{
			//背景画像のサイズを取得
			int back_w = gplib::graph::Draw_GetImageWidth("title_back");
			int back_h = gplib::graph::Draw_GetImageHeight("title_back");
			//背景を描画する
			gplib::graph::Draw_GraphicsLeftTop(0, 0, 0.7f, "title_back", 0, 0, back_w, back_h, 0.f, nullptr,
				gplib::system::WINW / (float)back_w,
				gplib::system::WINH / (float)back_h);

			//logo画像のサイズを取得
			int logo_w = gplib::graph::Draw_GetImageWidth("title_logo");
			int logo_h = gplib::graph::Draw_GetImageHeight("title_logo");
			//logoを描画する
			gplib::graph::Draw_Graphics(gplib::system::WINW / 2, gplib::system::WINH / 2 - 180,
				0.6f, "title_logo", 0, 0, logo_w, logo_h, 0.f, 1.1f, 1.1f);

			//startボタンを描画する
			gplib::graph::Draw_Graphics(gplib::system::WINW / 3, gplib::system::WINH / 2 + 170,
				0.6f, "start", 0, 0, start_w, start_h, 0.f, 1.f, 1.f, alpha_[0]);

			//exitボタンを描画する
			gplib::graph::Draw_Graphics(gplib::system::WINW - gplib::system::WINW / 3, gplib::system::WINH / 2 + 170,
				0.6f, "exit", 0, 0, exit_w, exit_h, 0.f, 1.f, 1.f, alpha_[1]);
			
			//描画チェック
//			for (int i = 0;i < 2;++i)
//				gplib::graph::Draw_CkRect(option[i], ARGB(255, 255, 0, 0));
			bgm_icon();
		}
		//スタートオプション
		//確認してステージへ
		void start_option()
		{
			gplib::se::DSound_Play("button");
			effect::Create(0, 0, effect::EFFECTTYPE::FADEOUTWHITE);
			insertToParent(new Stage("stage", tips_));
			//ステージへ移動する時にも停止させる
			bgmShow_ = false;
			kill();
		}
		//エグジットオプション
		//確認して終了に
		void exit_option()
		{
			gplib::se::DSound_Play("button");
			effect::Create(0, 0, effect::EFFECTTYPE::FADEOUTBLACK);
			exit(0);
		}
		//再生制御
		void title_bgm()
		{
			if (bgmShow_)
				gplib::bgm::DShow_Play("title_bgm");
			else if (!bgmShow_)
				gplib::bgm::DShow_Pause("title_bgm");
		}

		//BGM再生かどうか確認するアイコン
		//アイコンサイズ　144*144
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

		//キーにより再生制御を行い
		void bgm_volume()
		{
			if (gplib::input::CheckPush(gplib::input::KEY_F1))
			{
				//再生
				bgmShow_ = true;
				//透明度を戻す
				bgm_alpha_ = 255;
			}
			if (gplib::input::CheckPush(gplib::input::KEY_F2))
			{
				//停止
				bgmShow_ = false;
				//透明度を戻す
				bgm_alpha_ = 255;
			}
		}
		//選択し操作
		void operations()
		{
			//マウス
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
			//キーポート
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
			//左クリックとスペースキーで決める
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

		//隠し技 コナミコマンド 上上下下左右左右BA
		//プレイヤーの一般攻撃は２連射->４連射に変化
		//-----------------------------------------------------------------
		//フレーム制御することできていないので、速押しするとできなくなることはある
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
				//連射弾＋２
				tips_ = 2;
				//正確に押したら音を鳴らす
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