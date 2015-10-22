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
	//画像優先度ｚ座標設定説明　小さい順から画面の前に
	//0.05fレベル  ゲームクリア描画、ゲームオーバー描画、メッセージ描画、HP描画、気力描画
	//0.1fレベル 　すべての爆発アニメ、プレイヤー攻撃
	//0.15fレベル　なし
	//0.2fレベル　 地雷、UFO、ヘリコプター
	//0.25fレベル　エネミー爆弾、UFO攻撃、ヘリコプター攻撃
	//0.3fレベル　 プレイヤー、ボス
	//0.35fレベル　ミニエネミー、道具
	//0.4fレベル 　雷、道具ゲットアニメ、プレイヤーの影
	//0.45fレベル  BGMアイコン
	//0.5fレベル　 ステージ背景描画、地面描画、撃殺数描画
	//0.6fレベル   LOGO、選択し
	//0.7fレベル   タイトル背景
	///////////////////////////////////////////////////////////////////////////
	//Keyの設定　     キーボードに対応説明
	//KEY_BTN0        -> A
	//KEY_BTN1        -> S
	//KEY_BTN2        -> D
	//KEY_B           -> B
	//KEY_SPACE       -> space
	//KEY_MOUSE_LBTN  -> 左クリック
	//KEY_MOUSE_RBTN  -> 右クリック
	//////////////////////////////////////////////////////////////////////////

	Stage::Stage(const std::string& objectName, int tips)
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
		DSound_LoadFile("e_explode", "res/se/se_11.wav");
		DSound_LoadFile("kill", "res/se/se_12.wav");
		DSound_LoadFile("landmine", "res/se/se_13.wav");
		DSound_LoadFile("all_explode", "res/se/se_14.wav");
		DSound_LoadFile("boss_atk", "res/se/se_15.wav");
		DSound_LoadFile("boss_atk2", "res/se/se_16.wav");
		DSound_LoadFile("win", "res/se/se_17.wav");
		DSound_LoadFile("lose", "res/se/se_18.wav");
	}

	//プレイヤーのオブジェクトを取得
	shared_ptr<Player> Stage::getPlayerPrt()
	{
		auto object = getObjectFromRoot({ "player" });
		shared_ptr<game::Player> player = ci_ext::weak_to_shared<game::Player>(object);
		//プレイヤーの値を返し
		return player;
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

		sel_alpha = 0;          //透明度初期化
		mes_alpha = 0;
		mes_count = 0;          //メッセージカウント数初期化
		se_show = true;         //SE再生制御
		bgm_show = true;        //BGM再生制御
		icon_alpha = 255;       //アイコン透明度制御
		kill_count = 0;         //撃殺数を初期化
		kill_total = 0;
		kill_num = 50;          //撃殺数５０／回、ｈｐ回復する
		kill_healing = 0.f;     //回復値初期化
		kill_heal_using = 0.5f; //プレイヤーの最大ｈｐにより回復倍数
		player_hp = 0;          //プレイヤー損したｈｐ値初期化
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
	void Stage::result(int result)
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
	int Stage::e_kill(int count)
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
				num_3 = 0;
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
			//プレイヤーのｈｐ設定により回復値変化するa

			//hp描画は枠を超えないように
			if (p->playerHp() >= p->playerHpRight())
			{
				p->p_receiving_atk(0);
			}
			else if (p->playerHp() < p->playerHpRight())
			{
				//hpの描画位置の座標は逆なので、＋と-も逆にする
				player_hp = p->playerHpRight() - (int)p->playerHp();
				//損したｈｐは回復値より上回り->回復値で回復する
				if (player_hp >= (int)kill_healing)
					p->p_receiving_atk(-kill_healing);
				//損したｈｐは回復値より下回り->損したｈｐだけを回復する
				else if (player_hp < (int)kill_healing)
					p->p_receiving_atk(-(float)player_hp);
			}
			kill_total = 0;
		}
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
	}

	void Stage::update()
	{
		//アテイムを確率で出現処理
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