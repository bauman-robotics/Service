#pragma hdrstop
#pragma argsused

#include "stdafx.h"
#include "COMPort.h"
#include <stdio.h>
#include "BP_v1Dlg.h"
//#include "Tpv_calibr.h"
//#include "Tpv_calibrDlg.h"



#ifdef USE_CHAR
#define _itot 		itoa
#define stprintf	sprintf
#else
#define _itot 		_itow
#define stprintf	swprintf
#endif

			unsigned long BytesRW;
			bool bw = 0;
			int x = 0;
//	DWORD btr;

extern	CBP_v1Dlg * pDlg;
//extern CTpv_calibrDlg * pDlg;
//extern HANDLE hMutex;

 //Ccom_pDlg * pDlg;

//CRITICAL_SECTION critSec1; 	// критическая секция для доступа к данным	


//COMMTIMEOUTS timeouts1; 	// структура таймаутов

int COMPortLib::COMPort::DevKolvo; // определение count

//DCB testPlate1; 	
//SetCommTimeouts(port.handle, &timeouts1);

namespace COMPortLib
{
	COMPort::CircleBuffer::CircleBuffer()
	{
		size = 8192;
		buf = new Uint8[size];
		start = 0;
		end = 0;
		full = 0;


		InitializeCriticalSection(&critSec);
	//	InitializeCriticalSection(&critSec1);
	}
	// ---------------------------------------------------------------------------
	COMPort::CircleBuffer::~CircleBuffer()
	{
		EnterCriticalSection(&critSec);
		delete []buf;
		LeaveCriticalSection(&critSec);
		DeleteCriticalSection(&critSec);
	}
	// ---------------------------------------------------------------------------
	void COMPort::CircleBuffer::SetSize(Uint16 value)
	{
		if(value == 0) return;
		EnterCriticalSection(&critSec);
		delete []buf;

		size = value;
		buf = new Uint8[size];
		start = 0;
		end = 0;
		full = 0;

		LeaveCriticalSection(&critSec);
	}
	// ---------------------------------------------------------------------------
	//errno_t CircleBuffer::Add(Uint8 *source, Uint16 length)
	errno_t COMPort::CircleBuffer::Add(unsigned char *source, Uint16 length)
	{
		errno_t returnValue = 0;
		if(length > size)
		{
			returnValue = 1;
			source += length - size;
			length = size;
		}

		EnterCriticalSection(&critSec);
		if(!full)
		{
			if(length < size - end)	// длина сообщения меньше, чем осталось места до конца буфера
			{
				memcpy(buf + end, source, length);
				end += length;
			}
			else	// длина сообщения больше, чем осталось места до конца буфера
			{
				memcpy(buf + end, source, size - end);
				memcpy(buf, source + (length - (size - end)), length - (size - end));
				end = length - (size - end);
				if(start < end) start = end;
				full = 1;
			}
		}
		else
		{
			if(length < size - end)	// длина сообщения меньше, чем осталось места до конца буфера
			{
				memcpy(buf + end, source, length);
				end += length;
				if(start < end) start = end;
			}
			else	// длина сообщения больше, чем осталось места до конца буфера
			{
				memcpy(buf + end, source, size - end);
				memcpy(buf, source + (length - (size - end)), length - (size - end));
				end = length - (size - end);
				start = end;
			}
        }
		LeaveCriticalSection(&critSec);

		return returnValue;
	}
	// ---------------------------------------------------------------------------
	//int CircleBuffer::GetBuffer(Uint8 *dest, Uint16 destMaxLength)
			int COMPort::CircleBuffer::GetBuffer(unsigned char *dest, Uint16 destMaxLength)
	{
		int length;

		EnterCriticalSection(&critSec);
		if(!full) length = end - start;
		else length = size - start + end;
		if(destMaxLength < length)	// данных в буфере больше, чем размер входящего буфера
		{
			if(!full || (full && size - start >= destMaxLength))
			{
				memcpy(dest, buf + start, destMaxLength);
				start += destMaxLength;
			}
			else
			{
				length = size - start;
				memcpy(dest, buf + start, length);
				memcpy(dest + length, buf, destMaxLength - length);
				start = destMaxLength - length;
				full = 0;
			}
			length = destMaxLength;
		}
		else
		{
			if(!full)
			{
				memcpy(dest, buf + start, end - start);
			}
			else
			{
				memcpy(dest, buf + start, size - start);
				memcpy(dest + (size - start), buf, end);
			}
			start = 0;
			end = 0;
			full = 0;
		}
		LeaveCriticalSection(&critSec);

		return length;
	}
	// ---------------------------------------------------------------------------
	int COMPort::CircleBuffer::GetSize()
	{
		return size;
	}
	// ---------------------------------------------------------------------------
	COMPort::COMPort()
	{
	//	CircleBuffer bufRead, bufWrite;
		port.handle = 0;
		readThread.handle = 0;
		writeThread.handle = 0;
		stop = 0;
		

		overlappedRead.hEvent = 0;
		overlappedRead.Internal =0;
		overlappedRead.InternalHigh = 0;
		overlappedRead.Offset = 0;
		overlappedRead.OffsetHigh = 0;
		overlappedRead.Pointer = 0;

		overlappedWrite.hEvent = 0;
		overlappedWrite.Internal =0;
		overlappedWrite.InternalHigh = 0;
		overlappedWrite.Offset = 0;
		overlappedWrite.OffsetHigh = 0;
		overlappedWrite.Pointer = 0;

		DevKolvo = 0;

		fClbLogIsCreate = 0;

		fQfree = 1;
		CurrCmdStr = "";
		fEndOfWaitTime = 1;   // время ожидания закончено, 
		fQfree = 1;
		CmdCode = 0;  // код команды
		OldCmdCode = 0;
		 ClbTryCount = 0;
		 ErrClbCount = 0;
		 fTmp100Auto = 1; // флаг авто тмп для каждого модуля

		 packetMode = 0; // пакетный режим передачи выключен по умолчанию

		 fClbRangeDataReady = 0; // флаг готовности данных после команды clb_range_def_mute
		 fClbRangeOutOfTime = 0; // флаг выхода времени ожидания за пределы после команды clb_range_def_mute

		 fClbRepeat = 0;

		 bpCount = 0; // Счетчик символов в пакете с Bad Crc (пакетная передача)
		 //packLenght = 0; // Длина пакета вычитанная из пакета Pack[3]
		 packLenghtTotal = 0;  // Длина пакета вычитанная из пакета Pack[3]
		 packLenghtExpected = 0; // Длина пакета ожидаемая
		 packLenghtReceived = 0; // Длина пакета полученная 

		 packetPartCount = 0; //  cчетчик частей пакета, (первая часть, вторая часть, третья часть.... разделенных 0x0d)
		 packetMem1part = 0;  // счетчик принятых пакетов из одной части
		 packetMem2part = 0;  // счетчик принятых пакетов из двух частей
		 packetMem3part = 0;  // счетчик принятых пакетов из трёх частей
		 packetMem4part = 0;  // счетчик принятых пакетов из четырёх частей
		 packetBadCrcCount = 0;  // // счетчик принятых пакетов c неправильной контрольной суммой, при наличии старта пакета, конца, и совпадающей длины
		 iTmpNum = 1; // счетчик записей в температурный файл

	}
	// ---------------------------------------------------------------------------
	COMPort::~COMPort()
	{
		CloseComPort();
	}
	// ---------------------------------------------------------------------------
	errno_t COMPort::SetPort(Uint8 value)
	{
		if(port.handle) return 2;
		port.number = value;
		return 0;
    }
	// ---------------------------------------------------------------------------
	errno_t COMPort::SetRate(DWORD value)
	{
        if(port.handle) return 2;
		port.rate = value;
		return 0;
    }
	// ---------------------------------------------------------------------------
	errno_t COMPort::SetParity(BYTE value)
	{
        if(port.handle) return 2;
		port.parity = value;
		return 0;
    }

