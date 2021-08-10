#ifndef _PARAMAS_H
#define _PARAMAS_H

#pragma once

#include "ini_file.h"

//====================================================================================================

class Params
{
	public:

	void Init();				 // создать/открыть ini-файл
	void Load();				 // загрузить сохраненные параметры


	//------------------- Устанавливаемые параметры --------------------------

	char CmdFilePath[250];	


		//CBR_115200, ONESTOPBIT,  NOPARITY, BS_EIGHT
	int TmsComPortNum;
	int TmsBaudRate;
    double TmsStopBits;
    int TmsParity;
    int TmsByteSize;


	private:

	INIFile ini_file;
};

//====================================================================================================

extern Params params; 

//====================================================================================================

#endif;