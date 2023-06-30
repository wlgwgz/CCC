#include "stdafx.h"
#include "DataBinding.h"

void CDevCodeRs::FillFieldsArray(COleSafeArray& vaFieldlist, COleSafeArray& vaValuelist)
{
	vaFieldlist.CreateOneDim(VT_VARIANT,2);
	long lArrayIndex[1];
	lArrayIndex[0] = 0;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("code")));
	lArrayIndex[0] = 1;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("name")));

	vaValuelist.CreateOneDim(VT_VARIANT,2);
	lArrayIndex[0] = 0;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_code)));
	lArrayIndex[0] = 1;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_name)));
}

void CDevNeedRs::FillFieldsArray(COleSafeArray& vaFieldlist, COleSafeArray& vaValuelist)
{
	vaFieldlist.CreateOneDim(VT_VARIANT,5);
	long lArrayIndex[1];
	lArrayIndex[0] = 0;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("code")));
	lArrayIndex[0] = 1;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("department")));
	lArrayIndex[0] = 2;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("begin_date")));
	lArrayIndex[0] = 3;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("end_date")));
	lArrayIndex[0] = 4;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("need_number")));

	vaValuelist.CreateOneDim(VT_VARIANT,5);
	lArrayIndex[0] = 0;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_code)));
	lArrayIndex[0] = 1;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_dept)));
	lArrayIndex[0] = 2;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_start)));
	lArrayIndex[0] = 3;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_end)));
	lArrayIndex[0] = 4;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_number)));
}

void CDevRs::FillFieldsArray(COleSafeArray& vaFieldlist, COleSafeArray& vaValuelist)
{
	vaFieldlist.CreateOneDim(VT_VARIANT,5);
	long lArrayIndex[1];
	lArrayIndex[0] = 0;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("code")));
	lArrayIndex[0] = 1;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("now_number")));
	lArrayIndex[0] = 2;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("high_number")));
	lArrayIndex[0] = 3;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("low_number")));
	lArrayIndex[0] = 4;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("total_number")));

	vaValuelist.CreateOneDim(VT_VARIANT,5);
	lArrayIndex[0] = 0;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_code)));
	lArrayIndex[0] = 1;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_f_cur)));
	lArrayIndex[0] = 2;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_f_max)));
	lArrayIndex[0] = 3;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_f_min)));
	lArrayIndex[0] = 4;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_f_total)));
}

void CDevRetRs::FillFieldsArray(COleSafeArray& vaFieldlist, COleSafeArray& vaValuelist)
{
	vaFieldlist.CreateOneDim(VT_VARIANT,5);
	long lArrayIndex[1];
	lArrayIndex[0] = 0;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("code")));
	lArrayIndex[0] = 1;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("return_date")));
	lArrayIndex[0] = 2;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("keeper")));
	lArrayIndex[0] = 3;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("return_number")));
	lArrayIndex[0] = 4;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("return_person")));

	vaValuelist.CreateOneDim(VT_VARIANT,5);
	lArrayIndex[0] = 0;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_code)));
	lArrayIndex[0] = 1;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_date)));
	lArrayIndex[0] = 2;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_keeper)));
	lArrayIndex[0] = 3;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_f_number)));
	lArrayIndex[0] = 4;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_reter)));
}

void CDevOutRs::FillFieldsArray(COleSafeArray& vaFieldlist, COleSafeArray& vaValuelist)
{
	vaFieldlist.CreateOneDim(VT_VARIANT,8);
	long lArrayIndex[1];
	lArrayIndex[0] = 0;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("code")));
	lArrayIndex[0] = 1;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("department")));
	lArrayIndex[0] = 2;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("out_date")));
	lArrayIndex[0] = 3;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("out_state")));
	lArrayIndex[0] = 4;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("out_person")));
	lArrayIndex[0] = 5;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("out_number")));
	lArrayIndex[0] = 6;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("taker")));
	lArrayIndex[0] = 7;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("usage")));

	vaValuelist.CreateOneDim(VT_VARIANT,8);
	lArrayIndex[0] = 0;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_code)));
	lArrayIndex[0] = 1;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_dept)));
	lArrayIndex[0] = 2;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_date)));
	lArrayIndex[0] = 3;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_state)));
	lArrayIndex[0] = 4;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_keeper)));
	lArrayIndex[0] = 5;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_f_number)));
	lArrayIndex[0] = 6;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_taker)));
	lArrayIndex[0] = 7;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_usage)));
}


void CDevInRs::FillFieldsArray(COleSafeArray& vaFieldlist, COleSafeArray& vaValuelist)
{
	vaFieldlist.CreateOneDim(VT_VARIANT,7);
	long lArrayIndex[1];
	lArrayIndex[0] = 0;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("code")));
	lArrayIndex[0] = 1;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("in_date")));
	lArrayIndex[0] = 2;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("provider")));
	lArrayIndex[0] = 3;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("teleno")));
	lArrayIndex[0] = 4;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("in_number")));
	lArrayIndex[0] = 5;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("price")));
	lArrayIndex[0] = 6;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("buyer")));

	vaValuelist.CreateOneDim(VT_VARIANT,7);
	lArrayIndex[0] = 0;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_code)));
	lArrayIndex[0] = 1;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_date)));
	lArrayIndex[0] = 2;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_provider)));
	lArrayIndex[0] = 3;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_tel)));
	lArrayIndex[0] = 4;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_f_number)));
	lArrayIndex[0] = 5;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_f_price)));
	lArrayIndex[0] = 6;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_buyer)));
}

void CDevBuyRs::FillFieldsArray(COleSafeArray& vaFieldlist, COleSafeArray& vaValuelist)
{
	vaFieldlist.CreateOneDim(VT_VARIANT,8);
	long lArrayIndex[1];
	lArrayIndex[0] = 0;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("code")));
	lArrayIndex[0] = 1;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("now_number")));
	lArrayIndex[0] = 2;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("total_number")));
	lArrayIndex[0] = 3;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("max_number")));
	lArrayIndex[0] = 4;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("buy_number")));
	lArrayIndex[0] = 5;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("provider")));
	lArrayIndex[0] = 6;
    vaFieldlist.PutElement(lArrayIndex, &(_variant_t("price")));
	lArrayIndex[0] = 7;
	vaFieldlist.PutElement(lArrayIndex, &(_variant_t("buy_date")));

	vaValuelist.CreateOneDim(VT_VARIANT,8);
	lArrayIndex[0] = 0;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_code)));
	lArrayIndex[0] = 1;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_now)));
	lArrayIndex[0] = 2;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_total)));
	lArrayIndex[0] = 3;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_max)));
	lArrayIndex[0] = 4;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_buy)));
	lArrayIndex[0] = 5;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_provider)));
	lArrayIndex[0] = 6;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_price)));
	lArrayIndex[0] = 7;
	vaValuelist.PutElement(lArrayIndex, &(_variant_t(m_sz_date)));
}
 