#include <memory>
#include <string>
#include <iostream>
#include "Stage.h"
#include "player.h"
#include "Enemy.h"
#include "shot.h"
#include "tool.h"
#include "bomb.h"
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

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//画像優先度ｚ座標設定説明　小さい順から画面の前に
	//0.05fレベル  ゲームクリア描画、ゲームオーバー描画、メッセージ描画、HP描画、気力描画、レベルアップ描画、警告描画
	//0.1fレベル 　すべての爆発アニメ、プレイヤー攻撃
	//0.15fレベル　なし
	//0.2fレベル　 地雷、UFO、ヘリコプター、投げ弾
	//0.25fレベル　エネミー爆弾、UFO攻撃、ヘリコプター攻撃
	//0.3fレベル　 プレイヤー、ボス
	//0.35fレベル　ミニエネミー、道具
	//0.4fレベル 　雷、道具ゲットアニメ、プレイヤーの影
	//0.45fレベル  BGMアイコン、HP描字
	//0.5fレベル　 ステージ背景描画、地面描画、撃殺数描画、レベル描画
	//0.6fレベル   LOGO、選択し
	//0.7fレベル   タイトル背景
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Keyの設定　     キーボードに対応説明
	//KEY_BTN0        -> A
	//KEY_BTN1        -> S
	//KEY_BTN2        -> D
	//KEY_B           -> B
	//KEY_SPACE       -> space
	//KEY_MOUSE_LBTN  -> 左クリック
	//KEY_MOUSE_RBTN  -> 右クリック
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	Stage::Stage(const std::string& objectName,const int& tips)
		:
		Object(objectName)
	{
		//隠し技コマンド　値渡し
		tips_ = tips;
		//ファイル読み込み
		//画像
		graph::Draw_LoadObject("stage_back", "res/gra/Stage/back.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("ground", "res/gra/Stage/ground.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("over", "res/gra/Stage/over.png", 0x00000000);
		graph::Draw_LoadObject("clear", "res/gra/Stage/clear.png", 0x00000000);
		graph::Draw_LoadObject("message", "res/gra/Stage/message.png", 0x00000000);
		graph::Draw_LoadObject("count", "res/gra/Stage/count.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("kill", "res/gra/Stage/kill.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("stage_warning", "res/gra/Stage/warning.png", 0x00000000);
		graph::Draw_LoadObject("stage_side", "res/gra/Stage/side.png", 0x00000000);
		graph::Draw_LoadObject("player", "res/gra/Player/player.png");
		graph::Draw_LoadObject("all_explosion", "res/gra/Player/all_explosion.png", 0x00000000);
		graph::Draw_LoadObject("get_tool", "res/gra/Player/esc.png", 0x00000000);
		graph::Draw_LoadObject("ammo", "res/gra/Player/ammo.png", 0x00000000);
		graph::Draw_LoadObject("p_bullet", "res/gra/Player/bullet.png");
		graph::Draw_LoadObject("levelup", "res/gra/Player/levelup.png",0x00000000);
		graph::Draw_LoadObject("level", "res/gra/Player/level.png");
		graph::Draw_LoadObject("level_count", "res/gra/Player/count.png");
		graph::Draw_LoadObject("heal", "res/gra/Player/heal.png",0x00000000);
		graph::Draw_LoadObject("boss", "res/gra/Enemy/boss.png");
		graph::Draw_LoadObject("bullet_explosion", "res/gra/Enemy/bullet_explosion.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("e_explosion", "res/gra/Enemy/explosion.png", 0x00000000);
		graph::Draw_LoadObject("bomb2", "res/gra/Enemy/mini_explosion.png", 0x00000000);
		graph::Draw_LoadObject("ld_explosion", "res/gra/Enemy/ld_explosion.png", 0xFFFFFFFF);
		graph::Draw_LoadObject("ufo_explosion", "res/gra/Enemy/ufo_explosion.png");
		graph::Draw_LoadObject("e_missiles", "res/gra/Enemy/e_missile.png");
		graph::Draw_LoadObject("mini", "res/gra/Enemy/miniboss.png", 0x00000000);
		graph::Draw_LoadObject("lightning", "res/gra/Enemy/lightning.png");
		graph::Draw_LoadObject("landmine", "res/gra/Enemy/landmine.png");
		graph::Draw_LoadObject("ufo_atk", "res/gra/Enemy/miniufo_attack.png");
		graph::Draw_LoadObject("ufo", "res/gra/Enemy/miniufo.png");
		graph::Draw_LoadObject("heli", "res/gra/Enemy/helicopter.png");
		graph::Draw_LoadObject("e_bullet", "res/gra/Enemy/bullet.png");
		graph::Draw_LoadObject("track", "res/gra/Enemy/track.png");
		graph::Draw_LoadObject("rotate", "res/gra/Enemy/rotate.png");
		graph::Draw_LoadObject("throw_explosion", "res/gra/Enemy/throw_explosion.png");
		//BGM、SE
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
		DSound_LoadFile("e_explosion", "res/se/se_11.wav");
		DSound_LoadFile("kill", "res/se/se_12.wav");
		DSound_LoadFile("landmine", "res/se/se_13.wav");
		DSound_LoadFile("all_explosion", "res/se/se_14.wav");
		DSound_LoadFile("boss_atk", "res/se/se_15.wav");
		DSound_LoadFile("boss_atk2", "res/se/se_16.wav");
		DSound_LoadFile("win", "res/se/se_17.wav");
		DSound_LoadFile("lose", "res/se/se_18.wav");
		DSound_LoadFile("throw", "res/se/se_19.wav");
		DSound_LoadFile("throw_explosion", "res/se/se_20.wav");
		DSound_LoadFile("level_up", "res/se/se_21.wav");
		DSound_LoadFile("heal_se", "res/se/se_22.wav");
		DSound_LoadFile("def", "res/se/se_23.wav");
		DSound_LoadFile("warning_se", "res/se/se_24.wav");
	}

	//プレイヤーのオブジェクトを取得
	shared_ptr<Player> Stage::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//プレイヤーの値を返し
		return player;
	}
	
	//デストラクタ
	Stage::~Stage()
	{

	}
	//初期化
	void Stage::init()
	{
		//出現判定
		insertAsChild(new Player("player", tips_));
		insertAsChild(new Enemy("boss"));

		//エネミー->プレイヤーの当たり判定
		insertAsChild(new Collider("判定", "ammo", "player"));
		insertAsChild(new Collider("判定", "bomb4", "player"));
		insertAsChild(new Collider("判定", "bomb5", "player"));
		insertAsChild(new Collider("判定", "landmine", "player"));
		insertAsChild(new Collider("判定", "bomb2", "player"));
		insertAsChild(new Collider("判定", "e_bullet", "player"));
		insertAsChild(new Collider("判定", "lightning", "player"));
		insertAsChild(new Collider("判定", "e_missiles", "player"));
		//プレイヤーエネミーの当たり判定
		insertAsChild(new Collider("判定", "p_bullet", "boss"));
		insertAsChild(new Collider("判定", "p_bullet", "mini"));
		insertAsChild(new Collider("判定", "p_bullet", "landmine"));
		insertAsChild(new Collider("判定", "p_bullet", "heli"));
		insertAsChild(new Collider("判定", "p_bullet", "ufo"));
		insertAsChild(new Collider("判定", "p_bullet", "track"));
		

		sel_alpha = 0;          //透明度初期化
		mes_alpha = 0;
		mes_count = 0;          //メッセージカウント数初期化
		se_show = true;         //SE再生制御
		bgm_show = true;        //BGM再生制御
		icon_alpha = 255;       //アイコン透明度制御
		warn_alpha = 255;       //warning透明度制御
		side_alpha = 255;       //side透明度制御
		hide_count = 0;         //warn描画表現カウント初期化
		warn_show = true;       //warn描画表現制御
		side_posX = 0;          //描画移動関数初期化
		kill_count = 0;         //撃殺数を初期化
		kill_total = 0;
		kill_num = 50;          //撃殺数５０／回、ｈｐ回復する
		kill_regen = 0.f;       //回復値初期化
		kill_heal_times = 0.5f; //プレイヤーの最大ｈｐにより回復倍数、０.５f->５０％
		player_hp = 0.f;        //プレイヤー損したｈｐ値初期化
		count_w = 0;            //画像サイズ変数初期化
		count_h = 0;
		result_ = 0;            //結果パタンを初期化
		clear_scale = 1.3f;     //描画する画像の大きさ初期化
		over_scale = 1.3f;
		mes_scale = 0.9f;
		icon_scale = 0.5f;
		count_scale = 0.9f;
		num_1 = 0;              //撃殺数表示の初期化、個位
		num_2 = 0;              //十位
		num_3 = 0;              //百位
	}

	//状態により表示するmessageを変化
	void Stage::result(const int& result)
	{
		result_ = result;

		//gameclear状態
		if (result_ == 1)
		{
			sel_alpha += 5;
			if (sel_alpha > 255) sel_alpha = 255;
			//gameclear画像のサイズを取得
			float clear_w = (float)gplib::graph::Draw_GetImageWidth("clear");
			float clear_h = (float)gplib::graph::Draw_GetImageHeight("clear");
			//gameclearを描画する
			gplib::graph::Draw_Graphics(gplib::system::WINW / 2, gplib::system::WINH / 2,
				0.05f, "clear", 0, 0, (int)clear_w, (int)clear_h, 0.f, clear_scale, clear_scale, sel_alpha);
			//通常BGMを停止し、WINのSEを流せ
			DShow_Stop("stage_bgm");
			if (se_show)
			{
				DSound_Play("win");
				se_show = false;
			}
			message();
		}
		//gameover状態
		else if (result_ == 2)
		{
			sel_alpha += 5;
			if (sel_alpha > 255) sel_alpha = 255;
			//gameover画像のサイズを取得
			float over_w = (float)gplib::graph::Draw_GetImageWidth("over");
			float over_h = (float)gplib::graph::Draw_GetImageHeight("over");
			//gameoverを描画する
			gplib::graph::Draw_Graphics(gplib::system::WINW / 2, gplib::system::WINH / 2,
				0.05f, "over", 0, 0, (int)over_w, (int)over_h, 0.f, over_scale, over_scale, sel_alpha);
			//通常BGMを停止し、LOSEのSEを流せ
			DShow_Stop("stage_bgm");
			if (se_show)
			{
				DSound_Play("lose");
				se_show = false;
			}
			message();
		}
	}

	//message描画
	void Stage::message()
	{
		//カウント数でメッセージを制御する
		//見え隠れようにする
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

		//message画像のサイズを取得
		float message_w = (float)gplib::graph::Draw_GetImageWidth("message");
		float message_h = (float)gplib::graph::Draw_GetImageHeight("message");
		//messageを描画する
		gplib::graph::Draw_Graphics(gplib::system::WINW / 2, gplib::system::WINH / 2 + 80,
			0.05f, "message", 0, 0, (int)message_w, (int)message_h, 0.f, mes_scale, mes_scale, mes_alpha);

		//スペースでタイトルへ
		if (input::CheckPush(input::KEY_SPACE))
		{
			insertToParent(new Logo("logo"));
			DSound_Play("button");
			kill();
		}
	}

	//BGM再生制御
	void Stage::stage_bgm()
	{
		if (bgm_show)
			DShow_Play("stage_bgm");
		if (!bgm_show)
			DShow_Pause("stage_bgm");
	}

	//キーにより再生制御を行い
	void Stage::volume()
	{
		if (input::CheckPush(input::KEY_F1))
		{
			//BGM再生
			bgm_show = true;
			//透明度を戻す
			icon_alpha = 255;
		}
		if (input::CheckPush(input::KEY_F2))
		{
			//BGM停止
			bgm_show = false;
			//透明度を戻す
			icon_alpha = 255;
		}
	}

	//BGM再生かどうか確認するアイコン
	//アイコンサイズ　144*144
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
	//intで敵を倒し時の戻り値を返す
	int Stage::e_kill(const int& count)
	{
		kill_count = count;
		return kill_count;
	}
	//撃殺数描画
	void Stage::total()
	{
		//撃殺数
		kill_total = kill_total + kill_count;
		
		//プレイヤーのオブジェクトを取得
		auto p = getPlayerPrt();

		//敵を倒したら、個位の描画から＋１に
		if (kill_count > 0)
		{
			num_1 += count_w;
			if (num_1 > count_w * 9)
			{
				num_1 = 0;
				num_2 += count_w;
			}
			else if (num_2 > count_w * 9)
			{
				num_1 = 0;
				num_2 = 0;
				num_3 += count_w;
			}
			//もし最大数を超えたら、０に戻す
			else if (num_3 > count_w * 9)
			{
				num_1 = 0;
				num_2 = 0;
				num_3 = 0;
			}
			kill_count = 0;
		}
		//合計５０の敵を撃殺すると、プレイヤーのｈｐは大幅に回復する
		if (kill_total == kill_num)
		{
			//プレイヤーのｈｐ設定により回復値変化する
			kill_regen = p->playerMaxHp() * kill_heal_times;
			//プレイヤーはＭａｘｈｐなら回復０、それ以来に設定値に回復する
			player_hp = p->playerHp() >= p->playerMaxHp() ? 0 : kill_regen;
			if (p->playerHp() + kill_regen > p->playerMaxHp())
				player_hp = p->playerMaxHp() - p->playerHp();
			p->p_receiving_atk(-player_hp);
			//０に戻る
			kill_total = 0;
		}
	}
	//warning
	void Stage::warning()
	{
		int warn_w = gplib::graph::Draw_GetImageWidth("stage_warning");
		int warn_h = gplib::graph::Draw_GetImageHeight("stage_warning");
		int side_w = gplib::graph::Draw_GetImageWidth("stage_side");
		int side_h = gplib::graph::Draw_GetImageHeight("stage_side");
		int side_length = system::WINW / side_w;

		if (warn_show)
		{
			++hide_count;
			if ((hide_count >= 0 && hide_count < 40) || (hide_count >= 80 && hide_count < 120) || hide_count >= 160)
			{
				//255 / 7 = 36　３６フレームに一回隠す
				warn_alpha -= 7;
				if (warn_alpha < 0) warn_alpha = 0;
				//ＳＥは最初の1フレームに流させる
				if (hide_count <= 1) DSound_Play("warning_se");
				//最後にsideも隠す
				if (hide_count >= 160)
				{
					side_alpha -= 7;
					if (side_alpha < 0) side_alpha = 0;
				}
			}
			if ((hide_count >= 40 && hide_count < 80) || (hide_count >= 120 && hide_count < 160))
			{
				warn_alpha += 7;
				if (warn_alpha > 255) warn_alpha = 255;
			}

			//warning文字
			graph::Draw_Graphics(system::WINW / 2, system::WINH / 2, 0.05f, "stage_warning", 0, 0, warn_w, warn_h, 0.f, 1.8f, 1.8f, warn_alpha);

			//上辺
			for (int i = 0;i <= side_length + 2;++i)
				graph::Draw_Graphics(i*side_w - side_posX, side_h / 2, 0.05f, "stage_side", 0, 0, side_w, side_h, 0.f, 1.f, 1.f, side_alpha);
			//下辺
			for (int i = 0;i <= side_length + 2;++i)
				graph::Draw_Graphics(i*side_w - side_posX, system::WINH - side_h / 2, 0.05f, "stage_side", 0, 0, side_w, side_h, 0.f, 1.f, 1.f, side_alpha);
			
			side_posX += 4;
			if (side_posX > side_w) side_posX = 0;
		}
		//初期だけ描画に制御
		if (hide_count > 200 && warn_alpha == 0) warn_show = false;
	}

	void Stage::render()
	{
		graph::Draw_Clear();
		//背景
		//サイズを取る
		int back_w = gplib::graph::Draw_GetImageWidth("stage_back");
		int back_h = gplib::graph::Draw_GetImageHeight("stage_back");
		graph::Draw_Graphics(system::WINW / 2.f, system::WINH / 2.f, 0.5f, "stage_back", 0, 0,
			back_w, back_h, 0, (float)system::WINW / (float)back_w, (float)system::WINH / (float)back_h);
		//地面
		int ground_w = gplib::graph::Draw_GetImageWidth("ground");
		int ground_h = gplib::graph::Draw_GetImageHeight("ground");
		int ground_length = system::WINW / ground_w;
		int ground_posY = system::WINH - ground_h;
		for (int i = 0; i < ground_length + 1; ++i)
			graph::Draw_GraphicsLeftTop((i*ground_w), ground_posY, 0.5f, "ground", 0, 0, ground_w, ground_h);
		//撃殺数描画
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
		warning();
	}

	void Stage::update()
	{
		//アテイムを確率で出現処理
		if (math::GetRandom(0, 300) == 0)
		{
			insertAsChild(new Tool1("ammo"));
		}
		
		total();
		stage_bgm();
		volume();
		e_kill(kill_count);
	}
}