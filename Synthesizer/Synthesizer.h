//Synthesizer.h

#pragma once

//合成音設定
struct SynthSetting{
	//サンプルあたりのビット数
	int bits_per_sample;
	//サンプリングレート[Hz]
	int samples_per_sec;
};

//シンセサイズパラメータ
struct SynthParam{
	//キャリア振幅
	double carrier_amp;
	//キャリア周波数
	double carrier_freq;
	//変調指数
	double mod_index;
	//モジュレータ周波数
	double mod_freq;
	//再生時間
	double time;
};

class Synthesizer{
//メソッド
public:
	//コンストラクタ
	Synthesizer(SynthSetting setting);
	//シンセサイズ
	void synthesize(std::vector<double> *wave, SynthParam param);

//メンバ変数
private:
	//合成音設定
	SynthSetting setting;
	//8bitサンプル最大値
	static const int EIGHT_BITS_MAX;
	//16bitサンプル最大値
	static const int SIXTEEN_BITS_MAX;
};