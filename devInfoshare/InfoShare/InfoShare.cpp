#undef NDEBUG
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cstdint>

#include "InfoShare.h"
#include <boost/bind.hpp>
#include <boost/math/constants/constants.hpp>

namespace CitBrains {

static inline double clamp(double val, double minv, double maxv) {
	return std::max(std::min(val, maxv), minv);
}

/*
 * @brief コンストラクタ
 * @param[in] id 1-6 (0がないことに注意)
 * @param[in] self_id 1-6
 * @param[in] our_color COLOR_MAGENTA(4), COLOR_CYAN(5)
 * @param[in] ipaddress ex. "192.168.1.131"
 * @param[in] itime 現在の時間(入れないほうがいいかも)
 */
InfoShare::InfoShare(int id = 1, int self_id = 1, int our_color = COLOR_MAGENTA, std::string iipaddress = "127.0.0.1")
	: socket(io_service), terminated(false), exist_send_data(false), is_detect_ball(0), cf_own(0), cf_ball(0), timeFunc(0)
{
	SetID(id, our_color);
	SetIPAddress(iipaddress);
	recvTime = getTime();
#if !defined VREP_SIMULATOR
	if(id == self_id)
		OpenPort(THREADTYPE_SEND);
	else
		OpenPort(THREADTYPE_RECV);
#endif
}

/*
 * @brief デストラクタ
 */
InfoShare::~InfoShare()
{
#if !defined VREP_SIMULATOR
	ClosePort();
#endif
}

/*
 * @brief 通信で受信する関数をループさせる
 */
void InfoShare::RecvThreadRun()
{
	terminated = false;
	while(!terminated) {
		RecvCommonInfo(*this);
	}
	std::cout << "Infoshare thread " << id << " terminated" << std::endl;
}

/*
 * @brief 通信で送信する関数をループさせる
 */
void InfoShare::SendThreadRun()
{
	terminated = false;
	while(!terminated) {
		SendCommonInfoThread();
	}
	std::cout << "Infoshare thread " << id << " terminated" << std::endl;
}

/*
 * @brief 通信ポートのオープン
 * 受信スレッドを起動して受信を行えるようにする．
 * @param[in] thread type THREADTYPE_RECV: receiving, THREADTYPE_SEND: sending
 * @return 0:正常 -1:エラー
 */
int InfoShare::OpenPort(int recv_or_send)
{
	try {
		if (socket.is_open()) ClosePort();
#ifdef WEBOTS_VHSC
		std::cout << "--------------------------OpenPort in WEBOTS_VHSC-----------------------------------------------" << std::endl;
		auto getSimulatorPort = []() -> int32_t //これ恐らくもういらないはず。
		{
			int32_t color = 0;
			int32_t ID = static_cast<int32_t>(getenv("ROBOCUP_ROBOT_ID")[0] - '0');
			if (std::string("blue") == std::string(getenv("ROBOCUP_TEAM_COLOR")))
			{
				color = 20;
			}
			return 10000 + color + ID;
		};
		auto getPlayerAddr = []() -> std::string
		{		
			int32_t ID = static_cast<int32_t>(getenv("ROBOCUP_ROBOT_ID")[0] - '0');
			std::string addr;
			if(ID == 1){
				addr = std::string(getenv("ROBOCUP_TEAM_PLAYER1_IP"));
			}
			else if(ID == 2){
				addr = std::string(getenv("ROBOCUP_TEAM_PLAYER2_IP"));
			}
			else if(ID == 3){
				addr = std::string(getenv("ROBOCUP_TEAM_PLAYER3_IP"));
			}
			else{
				addr = std::string(getenv("ROBOCUP_TEAM_PLAYER4_IP"));
			}
			addr.erase(addr.find_first_of(":"));
			std::cout << "player address is " << addr << std::endl;
			return addr;
		};
		static const int32_t ROBOCUP_SIMULATOR_BROADCAST_PORT = 3737; //送るのも受け取るのも3737でOKっぽい
		auto getSimulatorAddr = []()->std::string{				//どこから送られたかによってどのロボットからなのかを判断しているのでそこをどうにかする必要あり
			std::string addr(getenv("ROBOCUP_SIMULATOR_ADDR"));
        	addr.erase(addr.find_first_of(":"));
			std::cout << "simulator address is " << addr << std::endl;
			return addr;
		};
		static const std::string ROBOCUP_SIMULATOR_ADDR(getSimulatorAddr());
		static const std::string RECEIVE_ADDR(getPlayerAddr());
		// recv_endpoint = udp::endpoint(boost::asio::ip::address::from_string(RECEIVE_ADDR), ROBOCUP_SIMULATOR_BROADCAST_PORT);
		send_endpoint = udp::endpoint(boost::asio::ip::address::from_string(ROBOCUP_SIMULATOR_ADDR), ROBOCUP_SIMULATOR_BROADCAST_PORT);
		socket.open(udp::v4());
		// socket.bind(recv_endpoint);
#else
		recv_endpoint = udp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), comm_info_port);
		send_endpoint = udp::endpoint(boost::asio::ip::address::from_string(broadcastip), comm_info_port);
		socket.open(udp::v4());
		socket.bind(recv_endpoint);
		socket.set_option(boost::asio::socket_base::broadcast(true));
#endif
#if !defined VREP_SIMULATOR
		if(recv_or_send == THREADTYPE_RECV)
			th = boost::thread(boost::bind(&InfoShare::RecvThreadRun, this));
		else if(recv_or_send == THREADTYPE_SEND)
			th = boost::thread(boost::bind(&InfoShare::SendThreadRun, this));
		else
			return -1; /* invalid thread type */
#endif
	} catch (std::exception &e) {
		std::cout  << e.what() << std::endl;
		return -1;
	}
	return 0;
} //receiver のスレッドをゼロにして一つで受け取って他のオブジェクトに渡す

