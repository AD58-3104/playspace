
#pragma once

#include <string>
#include <ctime>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "hpl_types.h"
#include "hajime_command.h"

using boost::asio::ip::udp;

namespace CitBrains {

class InfoShare
{
	friend class InfoShareBroker;
public:
	// 定数
	static const int NUM_PLAYERS = 6;
	static const int COMM_INFO_PORT0                = 7110; //!< CommInfoで使用するPORTのはじめのポート
	                                                        //1:7110, 2:7111, 3:7112, 4:7113, 5:7114
	static const int MAX_COMM_INFO_OBJ              = 7;    //!< 共有するオブジェクトの最大値(はじめはボール)
	static const int MAX_STRING                     = 42;   //!< メッセージの最大値
	static const int MAX_BEHAVIOR_STRING            = 32;

	static const unsigned char COMM_EXIST           = 0x80; //!< 発見しかたどうかのフラグ
	static const unsigned char COMM_OUR_SIDE        = 0x40; //!< 自チームかどうかのフラグ
	static const unsigned char COMM_OPPOSITE_SIDE   = 0x20; //!< 敵チームかどうかのフラグ
	static const unsigned char COMM_NOT_EXIST       = 0x00; //!< 発見しないときの値

	static const int MAX_BLACK_POLES                = 6;    //!< 黒ポールの数
	static const int MAX_TARGET_POS                 = 1;
	static const int MAX_MAGENTA_OBJECTS            = 3;    //!< マゼンタロボットの数
	static const int MAX_CYAN_OBJECTS = MAX_MAGENTA_OBJECTS;//!< シアンロボットの数

	static const int NO_MAGENTA                     = 0;    //!< マゼンタの番号(色の番号とは異なる)
	static const int NO_CYAN                        = 1;    //!< シアンの番号

	static const int THREADTYPE_RECV                = 0;
	static const int THREADTYPE_SEND                = 1;

	//! 変数
	int id;                                                 //!< ID : 1-6
	int our_color;                                          //!< our_color (0:magenta,1:cyan) <<7
	int comm_info_port;                                     //!< udpを送信するポート
	int cf_own;                                             // 自己位置の確信度(0-100)
	int cf_ball;                                            // ボール位置の確信度(0-100)
	bool is_detect_ball;                                    // ボールを検出しているかどうかのフラグ
	Pos2D ball_gl;                                          // フィールド座標でのボールの位置
	std::vector<Pos2D> our_robot_gl;                        // フィールド座標での自チームのロボットの位置
	std::vector<Pos2D> enemy_robot_gl;                      // フィールド座標での敵チームのロボットの位置
	std::vector<Pos2D> black_pole_gl;                       // フィールド座標での黒ポールの位置
	int status;                                             // ロボットのステータス
	int strategy_no;                                        // 戦略の番号　(現在未使用)
	int voltage;                                            // 電圧（10mV単位）
	int temperature;                                        // 温度（deg）
	int highest_servo;                                      // 最も高いサーボの番号
	std::string command;                                    // メッセージ
	std::string current_behavior_name;
	std::vector<Pos2D> target_pos_vec;

	std::string ipaddress;                                  //!< 受信用のIPアドレス
	std::string broadcastip;                                //!< 送信用のIPアドレス(最後は255にする)
	float recvTime;                                         //!< 保存した時間


	// 情報共有される構造体
	struct comm_info_T
	{
		unsigned char id;                                   // our_color << 7 | ID : 1-6
		unsigned char cf_own;                               // 0-100 : 自己位置の確信度     (0-100)
		unsigned char cf_ball;                              // 0-100 : ボールの位置の確信度 (0-100)
		unsigned char object[MAX_COMM_INFO_OBJ][4];         // ボール，敵，味方，障害物の有無と座標値
		unsigned char status;                               // ロボットの状態
		unsigned char fps;                                  // フレームレート
		unsigned char voltage;                              // 電圧値(10mV単位) >> 3
		unsigned char temperature;                          // 温度
		unsigned char highest_servo;                        // 最も温度の高いサーボの番号
		unsigned char command[MAX_STRING];
		unsigned char current_behavior_name[MAX_BEHAVIOR_STRING];
		comm_info_T ()                                      // comm_info_Tの初期化
			: id(0), cf_own(0), cf_ball(0), status(0), fps(0),
				voltage(0), temperature(0), highest_servo(0)
		{
			int i;
			for(i = 0; i < MAX_COMM_INFO_OBJ; i ++)
			{
				memset(object[i], 0, 4);
			}
			memset(command, 0, MAX_STRING);
			memset(current_behavior_name, 0, MAX_BEHAVIOR_STRING);
		}
	} comm_info;

	/*
	 * @brief コンストラクタ
	 * @param[in] id 1-6 (0がないことに注意)
	 * @param[in] self_id 1-6
	 * @param[in] our_color 0,1 (0:magenta, 1:cyan)
	 * @param[in] ipaddress ex. "192.168.1.131"
	 */
	InfoShare(int id, int self_id, int our_color, std::string ipaddress);

	/*
	 * @brief デストラクタ
	 */
	~InfoShare();

	/*
	 * @brief IDの設定
	 * @param[in] id 1-6 (0がないことに注意)
	 * @param[in] our_color 味方の色
	 * @return 0:正常 -1:エラー
	 */
	int SetID(int id, int our_color);

	/*
	 * @brief IDの取得
	 * @param[out] id 1-6 (0がないことに注意)
	 * @param[out] our_color 味方の色
	 * @return 0:正常
	 */
	int GetID(int *id, int *our_color);

