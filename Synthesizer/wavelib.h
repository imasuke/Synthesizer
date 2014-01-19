/* wave �t�@�C���p�ȈՃw�b�_�t�@�C�� wavelib.h */
/* c++�p�Ɉꕔ�v���O������ύX*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>

/* �^�̒�` */
/* RIFF�`�����N */
struct RIFF {
    char ID[5] = "RIFF";         /* RIFF�`�����NID("RIFF")   */
    int SIZE;           /* RIFF�`�����N�T�C�Y[byte] */
    char TYPE[5] = "WAVE";       /* RIFF�̎��("WAVE")       */
};
/* fmt�`�����N */
struct fmt {
    char ID[5] = "fmt ";         /* fmt�`�����NID("fmt ")    */
    int SIZE;           /* fmt�`�����N�T�C�Y[byte]  */
    int TYPE;           /* �t�H�[�}�b�g�^�C�v       */
    int Channel;        /* �`�����l����             */
    int SamplesPerSec;  /* �T���v�����O���[�g[Hz]   */
    int BytesPerSec;    /* �f�[�^���x[byte/sec]     */
    int BlockSize;      /* �u���b�N�T�C�Y           */
                        /* [byte/sample x channel]  */
    int BitsPerSample;  /* �T���v��������̃r�b�g�� */
                        /* [bit/sample]             */
};
/* data�`�����N */
struct data {
    char ID[5] = "data";          /* data�`�����NID("data")   */
    int size_of_sounds;  /* data�`�����N�T�C�Y[byte] */
    unsigned char *sounds; /* �g�`�f�[�^�ւ̃|�C���^ */
};


/* �֐��̃v���g�^�C�v�錾 */
/* WAVE�f�[�^�̃t�@�C������ */
void load_wave_data( struct RIFF *RIFF1, struct fmt *fmt1,
                     struct data *data1, const char name[] );
/* WAVE�f�[�^�̃t�@�C���o�� */
void save_wave_data( struct RIFF *RIFF1, struct fmt *fmt1,
                     struct data *data1, char const name[] );
/* �ȉ��� load/save_wave_data ���ŗ��p����Ă���֐� */
void read_char( FILE *fp, int n, char c[] );
void read_int( FILE *fp, int n, int *number );
void write_char( FILE *fp, int n, const char c[] );
void write_int( FILE *fp, int n, int number );
void error_process( const char message[] );
void error_process2( const char c1[], const char c2[] );


/* �ȉ��͊֐��̖{�� */
void load_wave_data( struct RIFF *RIFF1, struct fmt *fmt1, 
                     struct data *data1, const char name[] )
/* �t�@�C������WAVE�f�[�^��ǂݍ��ށD */
/* name[] �� "" �Ȃ�t�@�C��������́D*/
{
    char fname[256];            /* �t�@�C�����p�̕����z�� */
    FILE *fp;                   /* �t�@�C���|�C���^       */
    int i;                      /* ��ƕϐ�               */

    /* ���̓t�@�C���̃I�[�v�� */
    if ( name[0]=='\0' ){
        printf("�ǂݍ��� WAVE�t�@�C���̖��O (�g���q��wav) : ");
		std::cin >> fname;
    } else strcpy_s( fname, name );
    if ( fopen_s(&fp, fname, "rb" ) != 0 )
        error_process("�t�@�C�����I�[�v���ł��܂���ł����D");
    /* �f�[�^�̓ǂݍ��� */
    /* ========== RIFF 1 ========== */
    read_char( fp, 4, RIFF1->ID );             /* ID            */
      error_process2( RIFF1->ID, "RIFF" );     /* Error Process */
    read_int( fp, 4, &RIFF1->SIZE );           /* SIZE          */
    read_char( fp, 4, RIFF1->TYPE );           /* Type          */
      error_process2( RIFF1->TYPE, "WAVE" );   /* Error Process */
    /* ========== fmt 1 ========== */
    read_char( fp, 4, fmt1->ID );              /* ID            */
      error_process2( fmt1->ID, "fmt " );      /* Error Process */
    read_int( fp, 4, &fmt1->SIZE );            /* SIZE          */
    read_int( fp, 2, &fmt1->TYPE );            /* TYPE          */
    read_int( fp, 2, &fmt1->Channel );         /* Channel       */
    read_int( fp, 4, &fmt1->SamplesPerSec );   /* SamplesPerSec */
    read_int( fp, 4, &fmt1->BytesPerSec );     /* BytesPerSec   */
    read_int( fp, 2, &fmt1->BlockSize );       /* BlockSize     */
    read_int( fp, 2, &fmt1->BitsPerSample );   /* BitsPerSample */
    /* ========== data 1 ========== */
    read_char( fp, 4, data1->ID );             /* ID            */
      error_process2( data1->ID, "data" );     /* Error Process */
    read_int( fp, 4, &data1->size_of_sounds ); /* SIZE          */
	data1->sounds = new unsigned char[data1->size_of_sounds];
    if ( data1->sounds == NULL ){
        printf("���������m�ۂł��܂���D�v���O�������I�����܂��D\n");
        exit(1);
    } else {
        for(i=0;i<data1->size_of_sounds;i++){
            *(data1->sounds + i) = fgetc( fp );
        }
    }
    fclose(fp);
}