	// ---------------------------------------------------------------------------
	errno_t COMPort::SetIndex(BYTE value)
	{
        if(port.handle) return 2;
		port.TpvIndex = value;
		return 0;
    }

	
	// ---------------------------------------------------------------------------

	//errno_t COMPort::Init(Ccom_pDlg  * MyCallBackClass, Uint8 port, DWORD rate, BYTE parity)
errno_t COMPort::Init(Uint8 port, DWORD rate, BYTE parity, BYTE index)
	{
	int Ret1, Ret2, Ret3, Ret4, Ret5;

	Ret1 = SetPort(port);
	Ret2 = SetRate(rate);
	Ret3 = SetParity(parity);
	Ret4 = SetIndex(index);
	Ret5 = OpenComPort();

	

		if ( (!Ret1) && (!Ret2) && (!Ret3) && (!Ret4) && (!Ret5) ) 	
		{
			COMPort::DevKolvo ++; 	

			return 0; // OK
		}
		return 2;
		}



	errno_t COMPort::OpenComPort()
	{
		DCB testPlate; 			// структура для общей инициализации порта
		COMMTIMEOUTS timeouts; 	// структура таймаутов

		TCHAR textPort[11];
		sprintf(textPort, TEXT("\\\\.\\COM%d"), port.number);
	//textPort
	port.handle = CreateFile(textPort, 
						GENERIC_READ | GENERIC_WRITE, 
						0, 
						0,
						OPEN_EXISTING, 
						FILE_FLAG_OVERLAPPED, 0); 	// открытие порта



	//port.handle = CreateFile(textPort, GENERIC_READ | GENERIC_WRITE, 0, 0,
	//						OPEN_EXISTING, 0, NULL ); 	// открытие порта


		if (port.handle == INVALID_HANDLE_VALUE)
			return 10;

		testPlate.DCBlength = sizeof(DCB);
		if (!GetCommState(port.handle, &testPlate))	// считывание структуры dcb из порта
		{
			CloseHandle(port.handle); // закрытие порта
			port.handle = 0;
			return 11;
		}
		testPlate.BaudRate = port.rate; 		// скорость в бодах
		testPlate.fBinary = 1; 			// двоичный режим
		testPlate.fOutxCtsFlow = 0; 	// слежение за сигналом CTS
		testPlate.fOutxDsrFlow = 0; 	// слежение за сигналом DSR
		testPlate.fDtrControl = 0; 		// использование линии DTR
		testPlate.fDsrSensitivity = 0; 	// восприимчивость к состоянию DSR
		testPlate.fNull = 0; 			// разрешить прием 0 байтов
		testPlate.fRtsControl = RTS_CONTROL_DISABLE;
		testPlate.fAbortOnError = 0; 	// отключаем остановку при ошибке
		testPlate.ByteSize = 8; 		// размер байта
		testPlate.Parity = port.parity; 			// проверка четности
		testPlate.StopBits = 0; 		   // 1 стоп бит

	testPlate.EvtChar = 13; // 0x0d   


		if (!SetCommState(port.handle, &testPlate))	// загрузка структуры dcb в порт
		{
			CloseHandle(port.handle); // закрытие порта
			port.handle = 0;
			return 12;
		}
/*
	timeouts.ReadIntervalTimeout         = 50; // время между окончанием приема предыдущего байта и началом следующего
	timeouts.ReadTotalTimeoutConstant    = 10; // + период * размер буфера
	timeouts.ReadTotalTimeoutMultiplier  = 30; // период * размер буфера // 20 мало для clb
	timeouts.WriteTotalTimeoutConstant   = 10;
	timeouts.WriteTotalTimeoutMultiplier = 10; 
*/

		if (!SetCommTimeouts(port.handle, &timeouts))
			// загрузка структуры таймаутов в порт
		{
			CloseHandle(port.handle); // закрытие порта
			port.handle = 0;
			return 13;
		}
		SetupComm(port.handle, 8192, 8192); // размеры очереди приема и записи
		PurgeComm(port.handle, PURGE_TXCLEAR | PURGE_RXCLEAR);
		// очистка принимающего буфера порта

		hEventRead = CreateEvent(0, 0, 0, 0);
		hEventWrite = CreateEvent(0, 0, 0, 0);

		stop = 0;

		readThread.handle = CreateThread(0, 0, ReadThread, this, 0, &readThread.id);
		writeThread.handle = CreateThread(0, 0, WriteThread, this, 0, &writeThread.id);

		readThread.stopEvent = CreateEvent(0, 0, 0, 0);
		writeThread.stopEvent = CreateEvent(0, 0, 0, 0);

		return 0;
	}
	// ---------------------------------------------------------------------------
	bool COMPort::CloseComPort()
	{
		if (!port.handle)
			return 0;

		//if (port.handle)  //was
		//	return 0;

		stop = 1;

		//если в этот момент поток находится в WaitCommEvent
		// - принудительно заставляем завершиться 
		  SetCommMask(port.handle,0);
		//передаем ресурсы системе, чтобы поток получил ресурсы
		  Sleep(10);
		//закрываем хэндл порта
		//  CloseHandle(cId);


		CloseHandle(port.handle);

		SetEvent(hEventWrite);
		WaitForSingleObject(writeThread.stopEvent, INFINITE);
		CloseHandle(writeThread.handle);
		CloseHandle(writeThread.stopEvent);

		WaitForSingleObject(readThread.stopEvent, INFINITE);
		CloseHandle(readThread.handle);
		CloseHandle(readThread.stopEvent);

		CloseHandle(hEventRead);
		CloseHandle(hEventWrite);


		port.handle = 0;
		readThread.handle = 0;
		writeThread.handle = 0;
		stop = 0;

		return 1;
	}
	// ---------------------------------------------------------------------------
/*	int COMPort::LoadCOMPortList(TCHAR **list)
	{
		HKEY hKey;
		int i = 0;
		TCHAR Name[256];
		DWORD nameSize;
		DWORD valueSize;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
			0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			while (RegEnumValue(hKey, i, Name, &(nameSize = 256), NULL, NULL,
				(LPBYTE)list[i], &(valueSize = 32)) == ERROR_SUCCESS)
			{
				i++;
			}
		}
		return i;
	}
*/
	int COMPort::LoadCOMPortList(TCHAR **list)
	{
		HKEY hKey;
		int ii = 0;
		TCHAR Name[256];
		char * comList[10];
		//char comList2[10] = new char[40];
		char str1[20];
		char mstr1[10];
		DWORD nameSize;
		DWORD valueSize;
   char * pValue1=new char[40];

   char ** ptr = new char*[10];
	for (int i = 0; i < 10; ++i) 
    ptr[i] = new char[20];


		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
			0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			while (RegEnumValue(hKey, ii, Name, &(nameSize = 256), NULL, NULL,
				//(LPBYTE)pValue1, &(valueSize = 32)) == ERROR_SUCCESS)
				//(LPBYTE)ptr[ii], &(valueSize = 32)) == ERROR_SUCCESS)
				(LPBYTE)list[ii], &(valueSize = 32)) == ERROR_SUCCESS)
			{
					comList[ii] = pValue1;
				ii++;
			
					//	pDlg->GetDlgItem( IDC_STATIC_COM ) ->SetWindowText((LPCTSTR)pValue1);
			}
		}
	//	pDlg->GetDlgItem( IDC_STATIC_COM ) ->SetWindowText((LPCTSTR)pValue1);
	//	pDlg->GetDlgItem( IDC_STATIC_COM_2 ) ->SetWindowText((LPCTSTR)list[1]);
		
