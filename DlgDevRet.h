#if !defined(AFX_DLGDEVRET_H__2796C197_2A14_4784_AD27_A48E2592CC82__INCLUDED_)
#define AFX_DLGDEVRET_H__2796C197_2A14_4784_AD27_A48E2592CC82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDevRet.h : header file
//

#include "LogMngr.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgDevRet dialog

class CDlgDevRet : public CDialog
{
// Construction
public:
	CDlgDevRet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDevRet)
	enum { IDD = IDD_DLG_DEVRET };
	CComboBox	m_devs;
	CTime	m_date;
	CTime	m_time;
	CString	m_keeper;
	CString	m_reter;
	float	m_number;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDevRet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDevRet)
	afx_msg void OnBtnDradd();
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

#endif // !defined(AFX_DLGDEVRET_H__2796C197_2A14_4784_AD27_A48E2592CC82__INCLUDED_)