void save_wave_data( struct RIFF *RIFF1, struct fmt *fmt1,
                     struct data *data1, const char name[] )
/* WAVE�f�[�^���t�@�C���ɏ������ށD   */
/* name[] �� "" �Ȃ�t�@�C��������́D*/
{
    char fname[256];            /* �t�@�C�����p�̕����z�� */
    FILE *fp;                   /* �t�@�C���|�C���^       */
    int i;               /* ��ƕϐ�               */

    /* �o�̓t�@�C���̃I�[�v�� */
    if ( name[0]=='\0' ){
        printf("�o�͂��� WAVE�t�@�C���̖��O (�g���q��wav) : ");
		std::cin >> fname;
    } else strcpy_s( fname, name );
    if ( fopen_s(&fp, fname, "wb" ) != 0 )
        error_process("�t�@�C�����I�[�v���ł��܂���ł����D");
    /* �f�[�^�̏������� */
    /* ========== RIFF 1 ========== */
    write_char( fp, 4, RIFF1->ID );    /* ID   */
    write_int(  fp, 4, RIFF1->SIZE );  /* SIZE */
    write_char( fp, 4, RIFF1->TYPE );  /* TYPE */
    /* ========== fmt 1 ========== */
    write_char( fp, 4, fmt1->ID );            /* ID             */
    write_int(  fp, 4, fmt1->SIZE );          /* SIZE           */
    write_int(  fp, 2, fmt1->TYPE );          /* TYPE           */
    write_int(  fp, 2, fmt1->Channel );       /* Channel        */
    write_int(  fp, 4, fmt1->SamplesPerSec ); /* SamplesPerSec  */
    write_int(  fp, 4, fmt1->BytesPerSec );   /* BytesPerSec    */
    write_int(  fp, 2, fmt1->BlockSize );     /* BlockSize      */
    write_int(  fp, 2, fmt1->BitsPerSample ); /* BitsPerSample  */
    /* ========== data 1 ========== */
    write_char( fp, 4, data1->ID );              /* ID   */
    write_int(  fp, 4, data1->size_of_sounds );  /* SIZE */
    for(i=0;i<data1->size_of_sounds;i++){
        fputc( *(data1->sounds + i), fp );
    }
    fclose(fp);
}


void read_char( FILE *fp, int n, char c[] )
/* n�o�C�g�̕�������t�@�C������ǂݍ��� */
{
    int i;

    for(i=0;i<n;i++){
        c[i] = fgetc( fp );
    }
    c[n]='\0';  /* ������̍Ō��\��NULL�L�� */
}


void read_int( FILE *fp, int n, int *number )
/* n�o�C�g��int���t�@�C������ǂݍ��� */
/* Intel�o�C�g�I�[�_�ibyte�t���j      */
{
    int i,j,num;

    *number = 0;
    for(i=0;i<n;i++){
        num = fgetc( fp );
        for(j=0;j<i;j++){
            num = num * 256;
        }
        *number = *number + num;
    }
}


void write_char( FILE *fp, int n, const char c[] )
/* n�o�C�g�̕�������t�@�C���֏������� */
{
    int i;

    for(i=0;i<n;i++){
        fputc( c[i], fp );
    }
}


void write_int( FILE *fp, int n, int number )
/* n�o�C�g��number�̃f�[�^���t�@�C���֏������ށi�t���j */
{
    int i;

    for(i=0;i<n;i++){
        fputc( number % 256, fp );
        number = number / 256;
    }
}


void error_process( const char message[] )
/* �G���[���� */
{
    if ( message[0]=='\0' )
        printf("�\�z���Ȃ��G���[���������܂����D\n");
    else
        printf("%s\n",message);
    printf("�v���O�������I�����܂��D\n");
    exit(1);  /* �ُ�I�� */
}


void error_process2( const char c1[], const char c2[] )
/* �G���[�����Q */
/* c1 �� c2 ���قȂ��Ă����狭���I������ */
{
    if ( strcmp( c1, c2 ) != 0 ){
        printf("ID���قȂ�t�@�C���ł��D\n");
        printf("�v���O�������ُ�I�����܂��D\n");
        exit(1);  /* �ُ�I�� */
    }
}

