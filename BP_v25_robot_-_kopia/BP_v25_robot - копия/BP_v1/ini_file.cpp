
#include "StdAfx.h"
#pragma unmanaged

#include "ini_file.h"
#include <Windows.h>

//====================================================================================================

static const char* default_string = "def_string";

//====================================================================================================

static bool equal_to_default (const char* str)
{
    if (strcmp(str, default_string) == 0) return true;
    else return false;
}

//====================================================================================================

INIFile::INIFile (const char* file_name) : fname(file_name)
{
}

//====================================================================================================

void INIFile::Assign (const char* file_name)
{
    fname = file_name;
}

//====================================================================================================

bool INIFile::IsExist()
{
	WIN32_FIND_DATA FileData;
	return !( FindFirstFile( fname.c_str(), &FileData ) == INVALID_HANDLE_VALUE ); 
}

//====================================================================================================

bool INIFile::GetBool (const char* key, bool& b, const char* sect )
{
    int i;
    if (!GetInt(key, i, sect )) return false;

    b = (i != 0);
    return true;
}

//====================================================================================================

bool INIFile::GetInt (const char* key, int& i, const char* sect )
{
	char data[20];
	if (!GetString (key, data, sizeof (data), sect )) return false;

	int ii;
	if (sscanf (data, "%d", &ii) != 1) return false;

	i = ii;
	return true;
}

//====================================================================================================

bool INIFile::GetHex (const char* key, int& i, const char* sect )
{
    char data[20];
    if (!GetString (key, data, sizeof (data), sect )) return false;

    int ii;
    if (sscanf (data, "%X", &ii) != 1) return false;

    i = ii;
    return true;
}

//====================================================================================================

bool INIFile::GetDouble (const char* key, double& d, const char* sect )
{
    char data[50];
    if (!GetString (key, data, sizeof (data), sect )) return false;

    double dd;
    if (sscanf (data, "%lg", &dd) != 1) return false;

    d = dd;
    return true;
}

//====================================================================================================

bool INIFile::GetString (const char* key, char* str, int sizeof_str, const char* sect )
{
    GetPrivateProfileString (sect, key, default_string, str, sizeof_str, fname.c_str());
    return !equal_to_default(str);
}

//====================================================================================================

bool INIFile::SetBool (const char* key, bool b, const char* sect )
{
    return SetInt(key, int(b), sect );
}

//====================================================================================================

bool INIFile::SetInt (const char* key, int i, const char* sect )
{
	char data[20];
	sprintf (data, " %d", i);
	return SetString (key, data, sect );
}

//====================================================================================================

bool INIFile::SetHex (const char* key, int i, const char* sect )
{
    char data[20];
    sprintf (data, " %X", i);
    return SetString (key, data, sect );
}

//====================================================================================================

bool INIFile::SetDouble (const char* key, double d, const char* sect )
{
    char data[50];
    sprintf (data, "%.40lg", d);
    return SetString (key, data, sect );
}

//====================================================================================================

bool INIFile::SetString (const char* key, const char* str, const char* sect )
{
    return WritePrivateProfileString (sect, key, str, fname.c_str()) != 0;
}

//====================================================================================================
//====================================================================================================


