#if !defined(AFX_DLGVIEWRET_H__13AD81D5_BCD8_42C5_AA9E_B217E3F6553A__INCLUDED_)
#define AFX_DLGVIEWRET_H__13AD81D5_BCD8_42C5_AA9E_B217E3F6553A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewRet.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgViewRet dialog

class CDlgViewRet : public CDialog
{
// Construction
public:
	CDlgViewRet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewRet)
	enum { IDD = IDD_DLG_INFORET };
	CListCtrl	m_list;
	CString	m_code;
	CString	m_date;
	CString	m_keeper;
	float	m_number;
	CString	m_reter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewRet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewRet)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListVr(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnVrupdt();
	afx_msg void OnBtnVrdel();
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

#endif // !defined(AFX_DLGVIEWRET_H__13AD81D5_BCD8_42C5_AA9E_B217E3F6553A__INCLUDED_)
