#if !defined(AFX_DLGDEVCODE_H__E04A2477_2F0F_43B8_9971_F4981AE639AA__INCLUDED_)
#define AFX_DLGDEVCODE_H__E04A2477_2F0F_43B8_9971_F4981AE639AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDevcode.h : header file
//

#include "LogMngr.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDevcode dialog

class CDlgDevcode : public CDialog
{
// Construction
public:
	CDlgDevcode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDevcode)
	enum { IDD = IDD_DLG_DEVCODE };
	CListCtrl	m_list;
	CString	m_code;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDevcode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDevcode)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDevcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnDcadd();
	afx_msg void OnBtnDcdel();
	afx_msg void OnBtnDcupd();
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

#endif // !defined(AFX_DLGDEVCODE_H__E04A2477_2F0F_43B8_9971_F4981AE639AA__INCLUDED_)
