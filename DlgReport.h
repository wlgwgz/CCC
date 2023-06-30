#if !defined(AFX_DLGREPORT_H__7C7784EF_610B_4FEE_9FF7_B68A3DEAE536__INCLUDED_)
#define AFX_DLGREPORT_H__7C7784EF_610B_4FEE_9FF7_B68A3DEAE536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReport.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgReport dialog

class CDlgReport : public CDialog
{
// Construction
public:
	CDlgReport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgReport)
	enum { IDD = IDD_DLG_REPORT };
	CComboBox	m_devs;
	CTime	m_date;
	CTime	m_time;
	float	m_buy;
	float	m_cur;
	float	m_max;
	float	m_price;
	CString	m_name;
	CString	m_provider;
	float	m_total;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgReport)
	afx_msg void OnBtnPrint();
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseupComboRpdevs();
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

#endif // !defined(AFX_DLGREPORT_H__7C7784EF_610B_4FEE_9FF7_B68A3DEAE536__INCLUDED_)
