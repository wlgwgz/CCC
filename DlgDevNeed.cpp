// DlgDevNeed.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgDevNeed.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDevNeed dialog


CDlgDevNeed::CDlgDevNeed(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevNeed::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDevNeed)
	m_end = 0;
	m_start = 0;
	m_dept = _T("");
	m_number = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgDevNeed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDevNeed)
	DDX_Control(pDX, IDC_COMBO_DNCODE, m_devs);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DNEND, m_end);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DNSTART, m_start);
	DDX_Text(pDX, IDC_EDIT_DNDEPT, m_dept);
	DDV_MaxChars(pDX, m_dept, 10);
	DDX_Text(pDX, IDC_EDIT_DNNUMBER, m_number);
	DDV_MinMaxFloat(pDX, m_number, 1.f, 10000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDevNeed, CDialog)
	//{{AFX_MSG_MAP(CDlgDevNeed)
	ON_BN_CLICKED(IDC_BTN_DNADD, OnBtnDnadd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDevNeed message handlers

void CDlgDevNeed::OnBtnDnadd() 
{
	if (m_devs.GetCurSel()==CB_ERR)
	{
		MessageBox("请选择一个设备");
		return;
	}

	if (!UpdateData())
		return;

	if (m_dept.GetLength()<=0)
	{
		MessageBox("部门一项不能为空！");
		return;
	}

	_RecordsetPtr pRst = NULL;
	CDevNeedRs rs;

	try
	{
		TESTHR(pRst.CreateInstance(__uuidof(Recordset)));

		pRst->Open("device_need",_variant_t((IDispatch *) m_DBCnt, true),adOpenKeyset,adLockOptimistic,adCmdTable);
      		
		m_devs.GetWindowText(rs.m_sz_code,11);
		strcpy(rs.m_sz_dept, m_dept);
		sprintf(rs.m_sz_start,"%d-%d-%d",
			m_start.GetYear(), m_start.GetMonth(), m_start.GetDay());
		sprintf(rs.m_sz_end,"%d-%d-%d",
			m_end.GetYear(), m_end.GetMonth(), m_end.GetDay());
		sprintf(rs.m_sz_number, "%f",m_number);

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
	m_log->AddLog("添加需求信息");

	EndDialog(0);
}

BOOL CDlgDevNeed::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_start = CTime::GetCurrentTime();
	m_end = m_start;

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