/*
 * @brief 通信ポートのクローズ
 * @return 0:正常 -1:エラー
 */
int InfoShare::ClosePort()
{
	terminated = true;
	socket.close();
	th.join();
	return 0;
}

/*
 * @brief IDの設定
 * @param[in] id 1-6 (0がないことに注意)
 * @param[in] our_color 味方の色
 * @return 0:正常 -1:エラー
 */
int InfoShare::SetID(int id, int our_color)
{
	assert((id > 0) && (id <= NUM_PLAYERS));
	assert((our_color == COLOR_MAGENTA) || (our_color == COLOR_CYAN));
	this->id        = id;
	this->our_color = our_color;
	comm_info_port  = COMM_INFO_PORT0 + id - 1;
	return 0;
}

/*
 * @brief IDの取得
 * @param[out] id 1-6 (0がないことに注意)
 * @param[out] our_color 味方の色
 * @return 0:正常
 */
int InfoShare::GetID(int *id, int *our_color)
{
	*id        = this->id;
	*our_color = this->our_color;
	return 0;
}

/*
 * @brief 色を変更する
 * @param[in] our_color 味方の色
 * @return 0:正常
 */
int InfoShare::ChangeColor(int our_color)
{
	assert((our_color == COLOR_MAGENTA) || (our_color == COLOR_CYAN));
	this->our_color = our_color;
	return 0;
}

/*
 * @brief IPアドレスを設定
 * @param[in] ipaddress IPアドレス
 * @return 0:正常
 */
int InfoShare::SetIPAddress(std::string ipaddress)
{
	// ブロードキャスト用にIPアドレスを変更
	this->ipaddress = ipaddress;
	try
	{
		broadcastip = ipaddress;
		broadcastip.erase(broadcastip.find_last_of("."));
		broadcastip += ".255";
	}
	catch (std::exception)
	{
		broadcastip = "255.255.255.255";
	}

	return 0;
}

/*
 * @brief 送信すべきデータがセットされたときに送信する
 * @return 送信したデータサイズ
 */
