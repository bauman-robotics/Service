#ifndef _INI_FILE_H_
#define _INI_FILE_H_


// ����� INIFile ��������� ������, ���������� � ini-������ Windows
// ������� ������/������ ���������� 0 � ������ ������.
//

#pragma once

#include <string>

//
// key - ��� ���������� � ini-�����.
// ������� ���� � ��������� ���������� ini-����� �������� �� �����.
//

//====================================================================================================

class INIFile
{
public:
    INIFile (){}                                     // ������������
	INIFile (const char* file_name);


    void Assign (const char* file_name);             // ������� ������ INIFile 
                                                     // � ������ �������� ini-������ 

	bool IsExist();									 // ���������� �� ����� ����?

    bool GetBool   (const char* key, bool& i, const char* sect = "common" );       // ������
    bool GetInt    (const char* key, int& i, const char* sect = "common" );         
	bool GetHex    (const char* key, int& i, const char* sect = "common" );         
    bool GetDouble (const char* key, double& d, const char* sect = "common" );     

    bool SetBool   (const char* key, bool b, const char* sect = "common" );        // ������
    bool SetInt    (const char* key, int i, const char* sect = "common" );   
	bool SetHex    (const char* key, int i, const char* sect = "common" );   
    bool SetDouble (const char* key, double d, const char* sect = "common" );     

    bool GetString (const char* key, char* str, int sizeof_str, const char* sect = "common" );
    bool SetString (const char* key, const char* str, const char* sect = "common" );

protected:

    std::string fname;
};

//====================================================================================================


#endif;