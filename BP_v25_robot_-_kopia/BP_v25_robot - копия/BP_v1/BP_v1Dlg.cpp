// BP_v1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "BP_v1.h"
#include "BP_v1Dlg.h"
#include "comport.h"
#include "ini_file.h"
#include <ctime>
#include "params.h"

using namespace COMPortLib;  // теперь все имена из пространства имён COMPortLib будут видны здесь без дополнительных префиксов


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBP_v1Dlg dialog

//extern CBP_v1Dlg * pDlg;

COMPort tpv;
Params params;         // настраиваемые параметры системы
bool comConnect = 0;

int iReplacePoint_x = 0;
int iReplacePoint_y = 0;
int krest_m = 1;
Uint8 com_number = 0;
int com_is_check = 0;
int iNewPoint = 0;  // добавлены новые точки 
int iTabSel = 0;

//-----------------------
TCHAR comList[20]; // Переменная типа MyClass, которую мы создать, к сожалению, не можем
TCHAR* pX = &comList[20]; // Указатель на переменную типа MyClass
TCHAR** ppX = &pX; // Указатель на указатель на переменную типа MyClass
char list1[20];

//*ppX;  // Если разыменуем ppX, то получим то же самое, что и просто pX
//**ppX; // А если разыменуем ppX дважды, то откатимся на исходную переменную x

//	TCHAR comList[20];
int i = 10;

int com_speed[] = {4800, 9600, 14400, 19200, 28800, 38400, 56000, 57600, 115200, 128000, 256000}; // 11 шт 
int num_com_speed = 8;
int sel_com_speed = 115200;

bool bLastCmb = 0; //Последняя команда введена вручную, если = 1

//----- Time ------

time_t seconds;
tm* timeinfo;

//-----------------

///--------------------

CBP_v1Dlg::CBP_v1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBP_v1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBP_v1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_CHECK_STOP_KADR, m_button_stop_frame_1);
	DDX_Control(pDX, IDC_LIST_TERMINAL, m_list_tpv);
	DDX_Control(pDX, IDC_CHECK_VKL_KRESTA, m_button_no_chrest_2);
	DDX_Control(pDX, IDC_EDIT_FIND_IN_2_FRAME, m_edit_find_in_2_frames);
	DDX_Control(pDX, IDC_EDIT_FIND_IN_1_FRAME, m_edit_find_in_1_frames);
	DDX_Control(pDX, IDC_SPIN_FIND_IN_2_FRAME, m_spin_2p);
	DDX_Control(pDX, IDC_SPIN_FIND_IN_1_FRAME, m_spin_1p);
	DDX_Control(pDX, IDC_SLIDER1, m_slider_color);
	DDX_Control(pDX, IDC_EDIT_CMD, m_edit_cmd);
	DDX_Control(pDX, IDC_COMBO_COM, m_combo_com);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_combo_speed);
	DDX_Control(pDX, IDC_RADIO_CONNECT_INDICATOR, m_radio_indicator_of_connection);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_button_connect);
	DDX_Control(pDX, IDC_BUT_ENTER, m_button_enter);
}

