//main.cpp

#include <iostream>
#include <vector>

#include "WaveFileManager.h"
#include "Synthesizer.h"

int main(void){
	WaveFileManager manager;
	WaveHeader head;
	std::vector<double> wave;

	//�V���Z�T�C�U�̐ݒ�
	SoundSetting setting;
	setting.bits_per_sample = 16;
	setting.samples_per_sec = 44100;
	//�V���Z�T�C�U
	Synthesizer synth(setting);

	//�V���Z�T�C�U�̃p�����[�^
	SynthParam param;

	//�V���Z�T�C�Y
	synth.synthesize(&wave, param);

	//Wave�t�@�C���w�b�_�̐ݒ�
	head.bits_per_sample = 16;
	head.block_size = 2;
	head.channel = 1;
	head.samples_per_sec = 44100;
	head.bytes_per_sec = head.samples_per_sec * (head.bits_per_sample / 8) * head.channel;
	//�o��
	manager.save_sound(wave, NULL, "synth.wav", &head);

	return 0;
}