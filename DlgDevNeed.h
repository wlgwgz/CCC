#if !defined(AFX_DLGDEVNEED_H__35C8EBC5_918B_4450_8A12_7FF6507475ED__INCLUDED_)
#define AFX_DLGDEVNEED_H__35C8EBC5_918B_4450_8A12_7FF6507475ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDevNeed.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDevNeed dialog

class CDlgDevNeed : public CDialog
{
// Construction
public:
	CDlgDevNeed(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDevNeed)
	enum { IDD = IDD_DLG_DEVNEED };
	CComboBox	m_devs;
	CTime	m_end;
	CTime	m_start;
	CString	m_dept;
	float	m_number;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDevNeed)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDevNeed)
	afx_msg void OnBtnDnadd();
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
	_ConnectionPtr m_DBCnt;
	CLogMngr* m_log;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEVNEED_H__35C8EBC5_918B_4450_8A12_7FF6507475ED__INCLUDED_)
