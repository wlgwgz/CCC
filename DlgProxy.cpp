// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgProxy.h"
#include "DMSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMSDlgAutoProxy

IMPLEMENT_DYNCREATE(CDMSDlgAutoProxy, CCmdTarget)

CDMSDlgAutoProxy::CDMSDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CDMSDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CDMSDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CDMSDlgAutoProxy::~CDMSDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CDMSDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CDMSDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CDMSDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDMSDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDMSDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDMS to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {081EFDD7-5C34-4188-942F-52C8E73314B2}
static const IID IID_IDMS =
{ 0x81efdd7, 0x5c34, 0x4188, { 0x94, 0x2f, 0x52, 0xc8, 0xe7, 0x33, 0x14, 0xb2 } };

BEGIN_INTERFACE_MAP(CDMSDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CDMSDlgAutoProxy, IID_IDMS, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {0B38DA47-D883-4900-AAEE-208EB41D0695}
IMPLEMENT_OLECREATE2(CDMSDlgAutoProxy, "DMS.Application", 0xb38da47, 0xd883, 0x4900, 0xaa, 0xee, 0x20, 0x8e, 0xb4, 0x1d, 0x6, 0x95)

/////////////////////////////////////////////////////////////////////////////
// CDMSDlgAutoProxy message handlers
