#ifndef _PARAMAS_H
#define _PARAMAS_H

#pragma once

#include "ini_file.h"

//====================================================================================================

class Params
{
	public:

	void Init();				 // �������/������� ini-����
	void Load();				 // ��������� ����������� ���������


	//------------------- ��������������� ��������� --------------------------

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