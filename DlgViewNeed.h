#if !defined(AFX_DLGVIEWNEED_H__65527284_1352_43E5_AFCE_938CFD67EADE__INCLUDED_)
#define AFX_DLGVIEWNEED_H__65527284_1352_43E5_AFCE_938CFD67EADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewNeed.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgViewNeed dialog

class CDlgViewNeed : public CDialog
{
// Construction
public:
	CDlgViewNeed(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewNeed)
	enum { IDD = IDD_DLG_INFONEED };
	CListCtrl	m_list;
	CString	m_code;
	CString	m_dept;
	CString	m_end;
	CString	m_number;
	CString	m_start;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewNeed)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewNeed)
	afx_msg void OnBtnVndel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListVn(NMHDR* pNMHDR, LRESULT* pResult);
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

#endif // !defined(AFX_DLGVIEWNEED_H__65527284_1352_43E5_AFCE_938CFD67EADE__INCLUDED_)