BEGIN_MESSAGE_MAP(CBP_v1Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CBP_v1Dlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_CHECK_STOP_KADR, &CBP_v1Dlg::OnBnClickedCheckStopKadr)
	ON_BN_CLICKED(IDC_CHECK_VKL_KRESTA, &CBP_v1Dlg::OnBnClickedCheckVklKresta)
	ON_BN_CLICKED(IDC_RADIO_FILTER_RCOS, &CBP_v1Dlg::OnBnClickedRadioFilterRcos)
	ON_BN_CLICKED(IDC_RADIO_FILTER_BIKUBE, &CBP_v1Dlg::OnBnClickedRadioFilterBikube)
	ON_BN_CLICKED(IDC_RADIO_FILTER_BILINE, &CBP_v1Dlg::OnBnClickedRadioFilterBiline)
	ON_BN_CLICKED(IDC_RADIO_FILTER_NOT, &CBP_v1Dlg::OnBnClickedRadioFilterNot)
	ON_BN_CLICKED(IDC_BOT_FIND_HOT_FRMAE, &CBP_v1Dlg::OnBnClickedBotFindHotFrmae)
	ON_BN_CLICKED(IDC_BOT_FIND_COOL_FRMAE, &CBP_v1Dlg::OnBnClickedBotFindCoolFrmae)
	ON_BN_CLICKED(IDC_BUT_FIND_IN_2_FRAME, &CBP_v1Dlg::OnBnClickedButFindIn2Frame)
	ON_BN_CLICKED(IDC_BUT_FIND_IN_1_FRAME, &CBP_v1Dlg::OnBnClickedButFindIn1Frame)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FIND_IN_2_FRAME, &CBP_v1Dlg::OnDeltaposSpinFindIn2Frame)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FIND_IN_1_FRAME, &CBP_v1Dlg::OnDeltaposSpinFindIn1Frame)

	ON_BN_CLICKED(IDC_BOT_RESET_LIST_POINTS, &CBP_v1Dlg::OnBnClickedBotResetListPoints)
	ON_BN_CLICKED(IDC_BOT_LIST_OF_POINTS_REPORT, &CBP_v1Dlg::OnBnClickedBotListOfPointsReport)
	ON_BN_CLICKED(IDC_BOT_SAVE_TO_FLASH2, &CBP_v1Dlg::OnBnClickedBotSaveToFlash2)
	ON_BN_CLICKED(IDC_BOT_READ_FROM_FLASH, &CBP_v1Dlg::OnBnClickedBotReadFromFlash)
	ON_BN_CLICKED(IDC_RADIO_N2_N2, &CBP_v1Dlg::OnBnClickedRadioN2N2)
	ON_BN_CLICKED(IDC_RADIO_N1_N2, &CBP_v1Dlg::OnBnClickedRadioN1N2)
	ON_BN_CLICKED(IDC_RADIO_0_N2, &CBP_v1Dlg::OnBnClickedRadio0N2)
	ON_BN_CLICKED(IDC_RADIO_P1_N2, &CBP_v1Dlg::OnBnClickedRadioP1N2)
	ON_BN_CLICKED(IDC_RADIO_P2_N2, &CBP_v1Dlg::OnBnClickedRadioP2N2)
	ON_BN_CLICKED(IDC_RADIO_N2_N1, &CBP_v1Dlg::OnBnClickedRadioN2N1)
	ON_BN_CLICKED(IDC_RADIO_N1_N1, &CBP_v1Dlg::OnBnClickedRadioN1N1)
	ON_BN_CLICKED(IDC_RADIO_0_N1, &CBP_v1Dlg::OnBnClickedRadio0N1)
	ON_BN_CLICKED(IDC_RADIO_P1_N1, &CBP_v1Dlg::OnBnClickedRadioP1N1)
	ON_BN_CLICKED(IDC_RADIO_P2_N1, &CBP_v1Dlg::OnBnClickedRadioP2N1)
	ON_BN_CLICKED(IDC_RADIO_N2_0, &CBP_v1Dlg::OnBnClickedRadioN20)
	ON_BN_CLICKED(IDC_RADIO_N1_0, &CBP_v1Dlg::OnBnClickedRadioN10)
	ON_BN_CLICKED(IDC_RADIO_0_0, &CBP_v1Dlg::OnBnClickedRadio00)
	ON_BN_CLICKED(IDC_RADIO_P1_0, &CBP_v1Dlg::OnBnClickedRadioP10)
	ON_BN_CLICKED(IDC_RADIO_P2_0, &CBP_v1Dlg::OnBnClickedRadioP20)
	ON_BN_CLICKED(IDC_RADIO_N2_P1, &CBP_v1Dlg::OnBnClickedRadioN2P1)
	ON_BN_CLICKED(IDC_RADIO_N1_P1, &CBP_v1Dlg::OnBnClickedRadioN1P1)
	ON_BN_CLICKED(IDC_RADIO_0_P1, &CBP_v1Dlg::OnBnClickedRadio0P1)
	ON_BN_CLICKED(IDC_RADIO_P1_P1, &CBP_v1Dlg::OnBnClickedRadioP1P1)
	ON_BN_CLICKED(IDC_RADIO_P2_P1, &CBP_v1Dlg::OnBnClickedRadioP2P1)
	ON_BN_CLICKED(IDC_RADIO_N2_P2, &CBP_v1Dlg::OnBnClickedRadioN2P2)
	ON_BN_CLICKED(IDC_RADIO_N1_P2, &CBP_v1Dlg::OnBnClickedRadioN1P2)
	ON_BN_CLICKED(IDC_RADIO_0_P2, &CBP_v1Dlg::OnBnClickedRadio0P2)
	ON_BN_CLICKED(IDC_RADIO_P1_P2, &CBP_v1Dlg::OnBnClickedRadioP1P2)
	ON_BN_CLICKED(IDC_RADIO_P2_P2, &CBP_v1Dlg::OnBnClickedRadioP2P2)
	ON_BN_CLICKED(IDC_BOT_ADD_POINT_UNDER_KREST, &CBP_v1Dlg::OnBnClickedBotAddPointUnderKrest)
	ON_BN_CLICKED(IDC_BOT_DEL_POINT_UNDER_KREST, &CBP_v1Dlg::OnBnClickedBotDelPointUnderKrest)
	ON_BN_CLICKED(IDC_RADIO_X1, &CBP_v1Dlg::OnBnClickedRadioX1)
	ON_BN_CLICKED(IDC_RADIO_X3, &CBP_v1Dlg::OnBnClickedRadioX3)
	ON_BN_CLICKED(IDC_RADIO_X10, &CBP_v1Dlg::OnBnClickedRadioX10)
	ON_BN_CLICKED(IDC_RADIO_X100, &CBP_v1Dlg::OnBnClickedRadioX100)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_NW, &CBP_v1Dlg::OnBnClickedButDriveKrestNw)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_N, &CBP_v1Dlg::OnBnClickedButDriveKrestN)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_NE, &CBP_v1Dlg::OnBnClickedButDriveKrestNe)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_W, &CBP_v1Dlg::OnBnClickedButDriveKrestW)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_CENTR, &CBP_v1Dlg::OnBnClickedButDriveKrestCentr)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_E, &CBP_v1Dlg::OnBnClickedButDriveKrestE)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_SW, &CBP_v1Dlg::OnBnClickedButDriveKrestSw)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_S, &CBP_v1Dlg::OnBnClickedButDriveKrestS)
	ON_BN_CLICKED(IDC_BUT_DRIVE_KREST_SE, &CBP_v1Dlg::OnBnClickedButDriveKrestSe)
	ON_BN_CLICKED(IDC_BUT_AREA_KREST, &CBP_v1Dlg::OnBnClickedButAreaKrest)
	ON_BN_CLICKED(IDC_BUT_AREA_CENTER, &CBP_v1Dlg::OnBnClickedButAreaCenter)
	ON_BN_CLICKED(IDC_BUT_ENTER, &CBP_v1Dlg::OnBnClickedButEnter)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CBP_v1Dlg::OnNMCustomdrawSlider1)
	ON_CBN_SELCHANGE(IDC_COMBO_COM, &CBP_v1Dlg::OnCbnSelchangeComboCom)
	ON_BN_CLICKED(IDC_RADIO_M11, &CBP_v1Dlg::OnBnClickedRadioM11)
	ON_BN_CLICKED(IDC_RADIO_M12, &CBP_v1Dlg::OnBnClickedRadioM12)
	ON_BN_CLICKED(IDC_RADIO_M13, &CBP_v1Dlg::OnBnClickedRadioM13)
	ON_BN_CLICKED(IDC_RADIO_M14, &CBP_v1Dlg::OnBnClickedRadioM14)
	ON_BN_CLICKED(IDC_RADIO_M15, &CBP_v1Dlg::OnBnClickedRadioM15)
	ON_BN_CLICKED(IDC_RADIO_M16, &CBP_v1Dlg::OnBnClickedRadioM16)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, &CBP_v1Dlg::OnCbnSelchangeComboSpeed)
	ON_EN_CHANGE(IDC_EDIT_FIND_IN_2_FRAME, &CBP_v1Dlg::OnEnChangeEditFindIn2Frame)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CBP_v1Dlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_RADIO_CONNECT_INDICATOR, &CBP_v1Dlg::OnBnClickedRadioConnectIndicator)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CBP_v1Dlg::OnNMReleasedcaptureSlider1)
	ON_EN_CHANGE(IDC_EDIT_CMD, &CBP_v1Dlg::OnEnChangeEditCmd)
	ON_EN_CHANGE(IDC_EDIT_FIND_IN_1_FRAME, &CBP_v1Dlg::OnEnChangeEditFindIn1Frame)
END_MESSAGE_MAP()


// CBP_v1Dlg message handlers


