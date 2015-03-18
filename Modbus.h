#include <vector>
using std::vector;
#define     pdu_max_size     253                                      //the size for modbus PDU

#define Char2WChar(lpMultiByteStr,cbMultiByte,lpWideCharStr,cbWideChar) \
	MultiByteToWideChar(CP_ACP,0,lpMultiByteStr,cbMultiByte,lpWideCharStr,cbWideChar)

#define WChar2Char(lpWideCharStr,cbWideChar,lpMultiByteStr,cbMultiByte) \
	WideCharToMultiByte(CP_ACP,0,lpWideCharStr,cbWideChar,lpMultiByteStr,cbMultiByte,NULL,NULL)//用于CString转换为char[]
#pragma once
class CModbus
{
public:
	CModbus(void);
	~CModbus(void);
protected:
	char                PDU[pdu_max_size];										 //define the PDU buf
	char				Response_PDU[pdu_max_size];								 //define the response PDU buf
	char                Response_Exception[2];                                   //define the response exception
	byte				Function_Code;                                           //define the function code
	bool				flag_endian;                                             //to judge big endian or little endian
	unsigned short      length_pdu;                                              //the length of pdu
	unsigned short      reg_addr;												 //the register address
	
public:
	char *				GetPDU();                                                //get PDU
	bool				SetPDU(char* Ppdu);										 //set PDU
	unsigned short      GetLengthPDU();											 //get length_pdu
	char *				GetResponse_PDU();										 //get response pdu
	bool				SetRegAddr(unsigned short addr);						 //set register address
	unsigned short		GetRegAddr();											 //get register address
	byte				GetFunctionCode();										 //;process the response pdu and get the function code
	char *				GetResponse_Exception();								 //get response exception code
	bool				FC01_encode(unsigned short addr_start,unsigned short quantity_coils);                //FC01,read coils
	bool				FC02_encode(unsigned short addr_start,unsigned short quantity_inputs);				 //FC02,read discrete inputs
	bool				FC03_encode(unsigned short addr_start,unsigned short quantity_registers);			 //FC03,read holding registers
	bool				FC04_encode(unsigned short addr_start,unsigned short quantity_input_registers);		 //FC04,read input registers
	bool				FC05_encode(unsigned short addr_output,unsigned short output_value);				 //FC05,write single coil
	bool				FC06_encode(unsigned short addr_register,unsigned short register_value);			 //FC06,write single register(16-bit unsigned integer)
	bool				FC06_encode(unsigned short addr_register,short register_value);						 //FC06,write single register(16-bit signed integer)
	bool				FC15_encode(unsigned short addr_start,unsigned short quantity_outputs,vector<byte>var_data);		//FC15,write multiple coils
	bool				FC16_encode(unsigned short addr_start,unsigned short quantity_registers,vector<unsigned short>var_data);		//FC16,write multiple registers
	bool				FC16_encode(unsigned short addr_start,unsigned short quantity_registers,vector<short>var_data);		//FC16,write multiple registers
	bool				FC16_encode(unsigned short addr_start,unsigned short quantity_registers,vector<float>var_data);		//FC16,write multiple registers
	bool				FC16_encode(unsigned short addr_start,CString str_data);		//FC16,write multiple registers

	vector< byte >				FC01_decode();                                           //FC01,decode and return all coils' value by type of byte
	vector< byte >				FC02_decode();											 //FC02,decode and return all discrete inputs' value by type of byte
	vector< unsigned short >    FC03_UShort_decode();									 //FC03,decode and return holding regs as unsigned short[]
	vector< short >				FC03_Short_decode();									 //FC03,decode and return holding regs as short[]
	vector< float >				FC03_Float_decode();									 //FC03,decode and return holding regs as float[]
	CString						FC03_CString_decode();									 //FC03,decode and return holding regs as CString
	vector< unsigned short >    FC04_UShort_decode();									 //FC04,decode and return input regs as unsigned short[]
	vector< short >				FC04_Short_decode();									 //FC04,decode and return input regs as short[]
	vector< float >				FC04_Float_decode();									 //FC04,decode and return input regs as float[]
	CString						FC04_CString_decode();									 //FC04,decode and return input regs as CString
	unsigned short				FC05_decode();											 //FC05,decode and return output address
	unsigned short				FC06_UShort_decode();									 //FC06,decode and return register value as unsigned short
	short						FC06_Short_decode();									 //FC06,decode and return register value as short
	unsigned short				FC15_decode();											 //FC15,decode and return starting address
	unsigned short				FC16_decode();											 //FC16,decode and return starting address
};

