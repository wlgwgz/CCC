// LogMngr.cpp: implementation of the CLogMngr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DMS.h"
#include "LogMngr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogMngr::CLogMngr()
{
}

CLogMngr::~CLogMngr()
{
}

bool CLogMngr::AddLog(LPCSTR op)
{
	CTime tm = CTime::GetCurrentTime();
	CString sql_;

	sql_.Format("INSERT INTO HOWDO (do_user,do_what,do_date) VALUES('%s','%s','%d-%d-%d %d:%d:%d')",
		m_user, op, 
		tm.GetYear(), tm.GetMonth(), tm.GetDay(),
		tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	_bstr_t sql = sql_;

	try
	{
		m_DBCnt->Execute(sql,NULL,adCmdText);
	}
	catch(_com_error& e)
    {
        CString Error = e.ErrorMessage();
        AfxMessageBox(e.ErrorMessage());
		return false;
    }                               

	return true;
}

