

#pragma once
#include "modbus.h"
#define   ModbusTCP_ADU_Max_Size           260
class CModbusTCP :
	public CModbus
{
public:
	CModbusTCP(void);
	~CModbusTCP(void);
private:
	char				ADU_ModbusTCP[ModbusTCP_ADU_Max_Size];                                   //define the ADU of modbus tcp
	char				MBAP[7];																//define the MBAP for modbus tcp
	char				Response_ADU[ModbusTCP_ADU_Max_Size];									//define the response adu
	char				Response_MBAP[7];
	byte				Slave_addr;															    //set the slave address,default 0
public:
	bool				SetSlaveAddr(byte addr);												//set the slave addr
	bool				SetMBAP();																//set MBAP
	char *				GetMBAP();																//get MBAP
	bool				SetADU_ModbusTCP();														//set adu_modbustcp
	char *				GetADU_ModbusTCP();														//get adu_modbustcp
	bool				SetResponse_ADU(char *tbuf);											//obtain or set response adu
	char *				GetResponse_ADU();														//get the response adu
	bool                IsModbusTCP_ResponseADU();												//to judge if the response adu is modbus tcp adu
	bool				Response_ADUProc();														//process the received response,to judge whether exception happens,and separate PDU and MBAP
};