BOOL CBP_v1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	 
	make_bold(IDC_TAB1);
	make_bold(IDC_BUTTON_CONNECT);
	make_bold(IDC_RADIO_CONNECT_INDICATOR);
	make_bold(IDC_STATIC_CONNECT_GROUP);
	make_bold(IDC_STATIC_CONNECTION);
	make_bold(IDC_CHECK_STOP_KADR);
	make_bold(IDC_CHECK_VKL_KRESTA);

	//GetDlgItem(IDC_RADIO_CONNECT_INDICATOR)->EnableWindow(0);


	m_TabCtrl.InsertItem(0, _T("Соединение"));
	m_TabCtrl.InsertItem(1, _T("Сбойные точки"));
	m_TabCtrl.InsertItem(2, _T("Настройки"));

	for (int i = 1010; i < 1091; i++)
	{
		GetDlgItem(i)->ShowWindow(SW_HIDE);

		if ((i != 1018) && (i != 1087) && (i != 1090) && (i != 1079) && (i != 1080))  // 79 
		//if ((i != 1018) && (i != 1087) && (i != 1090) )  // 79 , 80  жирный вывод
		{
		make_bold(i);
		}
	}


	make_bold(IDC_STATIC_COM_PORTS); // 1092
	make_bold(IDC_STATIC_BAUD_RATE); // 1093

	CheckRadioButton(IDC_RADIO_M11, IDC_RADIO_M16, IDC_RADIO_M11);
	CheckRadioButton(IDC_RADIO_X1, IDC_RADIO_X100, IDC_RADIO_X1);
	CheckRadioButton(IDC_RADIO_N2_N2, IDC_RADIO_P2_P2, IDC_RADIO_0_0);
	CheckRadioButton(IDC_RADIO_FILTER_RCOS, IDC_RADIO_FILTER_NOT, IDC_RADIO_FILTER_NOT);
	

//-----------------------------
	int i1 = 0;
	int i2 = 0;

	float f1 = 0.0;
	float f2 = 0.0;


	CString stStartFindIn2p = "88.4";
	CString stStartFindIn1p = "50.2";

	m_edit_find_in_2_frames.SetWindowTextA(stStartFindIn2p);
	m_edit_find_in_1_frames.SetWindowTextA(stStartFindIn1p);

	m_spin_2p.SetRange(0, 999);  // 0-99.9
	m_spin_1p.SetRange(0, 999);
	

	f2 = 10 * atof(stStartFindIn2p);
	i2 = (int)f2;
	m_spin_2p.SetPos(i2);

	f1 = 10 * atof(stStartFindIn1p);
	i1 = (int)f1;
	m_spin_1p.SetPos(i1);


	m_slider_color.SetRange(0, 255,0);
	m_slider_color.SetPos(255);  // черный цвет креста

	//m_button_no_chrest_2.SetCheck(1); // krest on

	params.Init(); // ini file


    char ** ptr = new char*[20];
    for (int i = 0; i < 20; ++i) // num of ports
    ptr[i] = new char[20];

	int i_com = tpv.LoadCOMPortList(ptr);

	for (int n = 0; n < i_com; n++)
	{
		m_combo_com.InsertString(n, ptr[n]);
	}
	m_combo_com.SetCurSel(0); //.ItemIndex = 0;
	//--------------------------------------------
	char str1[20];

	if (i_com > 0)
	{
		if (strlen(ptr[0]) > 3)
		{
			for (int n = 3; n < strlen(ptr[0]); n++)
			{
				str1[n-3] = ptr[0][n];			
			}
			str1[strlen(ptr[0]) - 3] = 0;
			com_number = atoi(str1);
			com_is_check = 1;
		}
	}

	for (int n = 0; n < 10; n++)
	{
	  char ch_speed[10];
	  sprintf(ch_speed, "%d", com_speed[n]);

	  m_combo_speed.InsertString(n, ch_speed);
	}
	m_combo_speed.SetCurSel(num_com_speed); //.ItemIndex = 11;

	// --------------------------------------------

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBP_v1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBP_v1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBP_v1Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int iSel;
	iSel = m_TabCtrl.GetCurSel();


	for (int i = 1010; i < 1093; i++)
	{
		if ((i != 1018) && (i != 1087) && (i != 1090) && (i != 1080) && (i != 1088) && (i != 1089) && (i != 1079) && (i != 1080) && (i != 1091))
		//if ((i != 1018) && (i != 1087) && (i != 1090) && (i != 1080) && (i != 1088) && (i != 1089) && (i != 1091))  // жирный вывод
		{
		make_bold(i); // 1080 1088 1089  1079
		}
	}


	if (iSel == 0)
	{
		iTabSel = 0;

		for (int i = 1010; i < 1091; i++)
		{
				GetDlgItem(i)->ShowWindow(SW_HIDE);
		}
		
			GetDlgItem(IDC_BUTTON_CONNECT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_RADIO_CONNECT_INDICATOR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_CONNECT_GROUP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_CONNECTION)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_COM)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_COM_PORTS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_BAUD_RATE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_SPEED)->ShowWindow(SW_SHOW);
	}
	else if (iSel == 1)  // Меню сбойные точки
	{
		iTabSel = 1;
	
		GetDlgItem(IDC_BUTTON_CONNECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CONNECT_INDICATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CONNECT_GROUP)->ShowWindow(SW_HIDE);	
		GetDlgItem(IDC_STATIC_CONNECTION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_COM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_COM_PORTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BAUD_RATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_SPEED)->ShowWindow(SW_HIDE);

		
		if (comConnect)
		{
			for (int i = 1010; i < 1091; i++)
			{
					GetDlgItem(i)->EnableWindow(1);
					GetDlgItem(i)->ShowWindow(SW_SHOW);
					//make_bold(i);  // err
			}

			OnBnClickedRadioFilterNot(); //  // IDC_RADIO_FILTER_NOT
		}
		else
		{
			for (int i = 1010; i < 1091; i++)
			{
					GetDlgItem(i)->EnableWindow(0);
					GetDlgItem(i)->ShowWindow(SW_SHOW);
			}
		
		}


	}
	else if (iSel == 2)
	{

		iTabSel = 2;

		GetDlgItem(IDC_BUTTON_CONNECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CONNECT_INDICATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CONNECT_GROUP)->ShowWindow(SW_HIDE);	
		GetDlgItem(IDC_STATIC_CONNECTION)->ShowWindow(SW_HIDE);	
		GetDlgItem(IDC_COMBO_COM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_COM_PORTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BAUD_RATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_SPEED)->ShowWindow(SW_HIDE);

		for (int i = 1010; i < 1091; i++)
		{
				GetDlgItem(i)->ShowWindow(SW_HIDE);
		}
	}


	*pResult = 0;
}
int nn = 0;




