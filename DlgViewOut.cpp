// DlgViewOut.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgViewOut.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewOut dialog


CDlgViewOut::CDlgViewOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewOut)
	m_code = _T("");
	m_date = _T("");
	m_dept = _T("");
	m_keeper = _T("");
	m_taker = _T("");
	m_usage = _T("");
	m_number = 0.0f;
	m_state = 0;
	//}}AFX_DATA_INIT
}


void CDlgViewOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewOut)
	DDX_Control(pDX, IDC_LIST_VO, m_list);
	DDX_Text(pDX, IDC_EDIT_VOCODE, m_code);
	DDX_Text(pDX, IDC_EDIT_VODATE, m_date);
	DDX_Text(pDX, IDC_EDIT_VODEPT, m_dept);
	DDX_Text(pDX, IDC_EDIT_VOKEEPER, m_keeper);
	DDX_Text(pDX, IDC_EDIT_VOTAKER, m_taker);
	DDX_Text(pDX, IDC_EDIT_VOUSAGE, m_usage);
	DDX_Text(pDX, IDC_EDIT_VONUMBER, m_number);
	DDV_MinMaxFloat(pDX, m_number, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_VOSTA, m_state);
	DDV_MinMaxUInt(pDX, m_state, 0, 9);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewOut, CDialog)
	//{{AFX_MSG_MAP(CDlgViewOut)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VO, OnClickListVo)
	ON_BN_CLICKED(IDC_BTN_VODEL, OnBtnVodel)
	ON_BN_CLICKED(IDC_BTN_VOUPD, OnBtnVoupd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewOut message handlers



void CDlgViewOut::OnBtnVodel() 
{
	if (!UpdateData())
		return;

	CString sql_;
	sql_.Format(CString("DELETE FROM DEVICE_OUT WHERE out_date=")+DATEFMT,m_date);
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
	m_log->AddLog("删除出库信息");

	RefreshData();
}

void CDlgViewOut::OnBtnVoupd() 
{
	if (!UpdateData())
		return;

	CString sql_;
	sql_.Format(CString("SELECT * FROM DEVICE_OUT WHERE out_date=")+DATEFMT, m_date);
	_bstr_t sql = sql_;

	_RecordsetPtr pRst = NULL;
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevOutRs rs;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));
		
		pRst->Open(sql,_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdText);
      		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		strcpy(rs.m_sz_code,m_code);
		strcpy(rs.m_sz_dept, m_dept);
		strcpy(rs.m_sz_date,m_date);
		sprintf(rs.m_sz_state, "%d",m_state);
		strcpy(rs.m_sz_keeper, m_keeper);
		rs.m_f_number = m_number;
		strcpy(rs.m_sz_taker, m_taker);
		strcpy(rs.m_sz_usage, m_usage);

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
	m_log->AddLog("修改还库信息");

	RefreshData();	
}

BOOL CDlgViewOut::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.InsertColumn(0,"设备号");
	m_list.InsertColumn(1,"使用部门");
	m_list.InsertColumn(2,"出库时间");
	m_list.InsertColumn(3,"出库状况");
	m_list.InsertColumn(4,"经手人");
	m_list.InsertColumn(5,"出库数量");
	m_list.InsertColumn(6,"领取人");
	m_list.InsertColumn(7,"用途");
	
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

void CDlgViewOut::RefreshData()
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);	

	_RecordsetPtr pRst = NULL;
	
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevOutRs rs;
	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE_out");

		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst = m_DBCnt->Execute(strSQL, NULL, adCmdText);
		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		int i = 0;
		char buf[64];
		while (!pRst->adoEOF)
		{
			m_list.InsertItem(0, rs.m_sz_code);
			m_list.SetItemText(i, 1, rs.m_sz_dept);
			m_list.SetItemText(i, 2, rs.m_sz_date);
			m_list.SetItemText(i, 3, rs.m_sz_state);
			m_list.SetItemText(i, 4, rs.m_sz_keeper);
			sprintf(buf,"%f", rs.m_f_number); 	m_list.SetItemText(i, 5, buf);
			m_list.SetItemText(i, 6, rs.m_sz_taker);
			m_list.SetItemText(i, 7, rs.m_sz_usage);

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

void CDlgViewOut::OnClickListVo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_list.GetSelectionMark();

	m_code = m_list.GetItemText(i,0);
	m_dept = m_list.GetItemText(i,1);
	m_date = m_list.GetItemText(i,2);
	m_state = atoi(m_list.GetItemText(i,3));
	m_keeper = m_list.GetItemText(i,4);
	m_number = (float)atof(m_list.GetItemText(i,5));
	m_taker = m_list.GetItemText(i,6);
	m_usage = m_list.GetItemText(i,7);
	
	UpdateData(FALSE);
	
	*pResult = 0;
}
