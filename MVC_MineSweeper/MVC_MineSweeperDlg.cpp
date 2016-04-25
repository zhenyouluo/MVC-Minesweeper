
// MVC_MineSweeperDlg.cpp : implementation file
//

#include <exception>
#include "stdafx.h"
#include "MVC_MineSweeper.h"
#include "MVC_MineSweeperDlg.h"
#include "afxdialogex.h"
#include"Changes.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMVC_MineSweeperDlg dialog



CMVC_MineSweeperDlg::CMVC_MineSweeperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMVC_MineSweeperDlg::IDD, pParent)
	, hardBtn(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mtable = ntable = 0;
}

void CMVC_MineSweeperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, bombCounterTextID, bombCounterTxt);
	DDX_Control(pDX, timerTextID, timerTxt);
	DDX_Control(pDX, restartButtonID, restartButton);
	DDX_Control(pDX, IDC_EDIT1, columnTxt);
	DDX_Control(pDX, IDC_EDIT2, rowTxt);
}

BEGIN_MESSAGE_MAP(CMVC_MineSweeperDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(restartButtonID, &CMVC_MineSweeperDlg::OnBnClickedrestartbuttonid)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_RADIOEasy, &CMVC_MineSweeperDlg::OnBnClickedRadioeasy)
	ON_BN_CLICKED(IDC_RADIOMedium, &CMVC_MineSweeperDlg::OnBnClickedRadiomedium)
	ON_BN_CLICKED(IDC_RADIOHard, &CMVC_MineSweeperDlg::OnBnClickedRadiohard)
	ON_BN_CLICKED(IDC_RADIO4, &CMVC_MineSweeperDlg::OnBnClickedRadio4)
END_MESSAGE_MAP()


// CMVC_MineSweeperDlg message handlers

void CMVC_MineSweeperDlg::JocNou(int m, int n, int numberOfBombs)
{
	tabel.resize(m);
	for (int i = 0;i < m;i++)
	{
		tabel[i].resize(n);
		for (int j = 0;j < n;j++)
			tabel[i][j] = 0;
	}
	theApp.GetModel()->InitTable(m, n, numberOfBombs);
}

