#if !defined(AFX_DLGVIEWLOG_H__7506E1FB_524A_4E27_9819_BADE6D9EF7F4__INCLUDED_)
#define AFX_DLGVIEWLOG_H__7506E1FB_524A_4E27_9819_BADE6D9EF7F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgViewLog dialog

class CDlgViewLog : public CDialog
{
// Construction
public:
	CDlgViewLog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewLog)
	enum { IDD = IDD_DLG_VIEWLOG };
	CListCtrl	m_list;
	CString	m_opr;
	CString	m_op;
	CString	m_date;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewLog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListVllog(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnVlrmall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetDBConnection(_ConnectionPtr cnnt)
	{
		m_DBCnt = cnnt;
	}
	void RefreshData();
protected:
	_ConnectionPtr m_DBCnt;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIEWLOG_H__7506E1FB_524A_4E27_9819_BADE6D9EF7F4__INCLUDED_)
