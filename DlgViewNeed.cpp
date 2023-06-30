// DlgViewNeed.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgViewNeed.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewNeed dialog


CDlgViewNeed::CDlgViewNeed(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewNeed::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewNeed)
	m_code = _T("");
	m_dept = _T("");
	m_end = _T("");
	m_number = _T("");
	m_start = _T("");
	//}}AFX_DATA_INIT
}


void CDlgViewNeed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewNeed)
	DDX_Control(pDX, IDC_LIST_VN, m_list);
	DDX_Text(pDX, IDC_EDIT_VNCODE, m_code);
	DDX_Text(pDX, IDC_EDIT_VNDEPT, m_dept);
	DDX_Text(pDX, IDC_EDIT_VNEND, m_end);
	DDX_Text(pDX, IDC_EDIT_VNNUMBER, m_number);
	DDX_Text(pDX, IDC_EDIT_VNSTART, m_start);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewNeed, CDialog)
	//{{AFX_MSG_MAP(CDlgViewNeed)
	ON_BN_CLICKED(IDC_BTN_VNDEL, OnBtnVndel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VN, OnClickListVn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewNeed message handlers

BOOL CDlgViewNeed::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.InsertColumn(0,"设备号");
	m_list.InsertColumn(1,"需求部门");
	m_list.InsertColumn(2,"开始时间");
	m_list.InsertColumn(3,"结束时间");
	m_list.InsertColumn(4,"数量");
	
	RECT rect;
	m_list.GetWindowRect(&rect);
	int wid = rect.right - rect.left;
	m_list.SetColumnWidth(0,wid/5);
	m_list.SetColumnWidth(1,wid/5);
	m_list.SetColumnWidth(2,wid/5);
	m_list.SetColumnWidth(3,wid/5);
	m_list.SetColumnWidth(4,wid/5);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	RefreshData();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgViewNeed::RefreshData()
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);	
	
	_RecordsetPtr pRst = NULL;
	
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevNeedRs rs;
	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE_NEED");

		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst = m_DBCnt->Execute(strSQL, NULL, adCmdText);
		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		int i = 0;
		while (!pRst->adoEOF)
		{
			m_list.InsertItem(0, rs.m_sz_code);
			m_list.SetItemText(i, 1, rs.m_sz_dept);
			m_list.SetItemText(i, 2, rs.m_sz_start);
			m_list.SetItemText(i, 3, rs.m_sz_end);
			m_list.SetItemText(i, 4, rs.m_sz_number);

			pRst->MoveNext();
		}
		picRs->Release();
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
		m_list.SetRedraw(TRUE);	
		return;
    } 

	m_list.SetRedraw(TRUE);	
}

void CDlgViewNeed::OnClickListVn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_list.GetSelectionMark();

	m_code = m_list.GetItemText(i,0);
	m_dept = m_list.GetItemText(i,1);
	m_start = m_list.GetItemText(i,2);
	m_end = m_list.GetItemText(i,3);
	m_number = m_list.GetItemText(i,4);
	
	UpdateData(FALSE);
	
	*pResult = 0;
}

void CDlgViewNeed::OnBtnVndel() 
{
	CString sql_;
	sql_.Format("DELETE FROM DEVICE_NEED WHERE code='%s' AND department='%s'",
		m_code, m_dept);
	_bstr_t sql = sql_;

 	try
	{
		m_DBCnt->Execute(sql, NULL, adCmdText);
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
		return;
    } 
	MessageBox("完成操作！");
	m_log->AddLog("删除需求信息");

	RefreshData();
}