		return ii;

		//SendDlgItemMessage(HWND)pDlg,IDC_LIST_TERMINAL,CB_ADDSTRING,NULL,(LPARAM)pValue1);
	}
	// ---------------------------------------------------------------------------
	DWORD WINAPI COMPort::ReadThread(LPVOID lpParameter)
	{
		COMPort *hCOMPort = (COMPort *)lpParameter;

		hCOMPort->overlappedRead.hEvent = CreateEvent(0, 1, 0, 0);  // ручной сброс

	//	SetCommMask(hCOMPort->port.handle, EV_RXCHAR); // маска = если принят байт
		SetCommMask(hCOMPort->port.handle, EV_RXFLAG);
		
		
		while (!hCOMPort->stop)
		{

			WaitCommEvent(hCOMPort->port.handle, &(hCOMPort->mask), &(hCOMPort->overlappedRead) );  // передаем адрес структуры


			hCOMPort->signal = WaitForSingleObject(hCOMPort->overlappedRead.hEvent, INFINITE); // 

	//	WaitForSingleObject( hMutex, INFINITE );

			if (hCOMPort->signal == WAIT_OBJECT_0) // если пришел байт  // 
			{
							if (GetOverlappedResult(hCOMPort->port.handle, &hCOMPort->overlappedRead, &hCOMPort->temp, 1))
								// если успешно завершилось WaitCommEvent
							{
								ResetEvent(hCOMPort->overlappedRead.hEvent);

									//if (hCOMPort->mask & EV_RXCHAR) // если пришел именно байт  ..// Any Character received
									if (hCOMPort->mask & EV_RXFLAG)
									{
									//	Sleep(100); // Без этого не верное число байт !!!  // 40 порог для ok!
																		
										ClearCommError(hCOMPort->port.handle, &hCOMPort->temp, &hCOMPort->curstatRead);
										// заполнение curstat
										
										hCOMPort->btr = hCOMPort->curstatRead.cbInQue;
										// получить кол-во принимаемых байт

										hCOMPort->err = GetLastError();

													if (hCOMPort->btr)
													{  
													ReadFile(hCOMPort->port.handle, hCOMPort->bufR, hCOMPort->btr, &hCOMPort->temp, &hCOMPort->overlappedRead);

													hCOMPort->bufR[hCOMPort->btr] =0; //терминируем нулём bufR 
													hCOMPort->bufRead.Add(hCOMPort->bufR, hCOMPort->btr);


													pDlg->fCallback(hCOMPort);	

													//	SetEvent(hCOMPort->hEventRead);
													
													
													}
			
									}
		
							}

				}

	
//	ReleaseMutex(hMutex);

		}
		CloseHandle(hCOMPort->overlappedRead.hEvent);
		SetEvent(hCOMPort->readThread.stopEvent);
		return 0;
	}
	// ---------------------------------------------------------------------------
	int COMPort::GetReadText(unsigned char *buf, int bufSize, DWORD waitTime)
	{
		int length, i, j;

		if(WAIT_TIMEOUT == WaitForSingleObject(hEventRead, waitTime))
			return 0;
		length = bufRead.GetBuffer(buf, bufSize);
		if(length == bufSize) SetEvent(hEventRead);
		return length;
	}
	// ---------------------------------------------------------------------------
	DWORD WINAPI COMPort::WriteThread(LPVOID lpParameter)
	{
		COMPort *hCOMPort = (COMPort *)lpParameter;
		OVERLAPPED overlapped;

		overlapped.hEvent = 0;
		overlapped.Internal =0;
		overlapped.InternalHigh = 0;
		overlapped.Offset = 0;
		overlapped.OffsetHigh = 0;
		overlapped.Pointer = 0;

		DWORD temp, signal;
		//Uint8 buf[8192];
		unsigned char	buf[8192];

		Uint16 bufLength;
		int i;

		overlapped.hEvent = CreateEvent(0, 1, 1, 0);
		while (!hCOMPort->stop)
		{
			WaitForSingleObject(hCOMPort->hEventWrite, INFINITE);
			do
			{
				bufLength = hCOMPort->bufWrite.GetBuffer(buf, 8192);
				WriteFile(hCOMPort->port.handle, buf, bufLength, &temp, &overlapped);
				signal = WaitForSingleObject(overlapped.hEvent, INFINITE);

				for(i = 0; i < 4; i++)
				{
					if (signal == WAIT_OBJECT_0 &&											 // процесс завершился
						GetOverlappedResult(hCOMPort->port.handle, &overlapped, &temp, 1))  
					{
						break;
					}
					else
					{
                        WriteFile(hCOMPort->port.handle, buf, bufLength, &temp, &overlapped);
						signal = WaitForSingleObject(overlapped.hEvent, INFINITE);
					}
				}

			} while(bufLength == 8192);
		}
		SetEvent(hCOMPort->writeThread.stopEvent);
		return 0;
	}



	// ---------------------------------------------------------------------------
	bool COMPort::SendText(unsigned char *buf, int length)
	{
		if (port.handle)
		{
			bufWrite.Add(buf, length);
			SetEvent(hEventWrite);
		}
		else
			return 0;
		return 1;
	}
	// ---------------------------------------------------------------------------
	int	COMPort::GetBufferSize()
	{
		return bufWrite.GetSize();
	}



// ---------------------------------------------------------------------------


};


