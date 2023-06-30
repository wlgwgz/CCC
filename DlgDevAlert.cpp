// DlgDevAlert.cpp : implementation file
//

#include "stdafx.h"
#include "DMS.h"
#include "DlgDevAlert.h"
#include "DataBinding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDevAlert dialog


CDlgDevAlert::CDlgDevAlert(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevAlert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDevAlert)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDevAlert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDevAlert)
	DDX_Control(pDX, IDC_LIST_DL, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDevAlert, CDialog)
	//{{AFX_MSG_MAP(CDlgDevAlert)
	ON_BN_CLICKED(IDC_BTN_DLALL, OnBtnDlall)
	ON_BN_CLICKED(IDC_BTN_DLOVER, OnBtnDlover)
	ON_BN_CLICKED(IDC_BTN_DLBELOW, OnBtnDlbelow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDevAlert message handlers

void CDlgDevAlert::OnBtnDlall() 
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);	
	SearchBelow();
	SearchOver();
	m_list.SetRedraw(TRUE);	
}

void CDlgDevAlert::OnBtnDlover() 
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);	
	SearchOver();
	m_list.SetRedraw(TRUE);	
}

void CDlgDevAlert::OnBtnDlbelow() 
{
	m_list.DeleteAllItems();
	m_list.SetRedraw(FALSE);	
	SearchBelow();
	m_list.SetRedraw(TRUE);	
}

BOOL CDlgDevAlert::OnInitDialog() 
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDevAlert::SearchOver()
{
	_RecordsetPtr pRst = NULL;
	
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevRs rs;
	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE WHERE now_number>high_number");

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
		return;
    } 
}

void CDlgDevAlert::SearchBelow()
{
	_RecordsetPtr pRst = NULL;
	
	IADORecordBinding   *picRs = NULL;  //Interface Pointer declared.(VC++ Extensions)   
	CDevRs rs;
	try
	{
		_bstr_t strSQL("SELECT * FROM DEVICE WHERE now_number<low_number");

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
		return;
    } 
}
