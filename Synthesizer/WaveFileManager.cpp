//WaveFileManager.cpp

#include <map>
#include <vector>

#include "WaveFileManager.h"
#include "wavelib.h"

//using
using std::vector;
using std::map;
using std::string;

//ヘッダ管理用mapを持った構造体
struct WaveHeaderMap{
	map<int, RIFF> map_riff; //RIFF管理用map
	map<int, fmt> map_fmt; //fmt管理用map
};

//コンストラクタ
WaveFileManager::WaveFileManager(){
	header_map = new WaveHeaderMap;
}

//デストラクタ
WaveFileManager::~WaveFileManager(){
	delete header_map;
}

//音声データ取得
void WaveFileManager::load_sound(vector<double> &in, int song_id, std::string filename){
	RIFF load_riff;
	fmt load_fmt;
	data load_data;

	//waveファイルの読み込み
	load_wave_data(&load_riff, &load_fmt, &load_data, filename.c_str());

	//16bitモノラルのみ処理を続行
	if(load_fmt.BitsPerSample == 16 && load_fmt.Channel == 1){
		//vectorにデータをコピー
		short_to_double_vector(load_data.sounds, in, load_data.size_of_sounds);

		//管理用のmapに各種データをコピー
		header_map->map_riff.insert(map<int, RIFF>::value_type(song_id, load_riff));
		header_map->map_fmt.insert(map<int, fmt>::value_type(song_id, load_fmt));
	}
	else{
		std::cout << "このファイル形式には対応していません" << std::endl;
		exit(0);
	}
	//メモリ解放
	delete load_data.sounds;
}

//音声データ保存
void WaveFileManager::save_sound(const vector<double>& sound, int song_id, string filename, WaveHeader *head){
	data save_data; //保存する音声のdataチャンク
	short w_sample; //作業用変数

	//dataチャンクの作成
	save_data.size_of_sounds = sound.size()*2;
	save_data.sounds = new unsigned char[save_data.size_of_sounds];
	for(int i=0;i<sound.size();i++){
		w_sample = (short)sound[i];
		save_data.sounds[2*i] = (unsigned char)(w_sample & 0xFF);
		save_data.sounds[2*i+1] = (unsigned char)(w_sample >> 8);
	}

	//IDが指定されているとき
	if(song_id != NULL){
		//引数のIDが存在するとき
		if(header_map->map_riff.count(song_id) == 1){
			RIFF save_riff = header_map->map_riff[song_id];
			fmt save_fmt = header_map->map_fmt[song_id];
			save_wave_data(&save_riff, &save_fmt, &save_data, filename.c_str());
		}
		//IDがないとき
		else{
			std::cout << "IDが存在しません(ID:" << song_id << ")" << std::endl;
		}
	}
	//IDが指定されていないとき
	else{
		if(head != NULL){
			RIFF save_riff;
			fmt save_fmt;

			save_riff.SIZE = 36 + save_data.size_of_sounds; //36はヘッダサイズ
			save_fmt.SIZE = 16; //リニアPCMのfmtサイズ
			save_fmt.Channel = head->channel;
			save_fmt.SamplesPerSec = head->samples_per_sec;
			save_fmt.BytesPerSec = head->bytes_per_sec;
			save_fmt.BlockSize = head->block_size;
			save_fmt.BitsPerSample = head->bits_per_sample;

			save_wave_data(&save_riff, &save_fmt, &save_data, filename.c_str());
		}
		else{
			std::cout << "ヘッダー情報がありません" << std::endl;
		}
	}
}

//楽器音データの読み込み
void WaveFileManager::load_instrument(string filename, vector<double> &inst){
	RIFF ins_riff;
	fmt ins_fmt;
	data ins_data;

	//wavファイルを読み込む
	string pass("./instrument/");
	pass += filename;
	load_wave_data(&ins_riff,&ins_fmt,&ins_data, pass.c_str());

	//16bitモノラルのみ処理を続行
	if(ins_fmt.BitsPerSample == 16 && ins_fmt.Channel == 1){
		//vectorにデータをコピー
		short_to_double_vector(ins_data.sounds, inst, ins_data.size_of_sounds);
	}
	else{
		std::cout << "このファイル形式には対応していません" << std::endl;
		exit(0);
	}
	//メモリ解放
	delete ins_data.sounds;
}

//サンプリングレート取得
int WaveFileManager::get_samples_per_sec(int song_id){
	//IDが存在するとき
	if(header_map->map_fmt.count(song_id) == 1){
		return header_map->map_fmt[song_id].SamplesPerSec;
	}
	else{
		std::cerr << "IDが存在しません(ID:" << song_id << ")" << std::endl;
		return 0.0;
	}
}

//16bitWavデータをdouble型のvectorに変換
void WaveFileManager::short_to_double_vector(unsigned char *org, vector<double> &out, int org_length){
	short sample;

	out.resize(org_length/2);
	for(int i=0;i<org_length/2;i++){
		sample = (org[2*i+1] << 8) + org[2*i];
		out[i] = (double)sample;
	}
}
