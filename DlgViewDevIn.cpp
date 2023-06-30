// DlgViewDevIn.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgViewDevIn.h"
#include "dataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// EndAddNewH

/////////////////////////////////////////////////////////////////////////////
// CDlgViewDevIn dialog


CDlgViewDevIn::CDlgViewDevIn(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewDevIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewDevIn)
	m_tel = _T("");
	m_provider = _T("");
	m_date = _T("");
	m_code = _T("");
	m_buyer = _T("");
	m_price = 0.0f;
	m_number = 0.0f;
	//}}AFX_DATA_INIT

	m_DBCnt = NULL;
	m_log = NULL;
}


void CDlgViewDevIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewDevIn)
	DDX_Control(pDX, IDC_LIST_VIEWDEVIN, m_list);
	DDX_Text(pDX, IDC_EDIT_VITEL, m_tel);
	DDX_Text(pDX, IDC_EDIT_VIPROVIDER, m_provider);
	DDX_Text(pDX, IDC_EDIT_VIDATE, m_date);
	DDX_Text(pDX, IDC_EDIT_VICODE, m_code);
	DDX_Text(pDX, IDC_EDIT_VIBUYER, m_buyer);
	DDX_Text(pDX, IDC_EDIT_VIPRICE, m_price);
	DDV_MinMaxFloat(pDX, m_price, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_VINUMBER, m_number);
	DDV_MinMaxFloat(pDX, m_number, 1.f, 10000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewDevIn, CDialog)
	//{{AFX_MSG_MAP(CDlgViewDevIn)
	ON_BN_CLICKED(IDC_BTN_VIUPDATE, OnBtnViupdate)
	ON_BN_CLICKED(IDC_BTN_VIDEL, OnBtnVidel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VIEWDEVIN, OnClickListViewdevin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewDevIn message handlers

void CDlgViewDevIn::OnBtnViupdate() 
{
	if (!UpdateData())
		return;

	CString sql_;
	sql_.Format(CString("SELECT * FROM DEVICE_IN WHERE in_date=")+DATEFMT, m_date);
	_bstr_t sql = sql_;

	_RecordsetPtr pRst = NULL;
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevInRs rs;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));
		
		pRst->Open(sql,_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdText);
      		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		strcpy(rs.m_sz_code,m_code);
		strcpy(rs.m_sz_date,m_date);
		strcpy(rs.m_sz_provider, m_provider);
		strcpy(rs.m_sz_tel, m_tel);
		rs.m_f_number = m_number;
		rs.m_f_price = m_price;
		strcpy(rs.m_sz_buyer, m_buyer);

		TESTHR(picRs->Update(&rs));

		picRs->Release();
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
		return;
    } 

	MessageBox("完成操作！");
	m_log->AddLog("修改入库信息");

	RefreshData();
}

void CDlgViewDevIn::OnBtnVidel() 
{
	if (!UpdateData())
		return;

	CString sql_;
	sql_.Format(CString("DELETE FROM DEVICE_IN WHERE in_date=")+DATEFMT, m_date);
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
	m_log->AddLog("删除入库信息");

	RefreshData();
}

BOOL CDlgViewDevIn::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.InsertColumn(0,"设备号");
	m_list.InsertColumn(1,"入库时间");
	m_list.InsertColumn(2,"供应商");
	m_list.InsertColumn(3,"供应商电话");
	m_list.InsertColumn(4,"数量");
	m_list.InsertColumn(5,"价格");
	m_list.InsertColumn(6,"采购员");
	
	RECT rect;
	m_list.GetWindowRect(&rect);
	int wid = rect.right - rect.left;
	m_list.SetColumnWidth(0,wid/8);
	m_list.SetColumnWidth(1,wid*3/16);
	m_list.SetColumnWidth(2,wid/8);
	m_list.SetColumnWidth(3,wid*3/16);
	m_list.SetColumnWidth(4,wid/8);
	m_list.SetColumnWidth(5,wid/8);
	m_list.SetColumnWidth(6,wid/8);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	RefreshData();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgViewDevIn::RefreshData()
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);	

	_RecordsetPtr pRst = NULL;
	
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevInRs rs;
	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE_IN");

		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst = m_DBCnt->Execute(strSQL, NULL, adCmdText);
		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		int i = 0;
		char buf[64];
		while (!pRst->adoEOF)
		{
			m_list.InsertItem(0, rs.m_sz_code);
			m_list.SetItemText(i, 1, rs.m_sz_date);
			m_list.SetItemText(i, 2, rs.m_sz_provider);
			m_list.SetItemText(i, 3, rs.m_sz_tel);
			sprintf(buf, "%f", rs.m_f_number); m_list.SetItemText(i, 4, buf);
			sprintf(buf, "%f", rs.m_f_price); m_list.SetItemText(i, 5, buf);
			m_list.SetItemText(i, 6, rs.m_sz_buyer);

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

void CDlgViewDevIn::OnClickListViewdevin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_list.GetSelectionMark();

	m_code = m_list.GetItemText(i,0);
	m_date = m_list.GetItemText(i,1);
	m_provider = m_list.GetItemText(i,2);
	m_tel = m_list.GetItemText(i,3);
	m_number = (float)atof(m_list.GetItemText(i,4));
	m_price = (float)atof(m_list.GetItemText(i,5));
	m_buyer = m_list.GetItemText(i,6);
	
	UpdateData(FALSE);
	
	*pResult = 0;
}
