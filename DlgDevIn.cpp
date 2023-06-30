// DlgDevIn.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgDevIn.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDevIn dialog


CDlgDevIn::CDlgDevIn(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDevIn)
	m_date = 0;
	m_time = 0;
	m_buyer = _T("");
	m_provider = _T("");
	m_tel = _T("");
	m_number = 0.0f;
	m_price = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgDevIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDevIn)
	DDX_Control(pDX, IDC_COMBO_DIDEVS, m_devs);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DIDATE, m_date);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DITIME, m_time);
	DDX_Text(pDX, IDC_EDIT_DIBUYER, m_buyer);
	DDX_Text(pDX, IDC_EDIT_DIPROVIDER, m_provider);
	DDX_Text(pDX, IDC_EDIT_DITEL, m_tel);
	DDX_Text(pDX, IDC_EDIT_DINUMBER, m_number);
	DDV_MinMaxFloat(pDX, m_number, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_DIPRICE, m_price);
	DDV_MinMaxFloat(pDX, m_price, 1.f, 10000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDevIn, CDialog)
	//{{AFX_MSG_MAP(CDlgDevIn)
	ON_BN_CLICKED(IDC_BTN_DIADD, OnBtnDiadd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDevIn message handlers

BOOL CDlgDevIn::OnInitDialog() 
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
		picRs->Release();
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

void CDlgDevIn::OnBtnDiadd() 
{
	if (m_devs.GetCurSel()==CB_ERR)
	{
		MessageBox("请选择一个设备");
		return;
	}

	if (!UpdateData())
		return;

	_RecordsetPtr pRst = NULL;
	CDevInRs rs;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst->Open("device_in",_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdTable);
      	
		m_devs.GetWindowText(rs.m_sz_code,11);
		sprintf(rs.m_sz_date,"%d-%d-%d %d:%d:%d",
			m_date.GetYear(), m_date.GetMonth(), m_date.GetDay(),
			m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		strcpy(rs.m_sz_provider, m_provider);
		strcpy(rs.m_sz_tel, m_tel);
		rs.m_f_number = m_number;
		rs.m_f_price = m_price;
		strcpy(rs.m_sz_buyer, m_buyer);

		COleSafeArray vaFieldlist, vaValuelist;
		rs.FillFieldsArray(vaFieldlist,vaValuelist);

		m_DBCnt->BeginTrans();

		TESTHR(pRst->AddNew(vaFieldlist, vaValuelist));
		pRst->Close();

		CString sql_;
		sql_.Format("SELECT * FROM device WHERE code='%s'", rs.m_sz_code);
		_bstr_t sql = sql_;
		
		pRst->Open(sql, _variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdText);
		if (pRst->GetRecordCount()==0)
		{
			CDevRs rsDev;
			strcpy(rsDev.m_sz_code, rs.m_sz_code);

			rsDev.m_f_cur = rs.m_f_number;
			rsDev.m_f_total = rs.m_f_number;
			rsDev.m_f_max = 1;
			rsDev.m_f_min = 1;

			COleSafeArray vaFields, vaValues;
			rsDev.FillFieldsArray(vaFields,vaValues);
			TESTHR(pRst->AddNew(vaFields, vaValues));
		} else
		{
			CDevRs rsDev;
			IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
			TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
			TESTHR(picRs->BindToRecordset(&rsDev));

			strcpy(rsDev.m_sz_code, rs.m_sz_code);
			rsDev.m_f_cur = rs.m_f_number + rsDev.m_f_cur;
			rsDev.m_f_total = rs.m_f_number + rsDev.m_f_total;

			TESTHR(picRs->Update(&rsDev));
			picRs->Release();
		}
		pRst->Close();

		m_DBCnt->CommitTrans();
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.ErrorMessage());
		m_DBCnt->RollbackTrans();
		return;
    } 

	MessageBox("完成操作！");
	m_log->AddLog("添加入库信息");

	EndDialog(0);
}
