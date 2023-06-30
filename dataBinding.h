#ifndef DATA_BINDING_H
#define DATA_BINDING_H

class CDevCodeRs : public CADORecordBinding
{
BEGIN_ADO_BINDING(CDevCodeRs)
    
   //Column empid is the 1st field in the recordset   
   ADO_VARIABLE_LENGTH_ENTRY2(1, adVarChar, m_sz_code, 
         sizeof(m_sz_code), m_sts_code, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(2, adVarChar, m_sz_name, 
         sizeof(m_sz_name), m_sts_name, TRUE)
END_ADO_BINDING()

public:
   ULONG m_sts_code;
   ULONG m_sts_name;

   CHAR  m_sz_code[10];
   CHAR  m_sz_name[40];

public:
	void FillFieldsArray(COleSafeArray& fields, COleSafeArray& values);
};

class CDevNeedRs : public CADORecordBinding
{
BEGIN_ADO_BINDING(CDevNeedRs)
    
   //Column empid is the 1st field in the recordset   
   ADO_VARIABLE_LENGTH_ENTRY2(1, adVarChar, m_sz_code, 
         sizeof(m_sz_code), m_sts_code, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(2, adVarChar, m_sz_dept, 
         sizeof(m_sz_dept), m_sts_dept, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(3, adVarChar, m_sz_start, 
         sizeof(m_sz_start), m_sts_end, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(4, adVarChar, m_sz_end, 
		 sizeof(m_sz_end), m_sts_end, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(5, adVarChar, m_sz_number, 
         sizeof(m_sz_number), m_sts_number, TRUE)

END_ADO_BINDING()

public:

   CHAR  m_sz_code[10];
   CHAR  m_sz_dept[20];
   CHAR  m_sz_start[40];
   CHAR  m_sz_end[40];
   CHAR  m_sz_number[21];

   ULONG m_sts_code;
   ULONG m_sts_dept;
   ULONG m_sts_start;
   ULONG m_sts_end;
   ULONG m_sts_number;

public:
	void FillFieldsArray(COleSafeArray& fields, COleSafeArray& values);
};

class CDevRs : public CADORecordBinding
{
BEGIN_ADO_BINDING(CDevRs)
    
   //Column empid is the 1st field in the recordset   
   ADO_VARIABLE_LENGTH_ENTRY2(1, adVarChar, m_sz_code, 
         sizeof(m_sz_code), m_sts_code, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(2, adSingle, m_f_cur, 
         sizeof(m_f_cur), m_sts_cur, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(3, adSingle, m_f_max, 
         sizeof(m_f_max), m_sts_max, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(4, adSingle, m_f_min, 
		 sizeof(m_f_min), m_sts_min, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(5, adSingle, m_f_total, 
         sizeof(m_f_total), m_sts_total, TRUE)

END_ADO_BINDING()

public:

   CHAR  m_sz_code[10];
   float  m_f_cur;
   float  m_f_max;
   float  m_f_min;
   float  m_f_total;

   ULONG m_sts_code;
   ULONG m_sts_cur;
   ULONG m_sts_max;
   ULONG m_sts_min;
   ULONG m_sts_total;

public:
	void FillFieldsArray(COleSafeArray& fields, COleSafeArray& values);
};


class CDevRetRs : public CADORecordBinding
{
BEGIN_ADO_BINDING(CDevRetRs)
    
   //Column empid is the 1st field in the recordset   
   ADO_VARIABLE_LENGTH_ENTRY2(1, adVarChar, m_sz_code, 
         sizeof(m_sz_code), m_sts_code, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(2, adVarChar, m_sz_date, 
         sizeof(m_sz_date), m_sts_date, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(3, adVarChar, m_sz_keeper, 
         sizeof(m_sz_keeper), m_sts_keeper, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(4, adSingle, m_f_number, 
		 sizeof(m_f_number), m_sts_number, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(5, adVarChar, m_sz_reter, 
         sizeof(m_sz_reter), m_sts_reter, TRUE)

END_ADO_BINDING()

public:

   CHAR  m_sz_code[10];
   CHAR  m_sz_date[20];
   CHAR  m_sz_keeper[21];
   float  m_f_number;
   CHAR  m_sz_reter[21];

   ULONG m_sts_code;
   ULONG m_sts_date;
   ULONG m_sts_keeper;
   ULONG m_sts_number;
   ULONG m_sts_reter;

public:
	void FillFieldsArray(COleSafeArray& fields, COleSafeArray& values);
};

class CDevOutRs : public CADORecordBinding
{
BEGIN_ADO_BINDING(CDevOutRs)
    
   //Column empid is the 1st field in the recordset   
   ADO_VARIABLE_LENGTH_ENTRY2(1, adVarChar, m_sz_code, 
         sizeof(m_sz_code), m_sts_code, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(2, adVarChar, m_sz_dept, 
         sizeof(m_sz_dept), m_sts_dept, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(3, adVarChar, m_sz_date, 
         sizeof(m_sz_date), m_sts_date, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(4, adVarChar, m_sz_state, 
         sizeof(m_sz_state), m_sts_state, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(5, adVarChar, m_sz_keeper, 
         sizeof(m_sz_keeper), m_sts_keeper, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(6, adSingle, m_f_number, 
		 sizeof(m_f_number), m_sts_number, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(7, adVarChar, m_sz_taker, 
         sizeof(m_sz_taker), m_sts_taker, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(8, adVarChar, m_sz_usage, 
         sizeof(m_sz_usage), m_sts_usage, TRUE)

END_ADO_BINDING()

public:

   CHAR  m_sz_code[10];
   CHAR  m_sz_dept[21];
   CHAR  m_sz_date[20];
   CHAR  m_sz_state[20];
   CHAR  m_sz_keeper[21];
   float m_f_number;
   CHAR  m_sz_taker[21];
   CHAR  m_sz_usage[81];

   ULONG m_sts_code;
   ULONG m_sts_dept;
   ULONG m_sts_date;
   ULONG m_sts_state;
   ULONG m_sts_keeper;
   ULONG m_sts_number;
   ULONG m_sts_taker;
   ULONG m_sts_usage;

public:
	void FillFieldsArray(COleSafeArray& fields, COleSafeArray& values);
};

class CDevInRs : public CADORecordBinding
{
BEGIN_ADO_BINDING(CDevInRs)
    
   //Column empid is the 1st field in the recordset   
   ADO_VARIABLE_LENGTH_ENTRY2(1, adVarChar, m_sz_code, 
         sizeof(m_sz_code), m_sts_code, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(2, adVarChar, m_sz_date, 
         sizeof(m_sz_date), m_sts_date, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(3, adVarChar, m_sz_provider, 
         sizeof(m_sz_provider), m_sts_provider, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(4, adVarChar, m_sz_tel, 
         sizeof(m_sz_tel), m_sts_tel, TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(5, adSingle, m_f_number, 
		 sizeof(m_f_number), m_sts_number, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(6, adSingle, m_f_price,
		 sizeof(m_f_price), m_sts_price,  TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(7, adVarChar, m_sz_buyer, 
         sizeof(m_sz_buyer), m_sts_buyer, TRUE)

END_ADO_BINDING()

public:

   CHAR  m_sz_code[10];
   CHAR  m_sz_date[20];
   CHAR  m_sz_provider[21];
   CHAR  m_sz_tel[11];
   float  m_f_number;
   float  m_f_price;
   CHAR  m_sz_buyer[11];

   ULONG m_sts_code;
   ULONG m_sts_date;
   ULONG m_sts_provider;
   ULONG m_sts_tel; 
   ULONG m_sts_number;
   ULONG m_sts_price;
   ULONG m_sts_buyer;

public:
	void FillFieldsArray(COleSafeArray& fields, COleSafeArray& values);
};

class CDevBuyRs : public CADORecordBinding
{
BEGIN_ADO_BINDING(CDevBuyRs)
    
   //Column empid is the 1st field in the recordset   
   ADO_VARIABLE_LENGTH_ENTRY2(1, adVarChar, m_sz_code, 
         sizeof(m_sz_code), m_sts_code, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(2, adVarChar, m_sz_now, 
         sizeof(m_sz_now), m_sts_now, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(3, adVarChar, m_sz_total, 
		 sizeof(m_sz_total), m_sts_total, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(4, adVarChar, m_sz_max, 
         sizeof(m_sz_max), m_sts_max, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(5, adVarChar, m_sz_buy, 
         sizeof(m_sz_buy), m_sts_buy, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(6, adVarChar, m_sz_provider, 
         sizeof(m_sz_provider), m_sts_provider, TRUE)

   ADO_VARIABLE_LENGTH_ENTRY2(7, adVarChar, m_sz_price,
		 sizeof(m_sz_price), m_sts_price,  TRUE)
		 
   ADO_VARIABLE_LENGTH_ENTRY2(8, adVarChar, m_sz_date, 
         sizeof(m_sz_date), m_sts_date, TRUE)


END_ADO_BINDING()

public:

   CHAR  m_sz_code[10];
   CHAR  m_sz_now[20];
   CHAR  m_sz_total[20];
   CHAR  m_sz_max[20];
   CHAR  m_sz_buy[20];
   CHAR  m_sz_provider[21];
   CHAR  m_sz_price[20];
   CHAR  m_sz_date[20];

   ULONG m_sts_code;
   ULONG m_sts_now;
   ULONG m_sts_total;
   ULONG m_sts_max; 
   ULONG m_sts_buy;
   ULONG m_sts_provider;
   ULONG m_sts_price;
   ULONG m_sts_date;

public:
	void FillFieldsArray(COleSafeArray& fields, COleSafeArray& values);

};

#endif