void CBP_v1Dlg::Tpv_Write(CString str)
{

	char str1[200];
	CString cs2; 
	char NameStringLogTime[50] = {0};

	sprintf(str1, "%s",  str);

	//	strcpy(str1, str);
	//	strcpy(str, "tmp100 get;");
	//	strcpy(str, "clb_acc_save;");
	//	strcpy(str, "clb_vsk_range {2.8; 0.05; 2.9};");

	strcat(str1, "\r");

	GetMeCurrTime(NameStringLogTime);

		if (tpv.port.handle)
		{
		//cs2.Format("%s %s", "Cmd-> ", str);
		cs2.Format("%s%s %s", NameStringLogTime, ">", str);
				m_list_tpv.AddString(cs2);

	//		fPfileLogString(pStData, 9, 0, cs2);

	//		pFile_buf_log_create_close_add(pStData, cs2);


			tpv.SendText((unsigned char *)str1, strlen(str1));
		}



	// -------  auto scroll 
	// int nCount = m_list_tpv.GetCount();
	// if (nCount > 0) 	m_list_tpv.SetCurSel(nCount - 1);

	//------

	return;
};

int CBP_v1Dlg::fCallback(COMPort *hCOMPort)
{

	CmdAnsHandler(hCOMPort);


	// -------  auto scroll 
	int nCount = m_list_tpv.GetCount();
	if (nCount > 0) 	m_list_tpv.SetCurSel(nCount - 1);
	//------

return 1;
};


// Обработка ответов от модуля Текстовый режим
void CBP_v1Dlg::CmdAnsHandler(COMPort *hCOMPort)
{
	int length;
	int LenAns1;
	unsigned char StrAns1[8192];
	CString csAnsIn;
	CString cs1;
	char NameStringLogTime[50] = {0};

	GetMeCurrTime(NameStringLogTime);

	if (hCOMPort->port.handle)
	{
		LenAns1 = hCOMPort->bufRead.GetBuffer(StrAns1, hCOMPort->btr);  // Читаем в буфер StrAns1, hCOMPort->btr символов
	}

	// Формирование CString типа из принятой посылки, нужно ли в текстовом режиме?
	for (int n = 0; n < LenAns1; n++)
	{
		csAnsIn = csAnsIn + (char)StrAns1[n]; 			 
	}

	//cs1.Format("%s %s", "Ans->", csAnsIn);
	//cs1.Format("%s %s", "<", csAnsIn);
	cs1.Format("%s%s %s", NameStringLogTime, "<", csAnsIn);
	

	// listbox and Logs
	m_list_tpv.AddString(cs1);


}



void CBP_v1Dlg::OnBnClickedCheckStopKadr()
{
	//1. "fr_stick on;" или "fr_stick off;" 

	CString cs1; 
	
	bool check_btn_state;

	check_btn_state = m_button_stop_frame_1.GetCheck();

	if (check_btn_state) 
	{
		cs1 = "fr_stick on;";
	}
	else
		cs1 = "fr_stick off;";

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
	
}




