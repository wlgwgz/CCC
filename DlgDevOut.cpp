// DlgDevOut.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgDevOut.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDevOut dialog


CDlgDevOut::CDlgDevOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDevOut)
	m_date = 0;
	m_time = 0;
	m_dept = _T("");
	m_keeper = _T("");
	m_number = 0.0f;
	m_taker = _T("");
	m_usage = _T("");
	m_state = 0;
	m_max = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgDevOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDevOut)
	DDX_Control(pDX, IDC_COMBO_DODEVS, m_devs);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DODATE, m_date);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DOTIME, m_time);
	DDX_Text(pDX, IDC_EDIT_DODEPT, m_dept);
	DDX_Text(pDX, IDC_EDIT_DOKEEPER, m_keeper);
	DDX_Text(pDX, IDC_EDIT_DONUMBER, m_number);
	DDV_MinMaxFloat(pDX, m_number, 1.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_DOTAKER, m_taker);
	DDX_Text(pDX, IDC_EDIT_DOUSAGE, m_usage);
	DDX_Text(pDX, IDC_EDIT_DOSTATE, m_state);
	DDV_MinMaxUInt(pDX, m_state, 0, 9);
	DDX_Text(pDX, IDC_EDIT_DOMAX, m_max);
	DDV_MinMaxFloat(pDX, m_max, 1.f, 10000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDevOut, CDialog)
	//{{AFX_MSG_MAP(CDlgDevOut)
	ON_BN_CLICKED(IDC_BTN_DOADD, OnBtnDoadd)
	ON_CBN_CLOSEUP(IDC_COMBO_DODEVS, OnCloseupComboDodevs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDevOut message handlers

void CDlgDevOut::OnBtnDoadd() 
{
	if (m_devs.GetCurSel()==CB_ERR)
	{
		MessageBox("请选择一个设备");
		return;
	}

	if (!UpdateData())
		return;

	if ((m_max>0.0f) && (m_number>m_max))
	{
		MessageBox("超过最大限量！");
		return;
	}

	_RecordsetPtr pRst = NULL;
	CDevOutRs rs;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst->Open("device_out",_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdTable);
      		
		m_devs.GetWindowText(rs.m_sz_code,11);
		sprintf(rs.m_sz_date,"%d-%d-%d %d:%d:%d",
			m_date.GetYear(), m_date.GetMonth(), m_date.GetDay(),
			m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		strcpy(rs.m_sz_dept, m_dept);
		sprintf(rs.m_sz_state, "%d", m_state);
		strcpy(rs.m_sz_keeper, m_keeper);
		rs.m_f_number = m_number;
		strcpy(rs.m_sz_taker, m_taker);
		strcpy(rs.m_sz_usage, m_usage);

		COleSafeArray vaFieldlist, vaValuelist;
		rs.FillFieldsArray(vaFieldlist,vaValuelist);
		TESTHR(pRst->AddNew(vaFieldlist, vaValuelist));
		pRst->Close();

		CString sql_;
		sql_.Format("SELECT * FROM device WHERE code='%s'", rs.m_sz_code);
		_bstr_t sql = sql_;
		
		pRst->Open(sql, _variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdText);
		if (pRst->GetRecordCount()==1)
		{
			CDevRs rsDev;
			IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
			TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
			TESTHR(picRs->BindToRecordset(&rsDev));

			strcpy(rsDev.m_sz_code, rs.m_sz_code);
			rsDev.m_f_cur = rsDev.m_f_cur - rs.m_f_number;

			TESTHR(picRs->Update(&rsDev));
			picRs->Release();
		}
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
		return;
    } 

	MessageBox("完成操作！");
	m_log->AddLog("添加出库信息");

	EndDialog(0);
}

BOOL CDlgDevOut::OnInitDialog() 
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

void CDlgDevOut::OnCloseupComboDodevs() 
{
	char buf[64];

	m_devs.GetWindowText(buf,64);
	if (strlen(buf)<=0)
		return;	

	_RecordsetPtr pRst = NULL;
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevRs rs;
	_bstr_t strSQL;

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
			UpdateData(FALSE);
			return;
		}

		TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
		TESTHR(picRs->BindToRecordset(&rs));
		
		m_max = rs.m_f_cur;

		picRs->Release();
		pRst->Close();
	}
	catch(_com_error& e)
	{
        AfxMessageBox(e.ErrorMessage());
    } 

	UpdateData(FALSE);
}
