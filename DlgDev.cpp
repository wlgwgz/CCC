// DlgDev.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgDev.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDev dialog


CDlgDev::CDlgDev(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDev::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDev)
	m_cur = 0.0f;
	m_max = 0.0f;
	m_min = 0.0f;
	m_total = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgDev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDev)
	DDX_Control(pDX, IDC_COMBO_DEVDEV, m_devs);
	DDX_Control(pDX, IDC_LIST_DEV, m_list);
	DDX_Text(pDX, IDC_EDIT_DEVCUR, m_cur);
	DDV_MinMaxFloat(pDX, m_cur, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_DEVMAX, m_max);
	DDV_MinMaxFloat(pDX, m_max, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_DEVMIN, m_min);
	DDV_MinMaxFloat(pDX, m_min, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_DEVTOTAL, m_total);
	DDV_MinMaxFloat(pDX, m_total, 1.f, 10000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDev, CDialog)
	//{{AFX_MSG_MAP(CDlgDev)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DEV, OnClickListDev)
	ON_BN_CLICKED(IDC_BTN_DEVADD, OnBtnDevadd)
	ON_BN_CLICKED(IDC_BTN_DEVDEL, OnBtnDevdel)
	ON_BN_CLICKED(IDC_BTN_DEVUPD, OnBtnDevupd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDev message handlers

BOOL CDlgDev::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.InsertColumn(0,"设备号");
	m_list.InsertColumn(1,"现存数量");
	m_list.InsertColumn(2,"最大数量");
	m_list.InsertColumn(3,"最小数量");
	m_list.InsertColumn(4,"总数");
	
	RECT rect;
	m_list.GetWindowRect(&rect);
	int wid = rect.right - rect.left;
	m_list.SetColumnWidth(0,wid/5);
	m_list.SetColumnWidth(1,wid/5);
	m_list.SetColumnWidth(2,wid/5);
	m_list.SetColumnWidth(3,wid/5);
	m_list.SetColumnWidth(4,wid/5);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	_RecordsetPtr pRst = NULL;
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevCodeRs rs;

	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE_CODE");

		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst = m_DBCnt->Execute(strSQL, NULL, adCmdText);
		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		int i = 0;
		while (!pRst->adoEOF)
		{
			m_devs.AddString(rs.m_sz_code);
			pRst->MoveNext();
		}
		picRs->Release();
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
		EndDialog(0);
		return TRUE;
    } 

	RefreshData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDev::RefreshData()
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);

	_RecordsetPtr pRst = NULL;
	
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevRs rs;
	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE");

		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst = m_DBCnt->Execute(strSQL, NULL, adCmdText);
		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		int i = 0;
		char buf[64];
		while (!pRst->adoEOF)
		{
			m_list.InsertItem(0, rs.m_sz_code);

			sprintf(buf,"%f",rs.m_f_cur); 	m_list.SetItemText(i, 1, buf);
			sprintf(buf,"%f",rs.m_f_max);	m_list.SetItemText(i, 2, buf);
			sprintf(buf,"%f",rs.m_f_min);	m_list.SetItemText(i, 3, buf);
			sprintf(buf,"%f",rs.m_f_total);	m_list.SetItemText(i, 4, buf);

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

void CDlgDev::OnClickListDev(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i = m_list.GetSelectionMark();

	m_devs.SetWindowText(m_list.GetItemText(i,0));
	m_cur = (float)atof(m_list.GetItemText(i,1));
	m_max = (float)atof(m_list.GetItemText(i,2));
	m_min = (float)atof(m_list.GetItemText(i,3));
	m_total = (float)atof(m_list.GetItemText(i,4));

	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgDev::OnBtnDevadd() 
{
	if (!UpdateData())
		return;

	_RecordsetPtr pRst = NULL;
	CDevRs rs;

	m_devs.GetWindowText(rs.m_sz_code,10);

	if (m_devs.GetCurSel()==CB_ERR)
	{
		MessageBox("请选择一个设备");
		return;
	}

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst->Open("device",_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdTable);
      		
		m_devs.GetWindowText(rs.m_sz_code,10);
		rs.m_f_cur = m_cur;
		rs.m_f_max = m_max;
		rs.m_f_min = m_min;
		rs.m_f_total = m_total;
	
		COleSafeArray vaFieldlist, vaValuelist;
		rs.FillFieldsArray(vaFieldlist,vaValuelist);
		TESTHR(pRst->AddNew(vaFieldlist, vaValuelist));
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
		return;
    } 

	MessageBox("完成操作！");
	m_log->AddLog("修改库存信息");
	
	RefreshData();
}

void CDlgDev::OnBtnDevdel() 
{
	if (!UpdateData())
		return;

	CString code;
	m_devs.GetWindowText(code);

	CString sql_;
	sql_.Format("DELETE FROM DEVICE WHERE CODE='%s'",code);
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
	MessageBox("完成操作！");	
	m_log->AddLog("删除库存记录。");
      
	RefreshData();      
}

void CDlgDev::OnBtnDevupd() 
{
	if (!UpdateData())
		return;

	_RecordsetPtr pRst = NULL;
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevRs rs;

	m_devs.GetWindowText(rs.m_sz_code,10);

	if (strlen(rs.m_sz_code)<=0)
	{
		MessageBox("请选择一个设备");
		return;
	}

	CString sql_;
	sql_.Format("SELECT * FROM DEVICE WHERE code='%s'", rs.m_sz_code);
	_bstr_t sql = sql_;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst->Open(sql,_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdText);
      		
		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));

		rs.m_f_cur = m_cur;
		rs.m_f_max = m_max;
		rs.m_f_min = m_min;
		rs.m_f_total = m_total;

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
	m_log->AddLog("修改库存信息");
	
	RefreshData();
}
