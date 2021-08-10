// BP_v1Dlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"
#include "comport.h"


using namespace COMPortLib;

// CBP_v1Dlg dialog
class CBP_v1Dlg : public CDialog
{
// Construction
public:
	CBP_v1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BP_V1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();


	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabCtrl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	void Tpv_Write(CString str);
	afx_msg void OnBnClickedCheckStopKadr();
	CButton m_button_stop_frame_1;
	CListBox m_list_tpv;


		//---- Callback -----------
	int  fCallback(COMPort *hCOMPort);
	void CmdAnsHandler(COMPort *hCOMPort); // Разбор текстовых посылок
	int GetMeCurrTime(char *NameStringLogTime);


	afx_msg void OnBnClickedCheckVklKresta();
	CButton m_button_no_chrest_2;
	afx_msg void OnBnClickedRadioFilterRcos();
	afx_msg void OnBnClickedRadioFilterBikube();
	afx_msg void OnBnClickedRadioFilterBiline();
	afx_msg void OnBnClickedRadioFilterNot();

	afx_msg void OnBnClickedBotFindHotFrmae();
	afx_msg void OnBnClickedBotFindCoolFrmae();
	CEdit m_edit_find_in_2_frames;
	afx_msg void OnBnClickedButFindIn2Frame();
	afx_msg void OnBnClickedButFindIn1Frame();
	CEdit m_edit_find_in_1_frames;
	CSpinButtonCtrl m_spin_2p;
	afx_msg void OnDeltaposSpinFindIn2Frame(NMHDR *pNMHDR, LRESULT *pResult);
	CSpinButtonCtrl m_spin_1p;
	afx_msg void OnDeltaposSpinFindIn1Frame(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedBotResetListPoints();
	afx_msg void OnBnClickedBotListOfPointsReport();
	afx_msg void OnBnClickedBotSaveToFlash2();
	afx_msg void OnBnClickedBotReadFromFlash();
	afx_msg void OnBnClickedRadioN2N2();
	afx_msg void OnBnClickedRadioN1N2();
	afx_msg void OnBnClickedRadio0N2();
	afx_msg void OnBnClickedRadioP1N2();
	afx_msg void OnBnClickedRadioP2N2();
	afx_msg void OnBnClickedRadioN2N1();
	afx_msg void OnBnClickedRadioN1N1();
	afx_msg void OnBnClickedRadio0N1();
	afx_msg void OnBnClickedRadioP1N1();
	afx_msg void OnBnClickedRadioP2N1();
	afx_msg void OnBnClickedRadioN20();
	afx_msg void OnBnClickedRadioN10();
	afx_msg void OnBnClickedRadio00();
	afx_msg void OnBnClickedRadioP10();
	afx_msg void OnBnClickedRadioP20();
	afx_msg void OnBnClickedRadioN2P1();
	afx_msg void OnBnClickedRadioN1P1();
	afx_msg void OnBnClickedRadio0P1();
	afx_msg void OnBnClickedRadioP1P1();
	afx_msg void OnBnClickedRadioP2P1();
	afx_msg void OnBnClickedRadioN2P2();
	afx_msg void OnBnClickedRadioN1P2();
	afx_msg void OnBnClickedRadio0P2();
	afx_msg void OnBnClickedRadioP1P2();
	afx_msg void OnBnClickedRadioP2P2();
	afx_msg void OnBnClickedBotAddPointUnderKrest();
	afx_msg void OnBnClickedBotDelPointUnderKrest();
	afx_msg void OnBnClickedRadioX1();
	afx_msg void OnBnClickedRadioX3();
	afx_msg void OnBnClickedRadioX10();
	afx_msg void OnBnClickedRadioX100();
	afx_msg void OnBnClickedButDriveKrestNw();
	afx_msg void OnBnClickedButDriveKrestN();
	afx_msg void OnBnClickedButDriveKrestNe();
	afx_msg void OnBnClickedButDriveKrestW();
	afx_msg void OnBnClickedButDriveKrestCentr();
	afx_msg void OnBnClickedButDriveKrestE();
	afx_msg void OnBnClickedButDriveKrestSw();
	afx_msg void OnBnClickedButDriveKrestS();
	afx_msg void OnBnClickedButDriveKrestSe();
	CSliderCtrl m_slider_color;
	afx_msg void OnBnClickedButAreaKrest();
	afx_msg void OnBnClickedButAreaCenter();
	CEdit m_edit_cmd;
	afx_msg void OnBnClickedButEnter();
//	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);

	void make_bold(int id); // сделать жирным статический текст
	CComboBox m_combo_com;
	afx_msg void OnCbnSelchangeComboCom();
	afx_msg void OnStnClickedStaticCom2();
	afx_msg void OnBnClickedRadioM11();
	afx_msg void OnBnClickedRadioM12();
	afx_msg void OnBnClickedRadioM13();
	afx_msg void OnBnClickedRadioM14();
	afx_msg void OnBnClickedRadioM15();
	afx_msg void OnBnClickedRadioM16();
	CComboBox m_combo_speed;
	afx_msg void OnCbnSelchangeComboSpeed();

	//afx_msg void  OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags); 
	afx_msg void OnEnChangeEditFindIn2Frame();
	afx_msg void OnBnClickedButtonConnect();
	CButton m_radio_indicator_of_connection;
	CButton m_button_connect;
	afx_msg void OnBnClickedRadioConnectIndicator();
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnCancel();
public:
	CButton m_button_enter;
	afx_msg void OnEnChangeEditCmd();
	afx_msg void OnEnChangeEditFindIn1Frame();

};
