#pragma once
#include "modbus.h"
#define		ModbusSerialRTU_ADU_Max_Size		256

class CModbusSerialRTU :
	public CModbus
{
public:
	CModbusSerialRTU(void);
	~CModbusSerialRTU(void);
private:
	char				SlaveAddr;								//modbus serial RTU addr
	unsigned short		CRC16;									//CRC code (2 bytes)
	char				uchCRCHi;								//CRC16 high byte
	char				uchCRCLo;								//CRC16 low byte
	char				ADU_ModbusSerialRTU[ModbusSerialRTU_ADU_Max_Size];					//Modbus serial RTU ADU frame
	char				ResponseADU_ModbusSerialRTU[ModbusSerialRTU_ADU_Max_Size];			//response Modbus serial RTU ADU frame
	unsigned short		ResponseCRC16;							//response CRC CODE
	char				ResponseSlaveAddr;						//response slave addr
	DWORD				ResponseLen;							//response length
public:
	void				SetSlaveAddr(byte addrnum);				//set slave addr 
	byte				GetSlaveAddr();							//get slave addr
	void				CalCRC16();								//calculate CRC 16 by CRC generation function
	void				SetADU_ModbusSerRTU();					//combine addr,PDU,CRC16 to set ADU of modbus serial rtu frame
	char*				GetADU_ModbusSerRTU();					//get ADU of modbus serial rtu frame
	void				SetResponseADU_ModbusSerRTU(char* tBuf);		//set response ADU from received buffer
	void				SetResponseADUlength(DWORD recv_len);			//set response ADU length when receiving data
	char*				GetResponseADU_ModbusSerRTU();					//get response ADU
	bool				ResponseADU_Proc();								//processing response ADU and finish setting CRC16,SLAVEADDR,Response_PDU
	unsigned short		GetResponseCRC16();								//get response CRC16
	bool				IsResponseCRCWrong();							//judge if the response CRC is wrong
	char				GetResponseSlaveAddr();							//get response Slave Addr


};