std::size_t InfoShare::SendCommonInfoThread()
{
	std::size_t ret = 0; // return value: size of send data
	struct comm_info_T info;

	boost::mutex::scoped_lock send_lock(send_mutex);
	while(!exist_send_data) send_condition.wait(send_lock);
	exist_send_data = false;
	info = comm_info;
	
	int len = strlen((const char *)info.command);
	try {
#ifdef VREP_SIMULATOR
		for(int i=0; i<5; i++) {
			send_endpoint = udp::endpoint(boost::asio::ip::address::from_string(broadcastip), COMM_INFO_PORT0 + i);
			ret = socket.send_to(boost::asio::buffer(&info, sizeof(comm_info_T) - (MAX_STRING - len) + 1), send_endpoint);
		}
#elif defined WEBOTS_VHSC
		//各ロボットに直接送信する場合はここでendpointをそれぞれ指定する
		// static const int32_t ROBOCUP_SIMULATOR_BROADCAST_PORT = 3737; //送るのも受け取るのも3737でOKっぽい
		// auto getSimulatorAddr = []()->std::string{				//どこから送られたかによってどのロボットからなのかを判断しているのでそこをどうにかする必要あり
		// 	std::string addr(getenv("ROBOCUP_SIMULATOR_ADDR"));
        // 	addr.erase(addr.find_first_of(":"));
		// 	return addr;
		// };
		// static const std::string ROBOCUP_SIMULATOR_ADDR(getSimulatorAddr());
		// for(int i=0; i<5; i++) 
		{
			// send_endpoint = udp::endpoint(boost::asio::ip::address::from_string(ROBOCUP_SIMULATOR_ADDR), ROBOCUP_SIMULATOR_BROADCAST_PORT);
			ret = socket.send_to(boost::asio::buffer(&info, sizeof(comm_info_T) - (MAX_STRING - len) + 1), send_endpoint);
		}
#else
		ret = socket.send_to(boost::asio::buffer(&info, sizeof(comm_info_T)), send_endpoint);
#endif
	} catch (std::exception &e) {
		std::cout << "socket send" << e.what() << std::endl;
	}
	return ret;
}

/*
 * @brief 情報共有のデータを送信
 * 
 * @param[in] ball_gl フィールド座標でのボールの位置
 * @param[in] self_pos フィールド座標での自分の位置
 * @param[in] our_robot_gl フィールド座標での自チームのロボットの位置
 * @param[in] enemy_robot_gl フィールド座標での敵チームのロボットの位置
 * @param[in] black_pole_gl フィールド座標での黒ポールの位置
 * @param[in] message メッセージの文字列
 * @param[in] behavior_name
 * @param[in] target_pos_vec
 * @param[in] state 状態
 * @return 0:success, other:error
 */
