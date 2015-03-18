#include "StdAfx.h"
#include "ModbusSerial_RTU_Server.h"

CModbusSerial_RTU_Server::CModbusSerial_RTU_Server(void)
{
	LocalAddr = 0x01;
}

CModbusSerial_RTU_Server::~CModbusSerial_RTU_Server(void)
{
}


/*		set request ADU from received buffer		*/
void CModbusSerial_RTU_Server::SetRequestADU(char* pBuf,int len)
{
	//set request ADU
	for (int i=0;i<len;i++)
	{
		MB_SerialRTU_RequestADU[i] = *(pBuf+i);
	}
	//set CRC16
	RequestCRCLo = *(pBuf+len-2);		//CRC low byte
	RequestCRCHi = *(pBuf+len-1);		//CRC high byte
	RequestCRC16 = 0x0000;
	RequestCRC16 |= RequestCRCHi<<8;
	RequestCRC16 |= RequestCRCLo;
}



/*		set request ADU length for check request CRC16 code		*/
void CModbusSerial_RTU_Server::SetRequestADU_Length(int len)
{
	RequestADU_length = (unsigned short)len;
}

/*		cal and check if request CRC16 code is right			*/
bool CModbusSerial_RTU_Server::IsRequestCRC16Right()
{
	char* puchMsg;						//message to calculate CRC upon
	unsigned short usDataLen;			//quantity of bytes in message

	char uchCRCHi = 0xFF;				//high byte of CRC initialized
	char uchCRCLo = 0xFF;				//low byte of CRC initialized
	byte uIndex;						//will index into CRC lookup table
	
	puchMsg = MB_SerialRTU_RequestADU;
	usDataLen = RequestADU_length-2;	//exclude CRC16 2 bytes length

	//calculate CRC
	while(usDataLen--)
	{
		uIndex = uchCRCLo^*puchMsg++;		//cal the CRC
		uchCRCLo = uchCRCHi^auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
	}

	//check if request CRC16 is right
	if (uchCRCHi == RequestCRCHi && uchCRCLo == RequestCRCLo)
	{
		return true;
	}
	else
	{
		return false;
	}
}



/*		check if the reques slave addr is matching with local server(slave) addr		*/
bool CModbusSerial_RTU_Server::IsSlaveAddrRight()
{
	if (MB_SerialRTU_RequestADU[0] == LocalAddr)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*		set modbus request PDU									*/
void CModbusSerial_RTU_Server::SetRequestPDUfromADU()
{
	char* pReq_PDU;
	pReq_PDU = &MB_SerialRTU_RequestADU[1];		//get point to modbus request PDU
	SetRequestPDU(pReq_PDU);
}

/*		get modbus response PDU and slave address to cal response CRC16		*/
void CModbusSerial_RTU_Server::SetResponseADU()
{
	char* puchMsg;						//message to calculate CRC upon
	unsigned short usDataLen;			//quantity of bytes in message

	char uchCRCHi = 0xFF;				//high byte of CRC initialized
	char uchCRCLo = 0xFF;				//low byte of CRC initialized
	byte uIndex;						//will index into CRC lookup table

	char tempBuf[pdu_max_size+1];		//modbus response frame without 2bytes CRC16 code
	char* pReqPDU;						//the point to modbus server response PDU
	pReqPDU = GetResponsePDU();
	tempBuf[0] = LocalAddr;
	for (int j=0;j<GetRspPDU_Length();j++)
	{
		tempBuf[j+1] = *(pReqPDU+j);
	}

	puchMsg = tempBuf;
	unsigned short Length_ReqPDU;
	Length_ReqPDU = (unsigned short)GetRspPDU_Length();
	usDataLen = 1+Length_ReqPDU;
	//calculate CRC
	while(usDataLen--)
	{
		uIndex = uchCRCLo^*puchMsg++;		//cal the CRC
		uchCRCLo = uchCRCHi^auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
	}

	//set response CRC16
	ResponseCRCHi = uchCRCHi;
	ResponseCRCLo = uchCRCLo;
	ResponseCRC16 = 0x0000;
	ResponseCRC16 |= ResponseCRCHi<<8;
	ResponseCRC16 |= ResponseCRCLo;

	//set modbus serial rtu response ADU
	tempBuf[Length_ReqPDU+1] = ResponseCRCLo;
	tempBuf[Length_ReqPDU+2] = ResponseCRCHi;

	for (int i=0;i<Length_ReqPDU+3;i++)
	{
		MB_SerialRTU_ResponseADU[i] = tempBuf[i];
	}

}


/*		set response length for send				*/
void CModbusSerial_RTU_Server::SetResponseLength()
{
	ResponseADU_length = GetRspPDU_Length()+3;
}

char* CModbusSerial_RTU_Server::GetResponseADU()
{
	return MB_SerialRTU_ResponseADU;
}

unsigned short CModbusSerial_RTU_Server::GetResponseADU_length()
{
	return ResponseADU_length;
}