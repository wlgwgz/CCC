#if !defined(AFX_DLGDEVALERT_H__B07E82D0_58A6_4AB5_B4E5_70A13127D838__INCLUDED_)
#define AFX_DLGDEVALERT_H__B07E82D0_58A6_4AB5_B4E5_70A13127D838__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDevAlert.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDevAlert dialog

class CDlgDevAlert : public CDialog
{
// Construction
public:
	CDlgDevAlert(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDevAlert)
	enum { IDD = IDD_DLG_DEVALERT };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDevAlert)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDevAlert)
	afx_msg void OnBtnDlall();
	afx_msg void OnBtnDlover();
	afx_msg void OnBtnDlbelow();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Setup(_ConnectionPtr cnnt,CLogMngr* pl)
	{
		m_DBCnt = cnnt;
		m_log = pl;
	}
protected:
	void SearchBelow();
	void SearchOver();
protected:
	_ConnectionPtr m_DBCnt;
	CLogMngr* m_log;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEVALERT_H__B07E82D0_58A6_4AB5_B4E5_70A13127D838__INCLUDED_)