int InfoShare::SendCommonInfo(Pos2DCf ball_gl_cf, Pos2DCf self_pos_cf, std::vector<Pos2D> &our_robot_gl, std::vector<Pos2D> &enemy_robot_gl, std::vector<Pos2D> &black_pole_gl, int fps, std::string message, std::string behavior_name, std::vector<Pos2D> &target_pos_vec, RobotStatus state)
{
	unsigned int no = 0, i;
	boost::mutex::scoped_lock lock(mut);

	// 引数をクラス変数に保存する．
	cf_own = (int)self_pos_cf.cf;                           // 自己位置の確信度(0-100)
	cf_ball = (int)ball_gl_cf.cf;                           // ボール位置の確信度(0-100)
	is_detect_ball = ball_gl_cf.is_detect;                  // ボールを検出しているかどうか
	ball_gl = ball_gl_cf.pos;                               // ボールの位置
	this->our_robot_gl.clear();                             // 味方ロボットの位置
	for(i = 0; i < our_robot_gl.size(); i ++)
	{
		this->our_robot_gl.push_back(our_robot_gl[i]);
	}
	this->enemy_robot_gl.clear();                           // enemy robot is currently disabled and replace with target pos
	for(i = 0; i < enemy_robot_gl.size(); i ++)
	{
		this->enemy_robot_gl.push_back(enemy_robot_gl[i]);
	}
	this->target_pos_vec.clear();                           
	for(i = 0; i < target_pos_vec.size(); i ++)
	{
		this->target_pos_vec.push_back(target_pos_vec[i]);
	}
	this->black_pole_gl.clear();                            // 黒ポール（判別できないロボット）の位置
	for(i = 0; i < black_pole_gl.size(); i ++)
	{
		this->black_pole_gl.push_back(black_pole_gl[i]);
	}
	status = (state.Posture != STATE_POSTURE_STAND) ? 0x01 : 0x00; // 転倒のフラグ
	if (state.Active == STATE_IDLE) status |= 0x02;         // アイドル状態かどうかのフラグ
	strategy_no = 0;                                        // 戦略の番号（不使用）
	voltage = 0xff & (state.MotorVoltage >> 3);             // モータの電圧(mV) >> 3
	temperature = 0xff & state.Temperature;                 // モータの温度(degree)
	highest_servo = 0xff & (state.Temperature >> 8);        // 最も温度の高いモータ
	command = message;
	current_behavior_name = behavior_name;

	// 送信するデータの作成
	int our_color_flag = (our_color == COLOR_MAGENTA)? NO_MAGENTA:NO_CYAN;
	comm_info.id = (unsigned char)((our_color_flag << 7) | id); // 色とIDのセット
	comm_info.cf_own = (unsigned char)self_pos_cf.cf;       // 自己位置の確信度のセット
	comm_info.cf_ball = (unsigned char)ball_gl_cf.cf;       // ボール位置の確信度のセット
	if (ball_gl_cf.is_detect) {                             // ボールの位置(object[0])のセット
		SetCommInfoObject(ball_gl_cf.pos, comm_info.object[no], true, true);
		no++;
	} else {
		comm_info.object[no][0] = COMM_NOT_EXIST;
		no++;
	}

	// フィールド座標での自チームのロボット位置の設定(object[1]：自己位置)
	for(i = 0; i < MAX_MAGENTA_OBJECTS; i ++)
	{
		if (i >= our_robot_gl.size()) break;
		SetCommInfoObject(our_robot_gl[i], comm_info.object[no], true, false);
		no++;
		assert(no <= MAX_COMM_INFO_OBJ);
	}
	for(i = 0; i < MAX_MAGENTA_OBJECTS; i ++)
	{
		if (i >= enemy_robot_gl.size()) break;
		SetCommInfoObject(enemy_robot_gl[i], comm_info.object[no], false, true);
		no++;
		assert(no <= MAX_COMM_INFO_OBJ);
	}
	for(i = 0; i < MAX_BLACK_POLES; i ++)
	{
		if (i >= black_pole_gl.size()) break;
		if (no >= MAX_COMM_INFO_OBJ) break;
		SetCommInfoObject(black_pole_gl[i], comm_info.object[no], false, false);
		no++;
		assert(no <= MAX_COMM_INFO_OBJ);
	}
	for(i = 0; i < MAX_TARGET_POS; i ++)
	{
		if (i >= target_pos_vec.size()) break;
		if (no >= MAX_COMM_INFO_OBJ) break;
		SetCommInfoObject(target_pos_vec[i], comm_info.object[no], false, true);
		no++;
		assert(no <= MAX_COMM_INFO_OBJ);
	}
	// オブジェクトの数が7になるように不足分を足す
	for(i = no; i < MAX_COMM_INFO_OBJ; i ++)
	{
		comm_info.object[i][0] = COMM_NOT_EXIST;
	}

	// ロボットの状態の設定
	comm_info.status = (state.Posture != STATE_POSTURE_STAND) ? 0x01 : 0x00; // 転倒のフラグ
	if (state.Active == STATE_IDLE) comm_info.status |= 0x02;                // アイドル状態かどうかのフラグ
	comm_info.fps = fps;                                                     // フレームレート
	comm_info.voltage = 0xff & (state.MotorVoltage >> 3);                    // モータの電圧(mV) >> 3
	comm_info.temperature = 0xff & state.Temperature;                        // モータの温度(degree)
	comm_info.highest_servo = 0xff & (state.Temperature >> 8);               // 最も温度の高いモータ

	const unsigned int len_message = std::min<unsigned int>(message.size(), MAX_STRING - 1);
	for(unsigned int index = 0; index < len_message; index ++)
		comm_info.command[index] = message[index];
	comm_info.command[len_message] = '\0';
	const unsigned int len_behavior_name = std::min<unsigned int>(behavior_name.size(), MAX_BEHAVIOR_STRING - 1);
	for(unsigned int index = 0; index < len_behavior_name; index ++)
		comm_info.current_behavior_name[index] = behavior_name[index];
	comm_info.current_behavior_name[len_behavior_name] = '\0';

	recvTime = getTime();                                                    // 取得時間を保存
	{
		boost::mutex::scoped_lock send_lock(send_mutex);
		exist_send_data = true;
		send_condition.notify_one();
	}
	return 0;
}