BOOL CMVC_MineSweeperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	hardBtn = true;

	upperBarInPixels = 60;
	leftBarInPixels = 0;
	cellDimension = 20;

	mtable = 16;
	ntable = 30;
	numberOfBombs = 99;

	SetTimer(TIMER_ID, 1000, NULL);
	JocNou(mtable, ntable, numberOfBombs);

	//for (int i = 1; i <= 480; i++)
	//{
	//	buttonsListControl.InsertItem(LVIF_TEXT | LVIF_STATE, i, _T("0"),
	//		(i % 2) == 0 ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);
	//}

	CFont *myFont = new CFont();
	myFont->CreateFont(40, 0, 0, 0, FW_HEAVY, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Arial"));

	bombCounterTxt.SetFont(myFont);
	timerTxt.SetFont(myFont);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMVC_MineSweeperDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	for (int i = 0; i < mtable ; i++)
	{
		for (int j = 0; j < ntable; j++)
			if (tabel[i][j] == UNDISCOVERED)
			{
				dc.TextOut(j * cellDimension + leftBarInPixels, i * cellDimension + upperBarInPixels, _T("|_|"));
			}
			else if (tabel[i][j] == DISCOVERED)
			{
				dc.TextOut(j * cellDimension + leftBarInPixels, i * cellDimension + upperBarInPixels, _T("   "));
			}
			else if (tabel[i][j] == BOMB)
			{
				dc.TextOut(j * cellDimension + leftBarInPixels, i * cellDimension + upperBarInPixels, _T("|b|"));
			}
			else if (tabel[i][j] == FLAG)
			{
				dc.TextOut(j * cellDimension + leftBarInPixels, i * cellDimension + upperBarInPixels, _T("|F|"));
			}
			else
			{
				CString str;
				str.Format(_T("|%d|"), tabel[i][j]);

				dc.TextOut(j * cellDimension + leftBarInPixels, i * cellDimension + upperBarInPixels, str);
			}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMVC_MineSweeperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMVC_MineSweeperDlg::Update(void* t, Change change)
{
	switch (change)
	{
	case Change::TIME:
	{
		timerTxt.SetWindowTextW(theApp.GetModel()->GetTimePassed());
		CRect rect;

		timerTxt.GetClientRect(rect);

		InvalidateRect(rect, 0);
	}
		break;
	case Change::TABLE:
	{
		Model* model = (Model*)t;

		for (int i = 0; i < mtable; i++)
		{
			for (int j = 0; j < ntable; j++)
			{
				if (model->GetElementFromDiscoveredTableAt(i, j) == DiscoveredStates::DISCOVERED)
				{
					tabel[i][j] = model->GetElementFromTableAt(i, j);
				}
				else if (model->GetElementFromDiscoveredTableAt(i, j) == DiscoveredStates::UNDISCOVERED)
				{
					tabel[i][j] = UNDISCOVERED;
				}
				else if (model->GetElementFromDiscoveredTableAt(i, j) == DiscoveredStates::FLAG)
				{
					tabel[i][j] = FLAG;
				}
			}
		}
		Invalidate();
	}
		break;
	case Change::GAME_LOST:
	{
		restartButton.SetWindowTextW(_T("LOST"));
	}
		break;
	case Change::GAME_WON:
	{
		restartButton.SetWindowTextW(_T("WON"));
	}
		break;
	case Change::FLAGS:
	{
		Model* model = (Model*)t;
		SetNumberOfRemainingFlags(model->GetNumberOfRemainingFlags());
	}
	default: break;
	}


	
}

void CMVC_MineSweeperDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	theApp.GetController()->Click(point.x, point.y,leftBarInPixels);

	__super::OnLButtonDown(nFlags, point);
}


void CMVC_MineSweeperDlg::OnTimer(UINT_PTR nIDEvent)
{

	theApp.GetController()->IncremetTime();
	__super::OnTimer(nIDEvent);
}


void CMVC_MineSweeperDlg::OnBnClickedrestartbuttonid()
{
	theApp.GetController()->RestartClicked(mtable,ntable,numberOfBombs); 
	restartButton.SetWindowTextW(_T("Restart"));
}


void CMVC_MineSweeperDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	theApp.GetController()->RightClick(point.x, point.y,leftBarInPixels);
	__super::OnRButtonDown(nFlags, point);
}

void CMVC_MineSweeperDlg::SetNumberOfRemainingFlags(int n)
{
	CString numberOfRemainingFlags;
	numberOfRemainingFlags.Format(_T("%d"), n);
	bombCounterTxt.SetWindowTextW(numberOfRemainingFlags);
}


void CMVC_MineSweeperDlg::OnBnClickedRadioeasy()
{
	leftBarInPixels = 125;
	mtable = 16;
	ntable = 16;
	numberOfBombs = 33;

	theApp.GetController()->RestartClicked(mtable, ntable, numberOfBombs);
}


void CMVC_MineSweeperDlg::OnBnClickedRadiomedium()
{
	leftBarInPixels = 100;
	mtable = 16;
	ntable = 20;
	numberOfBombs = 66;

	theApp.GetController()->RestartClicked(mtable, ntable, numberOfBombs);
}


void CMVC_MineSweeperDlg::OnBnClickedRadiohard()
{
	leftBarInPixels = 0;
	mtable = 16;
	ntable = 30;
	numberOfBombs = 99;

	theApp.GetController()->RestartClicked(mtable, ntable, numberOfBombs);
}


void CMVC_MineSweeperDlg::OnBnClickedRadio4()
{
	CString rows;
	CString columns;
	columnTxt.GetWindowTextW(rows);
	rowTxt.GetWindowTextW(columns);

	mtable = _wtoi(rows);
	ntable = _wtoi(columns);

	if (mtable > 16 || ntable > 30 ||
		mtable <= 0 || ntable <= 0)
	{
		mtable = 16;
		ntable = 30;
		numberOfBombs = 99;

		return;
	}
	else
	{
		leftBarInPixels = 300 - (ntable / 2)*20;
		numberOfBombs =(int)((mtable * ntable)/4);
		theApp.GetController()->RestartClicked(mtable, ntable, numberOfBombs);
	}

	
}
