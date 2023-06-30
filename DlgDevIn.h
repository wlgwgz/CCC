#if !defined(AFX_DLGDEVIN_H__5AF24C6E_9A89_4130_89AC_7447F6FA0EDE__INCLUDED_)
#define AFX_DLGDEVIN_H__5AF24C6E_9A89_4130_89AC_7447F6FA0EDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDevIn.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDevIn dialog

class CDlgDevIn : public CDialog
{
// Construction
public:
	CDlgDevIn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDevIn)
	enum { IDD = IDD_DLG_DEVIN };
	CComboBox	m_devs;
	CTime	m_date;
	CTime	m_time;
	CString	m_buyer;
	CString	m_provider;
	CString	m_tel;
	float	m_number;
	float	m_price;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDevIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDevIn)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnDiadd();
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

#endif // !defined(AFX_DLGDEVIN_H__5AF24C6E_9A89_4130_89AC_7447F6FA0EDE__INCLUDED_)
