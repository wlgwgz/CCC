// DlgViewReport.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgViewReport.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewReport dialog


CDlgViewReport::CDlgViewReport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewReport)
	m_code = _T("");
	m_date = _T("");
	m_buy = _T("");
	m_cur = _T("");
	m_max = _T("");
	m_price = _T("");
	m_provider = _T("");
	m_total = _T("");
	//}}AFX_DATA_INIT
}


void CDlgViewReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewReport)
	DDX_Control(pDX, IDC_LIST_VRP, m_list);
	DDX_Text(pDX, IDC_EDIT_VPRCODE, m_code);
	DDX_Text(pDX, IDC_EDIT_VPRDATE, m_date);
	DDX_Text(pDX, IDC_EDIT_VRPBUY, m_buy);
	DDX_Text(pDX, IDC_EDIT_VRPCUR, m_cur);
	DDX_Text(pDX, IDC_EDIT_VRPMAX, m_max);
	DDX_Text(pDX, IDC_EDIT_VRPPRICE, m_price);
	DDX_Text(pDX, IDC_EDIT_VRPPROVIDER, m_provider);
	DDX_Text(pDX, IDC_EDIT_VRPTOTAL, m_total);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewReport, CDialog)
	//{{AFX_MSG_MAP(CDlgViewReport)
	ON_BN_CLICKED(IDC_BTN_VRPDEL, OnBtnVrpdel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VRP, OnClickListVrp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewReport message handlers

void CDlgViewReport::OnBtnVrpdel() 
{
	if (!UpdateData())
		return;

	CString sql_;
	sql_.Format(CString("DELETE FROM DEVICE_WANTBUY WHERE buy_date=")+DATEFMT, m_date);
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
	m_log->AddLog("删除还库信息");

	RefreshData();
}

BOOL CDlgViewReport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.InsertColumn(0,"设备号");
	m_list.InsertColumn(1,"现有库存");
	m_list.InsertColumn(2,"总库存");
	m_list.InsertColumn(3,"最大库存");
	m_list.InsertColumn(4,"购买数量");
	m_list.InsertColumn(5,"供应商");
	m_list.InsertColumn(6,"价格");
	m_list.InsertColumn(7,"计划时间");
	
	RECT rect;
	m_list.GetWindowRect(&rect);
	int wid = rect.right - rect.left;
	m_list.SetColumnWidth(0,wid/8);
	m_list.SetColumnWidth(1,wid/8);
	m_list.SetColumnWidth(2,wid/8);
	m_list.SetColumnWidth(3,wid/8);
	m_list.SetColumnWidth(4,wid/8);
	m_list.SetColumnWidth(5,wid/8);
	m_list.SetColumnWidth(6,wid/8);
	m_list.SetColumnWidth(7,wid/8);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	RefreshData();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgViewReport::RefreshData()
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);	

	_RecordsetPtr pRst = NULL;
	
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevBuyRs rs;
	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE_WANTBUY");

		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst = m_DBCnt->Execute(strSQL, NULL, adCmdText);
		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		int i = 0;
		while (!pRst->adoEOF)
		{
			m_list.InsertItem(0, rs.m_sz_code);
			m_list.SetItemText(i, 1, rs.m_sz_now);
			m_list.SetItemText(i, 2, rs.m_sz_total);
			m_list.SetItemText(i, 3, rs.m_sz_max);
			m_list.SetItemText(i, 4, rs.m_sz_buy);
			m_list.SetItemText(i, 5, rs.m_sz_provider);
			m_list.SetItemText(i, 6, rs.m_sz_price);
			m_list.SetItemText(i, 7, rs.m_sz_date);

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

void CDlgViewReport::OnClickListVrp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_list.GetSelectionMark();

	m_code = m_list.GetItemText(i,0);
	m_cur = m_list.GetItemText(i,1);
	m_total = m_list.GetItemText(i,2);
	m_max = m_list.GetItemText(i,3);
	m_buy = m_list.GetItemText(i,4);
	m_provider = m_list.GetItemText(i,5);
	m_price = m_list.GetItemText(i,6);
	m_date = m_list.GetItemText(i,7);
	
	UpdateData(FALSE);	
	*pResult = 0;
}
