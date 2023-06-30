// DMSDlg.h : header file
//

#if !defined(AFX_DMSDLG_H__252D258E_23AC_4ED8_BE6B_F865186332FB__INCLUDED_)
#define AFX_DMSDLG_H__252D258E_23AC_4ED8_BE6B_F865186332FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogMngr.h"

class CDMSDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CDMSDlg dialog

class CDMSDlg : public CDialog
{
	DECLARE_DYNAMIC(CDMSDlg);
	friend class CDMSDlgAutoProxy;

// Construction
public:
	CDMSDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CDMSDlg();

// Dialog Data
	//{{AFX_DATA(CDMSDlg)
	enum { IDD = IDD_DMS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDMSDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CDMSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnBuydev();
	afx_msg void OnBtnDevalert();
	afx_msg void OnBtnDevcode();
	afx_msg void OnBtnDevin();
	afx_msg void OnBtnDevout();
	afx_msg void OnBtnDevret();
	afx_msg void OnBtnInfodev();
	afx_msg void OnBtnInfoin();
	afx_msg void OnBtnInfoneed();
	afx_msg void OnBtnInfoout();
	afx_msg void OnBtnInforet();
	afx_msg void OnBtnNeed();
	afx_msg void OnBtnViewlog();
	afx_msg void OnBtnViewprt();
	afx_msg void OnDestroy();
	afx_msg void OnBtnAbout();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	_ConnectionPtr m_DBCnt;
	CLogMngr m_logMngr;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMSDLG_H__252D258E_23AC_4ED8_BE6B_F865186332FB__INCLUDED_)
