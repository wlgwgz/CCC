#if !defined(AFX_DLGDEVOUT_H__BA083AA7_8A40_443F_BBD8_C440625C5973__INCLUDED_)
#define AFX_DLGDEVOUT_H__BA083AA7_8A40_443F_BBD8_C440625C5973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDevOut.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDevOut dialog

class CDlgDevOut : public CDialog
{
// Construction
public:
	CDlgDevOut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDevOut)
	enum { IDD = IDD_DLG_DEVOUT };
	CComboBox	m_devs;
	CTime	m_date;
	CTime	m_time;
	CString	m_dept;
	CString	m_keeper;
	float	m_number;
	CString	m_taker;
	CString	m_usage;
	UINT	m_state;
	float	m_max;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDevOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDevOut)
	afx_msg void OnBtnDoadd();
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseupComboDodevs();
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

#endif // !defined(AFX_DLGDEVOUT_H__BA083AA7_8A40_443F_BBD8_C440625C5973__INCLUDED_)
