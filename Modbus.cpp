#include "StdAfx.h"
#include "Modbus.h"


CModbus::CModbus(void)
{
	//initialize
	ZeroMemory(PDU,sizeof(PDU));
	length_pdu = 0;
	reg_addr = 0;
	//test system is little endian or big endian
	union packet
	{	
		int a;
		char b;
	}test;
	test.a=1;
	if (test.b==1)
	{
		flag_endian = false;             //little endian
	}
	else
		flag_endian = true;				//big endian

}


CModbus::~CModbus(void)
{
}
char* CModbus::GetPDU()
{
	return PDU;
}
bool CModbus::SetPDU(char* Ppdu)
{
	for (int i=0;i<pdu_max_size;i++)
	{
		PDU[i] = *(Ppdu+i);
	}
	return true;
}
unsigned short CModbus::GetLengthPDU()
{
	return length_pdu;
}
char * CModbus::GetResponse_PDU()
{
	return Response_PDU;
}
bool CModbus::SetRegAddr(unsigned short addr)
{
	reg_addr = addr;
	return true;
}
unsigned short CModbus::GetRegAddr()
{
	return reg_addr;
}
byte CModbus::GetFunctionCode()
{
	
	Function_Code = Response_PDU[0];
	return Function_Code;
}
char * CModbus::GetResponse_Exception()
{
	return Response_Exception;
}
bool CModbus::FC01_encode(unsigned short addr_start,unsigned short quantity_coils)
{
	char tBuf[5];
	tBuf[0] =0x01;                                   //function code 0x01
	tBuf[1] =addr_start>>8;                         //set the high byte of starting address,try to change character set
	tBuf[2] =addr_start&0x00FF;                     //set the low  byte of starting address
	if(quantity_coils > 0x7D0)                       //0x7D0 max length
	{
		return FALSE;
	}
	else
	{
		tBuf[3] = quantity_coils >>8;                  //set the high byte of quantity
		tBuf[4] = quantity_coils&0x00FF;               //set the low byte of quantity
		//unsigned short tt = atoi(&tBuf[11]);      //atoi()function is to use when the char[] only includes numbers,which means every single char is just a number
		for(int j=0;j<5;j++) PDU[j] = tBuf[j]; 
		length_pdu = 5;
		return TRUE;
	}
}
bool CModbus::FC02_encode(unsigned short addr_start,unsigned short quantity_inputs)
{
	char tBuf[5];
	tBuf[0] =0x02;                                   //function code 0x01
	tBuf[1] =addr_start>>8;                         //set the high byte of starting address,try to change character set
	tBuf[2] =addr_start&0x00FF;                     //set the low  byte of starting address
	if(quantity_inputs > 0x7D0)                       //0x7D0 max length

	{
		return FALSE;
	}
	else
	{
		tBuf[3] = quantity_inputs >>8;                    //set the high byte of quantity
		tBuf[4] = quantity_inputs & 0x00FF;               //set the low byte of quantity
		//unsigned short tt = atoi(&tBuf[11]);      //atoi()function is to use when the char[] only includes numbers,which means every single char is just a number
		for(int j=0;j<5;j++) PDU[j] = tBuf[j]; 
		length_pdu = 5;
		return TRUE;
	}
}
bool CModbus::FC03_encode(unsigned short addr_start,unsigned short quantity_registers)
{
	char tBuf[5];
	                                //unit identifier,must match with device number
	tBuf[0] =0x03;                                   //function code 0x03
	tBuf[1] =addr_start>>8;                         //set the high byte of starting address
	tBuf[2] =addr_start & 0x00FF;                     //set the low  byte of starting address
	if(quantity_registers > 0x7D)                   //0x7D max length
	{
		return FALSE;
	}
	else
	{
		tBuf[3] = quantity_registers >>8;                  //set the high byte of quantity
		tBuf[4] = quantity_registers & 0x00FF;               //set the low byte of quantity
		for(int j=0;j<5;j++) PDU[j] = tBuf[j]; 
		length_pdu = 5;
		return TRUE;
	}
}
bool CModbus::FC04_encode(unsigned short addr_start,unsigned short quantity_input_registers)
{
	char tBuf[5];
	//unit identifier,must match with device number
	tBuf[0] =0x04;                                   //function code 0x03
	tBuf[1] =addr_start>>8;                         //set the high byte of starting address
	tBuf[2] =addr_start & 0x00FF;                     //set the low  byte of starting address
	if(quantity_input_registers > 0x7D)                   //0x7D max length
	{
		return FALSE;
	}
	else
	{
		tBuf[3] = quantity_input_registers >>8;                  //set the high byte of quantity
		tBuf[4] = quantity_input_registers & 0x00FF;               //set the low byte of quantity
		for(int j=0;j<5;j++) PDU[j] = tBuf[j]; 
		length_pdu =5;
		return TRUE;
	}
}
bool CModbus::FC05_encode(unsigned short addr_output,unsigned short output_value)
{
	char tBuf[5];
	tBuf[0] =0x05;                                   //function code 0x05
	tBuf[1] =addr_output>>8;                         //set the high byte of output address
	tBuf[2] =addr_output&0x00FF;                     //set the low  byte of output address
	if(output_value==0x0000 ||output_value==0xFF00)                   //0x7D max length
	{

		tBuf[3] = output_value >>8;                  //set the high byte of quantity
		tBuf[4] = output_value&0x00FF;               //set the low byte of quantity
		for(int j=0;j<5;j++) PDU[j] = tBuf[j]; 
		length_pdu =5;
		return TRUE;

	}
	else
	{
		return FALSE;
	}
}
bool CModbus::FC06_encode(unsigned short addr_register,unsigned short register_value)
{
	char tBuf[5];
	tBuf[0] =0x06;                                     //function code 0x05
	tBuf[1] =addr_register>>8;                         //set the high byte of output address
	tBuf[2] =addr_register&0x00FF;                     //set the low  byte of output address,maybe i shoud minus 0x01
	tBuf[3] = register_value >>8;                      //set the high byte of register
	tBuf[4] = register_value&0x00FF;                   //set the low byte of register
	for(int j=0;j<5;j++) PDU[j] = tBuf[j]; 
	length_pdu =5;
	return TRUE;
}
bool CModbus::FC06_encode(unsigned short addr_register,short register_value)
{

	char tBuf[5];
	tBuf[0] =0x06;                                     //function code 0x05
	tBuf[1] =addr_register>>8;                         //set the high byte of output address
	tBuf[2] =addr_register&0x00FF;                     //set the low  byte of output address,maybe i shoud minus 0x01
	tBuf[3] = register_value >>8;                      //set the high byte of register
	tBuf[4] = register_value&0x00FF;                   //set the low byte of register
	for(int j=0;j<5;j++) PDU[j] = tBuf[j]; 
	length_pdu =5;
	return TRUE;
}
bool CModbus::FC15_encode(unsigned short addr_start,unsigned short quantity_outputs,vector<byte>var_data)
{
	byte byte_count;
	if((quantity_outputs%8)==0)                             //get the byte count
		byte_count = quantity_outputs/8;
	else
		byte_count = quantity_outputs/8 +1;
	int count_pdu;                                          //get the pdu count
	count_pdu = 6+ byte_count;
	vector< char > tBuf(count_pdu);                         //define the pdu tmp buf
	tBuf[0] =0x0F;                                          //function code 0x0f
	tBuf[1] = addr_start>>8;								//set the high byte of start address
	tBuf[2] = addr_start&0x00FF;						    //set the low  byte of start address
	tBuf[3] = quantity_outputs >>8;						    //set the high byte of quantity
	tBuf[4] = quantity_outputs&0x00FF;					    //set the low byte of quantity
	tBuf[5] = byte_count;								    //set the byte count
	for(int j=0;j<byte_count;j++)                           //set the value
	{
		tBuf[6+j] = var_data[j];
	}
	for(int j=0;j<count_pdu;j++)                            //set the PDU
	{
		PDU[j] = tBuf[j];
	}
	length_pdu = count_pdu;
	return TRUE;
}
bool CModbus::FC16_encode(unsigned short addr_start,unsigned short quantity_registers,vector<unsigned short>var_data)
{
	byte byte_count;
	byte_count = quantity_registers * 2;                           //the byte count is double of quantity of registers
	int count_pdu;
	count_pdu = 6 + byte_count;                                 //set the pdu count
	vector < char > tBuf(count_pdu);                             //define the pdu tmp buf
	tBuf[0] = 0x10;                                            //function code 0x10(16)
	tBuf[1] =addr_start>>8;                         //set the high byte of start address
	tBuf[2] =addr_start&0x00FF;                     //set the low  byte of start address
	tBuf[3] = quantity_registers >>8;                  //set the high byte of quantity_registers
	tBuf[4] = quantity_registers&0x00FF;               //set the low byte of quantity_registers
	tBuf[5] = byte_count;                              //set the byte count
	for(int j=0;j<(byte_count/2);j++)
	{
		tBuf[6+2*j] = var_data[j]>>8;                     //set the high byte of register value
		tBuf[7+2*j] = var_data[j]&0x00FF;                 //set the low byte of register value
	}
	for(int i=0;i<count_pdu;i++)
	{
		PDU[i] = tBuf[i];
	}
	length_pdu =count_pdu;
	return true;
}
bool CModbus::FC16_encode(unsigned short addr_start,unsigned short quantity_registers,vector<short>var_data)
{
	byte byte_count;
	byte_count = quantity_registers * 2;                           //the byte count is double of quantity of registers
	int count_pdu;
	count_pdu = 6 + byte_count;                                 //set the pdu count
	vector < char > tBuf(count_pdu);                             //define the pdu tmp buf
	tBuf[0] = 0x10;                                            //function code 0x10(16)
	tBuf[1] =addr_start>>8;                         //set the high byte of start address
	tBuf[2] =addr_start&0x00FF;                     //set the low  byte of start address
	tBuf[3] = quantity_registers >>8;                  //set the high byte of quantity_registers
	tBuf[4] = quantity_registers&0x00FF;               //set the low byte of quantity_registers
	tBuf[5] = byte_count;                              //set the byte count
	for(int j=0;j<(byte_count/2);j++)
	{
		tBuf[6+2*j] = var_data[j]>>8;                     //set the high byte of register value
		tBuf[7+2*j] = var_data[j]&0x00FF;                 //set the low byte of register value
	}
	for(int i=0;i<count_pdu;i++)
	{
		PDU[i] = tBuf[i];
	}
	length_pdu = count_pdu;
	return true;
}
bool CModbus::FC16_encode(unsigned short addr_start,CString str_data)
{
	byte byte_count;
	byte_count = str_data.GetLength();                                              //get the count value of string
	unsigned short quantity_registers;
	if (byte_count%2)
	{
		quantity_registers = byte_count/2;
	}
	else
		quantity_registers = byte_count/2+1;
	char* buf_temp;
	ZeroMemory(buf_temp,byte_count);
	WChar2Char(str_data,-1,buf_temp,byte_count);                                    //string convert to char[]
	int count_pdu;
	count_pdu = 6 + byte_count;                                 //set the pdu count
	vector < char > tBuf(count_pdu);                             //define the pdu tmp buf
	tBuf[0] = 0x10;                                            //function code 0x10(16)
	tBuf[1] =addr_start>>8;                         //set the high byte of start address
	tBuf[2] =addr_start&0x00FF;                     //set the low  byte of start address
	tBuf[3] = quantity_registers >>8;                  //set the high byte of quantity_registers
	tBuf[4] = quantity_registers&0x00FF;               //set the low byte of quantity_registers
	tBuf[5] = byte_count;                              //set the byte count
	for (int j=0;j<byte_count;j++)
	{
		tBuf[6+j] = *(buf_temp+j);
	}
	for (int i=0;i<count_pdu;i++)
	{
		PDU[i] = tBuf[i];
	}
	length_pdu = count_pdu;
	return true;
}
bool CModbus::FC16_encode(unsigned short addr_start,unsigned short quantity_registers,vector<float>var_data)
{
	byte  byte_count;
	byte_count = var_data.size()*4;
	int count_pdu;
	count_pdu = 6 + byte_count;                                         //set the pdu count
	vector < char > tBuf(count_pdu);									//define the pdu tmp buf
	tBuf[0] = 0x10;														//function code 0x10(16)
	tBuf[1] =addr_start>>8;												//set the high byte of start address
	tBuf[2] =addr_start&0x00FF;											//set the low  byte of start address
	tBuf[3] = quantity_registers >>8;									//set the high byte of quantity_registers
	tBuf[4] = quantity_registers&0x00FF;								//set the low byte of quantity_registers
	tBuf[5] = byte_count;												//set the byte count
	for (int j=0;j<var_data.size();j++)
	{
		union packet_convert
		{
			float a;
			char  b[4];
		}test;
		test.a = var_data[j];
		if (flag_endian)                                                //default set:remote device is big endian;
		{
			for (int k=0;k<4;k++)                                       //big endian to big endian
			{
				tBuf[6+4*j+k] =test.b[k];
			}
		}
		else
		{
			for (int k=0;k<4;k++)										//little endian to big endian
			{
				tBuf[6+4*j+k] = test.b[3-k];
			}
		}
	}
	for(int i=0;i<count_pdu;i++)
	{
		PDU[i] = tBuf[i];
	}
	length_pdu = count_pdu;
	return true;
}