	/*
	 * @brief IPアドレスを設定
	 * @param[in] ipaddress IPアドレス
	 * @return 0:正常
	 */
	int SetIPAddress(std::string ipaddress);

	/*
	 * @brief 色を変更する
	 * @param[in] our_color 味方の色
	 * @return 0:正常
	 */
	int ChangeColor(int our_color);

	/*
	 * @brief 情報共有のデータを送信
	 * @param[in] ball_gl フィールド座標でのボールの位置
	 * @param[in] self_pos フィールド座標での自分の位置
	 * @param[in] our_robot_gl フィールド座標での自チームのロボットの位置
	 * @param[in] enemy_robot_gl フィールド座標での敵チームのロボットの位置
	 * @param[in] black_pole_gl フィールド座標での黒ポールの位置
	 * @param[in] message メッセージの文字列
	 * @param[in] behavior_name
	 * @param[in] target_pos_vec
	 * @param[in] state 状態
	 * @return 送信した文字数
	 */
	int SendCommonInfo(Pos2DCf ball_gl_cf, Pos2DCf self_pos_cf, std::vector<Pos2D> &our_robot_gl, std::vector<Pos2D> &enemy_robot_gl, std::vector<Pos2D> &black_pole_gl, int fps, std::string message, std::string behavior_name, std::vector<Pos2D> &target_pos_vec, RobotStatus state);

	/*
	 * @brief 情報共有のデータを受信
	 * @return 1:受信, 0:受信せず
	 */
	int RecvCommonInfo(InfoShare &out_info);

	/*
	 * @brief 自チームからのデータかの判別
	 * @param[in] our_color
	 * @return 1:自チーム 0:敵チーム
	 */
	int FromMyTeam(int our_color);

	boost::mutex mut; // 排他制御
	boost::mutex send_mutex;
	boost::condition_variable send_condition;
	void copyInfoTo(InfoShare &dest);
	float getTime();
	void setTimeFunc(float (*func)()); // get time func
private:
	/*
	 * @brief 通信ポートのオープン
	 * @param[in] thread type THREADTYPE_RECV: receiving, THREADTYPE_SEND: sending
	 * @return 0:正常 -1:エラー
	 */
	int OpenPort(int recv_or_send);

	/*
	 * @brief 通信ポートのクローズ
	 * @return 0:正常 -1:エラー
	 */
	int ClosePort();

	/*
	 * @brief 通信で受信する関数をループさせる
	 */
	void RecvThreadRun();

	/*
	 * @brief 通信で送信する関数をループさせる
	 */
	void SendThreadRun();

	/*
	 * @brief 送信すべきデータがセットされたときに送信する
	 * @return 送信したデータサイズ
	 */
	std::size_t  SendCommonInfoThread();

	bool terminated;
	bool exist_send_data;
	boost::thread th;                   // スレッド
	boost::asio::io_service io_service; // UDP通信用
	udp::socket socket;                 // ソケット
	udp::endpoint recv_endpoint;
	udp::endpoint send_endpoint;
	float (*timeFunc)();

	/*
	 * @brief 情報共有のためにdataに値を設定する
	 * @param[in] pos 位置データ
	 * @param[in] is_our_side 味方かどうかのフラグ
	 * @param[in] is_opposite_side 敵かどうかのフラグ
	 * @return data[4] 設定した値
	 */
	unsigned char *SetCommInfoObject(Pos2D pos, unsigned char data[4], bool is_our_side, bool is_opposite_side);

	/*
	 * @brief 情報共有のためにdataから値を取り出す
	 * @param[in] data[] 位置のデータ
	 * @return pos 位置データ
	 */
	static bool GetCommInfoObject(const unsigned char *data, Pos2D *pos, bool *is_our_side, bool *is_opposite_side);
};

class InfoShareBroker //ここに受信スレッド持たせる。ID判別して特定のオブジェクトに渡す。
{
public:
	InfoShareBroker() : mSelfID(-1)
	{
	}
	void setup(int id, int ourcolor, const std::string &ipaddress, float (*timefunc)())
	{
		mInfoShares.resize(InfoShare::NUM_PLAYERS);
		for(int i=0; i<InfoShare::NUM_PLAYERS; i++) {
			mInfoShares[i] = new CitBrains::InfoShare(i+1, id, ourcolor, ipaddress+".255");
			mInfoShares[i]->setTimeFunc(timefunc);
		}
		mSelfID = id;
		mTerminated = false;
#ifdef VREP_SIMULATOR
		mInfoShares[mSelfID-1]->OpenPort(InfoShare::THREADTYPE_SEND);
		mThread = boost::thread(boost::bind(&InfoShareBroker::thread_run, this));
#endif
	}
	~InfoShareBroker()
	{
		terminate();
	}
	InfoShare *getInfo(int id) //排他制御してないけど平気かな ここはコピーしたやつを返すべき
	{
		return mInfoShares[id-1];
	}
	void terminate() {
		mTerminated = true;
#ifdef VREP_SIMULATOR
		mThread.join();
		mInfoShares[mSelfID-1]->ClosePort();
#endif
		for(int i=0; i<mInfoShares.size(); i++) {
			delete mInfoShares[i];
		}
	}
private:
	void thread_run();
	int mSelfID;
#ifdef VREP_SIMULATOR
	boost::thread mThread;
#endif
	bool mTerminated;
	std::vector<CitBrains::InfoShare*> mInfoShares;
};

};

