#ifndef _INI_FILE_H_
#define _INI_FILE_H_


// Класс INIFile описывает данные, хранящиеся в ini-файлах Windows
// Функции чтения/записи возвращают 0 в случае ошибки.
//

#pragma once

#include <string>

//
// key - имя переменной в ini-файле.
// Регистр букв в названиях переменных ini-файла значения не имеет.
//

//====================================================================================================

class INIFile
{
public:
    INIFile (){}                                     // конструкторы
	INIFile (const char* file_name);


    void Assign (const char* file_name);             // связать обьект INIFile 
                                                     // с другим дисковым ini-файлом 

	bool IsExist();									 // существует ли такой файл?

    bool GetBool   (const char* key, bool& i, const char* sect = "common" );       // чтение
    bool GetInt    (const char* key, int& i, const char* sect = "common" );         
	bool GetHex    (const char* key, int& i, const char* sect = "common" );         
    bool GetDouble (const char* key, double& d, const char* sect = "common" );     

    bool SetBool   (const char* key, bool b, const char* sect = "common" );        // запись
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