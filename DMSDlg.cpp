// DMSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DMSDlg.h"
#include "DlgProxy.h"
#include "DlgLogIn.h"
#include "DlgDevcode.h"
#include "DlgReport.h"
#include "DlgViewLog.h"
#include "DlgViewDevIn.h"
#include "DlgViewRet.h"
#include "DlgViewOut.h"
#include "DlgViewNeed.h"
#include "DlgViewReport.h"
#include "DlgDev.h"
#include "DlgDevAlert.h"
#include "DlgDevIn.h"
#include "DlgDevOut.h"
#include "DlgDevRet.h"
#include "DlgDevNeed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMSDlg dialog

IMPLEMENT_DYNAMIC(CDMSDlg, CDialog);

CDMSDlg::CDMSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDMSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDMSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CDMSDlg::~CDMSDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CDMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDMSDlg, CDialog)
	//{{AFX_MSG_MAP(CDMSDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_BUYDEV, OnBtnBuydev)
	ON_BN_CLICKED(IDC_BTN_DEVALERT, OnBtnDevalert)
	ON_BN_CLICKED(IDC_BTN_DEVCODE, OnBtnDevcode)
	ON_BN_CLICKED(IDC_BTN_DEVIN, OnBtnDevin)
	ON_BN_CLICKED(IDC_BTN_DEVOUT, OnBtnDevout)
	ON_BN_CLICKED(IDC_BTN_DEVRET, OnBtnDevret)
	ON_BN_CLICKED(IDC_BTN_INFODEV, OnBtnInfodev)
	ON_BN_CLICKED(IDC_BTN_INFOIN, OnBtnInfoin)
	ON_BN_CLICKED(IDC_BTN_INFONEED, OnBtnInfoneed)
	ON_BN_CLICKED(IDC_BTN_INFOOUT, OnBtnInfoout)
	ON_BN_CLICKED(IDC_BTN_INFORET, OnBtnInforet)
	ON_BN_CLICKED(IDC_BTN_NEED, OnBtnNeed)
	ON_BN_CLICKED(IDC_BTN_VIEWLOG, OnBtnViewlog)
	ON_BN_CLICKED(IDC_BTN_VIEWPRT, OnBtnViewprt)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMSDlg message handlers

BOOL CDMSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	/**
	 *  在这里弹出用户登录对话框
	*/
	CDlgLogIn dlg;
	do
	{
		if (!dlg.DoModal())
			EndDialog(0);
	} while (dlg.m_UsrName.GetLength()==0);

	/**
	 *  登录数据库，若失败，则关闭程序。
	*/
	try
	{
		m_DBCnt.CreateInstance(__uuidof(Connection));
		CString sql_;
		sql_.Format("DSN=DMS;UID=%s;PWD=%s",dlg.m_UsrName,dlg.m_UsrPwd);
		_bstr_t sql=sql_;
		m_DBCnt->Open(sql,"","",-1);

		m_logMngr.Setup(m_DBCnt,dlg.m_UsrName);
		m_logMngr.AddLog("登录数据库");
	}
	catch(_com_error& e)
    {
		AfxMessageBox(e.ErrorMessage());
		this->EndDialog(0);
    }                                   

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDMSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDMSDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDMSDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CDMSDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CDMSDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CDMSDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CDMSDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CDMSDlg::OnBtnBuydev() 
{
	CDlgReport dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnDevalert() 
{
	CDlgDevAlert dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnDevcode() 
{
	CDlgDevcode dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnDevin() 
{
	CDlgDevIn dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);	
}

void CDMSDlg::OnBtnDevout() 
{
	CDlgDevOut dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);	
}

void CDMSDlg::OnBtnDevret() 
{
	CDlgDevRet dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);		
}

void CDMSDlg::OnBtnInfodev() 
{
	CDlgDev dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);		
}

void CDMSDlg::OnBtnInfoin() 
{
	CDlgViewDevIn dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnInfoneed() 
{
	CDlgViewNeed dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnInfoout() 
{
	CDlgViewOut dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnInforet() 
{
	CDlgViewRet dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnNeed() 
{
	CDlgDevNeed dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnViewlog() 
{
	CDlgViewLog dlg;
	dlg.SetDBConnection(m_DBCnt);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnBtnViewprt() 
{
	CDlgViewReport dlg;
	dlg.Setup(m_DBCnt,&m_logMngr);
	this->ShowWindow(SW_HIDE);
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CDMSDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_DBCnt->Close();	
}

void CDMSDlg::OnBtnAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CDMSDlg::OnBtnHelp() 
{
	WinExec("hh.exe dms.chm",SW_SHOW);
}
