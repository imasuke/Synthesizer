/* wave ファイル用簡易ヘッダファイル wavelib.h */
/* c++用に一部プログラムを変更*/
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>

/* 型の定義 */
/* RIFFチャンク */
struct RIFF {
    char ID[5] = "RIFF";         /* RIFFチャンクID("RIFF")   */
    int SIZE;           /* RIFFチャンクサイズ[byte] */
    char TYPE[5] = "WAVE";       /* RIFFの種類("WAVE")       */
};
/* fmtチャンク */
struct fmt {
    char ID[5] = "fmt ";         /* fmtチャンクID("fmt ")    */
    int SIZE;           /* fmtチャンクサイズ[byte]  */
    int TYPE;           /* フォーマットタイプ       */
    int Channel;        /* チャンネル数             */
    int SamplesPerSec;  /* サンプリングレート[Hz]   */
    int BytesPerSec;    /* データ速度[byte/sec]     */
    int BlockSize;      /* ブロックサイズ           */
                        /* [byte/sample x channel]  */
    int BitsPerSample;  /* サンプルあたりのビット数 */
                        /* [bit/sample]             */
};
/* dataチャンク */
struct data {
    char ID[5] = "data";          /* dataチャンクID("data")   */
    int size_of_sounds;  /* dataチャンクサイズ[byte] */
    unsigned char *sounds; /* 波形データへのポインタ */
};


/* 関数のプロトタイプ宣言 */
/* WAVEデータのファイル入力 */
void load_wave_data( struct RIFF *RIFF1, struct fmt *fmt1,
                     struct data *data1, const char name[] );
/* WAVEデータのファイル出力 */
void save_wave_data( struct RIFF *RIFF1, struct fmt *fmt1,
                     struct data *data1, char const name[] );
/* 以下は load/save_wave_data 内で利用されている関数 */
void read_char( FILE *fp, int n, char c[] );
void read_int( FILE *fp, int n, int *number );
void write_char( FILE *fp, int n, const char c[] );
void write_int( FILE *fp, int n, int number );
void error_process( const char message[] );
void error_process2( const char c1[], const char c2[] );


/* 以下は関数の本体 */
void load_wave_data( struct RIFF *RIFF1, struct fmt *fmt1, 
                     struct data *data1, const char name[] )
/* ファイルからWAVEデータを読み込む． */
/* name[] が "" ならファイル名を入力．*/
{
    char fname[256];            /* ファイル名用の文字配列 */
    FILE *fp;                   /* ファイルポインタ       */
    int i;                      /* 作業変数               */

    /* 入力ファイルのオープン */
    if ( name[0]=='\0' ){
        printf("読み込む WAVEファイルの名前 (拡張子はwav) : ");
		std::cin >> fname;
    } else strcpy_s( fname, name );
    if ( fopen_s(&fp, fname, "rb" ) != 0 )
        error_process("ファイルがオープンできませんでした．");
    /* データの読み込み */
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
        printf("メモリが確保できません．プログラムを終了します．\n");
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
/* WAVEデータをファイルに書き込む．   */
/* name[] が "" ならファイル名を入力．*/
{
    char fname[256];            /* ファイル名用の文字配列 */
    FILE *fp;                   /* ファイルポインタ       */
    int i;               /* 作業変数               */

    /* 出力ファイルのオープン */
    if ( name[0]=='\0' ){
        printf("出力する WAVEファイルの名前 (拡張子はwav) : ");
		std::cin >> fname;
    } else strcpy_s( fname, name );
    if ( fopen_s(&fp, fname, "wb" ) != 0 )
        error_process("ファイルがオープンできませんでした．");
    /* データの書き込み */
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
/* nバイトの文字列をファイルから読み込む */
{
    int i;

    for(i=0;i<n;i++){
        c[i] = fgetc( fp );
    }
    c[n]='\0';  /* 文字列の最後を表すNULL記号 */
}


void read_int( FILE *fp, int n, int *number )
/* nバイトのintをファイルから読み込む */
/* Intelバイトオーダ（byte逆順）      */
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
/* nバイトの文字列をファイルへ書き込む */
{
    int i;

    for(i=0;i<n;i++){
        fputc( c[i], fp );
    }
}


void write_int( FILE *fp, int n, int number )
/* nバイトでnumberのデータをファイルへ書き込む（逆順） */
{
    int i;

    for(i=0;i<n;i++){
        fputc( number % 256, fp );
        number = number / 256;
    }
}


void error_process( const char message[] )
/* エラー処理 */
{
    if ( message[0]=='\0' )
        printf("予想しないエラーが発生しました．\n");
    else
        printf("%s\n",message);
    printf("プログラムを終了します．\n");
    exit(1);  /* 異常終了 */
}


void error_process2( const char c1[], const char c2[] )
/* エラー処理２ */
/* c1 と c2 が異なっていたら強制終了する */
{
    if ( strcmp( c1, c2 ) != 0 ){
        printf("IDが異なるファイルです．\n");
        printf("プログラムを異常終了します．\n");
        exit(1);  /* 異常終了 */
    }
}

