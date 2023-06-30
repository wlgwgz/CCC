// DlgDevRet.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgDevRet.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDevRet dialog


CDlgDevRet::CDlgDevRet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevRet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDevRet)
	m_date = 0;
	m_time = 0;
	m_keeper = _T("");
	m_reter = _T("");
	m_number = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgDevRet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDevRet)
	DDX_Control(pDX, IDC_COMBO_DRDEVS, m_devs);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DRDATE, m_date);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DRTIME, m_time);
	DDX_Text(pDX, IDC_EDIT_DRKEEPER, m_keeper);
	DDX_Text(pDX, IDC_EDIT_DRRETER, m_reter);
	DDX_Text(pDX, IDC_EDIT_DRNUMBER, m_number);
	DDV_MinMaxFloat(pDX, m_number, 1.f, 10000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDevRet, CDialog)
	//{{AFX_MSG_MAP(CDlgDevRet)
	ON_BN_CLICKED(IDC_BTN_DRADD, OnBtnDradd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDevRet message handlers

void CDlgDevRet::OnBtnDradd() 
{
	if (m_devs.GetCurSel()==CB_ERR)
	{
		MessageBox("请选择一个设备");
		return;
	}

	if (!UpdateData())
		return;
	
	_RecordsetPtr pRst = NULL;
	CDevRetRs rs;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst->Open("device_return",_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdTable);
      		
		m_devs.GetWindowText(rs.m_sz_code,11);
		sprintf(rs.m_sz_date,"%d-%d-%d %d:%d:%d",
			m_date.GetYear(), m_date.GetMonth(), m_date.GetDay(),
			m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		strcpy(rs.m_sz_keeper, m_keeper);
		rs.m_f_number = m_number;
		strcpy(rs.m_sz_reter, m_reter);

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
			rsDev.m_f_cur = rs.m_f_number + rsDev.m_f_cur;

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
	m_log->AddLog("添加还库信息");

	EndDialog(0);
}

BOOL CDlgDevRet::OnInitDialog() 
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