void CBP_v1Dlg::OnBnClickedCheckVklKresta()
{
	//2. "cross on;" или "cross off;"

	CString cs1; 
	
	bool check_btn_state;

	check_btn_state = m_button_no_chrest_2.GetCheck();

	if (check_btn_state) 
	{
		cs1 = "cross on;";
	}
	else
		cs1 = "cross off;";

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioFilterRcos()
{
	//3. "rsz_ft rcos;"

	CString cs1; 

	cs1 = "rsz_ft rcos;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1

}

void CBP_v1Dlg::OnBnClickedRadioFilterBikube()
{
	//4. "rsz_ft bicub;"

	CString cs1; 

	cs1 = "rsz_ft bicub;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioFilterBiline()
{
	//5. "rsz_ft bilin;"

	CString cs1; 

	cs1 = "rsz_ft bilin;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioFilterNot()
{
    //6. "rsz_ft pass;"

	CString cs1; 

	cs1 = "rsz_ft pass;"; 
	cs1 = "TestMode on"; ///r
	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1

}




void CBP_v1Dlg::OnBnClickedBotFindHotFrmae()
{
	//7. "clb2hot;"

	CString cs1; 

	cs1 = "clb2hot;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedBotFindCoolFrmae()
{
	//8. "clb2cold;"

	CString cs1; 

	cs1 = "clb2cold;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButFindIn2Frame()
{
	// 9. "clb_bp_2p XX;", где XX - десятичное число, 
	//в котором может быть дробная часть (например, 87.5), берется из строки ввода 10.

	char str[50] = "clb_bp_2p ";
	char str1[10];
	CString cs1; 

	m_edit_find_in_2_frames.GetLine(0, str1, 10); // 0 строка, буфер, макс. число в буфере
	strcat(str, str1);
	m_list_tpv.AddString(str);

	cs1.Append(str); 
	cs1.Append(";"); 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1

}

void CBP_v1Dlg::OnBnClickedButFindIn1Frame()
{
	// 12. "clb_bp_plus XX;", 
	//где XX - десятичное число, в котором может быть дробная часть (например, 87.5), берется из строки ввода 13.

	char str[50] = "clb_bp_plus ";
	char str1[10];
	CString cs1; 

	m_edit_find_in_1_frames.GetLine(0, str1, 10);  // 0 строка, буфер, макс. число в буфере
	strcat(str, str1);
	m_list_tpv.AddString(str);

	cs1.Append(str); 
	cs1.Append(";"); 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnDeltaposSpinFindIn2Frame(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nPos = pNMUpDown->iPos;
	CString str;
	str.Format("%.1f", (nPos  + pNMUpDown->iDelta) / 10.0);
	UpdateData(FALSE);

	m_edit_find_in_2_frames.SetWindowText(str);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1

	*pResult = 0;
}



void CBP_v1Dlg::OnDeltaposSpinFindIn1Frame(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nPos = pNMUpDown->iPos;
	CString str;
	str.Format("%.1f", (nPos  + pNMUpDown->iDelta) / 10.0);
	UpdateData(FALSE);

	m_edit_find_in_1_frames.SetWindowText(str);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1

	*pResult = 0;
}


void CBP_v1Dlg::OnBnClickedBotResetListPoints()
{
	// 15. "bt_rst;"
	
	CString cs1; 

	cs1 = "bt_rst;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1

}

void CBP_v1Dlg::OnBnClickedBotListOfPointsReport()
{
	// 16. "bt_rep;"
	
	CString cs1; 

	cs1 = "bt_rep;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedBotSaveToFlash2()
{
	// 17. "sc;"

	CString cs1; 

	cs1 = "sc;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedBotReadFromFlash()
{
	// 18. "rc;"

	CString cs1; 

	cs1 = "rc;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN2N2()
{
	// 19.
	iReplacePoint_x = -2;
	iReplacePoint_y = -2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN1N2()
{
	// 19.
	iReplacePoint_x = -1;
	iReplacePoint_y = -2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadio0N2()
{
	// 19.
	iReplacePoint_x =  0;
	iReplacePoint_y = -2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP1N2()
{
	// 19.
	iReplacePoint_x =  1;
	iReplacePoint_y = -2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP2N2()
{
	// 19.
	iReplacePoint_x =  2;
	iReplacePoint_y = -2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN2N1()
{
	// 19.
	iReplacePoint_x = -2;
	iReplacePoint_y = -1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN1N1()
{
	// 19.
	iReplacePoint_x = -1;
	iReplacePoint_y = -1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadio0N1()
{
	// 19.
	iReplacePoint_x =  0;
	iReplacePoint_y = -1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP1N1()
{
	// 19.
	iReplacePoint_x =  1;
	iReplacePoint_y = -1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP2N1()
{
	// 19.
	iReplacePoint_x =  2;
	iReplacePoint_y = -1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN20()
{
	// 19.
	iReplacePoint_x = -2;
	iReplacePoint_y =  0;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN10()
{
	// 19.
	iReplacePoint_x = -1;
	iReplacePoint_y =  0;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadio00()
{
	// 19.
	iReplacePoint_x =  0;
	iReplacePoint_y =  0;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP10()
{
	// 19.
	iReplacePoint_x =  1;
	iReplacePoint_y =  0;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP20()
{
	// 19.
	iReplacePoint_x =  2;
	iReplacePoint_y =  0;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN2P1()
{
	// 19.
	iReplacePoint_x = -2;
	iReplacePoint_y =  1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN1P1()
{
	// 19.
	iReplacePoint_x = -1;
	iReplacePoint_y =  1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadio0P1()
{
	// 19.
	iReplacePoint_x =  0;
	iReplacePoint_y =  1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP1P1()
{
	// 19.
	iReplacePoint_x =  1;
	iReplacePoint_y =  1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP2P1()
{
	// 19.
	iReplacePoint_x =  2;
	iReplacePoint_y =  1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}


void CBP_v1Dlg::OnBnClickedRadioN2P2()
{
	// 19.
	iReplacePoint_x = -2;
	iReplacePoint_y =  2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioN1P2()
{
	// 19.
	iReplacePoint_x = -1;
	iReplacePoint_y =  2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadio0P2()
{
	// 19.
	iReplacePoint_x =  0;
	iReplacePoint_y =  2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP1P2()
{
	// 19.
	iReplacePoint_x =  1;
	iReplacePoint_y =  2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioP2P2()
{
	// 19.
	iReplacePoint_x =  2;
	iReplacePoint_y =  2;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedBotAddPointUnderKrest()
{
	// 20. "clb_bp_plus cross dX, dY;", где dX, dY - значения приращений, определяемые кнопками 19.

	iNewPoint = 1;  // добавлены новые точки 
	CString cs1; 

	char str[50] = "clb_bp_plus cross";
	char str_x[10];
	char str_y[10];

	if ((iReplacePoint_x == 0) && (iReplacePoint_y == 0))
	{

	}
	else
	{

	sprintf(str_x, "%d", iReplacePoint_x);
	sprintf(str_y, "%d", iReplacePoint_y);

	//m_edit_find_in_1_frames.GetLine(0, str1, 10);  // 0 строка, буфер, макс. число в буфере
	strcat(str, " ");
	strcat(str, str_x);
	strcat(str, ", ");
	strcat(str, str_y);
	}
	strcat(str, ";");
//	m_list_tpv.AddString(str);

	cs1.Append(str); 
	//cs1.Append(";"); 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedBotDelPointUnderKrest()
{
	// 21. "bp_del cross;"

	iNewPoint = 1;  // добавлены новые точки
	
	CString cs1; 

	cs1 = "bp_del cross;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}


void CBP_v1Dlg::OnBnClickedRadioX1()
{
	// 32. радиокнопки, задающие величины перемещений креста при нажатии кнопок 23-31.
	krest_m = 1;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioX3()
{
	// 32. радиокнопки, задающие величины перемещений креста при нажатии кнопок 23-31.
	krest_m = 3;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioX10()
{
	// 32. радиокнопки, задающие величины перемещений креста при нажатии кнопок 23-31.
	krest_m = 10;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedRadioX100()
{
	// 32. радиокнопки, задающие величины перемещений креста при нажатии кнопок 23-31.
	krest_m = 100;

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestNw()
{
	// 23. "cross_move -1, -1",  "cross_move -3, -3", "cross_move -10, -10", "cross_move -100, -100" в зависимости от положения кнопок 32. 
	
	CString cs1; 

	switch (krest_m)
	{
	case 1:
		cs1 = "cross_move -1, -1;"; 
		cs1 = "SetMotor -100 100 25 10 10 10;"; ///r
		break;
	case 3:
		cs1 = "cross_move -3, -3;"; 
		cs1 = "SetMotor -100 100 50 10 10 10;"; ///r
		break;
	case 10:
		cs1 = "cross_move -10, -10;"; 
		cs1 = "SetMotor -100 100 75 10 10 10;"; ///r
		break;
	case 100:
		cs1 = "cross_move -100, -100;";
		cs1 = "SetMotor -100 100 100 10 10 10;"; ///r
		break;
	}

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestN()
{
	//24. "cross_move 0, -1",  "cross_move 0, -3", "cross_move 0, -10", "cross_move 0, -100" в зависимости от положения кнопок 32. 

	CString cs1; 

	switch (krest_m)
	{
	case 1:
		cs1 = "cross_move 0, -1;"; 
		cs1 = "SetMotor 100 100 25 10 10 10;"; ///r
		break;
	case 3:
		cs1 = "cross_move 0, -3;"; 
		cs1 = "SetMotor 100 100 50 10 10 10;"; ///r
		break;
	case 10:
		cs1 = "cross_move 0, -10;"; 
		cs1 = "SetMotor 100 100 75 10 10 10;"; ///r
		break;
	case 100:
		cs1 = "cross_move 0, -100;"; 
		cs1 = "SetMotor 100 100 100 10 10 10;"; ///r
		break;
	}
	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestNe()
{
	// 25. "cross_move 1, -1",  "cross_move 3, -3", "cross_move 10, -10", "cross_move 100, -100" в зависимости от положения кнопок 32.

	CString cs1; 

	switch (krest_m)
	{
	case 1:
		cs1 = "cross_move 1, -1;"; 
		cs1 = "SetMotor 100 -100 25 10 10 10;"; ///
		break;
	case 3:
		cs1 = "cross_move 3, -3;"; 
		cs1 = "SetMotor 100 -100 50 10 10 10;"; ///
		break;
	case 10:
		cs1 = "cross_move 10, -10;";
		cs1 = "SetMotor 100 -100 75 10 10 10;"; ///
		break;
	case 100:
		cs1 = "cross_move 100, -100;"; 
		cs1 = "SetMotor 100 -100 100 10 10 10;"; ///
		break;
	}

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestW()
{
	// 26. "cross_move -1, 0",  "cross_move -3, 0", "cross_move -10, 0", "cross_move -100, 0" в зависимости от положения кнопок 32.

	CString cs1; 

	switch (krest_m)
	{
	case 1:
		cs1 = "cross_move -1, 0;"; 
		cs1 = "SetMotor -200 200 25 10 10 10;"; ///r
		break;
	case 3:
		cs1 = "cross_move -3, 0;"; 
		cs1 = "SetMotor -200 200 50 10 10 10;"; ///r
		break;
	case 10:
		cs1 = "cross_move -10, 0;";
		cs1 = "SetMotor -200 200 75 10 10 10;"; ///r
		break;
	case 100:
		cs1 = "cross_move -100, 0;"; 
		cs1 = "SetMotor -200 200 100 10 10 10;"; ///r
		break;
	}

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestCentr()
{ 
	// 27. "cross centr;" 

	CString cs1; 

	cs1 = "cross centr;"; 

	cs1 = "SetMotor 0 0 0 10 10 10;"; ///r
	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestE()
{
	// 28. "cross_move 1, 0",  "cross_move 3, 0", "cross_move 10, 0", "cross_move 100, 0" в зависимости от положения кнопок 32. 

	CString cs1; 

	switch (krest_m)
	{
	case 1:
		cs1 = "cross_move 1, 0;"; 
		cs1 = "SetMotor 200 -200 25 10 10 10;"; ///r
		break;
	case 3:
		cs1 = "cross_move 3, 0;"; 
			cs1 = "SetMotor 200 -200 50 10 10 10;"; ///r
		break;
	case 10:
		cs1 = "cross_move 10, 0;"; 	
			cs1 = "SetMotor 200 -200 75 10 10 10;"; ///r
		break;
	case 100:
		cs1 = "cross_move 100, 0;";
			cs1 = "SetMotor 200 -200 100 10 10 10;"; ///r
		break;
	}
	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestSw()
{
	// 29. "cross_move -1, 1",  "cross_move -3, 3", "cross_move -10, 10", "cross_move -100, 100" в зависимости от положения кнопок 32. 
	
	CString cs1; 

	switch (krest_m)
	{
	case 1:
		cs1 = "cross_move -1, 1;"; 
		cs1 = "SetMotor -100 100 25 10 10 10;"; ///r	
		break;
	case 3:
		cs1 = "cross_move -3, 3;";
		cs1 = "SetMotor -100 100 50 10 10 10;"; ///r	
		break;
	case 10:
		cs1 = "cross_move -10, 10;"; 
		cs1 = "SetMotor -100 100 75 10 10 10;"; ///r	
		break;
	case 100:
		cs1 = "cross_move -100, 100;"; 
		cs1 = "SetMotor -100 100 100 10 10 10;"; ///r	
		break;
	}
	
	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestS()
{
	// 30. "cross_move 0, 1",  "cross_move 0, 3", "cross_move 0, 10", "cross_move 0, 100" в зависимости от положения кнопок 32. 

	CString cs1; 

	switch (krest_m)
	{
	case 1:
		cs1 = "cross_move 0, 1;"; 
		cs1 = "SetMotor -100 -100 25 10 10 10;"; ///r
		break;
	case 3:
		cs1 = "cross_move 0, 3;"; 
		cs1 = "SetMotor -100 -100 50 10 10 10;"; ///r
		break;
	case 10:
		cs1 = "cross_move 0, 10;"; 
		cs1 = "SetMotor -100 -100 75 10 10 10;"; ///r
		break;
	case 100:
		cs1 = "cross_move 0, 100;";
		cs1 = "SetMotor -100 -100 100 10 10 10;"; ///r
		break;
	}

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButDriveKrestSe()
{
	// 31. "cross_move 1, 1",  "cross_move 3, 3", "cross_move 10, 10", "cross_move 100, 100" в зависимости от положения кнопок 32. 

	CString cs1; 

	switch (krest_m)
	{
	case 1:
		cs1 = "cross_move 1, 1;"; 
		cs1 = "SetMotor 100 -100 25 10 10 10;"; ///r
		break;
	case 3:
		cs1 = "cross_move 3, 3;"; 
		cs1 = "SetMotor 100 -100 50 10 10 10;"; ///r
		break;
	case 10:
		cs1 = "cross_move 10, 10;"; 
		cs1 = "SetMotor 100 -100 75 10 10 10;"; ///r
		break;
	case 100:
		cs1 = "cross_move 100, 100;"; 
		cs1 = "SetMotor 100 -100 100 10 10 10;"; ///r
		break;
	}
	//cs1 = "SetMotor 100 -100 100 10 10 10;"; ///r	
	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButAreaKrest()
{
	// 34. "rsz_centr cross;"

	CString cs1; 

	cs1 = "rsz_centr cross;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButAreaCenter()
{
	// 35. "rsz_centr def;"
	
	CString cs1; 

	cs1 = "rsz_centr def;"; 

	Tpv_Write(cs1);

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}

void CBP_v1Dlg::OnBnClickedButEnter()
{
	// 38. кнопка, по нажатию которой команда из строки ввода 37 подается на вход модуля, а также фиксируется в списке 36.

	CString cs1;

	m_edit_cmd.GetWindowText(cs1);

	Tpv_Write(cs1);

	bLastCmb = 1; //Последняя команда введена вручную, если = 1

}



void CBP_v1Dlg::make_bold(int id) // сделать жирным статический текст
{
	CFont m_fntCaptionFont; 
	CWnd *pCaption = GetDlgItem(id); 
	 CFont *pFont = pCaption->GetFont();

	// получаем описание страрого шрифта и редактируем его 
	LOGFONT lf; 
	pFont->GetObject(sizeof(lf), &lf); 
	//lf.lfHeight = lf.lfHeight*3/2; // увеличиваем высоту в 3 раза 
	lf.lfWeight = FW_BOLD; // делаем жирным

	// создаём новый шрифт из исправленного описания 
	BOOL g=m_fntCaptionFont.CreateFontIndirect(&lf);

	pCaption->SetFont(&m_fntCaptionFont);
	
}

void CBP_v1Dlg::OnCbnSelchangeComboCom()
{
	char str[20];
	char str1[20];
	//int cn = 0;
	int count = 0;
	m_combo_com.GetLBText(m_combo_com.GetCurSel(),str);
    //SetWindowText(stroka1);
	//GetDlgItem( IDC_STATIC_COM ) ->SetWindowText( str );

	if (strlen(str) > 3)
	{
		for (int n = 3; n < strlen(str); n++)
		{
			str1[n-3] = str[n];			
		}
		str1[strlen(str) - 3] = 0;
		com_number = atoi(str1);
		com_is_check = 1;
	}

}


void CBP_v1Dlg::OnBnClickedRadioM11()
{
	//22. "dm rsz; zoom 1;", "dm rsz; zoom 2;", "dm rsz; zoom 3;", "dm rsz; zoom 4;", "dm rsz; zoom 5;", "dm rsz; zoom 6;" 
	
	CString cs1; 

	cs1 = "dm rsz; zoom 1;"; 

	Tpv_Write(cs1);
}

void CBP_v1Dlg::OnBnClickedRadioM12()
{
	//22. "dm rsz; zoom 1;", "dm rsz; zoom 2;", "dm rsz; zoom 3;", "dm rsz; zoom 4;", "dm rsz; zoom 5;", "dm rsz; zoom 6;" 
	
	CString cs1; 

	cs1 = "dm rsz; zoom 2;"; 

	Tpv_Write(cs1);
}

void CBP_v1Dlg::OnBnClickedRadioM13()
{
	//22. "dm rsz; zoom 1;", "dm rsz; zoom 2;", "dm rsz; zoom 3;", "dm rsz; zoom 4;", "dm rsz; zoom 5;", "dm rsz; zoom 6;" 
	
	CString cs1; 

	cs1 = "dm rsz; zoom 3;"; 

	Tpv_Write(cs1);
}

void CBP_v1Dlg::OnBnClickedRadioM14()
{
	//22. "dm rsz; zoom 1;", "dm rsz; zoom 2;", "dm rsz; zoom 3;", "dm rsz; zoom 4;", "dm rsz; zoom 5;", "dm rsz; zoom 6;" 
	
	CString cs1; 

	cs1 = "dm rsz; zoom 4;"; 

	Tpv_Write(cs1);
}

void CBP_v1Dlg::OnBnClickedRadioM15()
{
	//22. "dm rsz; zoom 1;", "dm rsz; zoom 2;", "dm rsz; zoom 3;", "dm rsz; zoom 4;", "dm rsz; zoom 5;", "dm rsz; zoom 6;" 
	
	CString cs1; 

	cs1 = "dm rsz; zoom 5;"; 

	Tpv_Write(cs1);
}

void CBP_v1Dlg::OnBnClickedRadioM16()
{
	//22. "dm rsz; zoom 1;", "dm rsz; zoom 2;", "dm rsz; zoom 3;", "dm rsz; zoom 4;", "dm rsz; zoom 5;", "dm rsz; zoom 6;" 
	
	CString cs1; 

	cs1 = "dm rsz; zoom 6;"; 

	Tpv_Write(cs1);
}

void CBP_v1Dlg::OnCbnSelchangeComboSpeed()
{
	char str[20];
	char str1[20];
	//int cn = 0;
	int count = 0;
	m_combo_speed.GetLBText(m_combo_speed.GetCurSel(),str);
	sel_com_speed = atoi(str);

}


BOOL CBP_v1Dlg::PreTranslateMessage(MSG* pMsg) 
{

/*
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5
*/


	if (iTabSel == 1)
	{
			 if (pMsg->message != WM_KEYDOWN) return FALSE;

			 if (pMsg->wParam == VK_UP) 
			 {
				
				//  if ((GetKeyState( VK_LSHIFT ) & 0x80) > 0)   
				//  {
				//  OnDeltaposSpinFindIn2Frame(pDlg, *pResult);
				//  }
				//  else
				//  {
				//  	OnBnClickedButDriveKrestN();
				//  }
			   
				  OnBnClickedButDriveKrestN();

				  return TRUE;
			 }


			 if (pMsg->wParam == VK_DOWN) // 
			 {
			 // m_editNumber.SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
			  OnBnClickedButDriveKrestS();
			  return TRUE;
			 }

			 if (pMsg->wParam == VK_LEFT) // 
			 {
			  OnBnClickedButDriveKrestW();
			  return TRUE;
			 }

			 if (pMsg->wParam == VK_RIGHT) // 
			 {
			 OnBnClickedButDriveKrestE();
			  return TRUE;
			 }
			 if (bLastCmb == 1) // если последней командой было изменение поля ввода
			 {
				 if (pMsg->wParam == VK_RETURN) // 
				 {
				 OnBnClickedButEnter();
				  return TRUE;
				 }
			 }




	}
 


	// if (pMsg->wParam == VK_EXECUTE) 
	// {
	 // m_editNumber.SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
	  //MessageBox(_T("RIGHT_UP!"));
	//  OnBnClickedButEnter();
	//  return TRUE;
	// }
 
 
  return FALSE;

}


void CBP_v1Dlg::OnEnChangeEditFindIn2Frame()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	bLastCmb = 0; //Последняя команда введена вручную, если = 1

}

void CBP_v1Dlg::OnBnClickedButtonConnect()
{


	if (com_is_check)// если в имени выбранного ком-порта больше 3-х букв
	//	if (1 == 1)
	{
		if ((comConnect == 0) && (!tpv.Init(com_number, sel_com_speed, 0, 1) ) ) 	// если не было соединения и соединение установилось			
		{
			m_radio_indicator_of_connection.SetCheck(1);
		//	m_radio_indicator_of_connection.SetWindowTextA("Connected:");
			//IDC_BUTTON_CONNECT
			m_button_connect.SetWindowTextA("Disconnect");
			UpdateData(FALSE); 
			comConnect = 1;

			GetDlgItem(IDC_COMBO_COM)->EnableWindow(0);
			GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(0);
		}
		else if (comConnect == 1)
		{
			bool comState = tpv.CloseComPort();
			m_radio_indicator_of_connection.SetCheck(0);
		//	m_radio_indicator_of_connection.SetWindowTextA("Disconnected:");
			m_button_connect.SetWindowTextA("Connect");
			UpdateData(FALSE); 
			comConnect = 0;

			GetDlgItem(IDC_COMBO_COM)->EnableWindow(1);
			GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(1);
		}
	}
}


void CBP_v1Dlg::OnBnClickedRadioConnectIndicator()
{
	if (comConnect == 1)
	{
		m_radio_indicator_of_connection.SetCheck(1);
	}
	else
	{
		 m_radio_indicator_of_connection.SetCheck(0);
	}
}

//void CBP_v1Dlg::OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// Для этого средства требуется Windows Vista или более поздняя версия.
//	// Символ _WIN32_WINNT должен быть >= 0x0600.
//	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
//	// TODO: добавьте свой код обработчика уведомлений
//	*pResult = 0;
//}

//void CBP_v1Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: добавьте свой код обработчика уведомлений
//	*pResult = 0;
//}

//void CBP_v1Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// 33. слайдер с величиной в пределах 0...255. при изменении положения слайдера 
//	// выполняется команда "cross color XX;", где 
//    // XX - значение, выбранное на слайдере.
//
//	int pos = 0;
//
//	CString cs1;
//	char cPos[10];
//
//	cs1 = "cross color ";
//
//	pos = m_slider_color.GetPos();
//
//	sprintf(cPos, "%d", pos);
//
//	cs1.Append(cPos);
//
//	cs1.Append(";");	
//
//	Tpv_Write(cs1);
//
//	*pResult = 0;
//
//}

//void CBP_v1Dlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: добавьте свой код обработчика уведомлений
//	*pResult = 0;
//}

//void CBP_v1Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: добавьте свой код обработчика уведомлений
///*
//	int pos = 0;
//
//	CString cs1;
//	char cPos[10];
//
//	cs1 = "cross color ";
//
//	pos = m_slider_color.GetPos();
//
//	sprintf(cPos, "%d", pos);
//
//	cs1.Append(cPos);
//
//	cs1.Append(";");	
//
//	Tpv_Write(cs1);
//*/	
//
//	*pResult = 0;
//}

//void CBP_v1Dlg::OnNMThemeChangedSlider1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// Для этого средства требуется Windows XP или более поздняя версия.
//	// Символ _WIN32_WINNT должен быть >= 0x0501.
//	// TODO: добавьте свой код обработчика уведомлений
//
//	int pos = 0;
//
//	CString cs1;
//	char cPos[10];
//
//	cs1 = "cross color ";
//
//	pos = m_slider_color.GetPos();
//
//	sprintf(cPos, "%d", pos);
//
//	cs1.Append(cPos);
//
//	cs1.Append(";");	
//
//	Tpv_Write(cs1);
//
//
//	*pResult = 0;
//}



void CBP_v1Dlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	int pos = 0;

	CString cs1;
	char cPos[10];

	cs1 = "cross color ";

	pos = m_slider_color.GetPos();

	sprintf(cPos, "%d", pos);

	cs1.Append(cPos);

	cs1.Append(";");	

	Tpv_Write(cs1);

	*pResult = 0;
}

int dd;
void CBP_v1Dlg::OnCancel()
{
	int iNeedSave = 0;
	// TODO: добавьте специализированный код или вызов базового класса


	//AfxMessageBox(" Rigth Button Click ",MB_OKCANCEL);

	//ff = AfxMessageBox(" Rigth Button Click ", MB_OKCANCEL );

	if (iNewPoint == 1)   // добавлены новые точки 
	{
		iNeedSave =AfxMessageBox(" Сохранить новые точки на флеш? ", MB_OKCANCEL );
		
		if (iNeedSave == IDOK)
		{
			dd = 1;
			OnBnClickedBotSaveToFlash2(); 
			//Sleep(2000);

		} 
		else
		{
			CDialog::OnCancel();
		}
		iNewPoint = 0;
	    
	}
	else
	{
	 CDialog::OnCancel();
	}
	/*
	IDABORT - Abort нажата была
	IDCANCELM - Cancel нажата была
	IDIGNORE - Ignore нажата была
	IDNO - No нажата была
	IDOK - OK нажата была
	IDRETRY - Retry нажата была

	#define IDOK                1
	#define IDCANCEL            2
	#define IDABORT             3
	#define IDRETRY             4
	#define IDIGNORE            5
	#define IDYES               6
	#define IDNO                7
	*/
	//сохранить новые точки на флеш

	
}

void CBP_v1Dlg::OnEnChangeEditCmd()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления

	bLastCmb = 1; //Последняя команда введена вручную, если = 1

}

void CBP_v1Dlg::OnEnChangeEditFindIn1Frame()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления

	bLastCmb = 0; //Последняя команда введена вручную, если = 1
}


int CBP_v1Dlg::GetMeCurrTime(char * NameStringLogTime)
{
	//char NameStringLogTime1[50];
	char cTimeH[10] = {0};
	char cTimeM[10] = {0};
	char cTimeS[10] = {0};

	seconds = time(NULL);
	timeinfo = localtime(&seconds);
	sprintf(NameStringLogTime, "%s", asctime(timeinfo));
	
	sprintf(cTimeH, "%d", timeinfo ->tm_hour);
	if (strlen(cTimeH) == 1) {	sprintf(cTimeH, "%s%d", "0",  timeinfo ->tm_hour);	}
	sprintf(cTimeM, "%d", timeinfo ->tm_min);
	if (strlen(cTimeM) == 1) {	sprintf(cTimeM, "%s%d", "0",  timeinfo ->tm_min);	}
	sprintf(cTimeS, "%d", timeinfo ->tm_sec);
	if (strlen(cTimeM) == 1) {	sprintf(cTimeS, "%s%d", "0",  timeinfo ->tm_sec);	}

	sprintf(NameStringLogTime, "%s%s%s%s%s", cTimeH, ":", cTimeM, ":", cTimeS);

	return 1;
}

