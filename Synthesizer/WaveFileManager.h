//WaveFileManager.h

//�C���N���[�h�K�[�h
#pragma once

struct WaveHeaderMap;

struct WaveHeader{
	int channel; //�`�����l����
	int samples_per_sec; //�T���v�����O���[�g[Hz]
	int bytes_per_sec; //�f�[�^���x[byte/sec]
	int block_size; //�u���b�N�T�C�Y
	int bits_per_sample; //�T���v��������̃r�b�g��
};

class WaveFileManager{
//���\�b�h
public:
	//�R���X�g���N�^
	WaveFileManager();
	//�f�X�g���N�^
	~WaveFileManager();
	//�����f�[�^�擾
	void load_sound(std::vector<double> &in, int song_id, std::string filename="");
	//�����f�[�^�ۑ�
	void save_sound(const std::vector<double> &sound, int song_id, std::string filename="", WaveHeader *head=NULL);
	//�T���v�����O���[�g�擾
	int get_samples_per_sec(int song_id);
	//�y�퉹�f�[�^�̎擾
	void load_instrument(std::string filename, std::vector<double> &inst);

private:
	//16bit��wav�f�[�^��double�^��vector�ɕϊ�
	void short_to_double_vector(unsigned char *org, std::vector<double> &out, int org_length);

//�����o�ϐ�
private:
	//�w�b�_�Ǘ�
	WaveHeaderMap *header_map;
};