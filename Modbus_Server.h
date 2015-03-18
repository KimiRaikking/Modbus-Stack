/*					Note															*/
/*	This is a class for Modbus Server(Modbus slave)									*/
/*	If you want to see Modbus Client(Modbus Master),please see class of CModbus		*/
/*	Created Date:	2014/5/7														*/
/*	Author:			Kimi Zhang														*/
/*	Addr:			SEU(Tide)														*/


#include <vector>
using std::vector;
#define     pdu_max_size     253                                      //the size for modbus PDU

#define Char2WChar(lpMultiByteStr,cbMultiByte,lpWideCharStr,cbWideChar) \
	MultiByteToWideChar(CP_ACP,0,lpMultiByteStr,cbMultiByte,lpWideCharStr,cbWideChar)

#define WChar2Char(lpWideCharStr,cbWideChar,lpMultiByteStr,cbMultiByte) \
	WideCharToMultiByte(CP_ACP,0,lpWideCharStr,cbWideChar,lpMultiByteStr,cbMultiByte,NULL,NULL)//用于CString转换为char[]
#pragma once

// CModbus_Server command target



class CModbus_Server : public CObject
{
public:
	CModbus_Server();
	virtual ~CModbus_Server();

private:
	/*			define 4 separate data blocks for Modbus data model					*/
	/*	this four data blocks are used for store responding data in memory			*/
	vector<byte>Discretes_Inputs;						//data block for discretes input,supported for FC 02,one byte means one bit(one discrete input)
	vector<byte>Coils;									//data block for Coils,Supported for FC 01,05,15,one byte means one bit(one discrete input)
	vector<int>Input_regs;								//data block for Input Registers,supported for FC 04
	vector<int>Holding_regs;							//data block for Holding Registers,supported for FC 03,06,16

	/*			define response PDU ,exception response,request PDU							*/
	char MB_Response_PDU[pdu_max_size];						//modbus response PDU
	char MB_Exception_rsp_PDU[pdu_max_size];				//modbus exception response PDU
	char MB_Request_PDU[pdu_max_size];						//modbus request pdu
	char Length_Rsp_PDU;									//modbus response pdu length,no mattter mb_rsp or mb_exception_rsp

protected:
	bool IsResponsepduCorrect;								//flag for judging if response pdu is ok,not exception_rsp;
public:
	void	SetRequestPDU(char* pRequestPDU);				//set request PDU
	char*	GetRequestPDU();								//get request PDU	
	char	GetRequestPDU_FCcode();							//get function code of request pdu 
	char*	GetResponsePDU();								//get mb_response PDU
	char*	GetException_rspPDU();							//get mb_exception_rsp PDU
	void	SetRspPDU_Length(char length_pdu);				//set modbus response PDU length
	char	GetRspPDU_Length();								//get modbus response PDU length
	/*		define client request modbus PDU function								*/
	/*	first,analyze and judge if function code supported							*/
	/*	if ok, pass params to responding FC process function,like FC01_Proc()		*/
	/*	if not,build exception response PDU 										*/
	/*	return value: if ok,return true,if not, return false						*/
	bool	MB_RequestPDU_Proc();



	/*			define client(master) request process function						*/
	/*	Range from FC 01,02,03,04,05,06,15,16(DEC)									*/
	/*	first,analyze client modbus request											*/
	/*	judge if request is OK														*/
	/*  second,process client request												*/	
	/*	third,if request ok,build modbus response;if not,build exception code		*/
	/*	last,return true if ok, or return false if not	and cal rsp PDU length		*/

	bool	FC01_Proc(unsigned short addr_start,unsigned short quantity_coils);													//read coils
	bool	FC02_Proc(unsigned short addr_start,unsigned short quantity_inputs);												//read discrete inputs
	bool	FC03_Proc(unsigned short addr_start,unsigned short quantity_regs);													//read holding registers
	bool	FC04_Proc(unsigned short addr_start,unsigned short quantity_input_regs);											//read input registers
	bool	FC05_Proc(unsigned short output_addr,unsigned short output_value);													//write single coil
	bool	FC06_Proc(unsigned short register_addr,unsigned short register_value);												//write single register
	bool	FC15_Proc(unsigned short addr_start,unsigned short quantity_outputs,char byte_count,char* outputs_value);			//write multiple coils
	bool	FC16_Proc(unsigned short addr_start,unsigned short quantity_regs,char byte_count,char*	regs_value);				//write multiple registers

	

	



};


