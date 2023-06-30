// DlgDevcode.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgDevcode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDevcode dialog


CDlgDevcode::CDlgDevcode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevcode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDevcode)
	m_code = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT
	m_DBCnt = NULL;
	m_log = NULL;
}


void CDlgDevcode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDevcode)
	DDX_Control(pDX, IDC_LIST_DEVCODE, m_list);
	DDX_Text(pDX, IDC_EDIT_DCCODE, m_code);
	DDX_Text(pDX, IDC_EDIT_DCNAME, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDevcode, CDialog)
	//{{AFX_MSG_MAP(CDlgDevcode)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DEVCODE, OnClickListDevcode)
	ON_BN_CLICKED(IDC_BTN_DCADD, OnBtnDcadd)
	ON_BN_CLICKED(IDC_BTN_DCDEL, OnBtnDcdel)
	ON_BN_CLICKED(IDC_BTN_DCUPD, OnBtnDcupd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDevcode message handlers

void CDlgDevcode::RefreshData()
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);

	_bstr_t strSQL("SELECT * FROM DEVICE_CODE");
	_RecordsetPtr MySet; 

	int i = 0;
	try
	{
		MySet.CreateInstance(__uuidof(Recordset)); 
		MySet = m_DBCnt->Execute(strSQL,NULL,adCmdText);
		_variant_t Holder;
		while(!MySet->adoEOF)
	    { 
		    Holder = MySet->GetCollect("code");
			if(Holder.vt!=VT_NULL)
				m_list.InsertItem(i, (char*)(_bstr_t)Holder);

		    Holder = MySet->GetCollect("name");
			if(Holder.vt!=VT_NULL)
				m_list.SetItemText(i, 1, (char*)(_bstr_t)Holder);
			MySet->MoveNext(); 
		} 
	}
	catch(_com_error& e)
    {
        AfxMessageBox(e.ErrorMessage());
		m_list.SetRedraw(TRUE);	
		return;
    }                       
	m_list.SetRedraw(TRUE);	
}

BOOL CDlgDevcode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.InsertColumn(0,"设备号");
	m_list.InsertColumn(1,"设备名");
	
	RECT rect;
	m_list.GetWindowRect(&rect);
	int wid = rect.right - rect.left;
	m_list.SetColumnWidth(0,wid/2);
	m_list.SetColumnWidth(1,wid/2);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	RefreshData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDevcode::OnClickListDevcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_list.GetSelectionMark();

	m_code = m_list.GetItemText(i,0);
	m_name = m_list.GetItemText(i,1);

	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgDevcode::OnBtnDcadd() 
{
	UpdateData();

	CString sql_;
	sql_.Format("INSERT INTO DEVICE_CODE (code,name) VALUES('%s','%s')", m_code, m_name);
	_bstr_t sql = sql_;

	try
	{
		m_DBCnt->Execute(sql,NULL,adCmdText);
	}
	catch(_com_error& e)
    {
        AfxMessageBox(e.ErrorMessage());
		return;
    }       
	
	m_log->AddLog("添加设备记录。");
      
	RefreshData();            
}

void CDlgDevcode::OnBtnDcdel() 
{
	UpdateData();

	CString sql_;
	sql_.Format("DELETE FROM DEVICE_CODE WHERE CODE='%s'",m_code);
	_bstr_t sql = sql_;
	try
	{
		m_DBCnt->Execute(sql,NULL,adCmdText);
	} 
	catch(_com_error& e)
    {
        AfxMessageBox(e.ErrorMessage());
		return;
    }                               
	m_log->AddLog("删除设备记录。");
      
	RefreshData();            
}

void CDlgDevcode::OnBtnDcupd() 
{
	UpdateData();

	CString sql_;
	sql_.Format("UPDATE DEVICE_CODE SET NAME='%s' WHERE CODE='%s'",m_name,m_code);
	_bstr_t sql = sql_;

	try
	{
		m_DBCnt->Execute(sql,NULL,adCmdText);
	} 
	catch(_com_error& e)
    {
        AfxMessageBox(e.ErrorMessage());
		return;
    }                  
	
	m_log->AddLog("更新设备记录。");      
	RefreshData();            
}
