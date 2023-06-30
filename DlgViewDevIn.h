#if !defined(AFX_DLGVIEWDEVIN_H__DE35698D_0724_4C77_9C19_0A5173D420ED__INCLUDED_)
#define AFX_DLGVIEWDEVIN_H__DE35698D_0724_4C77_9C19_0A5173D420ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewDevIn.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgViewDevIn dialog

class CDlgViewDevIn : public CDialog
{
// Construction
public:
	CDlgViewDevIn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewDevIn)
	enum { IDD = IDD_DLG_INFOIN };
	CListCtrl	m_list;
	CString	m_tel;
	CString	m_provider;
	CString	m_date;
	CString	m_code;
	CString	m_buyer;
	float	m_price;
	float	m_number;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewDevIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewDevIn)
	afx_msg void OnBtnViupdate();
	afx_msg void OnBtnVidel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListViewdevin(NMHDR* pNMHDR, LRESULT* pResult);
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

#endif // !defined(AFX_DLGVIEWDEVIN_H__DE35698D_0724_4C77_9C19_0A5173D420ED__INCLUDED_)
