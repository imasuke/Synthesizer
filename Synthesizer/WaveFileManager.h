//WaveFileManager.h

//インクルードガード
#pragma once

struct WaveHeaderMap;

struct WaveHeader{
	int channel; //チャンネル数
	int samples_per_sec; //サンプリングレート[Hz]
	int bytes_per_sec; //データ速度[byte/sec]
	int block_size; //ブロックサイズ
	int bits_per_sample; //サンプルあたりのビット数
};

class WaveFileManager{
//メソッド
public:
	//コンストラクタ
	WaveFileManager();
	//デストラクタ
	~WaveFileManager();
	//音声データ取得
	void load_sound(std::vector<double> &in, int song_id, std::string filename="");
	//音声データ保存
	void save_sound(const std::vector<double> &sound, int song_id, std::string filename="", WaveHeader *head=NULL);
	//サンプリングレート取得
	int get_samples_per_sec(int song_id);
	//楽器音データの取得
	void load_instrument(std::string filename, std::vector<double> &inst);

private:
	//16bitのwavデータをdouble型のvectorに変換
	void short_to_double_vector(unsigned char *org, std::vector<double> &out, int org_length);

//メンバ変数
private:
	//ヘッダ管理
	WaveHeaderMap *header_map;
};