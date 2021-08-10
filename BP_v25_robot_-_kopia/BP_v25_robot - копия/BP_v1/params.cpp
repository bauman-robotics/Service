
#include "StdAfx.h"
#pragma unmanaged

#include "params.h"

//====================================================================================================

void Params::Init()
{
	//----------------- Значения переменных параметров по умолчанию --------------

	strcpy(CmdFilePath,"C:\\black_body_log\\");
	//strcat(CmdFilePath, "\0" );

	TmsComPortNum = 1;
	TmsBaudRate = 115200; 
	TmsStopBits = 1.0; // 1.0 , 1.5, 2.0
	TmsParity   = 0;     // 0-4=None,Odd,Even,Mark,Space   
	TmsByteSize = 8;    // 4, 5, 6, 7, 8

	/****  TmsParity ******	
	#define NOPARITY            0
	#define ODDPARITY           1
	#define EVENPARITY          2
	#define MARKPARITY          3
	#define SPACEPARITY         4
	*/

	//	strcpy(TmsCmdWhoAreYou, "id get;");
	//	strcpy(TmsCmdWhoAreYouAns, "ID =");

	//----------------------------------------------------------------------------
/*  отвязали файл с параметрами
	ini_file.Assign( ".\\params.ini" );   // как-то получать путь, откуда запущен exe

	if ( !ini_file.IsExist() )
	{
		ini_file.SetInt      ( "ComPortNum"	                  , TmsComPortNum    ,   "Com_Ports"	 );
		ini_file.SetInt      ( "ComPortBaudRate"	          ,  TmsBaudRate     ,   "Com_Ports"	 );
	}

    Load();
*/
}

//====================================================================================================

void Params::Load()
{
	ini_file.GetInt       ( "ComPortNum"	           , TmsComPortNum    ,   "Com_Ports"	 );
	ini_file.GetInt       ( "ComPortBaudRate"	       ,  TmsBaudRate     ,   "Com_Ports"	 );
}

//====================================================================================================
//====================================================================================================








