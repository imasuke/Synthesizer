//Synthesizer.cpp

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>

#include "Synthesizer.h"

//using
using std::vector;

//定数
const int Synthesizer::EIGHT_BITS_MAX = 255;
const int Synthesizer::SIXTEEN_BITS_MAX = 32767;

//コンストラクタ
Synthesizer::Synthesizer(SynthSetting setting){
	this->setting = setting;
}

//シンセサイズ
void Synthesizer::synthesize(vector<double> *wave, SynthParam param){
	vector<double> &r_wave = *wave;

	//vectorのリサイズ
	r_wave.resize((int)(param.time * setting.samples_per_sec));
	int len = r_wave.size();

	//振幅の決定
	double amp;
	switch(setting.bits_per_sample){
	case 8:
		amp = param.carrier_amp * EIGHT_BITS_MAX/2;
		break;
	case 16:
		amp = param.carrier_amp * SIXTEEN_BITS_MAX;
		break;
	default:
		std::cerr << "サンプルのビット数が不正です" << std::endl;
		exit(0);
		break;
	}

	//波形の生成
	double arg, t;
	for(int i=0;i<len;i++){
		t = (double)i / setting.samples_per_sec;
		arg = 2 * M_PI * param.carrier_freq * t + param.mod_index * sin(2*M_PI*param.mod_freq*t); 
		r_wave[i] = amp * sin(arg);
	}
	//エンベロープをつける
	generate_envelope(wave, param);
	//8bitWav用の補正
	if(setting.bits_per_sample == 8){
		for(int i=0;i<len;i++){
			r_wave[i] += 128;
		}
	}
}

//エンベロープジェネレータ
void Synthesizer::generate_envelope(vector<double> *wave, SynthParam param){
	vector<double> &r_wave = *wave;
	int len = r_wave.size();

	double env;
	int attack_i = param.attack * setting.samples_per_sec;
	int decay_i = (param.attack + param.decay) * setting.samples_per_sec;
	int release_i = len - param.release * setting.samples_per_sec;

	for(int i=0;i<len;i++){
		//Attack
		if(i <= attack_i){
			env = (double)i / attack_i;
		}
		//Decay
		else if(attack_i < i && i < decay_i){
			env = 1.0 - (1.0 - param.sustain) / (decay_i - attack_i) * (i - attack_i);
		}
		//Release
		else if(release_i < i){
			env = param.sustain * (len - i) / (len - release_i);
		}
		//Sustain
		else{
			env = param.sustain;
		}
		r_wave[i] *= env;
	}
}