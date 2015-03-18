#include "StdAfx.h"
#include "ModbusTCP.h"


CModbusTCP::CModbusTCP(void)
{
	Slave_addr = 0;															//set the default slave address 0
	ZeroMemory(MBAP,sizeof(MBAP));
	ZeroMemory(ADU_ModbusTCP,sizeof(ADU_ModbusTCP));
}


CModbusTCP::~CModbusTCP(void)
{
}
bool  CModbusTCP::SetSlaveAddr(byte addr)
{
	Slave_addr = addr;
	return true;
}
bool CModbusTCP::SetMBAP()
{
	for (int i=0;i<5;i++)
	{
		MBAP[i] = 0;
	}
	MBAP[5] = length_pdu+1 ;
	MBAP[6] = Slave_addr;															 //default slave addr is set to 0x00!
	return true;
}
char * CModbusTCP::GetMBAP()
{
	return MBAP;
}
bool CModbusTCP::SetADU_ModbusTCP()
{
	for (int i=0;i<7;i++)                                                      //copy the MBAP
	{
		ADU_ModbusTCP[i] = MBAP[i];
	}
	
	for (int j=0;j<length_pdu;j++)											  //copy the PDU
	{
		ADU_ModbusTCP[7+j] = PDU[j];
	}
	return true;
}
char * CModbusTCP::GetADU_ModbusTCP()
{
	return ADU_ModbusTCP;
}
bool CModbusTCP::SetResponse_ADU(char *tbuf)
{
	for (int i=0;i<ModbusTCP_ADU_Max_Size;i++)
	{
		Response_ADU[i] = *(tbuf+i);
	}
	return true;
}
char * CModbusTCP::GetResponse_ADU()
{
	return Response_ADU;
}
bool CModbusTCP::IsModbusTCP_ResponseADU()
{
	byte protocol_identifier[2];
	protocol_identifier[0] = Response_ADU[2];
	protocol_identifier[1] = Response_ADU[3];
	byte Num_bytes;
	Num_bytes = Response_ADU[5];
	if (protocol_identifier[0]==0&&protocol_identifier[1]==0&&Num_bytes)
	{
		return true;                                                                          //it is right modbus tcp response adu
	}
	return false;																			  //it is not right modbus tcp response adu
}
bool CModbusTCP::Response_ADUProc()                                          //if no exception,return true,or return false
{
	for (int i=0;i<7;i++)
	{
		Response_MBAP[i] = Response_ADU[i];
	}
	char FC;
	FC = Response_ADU[7];
	if (FC & 0x80)															//judge if exception error happens through the MSB of function code
	{
		//if true,exception happen
		Response_Exception[0] = Response_ADU[7];
		Response_Exception[1] = Response_ADU[8];
		return false;
	}
	else
	{
		//if false, no exception happen,response is good
		for (int j=0;j<pdu_max_size;j++)
		{
			Response_PDU[j] = Response_ADU[7+j];
		}
		return true;
	}
}