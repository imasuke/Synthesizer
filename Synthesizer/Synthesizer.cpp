//Synthesizer.cpp

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>

#include "Synthesizer.h"

//using
using std::vector;

//�萔
const int Synthesizer::EIGHT_BITS_MAX = 255;
const int Synthesizer::SIXTEEN_BITS_MAX = 32767;

//�R���X�g���N�^
Synthesizer::Synthesizer(SynthSetting setting){
	this->setting = setting;
}

//�V���Z�T�C�Y
void Synthesizer::synthesize(vector<double> *wave, SynthParam param){
	vector<double> &r_wave = *wave;

	//vector�̃��T�C�Y
	r_wave.resize((int)(param.time * setting.samples_per_sec));
	int len = r_wave.size();

	//�U���̌���
	double amp;
	switch(setting.bits_per_sample){
	case 8:
		amp = param.carrier_amp * EIGHT_BITS_MAX/2;
		break;
	case 16:
		amp = param.carrier_amp * SIXTEEN_BITS_MAX;
		break;
	default:
		std::cerr << "�T���v���̃r�b�g�����s���ł�" << std::endl;
		exit(0);
		break;
	}

	//�g�`�̍���
	double arg, t;
	for(int i=0;i<len;i++){
		t = (double)i / setting.samples_per_sec;
		arg = 2 * M_PI * param.carrier_freq * t + param.mod_index * sin(2*M_PI*param.mod_index*t); 
		r_wave[i] = amp * sin(arg) + (setting.bits_per_sample == 8 ? 128 : 0);
	}
}