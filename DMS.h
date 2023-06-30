// DMS.h : main header file for the DMS application
//

#if !defined(AFX_DMS_H__6D68D8CC_2416_4218_8732_D1FECEAD2E7A__INCLUDED_)
#define AFX_DMS_H__6D68D8CC_2416_4218_8732_D1FECEAD2E7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDMSApp:
// See DMS.cpp for the implementation of this class
//

class CDMSApp : public CWinApp
{
public:
	CDMSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDMSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMS_H__6D68D8CC_2416_4218_8732_D1FECEAD2E7A__INCLUDED_)
