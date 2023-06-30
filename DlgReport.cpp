// DlgReport.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgReport.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReport dialog


CDlgReport::CDlgReport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReport)
	m_date = 0;
	m_time = 0;
	m_buy = 0.0f;
	m_cur = 0.0f;
	m_max = 0.0f;
	m_price = 0.0f;
	m_name = _T("");
	m_provider = _T("");
	m_total = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReport)
	DDX_Control(pDX, IDC_COMBO_RPDEVS, m_devs);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PRDATE, m_date);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PRTIME, m_time);
	DDX_Text(pDX, IDC_EDIT_RPBUY, m_buy);
	DDV_MinMaxFloat(pDX, m_buy, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_RPCUR, m_cur);
	DDX_Text(pDX, IDC_EDIT_RPMAX, m_max);
	DDX_Text(pDX, IDC_EDIT_RPPRICE, m_price);
	DDV_MinMaxFloat(pDX, m_price, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_RPNAME, m_name);
	DDX_Text(pDX, IDC_EDIT_RPPROVIDER, m_provider);
	DDX_Text(pDX, IDC_EDIT_RPTOTAL, m_total);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReport, CDialog)
	//{{AFX_MSG_MAP(CDlgReport)
	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)
	ON_CBN_CLOSEUP(IDC_COMBO_RPDEVS, OnCloseupComboRpdevs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReport message handlers

void CDlgReport::OnBtnPrint() 
{
	if (m_devs.GetCurSel()==CB_ERR)
	{
		MessageBox("请选择一个设备");
		return;
	}

	if (!UpdateData())
		return;

	_RecordsetPtr pRst = NULL;
	CDevBuyRs rs;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst->Open("device_wantbuy",_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdTable);

		m_devs.GetWindowText(rs.m_sz_code,11);
		sprintf(rs.m_sz_date,"%d-%d-%d %d:%d:%d",
			m_date.GetYear(), m_date.GetMonth(), m_date.GetDay(),
			m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		sprintf(rs.m_sz_now, "%f",m_cur);
		sprintf(rs.m_sz_total, "%f",m_total);
		sprintf(rs.m_sz_max, "%f",m_max);
		sprintf(rs.m_sz_buy, "%f",m_buy);
		strcpy(rs.m_sz_provider, m_provider);
		sprintf(rs.m_sz_price, "%f",m_price);

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
	m_log->AddLog("添加还库信息");

	EndDialog(0);
}

BOOL CDlgReport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_date = CTime::GetCurrentTime();
	m_time = m_date;

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
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
		EndDialog(0);
		return TRUE;
    } 

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgReport::OnCloseupComboRpdevs() 
{
	char buf[64];

	m_devs.GetWindowText(buf,64);
	if (strlen(buf)<=0)
		return;	

	_RecordsetPtr pRst = NULL;
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevCodeRs rs1;
	CDevRs rs2;
	_bstr_t strSQL;

	try
	{
		CString sql;
		sql.Format("SELECT * FROM DEVICE_CODE WHERE code='%s'",buf);
		strSQL = sql;
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));
		pRst = m_DBCnt->Execute(strSQL, NULL, adCmdText);

		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs1));

		m_name.Format("%s",rs1.m_sz_name);

		picRs->Release();
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
    } 

	try
	{
		CString sql;
		sql.Format("SELECT * FROM DEVICE WHERE code='%s'",buf);
		strSQL = sql;

		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));
		pRst->Open(strSQL, _variant_t((IDispatch *) m_DBCnt, true), adOpenKeyset, adLockOptimistic, adCmdText);
	
		if (pRst->GetRecordCount()!=1)
		{
			m_max = 0;
			m_cur = 0;
			m_total = 0;
		} else
		{
			TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
			TESTHR(picRs->BindToRecordset(&rs2));
			
			m_max = rs2.m_f_max;
			m_cur = rs2.m_f_cur;
			m_total = rs2.m_f_total;
			picRs->Release();
		}
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
    } 

	UpdateData(FALSE);
}
