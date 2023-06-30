#if !defined(AFX_DLGVIEWREPORT_H__2F856C44_648A_4A66_875D_38C5A31BF84D__INCLUDED_)
#define AFX_DLGVIEWREPORT_H__2F856C44_648A_4A66_875D_38C5A31BF84D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewReport.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgViewReport dialog

class CDlgViewReport : public CDialog
{
// Construction
public:
	CDlgViewReport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewReport)
	enum { IDD = IDD_DLG_INFOPRT };
	CListCtrl	m_list;
	CString	m_code;
	CString	m_date;
	CString	m_buy;
	CString	m_cur;
	CString	m_max;
	CString	m_price;
	CString	m_provider;
	CString	m_total;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewReport)
	afx_msg void OnBtnVrpdel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListVrp(NMHDR* pNMHDR, LRESULT* pResult);
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

#endif // !defined(AFX_DLGVIEWREPORT_H__2F856C44_648A_4A66_875D_38C5A31BF84D__INCLUDED_)
