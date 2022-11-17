/*
 * Date : 2020.09.03
 * Author : T.Hasegawa
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ioData_base.hpp"

static FILE* isValidFile( const char* filename, const char* mode, const char* ext );
int ioData_base::initialize( const char *filename, const char* mode )
{
	sprintf( flag_mode, "%s", mode );
	
	int len = strlen( flag_mode );
	if( strncmp( flag_mode, "r", len ) == 0 ){
		// 設定ファイルをオープン
		char name[ 128 ];
		sprintf( name, "%s", filename );
		printf( "\nParameter File = %s\n\n", name );	// 確認用
		fp_base = isValidFile( filename, "r", "dat" );
//		readProperty( fp_config, path );
	} else if( strncmp( flag_mode, "w", len ) == 0 ){
		// 保存ファイルの作成
		char name[ 128 ];
		sprintf( name, "%s", filename );
		printf( "\nSave File = %s\n\n", name );	// 確認用
		fp_base = isValidFile( filename, "w", "dat" );
	} else {
		fprintf( stderr, "Error. Mismatch of File Mode\n" );
		exit( EXIT_FAILURE );
	}
	return EXIT_SUCCESS;
}
//*******************************
// ファイル名がcsvファイルになっているか確認
// 引数 char* filename : ファイル名
// 戻値 int : EXIT_FAILUREの場合、csvファイルを指定していない
//            EXIT_SUCCESSの場合、csvファイル
//*******************************
static int isValidFileName( const char* filename, const char* ext )
{
	char str[ 10 ], tmp[ 50 ];
	int len = strlen( filename );
//std::cout << len << std::endl;
	for( int i = 0 ; i < len ; i++ ){
		tmp[ i ] = filename[ i ];
	}
	len = strcspn( tmp, "." );
	if( len < 3 ){
		tmp[ 0 ] = 'a';
		tmp[ 1 ] = 'a';
		len = strcspn( tmp, "." );
	}
	if( len < 6 ){
		tmp[ 0 ] = 'a';
		tmp[ 1 ] = 'a';
		tmp[ 2 ] = 'a';
		tmp[ 3 ] = 'a';
		tmp[ 4 ] = 'a';
		len = strcspn( tmp, "." );
	}
//	std::cout << len << std::endl;
	for( int i = 0 ; i < 5 ; i++ ){
		str[ i ] = filename[ len+i+1 ];
	}
	if( strncmp( ext, str, 5 ) ){
		fprintf( stderr, "Cannot make .%s file\n", ext );
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
//*******************************
// ファイル名をオープンする関数
// 引数 char* filename : ファイル名
// 戻値 FILEポインタ
//*******************************
static FILE* isValidFile( const char* filename, const char* mode, const char* ext )
{
  FILE *fp1;

  if( isValidFileName( filename, ext ) == EXIT_FAILURE ) exit( EXIT_FAILURE );
  if( ( fp1 = fopen( filename, mode ) ) == NULL ){
    fprintf( stderr, "Cannot open %s\n", filename );
    exit( EXIT_FAILURE );
  }

  return fp1;
}
