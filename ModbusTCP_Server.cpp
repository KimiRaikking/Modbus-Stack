#include "StdAfx.h"
#include "ModbusTCP_Server.h"


CModbusTCP_Server::CModbusTCP_Server(void)
{
	//init variables
	local_identifier = 0x00;
}


CModbusTCP_Server::~CModbusTCP_Server(void)
{
}

/*				set modbus tcp request ADU								*/
void CModbusTCP_Server::SetMBTCP_RequestADU(char* pReq_ADU)
{
	for(int i=0;i<ModbusTCP_ADU_Max_Size;i++)
	{
		mb_TCP_RequestADU[i] = *(pReq_ADU+i);
	}
}

/*				get modbus tcp request ADU								*/
char* CModbusTCP_Server::GetMBTCP_RequestADU()
{
	return mb_TCP_RequestADU;
}

/*				set modbus tcp request MBAP,set unit identifier			*/
void CModbusTCP_Server::SetMBTCP_RequestMBAP(char* pReq_ADU)
{
	for (int i=0;i<7;i++)
	{
		mb_TCP_RequestMBAP[i] = *(pReq_ADU+i);
	}
	SetUnitIdentifier(mb_TCP_RequestMBAP[6]);
}

/*				get modbus tcp request MBAP								*/
char* CModbusTCP_Server::GetMBTCP_RequestMBAP()
{
	return mb_TCP_RequestMBAP;
}

/*				check if modbus request MBAP is correct					*/
bool CModbusTCP_Server::CheckRequestMBAP()
{
	//first,check if first 5 bytes are zero
	for (int i=0;i<5;i++)
	{
		if (mb_TCP_RequestMBAP[i] != 0x00) 
		{
			return false;
		}
	}

	//second,check unit identifier matching with local unit identifier
	if (mb_TCP_RequestMBAP[6] != local_identifier)
	{
		return false;
	}

	return true;
}

/*				set modbus request PDU from ADU							*/
void CModbusTCP_Server::SetRequestPDUfromADU(char* pReq_ADU)
{
	char* pReq_PDU;
	pReq_PDU = pReq_ADU+7;		//get point to request PDU
	SetRequestPDU(pReq_PDU);
}

/*				set local unit identifier								*/
void CModbusTCP_Server::SetLocalUnitIdentifier(char Local_id)
{
	local_identifier = Local_id;
}

/*				get local unit identifier								*/
char CModbusTCP_Server::GetLocalUnitIdentifier()
{
	return local_identifier;
}

/*				set modbus tcp req unit identifier						*/
void CModbusTCP_Server::SetUnitIdentifier(char unit_identifier)
{
	Unit_Identifier = unit_identifier;
}

/*				get modbus tcp req unit identifier						*/
char CModbusTCP_Server::GetUnitIdentifier()
{
	return Unit_Identifier;
}

/*				SET modbus tcp response MBAP							*/
void CModbusTCP_Server::SetMBTCP_ResponseMBAP()
{
	for (int i=0;i<5;i++)
		mb_TCP_ResponseMBAP[i] = 0x00;
	mb_TCP_ResponseMBAP[5] = GetRspPDU_Length()+1;					//number of bytes following
	mb_TCP_ResponseMBAP[6] = Unit_Identifier;						//unit identifier(slave addree),must check if rsp unit identifier is matching with local unit identifier!
}

/*				get modbus tcp response MBAP							*/
char* CModbusTCP_Server::GetMBTCP_ResponseMBAP()
{
	return mb_TCP_ResponseMBAP;
}

/*				set modbus tcp response ADU								*/
void CModbusTCP_Server::SetMBTCP_ResponseADU()
{
	//firsr part: MBAP
	for (int i=0;i<7;i++)
	{
		mb_TCP_ResponseADU[i] = mb_TCP_ResponseMBAP[i];
	}

	//second part: PDU
	//judge if the request pdu process return value
	//if true,PDU is mb_rsp
	//if false,PDU is mb_exception_rsp
	if (IsResponsepduCorrect)
	{
		//if true ,mb_rsp
		char* pResponsePDU;
		pResponsePDU = GetResponsePDU();
		for (int j=0;j<GetRspPDU_Length();j++)
		{
			mb_TCP_ResponseADU[7+j] = *(pResponsePDU+j);
		}

	}
	else
	{
		char* pException_rspPDU;
		pException_rspPDU = GetException_rspPDU();
		for (int j=0;j<GetRspPDU_Length();j++)
		{
			mb_TCP_ResponseADU[7+j] = *(pException_rspPDU+j);
		}
	}
	
}

/*				get modbus tcp response ADU								*/
char* CModbusTCP_Server::GetMBTCP_ResponseADU()
{
	return mb_TCP_ResponseADU;
}