/*
 * @brief 情報共有のデータを受信
 * @return 受信した文字数, -1:エラー
 */
int InfoShare::RecvCommonInfo(InfoShare &out_info) //この辺使ってバイト列から構築できるメソッドがあると便利(コンストラクタでもどっちでも)
{
	int i;
	bool is_our_side, is_opposite_side;

	boost::array<char, 2048> buf;
	size_t len = 0;

	try {
		len = socket.receive_from(boost::asio::buffer(buf), recv_endpoint);
	} catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		return -1;
	}

	// UDPの受信
	if (len < (sizeof(comm_info_T) - MAX_STRING)) return -1;                    // データが短すぎる場合は-1を戻す
	int our_color_flag = (our_color == COLOR_MAGENTA)? NO_MAGENTA:NO_CYAN;
	if ((buf[0] & (1 << 7)) != (our_color_flag << 7)) return 0;                 // 色が異なる場合は-1を戻す

	{
		char *p = (char *)&comm_info;
		for(i = 0; (i < len)&&(i < sizeof(comm_info_T)); i++)
		{
			*p ++ = buf[i];
		}
	}

	int no = 0;
	unsigned char *b;
	Pos2D pos;
	{
		boost::mutex::scoped_lock lock(mut);                                    // 排他制御
#ifdef VREP_SIMULATOR
		out_info.id = (comm_info.id & 0x7f);
#else
		assert(id == (comm_info.id & 0x7f));                                    // ポートとIDに不整合があるかのチェック
#endif
		no++;
		out_info.cf_own = comm_info.cf_own;                                     // 自己位置の確信度を代入 (0~100)
		out_info.cf_ball = comm_info.cf_ball;                                   // ボールの確信度を代入 (0~100)
		b = comm_info.object[0];                                                // 受信したデータを4バイト分bに代入
		out_info.is_detect_ball = GetCommInfoObject(b, &out_info.ball_gl, &is_our_side, &is_opposite_side);
		// フィールド座標でのボール位置
		out_info.our_robot_gl.clear();
		out_info.enemy_robot_gl.clear();
		out_info.black_pole_gl.clear();
		out_info.target_pos_vec.clear();

		for (i = 1; i < MAX_COMM_INFO_OBJ; i++)                                 // 残り６個分の座標値を取得
		{
			b = comm_info.object[i];

			// フィールド座標での位置を取得
			if (b[0] & COMM_EXIST)
			{
				GetCommInfoObject(b, &pos, &is_our_side, &is_opposite_side);    // もし発見していたら
				if (is_our_side)                                                // 自チームのロボットの場合
				{
					out_info.our_robot_gl.push_back(pos);
				}
				else if (is_opposite_side)                                      // enemy robot is currently disabled and replaced with target pos
				{
					//out_info.enemy_robot_gl.push_back(pos);
					out_info.target_pos_vec.push_back(pos);
				}
				else
				{
					out_info.black_pole_gl.push_back(pos);                      // それ以外は黒ポール
				}
			}
		}

		out_info.status        = comm_info.status;                              // ステータスの代入
		out_info.strategy_no   = comm_info.fps;                                 // 戦略番号の代入(現在不使用)
		out_info.voltage       = comm_info.voltage << 8;                        // 電圧の代入
		out_info.temperature   = comm_info.temperature;                         // サーボの温度の代入
		out_info.highest_servo = comm_info.highest_servo;                       // 最も温度の高いサーボの番号
		out_info.command       = (const char *)comm_info.command;
	}
	out_info.recvTime = getTime();                                              // 現在の時刻を取得

	return 0;
}

/*
 * @brief 自チームからのデータかの判別
 * @param[in] our_color
 * @return 1:自チーム 0:敵チーム
 */
int InfoShare::FromMyTeam(int our_color)
{
	if (this->our_color == our_color) return 1;
	return 0;
}

