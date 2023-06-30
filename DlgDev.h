#if !defined(AFX_DLGDEV_H__C10EDDFA_255E_4222_B9FD_A93497741374__INCLUDED_)
#define AFX_DLGDEV_H__C10EDDFA_255E_4222_B9FD_A93497741374__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDev.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDev dialog

class CDlgDev : public CDialog
{
// Construction
public:
	CDlgDev(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDev)
	enum { IDD = IDD_DLG_DEV };
	CComboBox	m_devs;
	CListCtrl	m_list;
	float	m_cur;
	float	m_max;
	float	m_min;
	float	m_total;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDev)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDev)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDev(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnDevadd();
	afx_msg void OnBtnDevdel();
	afx_msg void OnBtnDevupd();
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

#endif // !defined(AFX_DLGDEV_H__C10EDDFA_255E_4222_B9FD_A93497741374__INCLUDED_)
