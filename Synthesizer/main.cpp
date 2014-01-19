//main.cpp

#include <iostream>
#include <vector>

#include "WaveFileManager.h"
#include "Synthesizer.h"

int main(void){
	WaveFileManager manager;
	WaveHeader head;
	std::vector<double> wave;

	//シンセサイザの設定
	SynthSetting setting;
	setting.bits_per_sample = 16;
	setting.samples_per_sec = 44100;
	//シンセサイザ
	Synthesizer synth(setting);

	//シンセサイザのパラメータ
	SynthParam param;
	param.carrier_amp = 0.5;
	param.carrier_freq = 1040;
	param.mod_index = 10.0;
	param.mod_freq = 300;
	param.time = 5.0;

	//シンセサイズ
	synth.synthesize(&wave, param);

	//Waveファイルヘッダの設定
	head.bits_per_sample = 16;
	head.block_size = 2;
	head.channel = 1;
	head.samples_per_sec = 44100;
	head.bytes_per_sec = head.samples_per_sec * (head.bits_per_sample / 8) * head.channel;
	//出力
	manager.save_sound(wave, NULL, "synth.wav", &head);

	return 0;
}