/*
 * @brief 情報共有のためにdataに値を設定する
 * @param[in] pos 位置データ
 * @param[in] is_our_side 味方かどうかのフラグ
 * @param[in] is_opposite_side 敵かどうかのフラグ
 * @return data[4] 設定した値
 */
unsigned char *InfoShare::SetCommInfoObject(Pos2D pos, unsigned char data[4], bool is_our_side, bool is_opposite_side)
{
	const double MAX_LENGTH = 5000;
	data[0] = COMM_EXIST;                                           //!< 発見したかどうかのフラグ
	if (is_our_side) data[0] |= COMM_OUR_SIDE;                      //!< 味方チームかどうかのフラグ
	if (is_opposite_side) data[0] |= COMM_OPPOSITE_SIDE;            //!< 敵チームかどうかのフラグ
	double x = clamp(pos.x, -MAX_LENGTH, MAX_LENGTH);
	double y = clamp(pos.y, -MAX_LENGTH, MAX_LENGTH);
	double the = pos.th;
	while(the < -M_PI) the += 2.0*M_PI;
	while(the >  M_PI) the -= 2.0*M_PI;

	data[0] |= (unsigned char)((((int)x / 10) & 0x03c0) >> 6);  //!< x座標の設定
	data[1]  = (unsigned char)((((int)x / 10) & 0x003f) << 2);
	data[1] |= (unsigned char)((((int)y / 10) & 0x0300) >> 8);  //!< y座標の設定
	data[2]  = (unsigned char) (((int)y / 10) & 0x00ff);
	data[3]  = (unsigned char) (((int)(the * 180.0f / M_PI) + 180.0f) / 2.0f); //!< thetaの設定

	return data;
}

/*
 * @brief 情報共有のためにdataから値を取り出す
 * @param[in] data[] 位置のデータ
 * @return pos 位置データ
 */
bool InfoShare::GetCommInfoObject(const unsigned char *data, Pos2D *pos, bool *is_our_side, bool *is_opposite_side)
{
	bool res;
	int x, y, theta;

	if ((data[0] & COMM_EXIST) != 0)                                //!< オブジェクトを発見したかどうか
	{
		res = true;
		x = ((data[0] & 0x0f) << 6) + ((data[1] & 0xfc) >> 2);
		if ((x & 0x0200) != 0) x = -(0x0400 - x);
		x *= 10;
		pos->x = (float)x;
		y = ((data[1] & 0x03) << 8) + data[2];
		if ((y & 0x0200) != 0) y = -(0x0400 - y);
		y *= 10;
		pos->y = (float)y;
		theta = (data[3] * 2) - 180;
		pos->th = (float)(theta * boost::math::constants::pi<float>() / 180.0f);
		*is_our_side = ((data[0] & COMM_OUR_SIDE) != 0) ? true : false;
		*is_opposite_side = ((data[0] & COMM_OPPOSITE_SIDE) != 0) ? true : false;
	}
	else
	{
		res = false;
	}

	return res;
}

void InfoShare::copyInfoTo(InfoShare &dest) //コピー中はロックする
{
	boost::mutex::scoped_lock(mut);
	dest.ball_gl        = ball_gl;
	dest.our_robot_gl   = our_robot_gl;
	dest.enemy_robot_gl = enemy_robot_gl;
	dest.black_pole_gl  = black_pole_gl;
	dest.target_pos_vec = target_pos_vec;
	dest.cf_own         = cf_own;
	dest.cf_ball        = cf_ball;
	dest.recvTime       = recvTime;
	dest.command        = command;
}

void InfoShare::setTimeFunc(float (*func)())
{
	timeFunc = func;
}

float InfoShare::getTime()
{
	if (timeFunc) {
		return timeFunc();
	}
	return (float)time(0);
}

void InfoShareBroker::thread_run()
{
	InfoShare info;
	while(!mTerminated)
	{
		if (!mInfoShares[mSelfID-1]->RecvCommonInfo(info)) {
			if (1 <= info.id && info.id <= InfoShare::NUM_PLAYERS && info.id != mSelfID) {
				info.copyInfoTo(*mInfoShares[info.id-1]);
			}
		}
	}
}

};