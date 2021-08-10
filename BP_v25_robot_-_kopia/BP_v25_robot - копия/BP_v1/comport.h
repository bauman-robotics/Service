#ifndef _COMPORT_RW_H
#define _COMPORT_RW_H

#include "stdafx.h"
//#include "variables.h"

#include <queue>
using namespace std;


	#ifdef _WIN32
	#include <tchar.h>
	#endif


	#ifdef __DLL__
	# define DLL_EI __declspec(dllexport)
	#else
	# define DLL_EI __declspec(dllimport)
	#endif

#include <windows.h>


	#ifndef Uint
	# define Uint 		1
	# define Uint8 		unsigned char
	# define Uint16 	unsigned short int
	#endif


typedef void(*fCallBackType)(void);

class  Ccom_pDlg;

namespace COMPortLib
{
	class COMPort
	{
			
			class CircleBuffer
			{
				Uint8 *buf;		// сам буфер
				bool full;      // признак переполнености буфера
				Uint16 size;	// размер буфера
				Uint16 start, end;     		// номер первого и последнего байта
				CRITICAL_SECTION critSec; 	// критическая секция для доступа к данным

			public:
				DLL_EI CircleBuffer();
				DLL_EI ~CircleBuffer();
				void DLL_EI SetSize(Uint16 value);
				errno_t DLL_EI Add(unsigned char *source, Uint16 length);
				int DLL_EI GetBuffer(unsigned char *dest, Uint16 destMaxLength);

				int DLL_EI GetSize();
			};
		
				OVERLAPPED overlappedRead;

				OVERLAPPED overlappedWrite;

				COMSTAT curstatRead; // структура текущего состояния порта



		struct
		{
			HANDLE handle;
			HANDLE stopEvent;
			DWORD id;
		} readThread, writeThread;

		bool stop;		// признак требования остановить потоки

		HANDLE hEventRead;	// событие принятия нового пакета из компорта
		HANDLE hEventWrite;	// событие добавления нового сообщения для записи в компорт

	public:

		
		queue<int> TpvCmdQueue;     // создаем пустую локальную очередь типа int //new 19.11.2015
		bool fQfree;              //   локальная очередь команд пуста

		bool fEndOfWaitTime;  // флаг окончания времени ожидания ответа

		bool fCmdPause;  // флаг паузы между командами

		CString CurrCmdStr;  // текущая команда в очереди .. v_set

		CircleBuffer bufRead, bufWrite;

				DWORD btr, mask, signal, temp, err; // for ReadThread    нужно в private
				unsigned char bufR[8192]; // was 2015.11.16
					//			char bufR[16258];


	
	//	int TpvIndex;
		static int DevKolvo;
		CString DevId;

		int CmdCode; // код команды .. new 10.11.2015
		int OldCmdCode;
		int ClbTryCount;
		int ErrClbCount;
		bool fClbRepeat; // new

		CString ClbLogName;
		CString ClbLogName_tmp;
		int iTmpNum;
		bool fClbLogIsCreate;

		bool TpvType640;
		int TpvStage; // for future
		CString ClbCmd;
	//	CString ClbCmdDefMute;
		bool fT; // temp var for erase list
		bool fTmp100Auto;

		//  пакетный режим -------------------------
		bool packetMode; // // пакетный режим выключен или включен, флаг 
		int bpCount; // Счетчик символов в пакете с Bad Crc (пакетная передача)
		unsigned char badPack[51]; // буфер для пакетов с неправильным CRC
		int packLenghtTotal; 
		int packLenghtExpected;
		int packLenghtReceived;
		int packetPartCount; //  cчетчик частей пакета, (первая часть, вторая часть, третья часть.... разделенных 0x0d)
		int packetMem1part;  // счетчик принятых пакетов из одной части
		int packetMem2part;  // счетчик принятых пакетов из двух частей
		int packetMem3part;  // счетчик принятых пакетов из трёх частей
		int packetMem4part;  // счетчик принятых пакетов из четырёх частей
		int packetBadCrcCount;  // счетчик принятых пакетов c неправильной контрольной суммой, при наличии старта пакета, конца, и совпадающей длины
		//--------------------
		CString tmpGetCmd;

		bool fClbRangeDataReady; 
		bool fClbRangeOutOfTime; 
		//int 
		
	   // CmdCode = 0; => nothing  
       // CmdCode = 1; => id  
       // CmdCode = 2; => tmp100
       // CmdCode = ..; =>
       // CmdCode = 10; => clb
 		struct
		{
			Uint8 number;
			DWORD rate;
			BYTE parity;
			HANDLE handle;
			BYTE TpvIndex;
		
		} port;

		DLL_EI COMPort();
		DLL_EI ~COMPort();

		errno_t DLL_EI SetPort(Uint8 value);
		errno_t DLL_EI SetRate(DWORD value);
		errno_t DLL_EI SetParity(BYTE value);
		errno_t DLL_EI SetIndex(BYTE value);
		errno_t DLL_EI OpenComPort();

//errno_t DLL_EI Init(Ccom_pDlg  * MyCallBackClass, Uint8 port, DWORD rate, BYTE parity);
errno_t DLL_EI Init( Uint8 port, DWORD rate, BYTE parity, BYTE index);

		bool DLL_EI CloseComPort();
		int DLL_EI 	LoadCOMPortList(TCHAR **list);
		bool DLL_EI SendText(unsigned char *buf, int length);
		int DLL_EI 	GetReadText(unsigned char *buf, int bufSize, DWORD waitTime);
		int	DLL_EI	GetBufferSize();
	
	private:
		static DWORD WINAPI ReadThread(LPVOID lpParameter);
		static DWORD WINAPI WriteThread(LPVOID lpParameter);

	};
}
#endif

