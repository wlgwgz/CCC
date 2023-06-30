#if !defined(AFX_DLGVIEWOUT_H__4CD3E9D0_67B1_487E_9574_AE6F77A54035__INCLUDED_)
#define AFX_DLGVIEWOUT_H__4CD3E9D0_67B1_487E_9574_AE6F77A54035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewOut.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgViewOut dialog

class CDlgViewOut : public CDialog
{
// Construction
public:
	CDlgViewOut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewOut)
	enum { IDD = IDD_DLG_INFOOUT };
	CListCtrl	m_list;
	CString	m_code;
	CString	m_date;
	CString	m_dept;
	CString	m_keeper;
	CString	m_taker;
	CString	m_usage;
	float	m_number;
	UINT	m_state;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewOut)
	afx_msg void OnClickListVo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnVodel();
	afx_msg void OnBtnVoupd();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void RefreshData();
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

#endif // !defined(AFX_DLGVIEWOUT_H__4CD3E9D0_67B1_487E_9574_AE6F77A54035__INCLUDED_)
