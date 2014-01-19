//Synthesizer.h

#pragma once

//�������ݒ�
struct SoundSetting{
	//�T���v��������̃r�b�g��
	int bits_per_sample;
	//�T���v�����O���[�g[Hz]
	int samples_per_sec;
};

//�V���Z�T�C�Y�p�����[�^
struct SynthParam{
	//�L�����A�U��
	double carrier_amp;
	//�L�����A���g��
	double carrier_freq;
	//�ϒ��w��
	double mod_index;
	//���W�����[�^���g��
	double mod_freq;
	//�Đ�����
	double time;
};

class Synthesizer{
//���\�b�h
public:
	//�R���X�g���N�^
	Synthesizer(SoundSetting setting);
	//�V���Z�T�C�Y
	void synthesize(std::vector<double> *wave, SynthParam param);

//�����o�ϐ�
private:
	//�������ݒ�
	SoundSetting *setting;
};