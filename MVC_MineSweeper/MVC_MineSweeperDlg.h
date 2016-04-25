
// MVC_MineSweeperDlg.h : header file
//

#pragma once
#include"vector"
#include "afxcmn.h"
#include "afxwin.h"

// CMVC_MineSweeperDlg dialog
class CMVC_MineSweeperDlg : public CDialogEx, public Observer
{
// Construction
public:
	CMVC_MineSweeperDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MVC_MINESWEEPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int const FLAG = -3;
	int const BOMB = -1;
	int const UNDISCOVERED = -2;
	int const DISCOVERED = 0;

	int upperBarInPixels;
	int leftBarInPixels;
	int cellDimension;

	int numberOfBombs;

	int const TIMER_ID = 0;
	int currentLine = -1;
	int currentColumn = -1;
	int mtable, ntable;
	std::vector<std::vector<int>> tabel;
	void Update(void*, Change);
	void JocNou(int m, int n, int numberOfBombs);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CListCtrl buttonsListControl;
	CStatic bombCounterTxt;
	CStatic timerTxt;
	CButton restartButton;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedrestartbuttonid();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	void SetNumberOfRemainingFlags(int n);
	afx_msg void OnBnClickedRadioeasy();
	afx_msg void OnBnClickedRadiomedium();
	afx_msg void OnBnClickedRadiohard();
	bool hardBtn;
	CEdit columnTxt;
	CEdit rowTxt;
	afx_msg void OnBnClickedRadio4();
};