vector< byte > CModbus::					FC01_decode()
{
	
	byte byte_count;
	byte_count = Response_PDU[1];
	vector< byte >val_vector(byte_count);
	for (int i=0;i<byte_count;i++)
	{
		val_vector[i] = Response_PDU[2+i];
	}
	return val_vector;

}
vector< byte > CModbus::					FC02_decode()
{
	byte byte_count;
	byte_count = Response_PDU[1];
	vector< byte >val_vector(byte_count);
	for (int i=0;i<byte_count;i++)
	{
		val_vector[i] = Response_PDU[2+i];
	}
	return val_vector;
}
vector< unsigned short > CModbus::			FC03_UShort_decode()
{
	byte Byte_count;
	Byte_count = Response_PDU[1];                                     //get the byte count
	vector< unsigned short > val_vector(Byte_count/2);
	for (int i=0;i<(Byte_count/2);i++)									//get the value list
	{
		val_vector[i] = 0;
		val_vector[i] += (Response_PDU[2*i+2]<<8);
		val_vector[i] += (Response_PDU[2*i+3]&0x00ff);
	}
	return val_vector;
}
vector< short > CModbus::					FC03_Short_decode()
{
	byte Byte_count;
	Byte_count = Response_PDU[1];                                     //get the byte count
	vector< short > val_vector(Byte_count/2);
	for (int i=0;i<(Byte_count/2);i++)									//get the value list
	{
		val_vector[i] = 0;
		val_vector[i] += (Response_PDU[2*i+2]<<8);
		val_vector[i] += (Response_PDU[2*i+3]&0x00FF);
	}
	return val_vector;
}
vector< float > CModbus::					FC03_Float_decode()
{
	byte Byte_count;
	Byte_count = Response_PDU[1];                                     //get the byte count
	vector< float > val_vector(Byte_count/4);                         //define the float vector,4bytes = 1 float
	for (int i=0;i<Byte_count/4;i++)
	{
		union packet_convert
		{
			char a[4];
			float b;
		}test;
		if (flag_endian)
		{
			for (int j=0;j<4;j++)
			{
				test.a[j] = Response_PDU[i*4+2+j];
			}
		}
		else
		{
			for (int k=0;k<4;k++)
			{
				test.a[k] = Response_PDU[i*4+5-k];
			}
		}
		val_vector[i] = test.b;
	}
	return val_vector;
}
CString CModbus::							FC03_CString_decode()
{
	byte Byte_count;
	Byte_count = Response_PDU[1];                                     //get the byte count
	char* char_str;
	for (int i=0;i<Byte_count;i++)
	{
		*(char_str+i) = Response_PDU[2+i];
	}
	CString val_string;
	val_string = char_str;
	return val_string;
}
vector< unsigned short > CModbus::			FC04_UShort_decode()
{
	byte Byte_count;
	Byte_count = Response_PDU[1];                                     //get the byte count
	vector< unsigned short > val_vector(Byte_count/2);
	for (int i=0;i<(Byte_count/2);i++)									//get the value list
	{
		val_vector[i] = 0;
		val_vector[i] += (Response_PDU[2*i+2]<<8);
		val_vector[i] += (Response_PDU[2*i+3]&0x00FF);
	}
	return val_vector;
}
vector< short > CModbus::					FC04_Short_decode()
{
	byte Byte_count;
	Byte_count = Response_PDU[1];                                     //get the byte count
	vector< short > val_vector(Byte_count/2);
	for (int i=0;i<(Byte_count/2);i++)									//get the value list
	{
		val_vector[i] = 0;
		val_vector[i] += (Response_PDU[2*i+2]<<8);
		val_vector[i] += (Response_PDU[2*i+3]&0x00FF);
	}
	return val_vector;
}
vector< float > CModbus::					FC04_Float_decode()
{
	byte Byte_count;
	Byte_count = Response_PDU[1];                                     //get the byte count
	vector< float > val_vector(Byte_count/4);                         //define the float vector,4bytes = 1 float
	for (int i=0;i<Byte_count/4;i++)
	{
		union packet_convert
		{
			char a[4];
			float b;
		}test;
		if (flag_endian)
		{
			for (int j=0;j<4;j++)
			{
				test.a[j] = Response_PDU[i*4+2+j];
			}
		}
		else
		{
			for (int k=0;k<4;k++)
			{
				test.a[k] = Response_PDU[i*4+5-k];
			}
		}
		val_vector[i] = test.b;
	}
	return val_vector;
}
CString CModbus::							FC04_CString_decode()
{
	byte Byte_count;
	Byte_count = Response_PDU[1];                                     //get the byte count
	char* char_str;
	for (int i=0;i<Byte_count;i++)
	{
		*(char_str+i) = Response_PDU[2+i];
	}
	CString val_string;
	val_string = char_str;
	return val_string;
}
unsigned short CModbus::					FC05_decode()
{
	unsigned short val_addr;
	val_addr = 0;
	val_addr += (Response_PDU[1]<<8);
	val_addr += (Response_PDU[2]&0x00FF);
	return val_addr;
}
unsigned short CModbus::					FC06_UShort_decode()
{
	unsigned short val_reg;
	val_reg = 0;
	val_reg += (Response_PDU[3]<<8);
	val_reg += (Response_PDU[4]&0x00FF);
	reg_addr = 0;
	reg_addr +=Response_PDU[1]<<8;
	reg_addr +=Response_PDU[2]&0x00FF;
	return val_reg;
}
short CModbus::								FC06_Short_decode()
{
	short val_reg;
	val_reg = 0;
	val_reg += (Response_PDU[3]<<8);
	val_reg += (Response_PDU[4]&0x00FF);
	reg_addr = 0;
	reg_addr +=Response_PDU[1]<<8;
	reg_addr +=Response_PDU[2]&0x00FF;
	return val_reg;
}
unsigned short CModbus::					FC15_decode()
{
	unsigned short val_count;
	val_count = 0;
	val_count += (Response_PDU[3]<<8);
	val_count += (Response_PDU[4]&0x00FF);
	reg_addr = 0;
	reg_addr +=Response_PDU[1]<<8;
	reg_addr +=Response_PDU[2]&0x00FF;

	return val_count;
}
unsigned short CModbus::					FC16_decode()
{
	unsigned short val_count;
	val_count = 0;
	val_count += (Response_PDU[3]<<8);
	val_count += (Response_PDU[4]&0x00FF);
	reg_addr = 0;
	reg_addr  += Response_PDU[1]<<8;
	reg_addr  += Response_PDU[2]&0x00FF;

	return val_count;
}