// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__F188987B_6ECC_4A0A_BE86_64162C4673A2__INCLUDED_)
#define AFX_DLGPROXY_H__F188987B_6ECC_4A0A_BE86_64162C4673A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDMSDlg;

/////////////////////////////////////////////////////////////////////////////
// CDMSDlgAutoProxy command target

class CDMSDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CDMSDlgAutoProxy)

	CDMSDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CDMSDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMSDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDMSDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CDMSDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CDMSDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CDMSDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__F188987B_6ECC_4A0A_BE86_64162C4673A2__INCLUDED_)
