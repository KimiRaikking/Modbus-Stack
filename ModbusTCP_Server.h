#pragma once
#include "Modbus_Server.h"
#define   ModbusTCP_ADU_Max_Size           260
class CModbusTCP_Server :
	public CModbus_Server
{
public:
	CModbusTCP_Server(void);
	~CModbusTCP_Server(void);
private:
	char mb_TCP_RequestADU[ModbusTCP_ADU_Max_Size];							//Modbus TCP request ADU
	char mb_TCP_RequestMBAP[7];												//modbus tcp request MBAP
	char local_identifier;													//local unit identifier(default 0x01)
	char Unit_Identifier;													//modbus tcp request(response if match with local unit identifier) unit identifier(slave address)
	char mb_TCP_ResponseADU[ModbusTCP_ADU_Max_Size];						//modbus tcp response ADU
	char mb_TCP_ResponseMBAP[7];											//Modbus tcp response MBAP

public:
	void SetMBTCP_RequestADU(char* pReq_ADU);										//set MODBUS TCP REQUEST ADU
	char* GetMBTCP_RequestADU();													//get modbus tcp request ADU
	void SetMBTCP_RequestMBAP(char* pReq_ADU);										//set modbus tcp request MBAP
	char* GetMBTCP_RequestMBAP();													//get modbus tcp request MBAP
	bool CheckRequestMBAP();														//check request MBAP,if correct return true,or return false
	void SetRequestPDUfromADU(char* pReq_ADU);										//set modbus tcp request PDU 
	void SetLocalUnitIdentifier(char Local_id);										//set local unit identifier
	char GetLocalUnitIdentifier();													//get local unit identifier
	void SetUnitIdentifier(char unit_identifier);									//set unit identifier
	char GetUnitIdentifier();														//get unit identifier
	void SetMBTCP_ResponseMBAP();													//set modbus tcp response MBAP
	char* GetMBTCP_ResponseMBAP();													//get modbus tcp response MBAP
	void SetMBTCP_ResponseADU();													//set modbus tcp response ADU
	char* GetMBTCP_ResponseADU();													//get modbus tcp response ADU
};

