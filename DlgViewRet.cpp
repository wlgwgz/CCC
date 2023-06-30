// DlgViewRet.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgViewRet.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewRet dialog


CDlgViewRet::CDlgViewRet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewRet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewRet)
	m_code = _T("");
	m_date = _T("");
	m_keeper = _T("");
	m_number = 0.0f;
	m_reter = _T("");
	//}}AFX_DATA_INIT
}


void CDlgViewRet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewRet)
	DDX_Control(pDX, IDC_LIST_VR, m_list);
	DDX_Text(pDX, IDC_EDIT_VRCODE, m_code);
	DDX_Text(pDX, IDC_EDIT_VRDATE, m_date);
	DDX_Text(pDX, IDC_EDIT_VRKEEPER, m_keeper);
	DDX_Text(pDX, IDC_EDIT_VRNUMBER, m_number);
	DDV_MinMaxFloat(pDX, m_number, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_VRRETER, m_reter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewRet, CDialog)
	//{{AFX_MSG_MAP(CDlgViewRet)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VR, OnClickListVr)
	ON_BN_CLICKED(IDC_BTN_VRUPDT, OnBtnVrupdt)
	ON_BN_CLICKED(IDC_BTN_VRDEL, OnBtnVrdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewRet message handlers

BOOL CDlgViewRet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.InsertColumn(0,"设备号");
	m_list.InsertColumn(1,"还库时间");
	m_list.InsertColumn(2,"管理员");
	m_list.InsertColumn(3,"数量");
	m_list.InsertColumn(4,"归还人");
	
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

void CDlgViewRet::RefreshData()
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);	

	_RecordsetPtr pRst = NULL;
	
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevRetRs rs;
	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE_RETURN");

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
			m_list.SetItemText(i, 2, rs.m_sz_keeper);
			sprintf(buf, "%f", rs.m_f_number);	m_list.SetItemText(i, 3, buf);
			m_list.SetItemText(i, 4, rs.m_sz_reter);

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

void CDlgViewRet::OnClickListVr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_list.GetSelectionMark();

	m_code = m_list.GetItemText(i,0);
	m_date = m_list.GetItemText(i,1);
	m_keeper = m_list.GetItemText(i,2);
	m_number = (float)atof(m_list.GetItemText(i,3));
	m_reter = m_list.GetItemText(i,4);
	
	UpdateData(FALSE);
	
	*pResult = 0;
}

void CDlgViewRet::OnBtnVrupdt() 
{
	if (!UpdateData())
		return;

	CString sql_;
	sql_.Format(CString("SELECT * FROM DEVICE_RETURN WHERE return_date=")+DATEFMT, m_date);
	_bstr_t sql = sql_;

	_RecordsetPtr pRst = NULL;
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevRetRs rs;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));
		
		pRst->Open(sql,_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdText);
      		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		strcpy(rs.m_sz_code,m_code);
		strcpy(rs.m_sz_date,m_date);
		strcpy(rs.m_sz_keeper, m_keeper);
		rs.m_f_number = m_number;
		strcpy(rs.m_sz_reter, m_reter);

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

void CDlgViewRet::OnBtnVrdel() 
{
	if (!UpdateData())
		return;

	CString sql_;
	sql_.Format(CString("DELETE FROM DEVICE_RETURN WHERE return_date=")+DATEFMT, m_date);
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
