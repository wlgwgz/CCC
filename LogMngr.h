// LogMngr.h: interface for the CLogMngr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMNGR_H__B99C1212_779F_4312_AD2B_B9AAC968AE69__INCLUDED_)
#define AFX_LOGMNGR_H__B99C1212_779F_4312_AD2B_B9AAC968AE69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogMngr  
{
public:
	CLogMngr();
	virtual ~CLogMngr();
public:
	bool AddLog(LPCSTR op);
	void Setup(_ConnectionPtr cnnt, CString& user)
	{
		m_DBCnt = cnnt;
		m_user = user;
	}
protected:
	_ConnectionPtr m_DBCnt;
	CString m_user;
};

#endif // !defined(AFX_LOGMNGR_H__B99C1212_779F_4312_AD2B_B9AAC968AE69__INCLUDED_)
