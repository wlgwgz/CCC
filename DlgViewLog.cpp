// DlgViewLog.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgViewLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewLog dialog


CDlgViewLog::CDlgViewLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewLog)
	m_opr = _T("");
	m_op = _T("");
	m_date = _T("");
	//}}AFX_DATA_INIT
}


void CDlgViewLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewLog)
	DDX_Control(pDX, IDC_LIST_VLLOG, m_list);
	DDX_Text(pDX, IDC_EDIT_VLOPR, m_opr);
	DDX_Text(pDX, IDC_EDIT_VLOP, m_op);
	DDX_Text(pDX, IDC_EDIT_VLDATE, m_date);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewLog, CDialog)
	//{{AFX_MSG_MAP(CDlgViewLog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VLLOG, OnClickListVllog)
	ON_BN_CLICKED(IDC_BTN_VLRMALL, OnBtnVlrmall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewLog message handlers

BOOL CDlgViewLog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	m_list.InsertColumn(0,"操作员");
	m_list.InsertColumn(1,"操作日期");
	m_list.InsertColumn(2,"操作内容");
	
	RECT rect;
	m_list.GetWindowRect(&rect);
	int wid = rect.right - rect.left;
	m_list.SetColumnWidth(0,wid/3);
	m_list.SetColumnWidth(1,wid/3);
	m_list.SetColumnWidth(2,wid/3);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	RefreshData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgViewLog::RefreshData()
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);

	_bstr_t strSQL("SELECT * FROM HOWDO");
	_RecordsetPtr MySet; 

	int i = 0;
	try
	{
		MySet.CreateInstance(__uuidof(Recordset)); 
		MySet = m_DBCnt->Execute(strSQL,NULL,adCmdText);
		_variant_t Holder;
		while(!MySet->adoEOF)
	    { 
		    Holder = MySet->GetCollect("do_user");
			if(Holder.vt!=VT_NULL)
				m_list.InsertItem(i, (char*)(_bstr_t)Holder);

			Holder = MySet->GetCollect("do_date");
			if (Holder.vt!=VT_NULL)
				m_list.SetItemText(i, 1, (char*)(_bstr_t)Holder);

		    Holder = MySet->GetCollect("do_what");
			if(Holder.vt!=VT_NULL)
				m_list.SetItemText(i, 2, (char*)(_bstr_t)Holder);

			MySet->MoveNext(); 
		} 
		MySet->Close();
	}
	catch(_com_error& e)
    {
        AfxMessageBox(e.ErrorMessage());
		m_list.SetRedraw(TRUE);	
		return;
    }                       
	m_list.SetRedraw(TRUE);	
}

void CDlgViewLog::OnClickListVllog(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_list.GetSelectionMark();

	m_opr = m_list.GetItemText(i,0);
	m_date = m_list.GetItemText(i,1);
	m_op =  m_list.GetItemText(i,2);

	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgViewLog::OnBtnVlrmall() 
{
	_bstr_t strSQL("TRUNCATE TABLE HOWDO");

	try
	{
		m_DBCnt->Execute(strSQL,NULL,adCmdText);
	}
	catch(_com_error& e)
    {
        AfxMessageBox(e.ErrorMessage());
		EndDialog(0);
    }                       		
	RefreshData();
}
