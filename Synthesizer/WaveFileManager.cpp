//WaveFileManager.cpp

#include <map>
#include <vector>

#include "WaveFileManager.h"
#include "wavelib.h"

//using
using std::vector;
using std::map;
using std::string;

//�w�b�_�Ǘ��pmap���������\����
struct WaveHeaderMap{
	map<int, RIFF> map_riff; //RIFF�Ǘ��pmap
	map<int, fmt> map_fmt; //fmt�Ǘ��pmap
};

//�R���X�g���N�^
WaveFileManager::WaveFileManager(){
	header_map = new WaveHeaderMap;
}

//�f�X�g���N�^
WaveFileManager::~WaveFileManager(){
	delete header_map;
}

//�����f�[�^�擾
void WaveFileManager::load_sound(vector<double> &in, int song_id, std::string filename){
	RIFF load_riff;
	fmt load_fmt;
	data load_data;

	//wave�t�@�C���̓ǂݍ���
	load_wave_data(&load_riff, &load_fmt, &load_data, filename.c_str());

	//16bit���m�����̂ݏ����𑱍s
	if(load_fmt.BitsPerSample == 16 && load_fmt.Channel == 1){
		//vector�Ƀf�[�^���R�s�[
		short_to_double_vector(load_data.sounds, in, load_data.size_of_sounds);

		//�Ǘ��p��map�Ɋe��f�[�^���R�s�[
		header_map->map_riff.insert(map<int, RIFF>::value_type(song_id, load_riff));
		header_map->map_fmt.insert(map<int, fmt>::value_type(song_id, load_fmt));
	}
	else{
		std::cout << "���̃t�@�C���`���ɂ͑Ή����Ă��܂���" << std::endl;
		exit(0);
	}
	//���������
	delete load_data.sounds;
}

//�����f�[�^�ۑ�
void WaveFileManager::save_sound(const vector<double>& sound, int song_id, string filename, WaveHeader *head){
	data save_data; //�ۑ����鉹����data�`�����N
	short w_sample; //��Ɨp�ϐ�

	//data�`�����N�̍쐬
	save_data.size_of_sounds = sound.size()*2;
	save_data.sounds = new unsigned char[save_data.size_of_sounds];
	for(int i=0;i<sound.size();i++){
		w_sample = (short)sound[i];
		save_data.sounds[2*i] = (unsigned char)(w_sample & 0xFF);
		save_data.sounds[2*i+1] = (unsigned char)(w_sample >> 8);
	}

	//ID���w�肳��Ă���Ƃ�
	if(song_id != NULL){
		//������ID�����݂���Ƃ�
		if(header_map->map_riff.count(song_id) == 1){
			RIFF save_riff = header_map->map_riff[song_id];
			fmt save_fmt = header_map->map_fmt[song_id];
			save_wave_data(&save_riff, &save_fmt, &save_data, filename.c_str());
		}
		//ID���Ȃ��Ƃ�
		else{
			std::cout << "ID�����݂��܂���(ID:" << song_id << ")" << std::endl;
		}
	}
	//ID���w�肳��Ă��Ȃ��Ƃ�
	else{
		if(head != NULL){
			RIFF save_riff;
			fmt save_fmt;

			save_riff.SIZE = 36 + save_data.size_of_sounds; //36�̓w�b�_�T�C�Y
			save_fmt.SIZE = 16; //���j�APCM��fmt�T�C�Y
			save_fmt.Channel = head->channel;
			save_fmt.SamplesPerSec = head->samples_per_sec;
			save_fmt.BytesPerSec = head->bytes_per_sec;
			save_fmt.BlockSize = head->block_size;
			save_fmt.BitsPerSample = head->bits_per_sample;

			save_wave_data(&save_riff, &save_fmt, &save_data, filename.c_str());
		}
		else{
			std::cout << "�w�b�_�[��񂪂���܂���" << std::endl;
		}
	}
}

//�y�퉹�f�[�^�̓ǂݍ���
void WaveFileManager::load_instrument(string filename, vector<double> &inst){
	RIFF ins_riff;
	fmt ins_fmt;
	data ins_data;

	//wav�t�@�C����ǂݍ���
	string pass("./instrument/");
	pass += filename;
	load_wave_data(&ins_riff,&ins_fmt,&ins_data, pass.c_str());

	//16bit���m�����̂ݏ����𑱍s
	if(ins_fmt.BitsPerSample == 16 && ins_fmt.Channel == 1){
		//vector�Ƀf�[�^���R�s�[
		short_to_double_vector(ins_data.sounds, inst, ins_data.size_of_sounds);
	}
	else{
		std::cout << "���̃t�@�C���`���ɂ͑Ή����Ă��܂���" << std::endl;
		exit(0);
	}
	//���������
	delete ins_data.sounds;
}

//�T���v�����O���[�g�擾
int WaveFileManager::get_samples_per_sec(int song_id){
	//ID�����݂���Ƃ�
	if(header_map->map_fmt.count(song_id) == 1){
		return header_map->map_fmt[song_id].SamplesPerSec;
	}
	else{
		std::cerr << "ID�����݂��܂���(ID:" << song_id << ")" << std::endl;
		return 0.0;
	}
}

//16bitWav�f�[�^��double�^��vector�ɕϊ�
void WaveFileManager::short_to_double_vector(unsigned char *org, vector<double> &out, int org_length){
	short sample;

	out.resize(org_length/2);
	for(int i=0;i<org_length/2;i++){
		sample = (org[2*i+1] << 8) + org[2*i];
		out[i] = (double)sample;
	}
}
