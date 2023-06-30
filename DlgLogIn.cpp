// DlgLogIn.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgLogIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLogIn dialog


CDlgLogIn::CDlgLogIn(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLogIn)
	m_UsrName = _T("");
	m_UsrPwd = _T("");
	//}}AFX_DATA_INIT
}


void CDlgLogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLogIn)
	DDX_Text(pDX, IDC_EDIT_USRNAME, m_UsrName);
	DDX_Text(pDX, IDC_EDIT_USRPWD, m_UsrPwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLogIn, CDialog)
	//{{AFX_MSG_MAP(CDlgLogIn)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLogIn message handlers
