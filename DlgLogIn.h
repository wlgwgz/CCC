#if !defined(AFX_DLGLOGIN_H__9B61C8E7_EAFE_4390_A0E5_14B99E66A8B8__INCLUDED_)
#define AFX_DLGLOGIN_H__9B61C8E7_EAFE_4390_A0E5_14B99E66A8B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLogIn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLogIn dialog

class CDlgLogIn : public CDialog
{
// Construction
public:
	CDlgLogIn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLogIn)
	enum { IDD = IDD_DLG_LOGIN };
	CString	m_UsrName;
	CString	m_UsrPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLogIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLogIn)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGIN_H__9B61C8E7_EAFE_4390_A0E5_14B99E66A8B8__INCLUDED_)
