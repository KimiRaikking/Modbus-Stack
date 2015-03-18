// Modbus_Server.cpp : implementation file
//

#include "stdafx.h"
#include "TCP_Server1.h"
#include "Modbus_Server.h"


// CModbus_Server

CModbus_Server::CModbus_Server()
{
	//init data block for test only
	for (int i=0;i<10;i++)
	{
		Discretes_Inputs.push_back(0x01);
		Coils.push_back(0x00);
	}
	for (int i=0;i<15;i++)
	{
		Discretes_Inputs.push_back(0x00);
		Coils.push_back(0x01);
	}
	for (int i=0;i<20;i++)
	{
		Input_regs.push_back(i);
		Holding_regs.push_back(i);
	}

	//init variables
	IsResponsepduCorrect = false;
}

CModbus_Server::~CModbus_Server()
{
}


// CModbus_Server member functions
/*		set request PDU															*/
void CModbus_Server::SetRequestPDU(char* pRequestPDU)
{
	for (int i=0;i<pdu_max_size;i++)
		MB_Request_PDU[i] = *(pRequestPDU+i);
}

/*		get request PDU															*/
char* CModbus_Server::GetRequestPDU()
{
	return MB_Request_PDU;
}

/*		get function code of request											*/
char CModbus_Server::GetRequestPDU_FCcode()
{
	char FC_code;
	FC_code = MB_Request_PDU[0];				//get request function code
	return FC_code;
}

/*		get mb_response PDU														*/
char* CModbus_Server::GetResponsePDU()
{
	return MB_Response_PDU;
}

/*		get mb_exception_rsp PDU												*/
char* CModbus_Server::GetException_rspPDU()
{
	return MB_Exception_rsp_PDU;
}

/*		set modbus response PDU length											*/
void CModbus_Server::SetRspPDU_Length(char length_pdu)
{
	Length_Rsp_PDU = length_pdu;
}

/*		get modbus response PDU length											*/
char CModbus_Server::GetRspPDU_Length()
{
	return Length_Rsp_PDU;
}

/*		mb_request PDU processing and handling function							*/
/*	first,analyze and judge if function code supported							*/
/*	if ok, pass params to responding FC process function,like FC01_Proc()		*/
/*	if not,build exception response PDU 										*/
/*	return value: if ok,return true,if not, return false						*/
bool CModbus_Server::MB_RequestPDU_Proc()
{
	char fc_code;
	fc_code = GetRequestPDU_FCcode();
	//first,judge if the stack support this function code
	if (fc_code==0x01 || fc_code==0x02 || fc_code==0x03 ||fc_code==0x04 || fc_code==0x05 ||fc_code==0x06 || fc_code==0x0F || fc_code==0x10)
	{
		//if true,function code supported
		//now according to function code,process request PDU and pass params to responding FC code process function
		switch(fc_code)
		{
			case 0x01:					//function code 0x01
				{
					unsigned short addr_start,quantity_coils;
					addr_start = MB_Request_PDU[1]<<8;			//starting address Hi
					addr_start |= MB_Request_PDU[2];			//starting address Lo
					quantity_coils = MB_Request_PDU[3]<<8;		//quantity of coils Hi
					quantity_coils |= MB_Request_PDU[4];		//quantity of coils Lo
					//pass params to function FC01_Proc(),judge if request process is ok
					if (FC01_Proc(addr_start,quantity_coils)) 
					{
						IsResponsepduCorrect = true;
						return true;

					}
					else	
					{
						IsResponsepduCorrect = false;
						return false;
					}
					break;
				}
			case 0x02:					//function code 0x02
				{
					unsigned short addr_start,quantity_inputs;
					addr_start = MB_Request_PDU[1]<<8;			//starting address Hi
					addr_start |= MB_Request_PDU[2];			//starting address Lo
					quantity_inputs = MB_Request_PDU[3]<<8;		//quantity of inputs Hi
					quantity_inputs |= MB_Request_PDU[4];		//quantity of inputs Lo
					//pass params to function FC02_Proc(),judge if request process is ok
					if (FC02_Proc(addr_start,quantity_inputs)) 
					{
						IsResponsepduCorrect = true;
						return true;

					}
					else	
					{
						IsResponsepduCorrect = false;
						return false;
					}
					break;
				}
			case 0x03:					//function code 0x03
				{
					unsigned short addr_start,quantity_regs;
					addr_start = MB_Request_PDU[1]<<8;			//starting address Hi
					addr_start |= MB_Request_PDU[2];			//starting address Lo
					quantity_regs = MB_Request_PDU[3]<<8;		//quantity of holding registers Hi
					quantity_regs |= MB_Request_PDU[4];			//quantity of holding registers Lo
					//pass params to function FC03_Proc(),judge if request process is ok
					if (FC03_Proc(addr_start,quantity_regs)) 
					{
						IsResponsepduCorrect = true;
						return true;

					}
					else	
					{
						IsResponsepduCorrect = false;
						return false;
					}
					break;
				}
			case 0x04:					//function code 0x04
				{
					unsigned short addr_start,quantity_InputRegs;
					addr_start = MB_Request_PDU[1]<<8;					//starting address Hi
					addr_start |= MB_Request_PDU[2];					//starting address Lo
					quantity_InputRegs = MB_Request_PDU[3]<<8;			//quantity of input registers Hi
					quantity_InputRegs |= MB_Request_PDU[4];			//quantity of input registers Lo
					//pass params to function FC04_Proc(),judge if request process is ok
					if(FC04_Proc(addr_start,quantity_InputRegs)) 
					{
						IsResponsepduCorrect = true;
						return true;

					}
					else	
					{
						IsResponsepduCorrect = false;
						return false;
					}
					break;
				}
			case 0x05:					//function code 0x05
				{
					unsigned short addr_output,output_value;
					addr_output = MB_Request_PDU[1]<<8;					//output address Hi
					addr_output |= MB_Request_PDU[2];					//output address Lo
					output_value = MB_Request_PDU[3]<<8;				//output value Hi
					output_value |= MB_Request_PDU[4];					//output value Lo
					//pass params to function FC05_Proc(),judge if request process is ok
					if(FC05_Proc(addr_output,output_value)) 
					{
						IsResponsepduCorrect = true;
						return true;

					}
					else	
					{
						IsResponsepduCorrect = false;
						return false;
					}
					break;
				}
			case 0x06:					//function code 0x06
				{
					unsigned short addr_reg,reg_value;
					addr_reg = MB_Request_PDU[1]<<8;					//register address Hi
					addr_reg |=	MB_Request_PDU[2];						//register address Lo
					reg_value = MB_Request_PDU[3]<<8;					//register value Hi
					reg_value |= MB_Request_PDU[4];						//register value Lo
					//pass params to function FC06_Proc(),judge if request process is ok
					if(FC06_Proc(addr_reg,reg_value))
					{
						IsResponsepduCorrect = true;
						return true;

					}
					else	
					{
						IsResponsepduCorrect = false;
						return false;
					}
					break;
				}
			case 0x0F:					//function code 0x0F
				{
					unsigned short addr_start,quantity_outputs;
					char byte_counts;
					char*	pOutputsValue;
					addr_start = MB_Request_PDU[1]<<8;						//starting address Hi
					addr_start |= MB_Request_PDU[2];						//starting address Lo
					quantity_outputs = MB_Request_PDU[3]<<8;				//quantity of outputs Hi
					quantity_outputs |= MB_Request_PDU[4];					//quantity of outputs Lo
					byte_counts = MB_Request_PDU[5];						//byte count
					pOutputsValue = &MB_Request_PDU[6];						//point to outputs value
					//pass params to function FC15_Proc(),judge if request process is ok
					if (FC15_Proc(addr_start,quantity_outputs,byte_counts,pOutputsValue))	
					{
						IsResponsepduCorrect = true;
						return true;

					}
					else	
					{
						IsResponsepduCorrect = false;
						return false;
					}
					break;
				}
			case 0x10:					//function code 0x10
				{
					unsigned short addr_start,quantity_regs;
					char byte_counts;
					char* pRegsValue;
					addr_start = MB_Request_PDU[1]<<8;						//starting address Hi
					addr_start |= MB_Request_PDU[2];						//starting address Lo
					quantity_regs = MB_Request_PDU[3]<<8;					//quantity of registers Hi
					quantity_regs |= MB_Request_PDU[4];						//quantity of registers Lo
					byte_counts = MB_Request_PDU[5];						//byte count
					pRegsValue = &MB_Request_PDU[6];						//point to register value
					//pass params to function FC16_Proc(),judge if request process is ok
					if (FC16_Proc(addr_start,quantity_regs,byte_counts,pRegsValue)) 
					{
						IsResponsepduCorrect = true;
						return true;

					}
					else	
					{
						IsResponsepduCorrect = false;
						return false;
					}
					break;
				}
			default:
				return false;
				break;
		}
	}
	else
	{
		//if false ,function code not supported,build mb_exception_rsp,exception code 01
		MB_Exception_rsp_PDU[0] = fc_code+0x80;							//function code
		MB_Exception_rsp_PDU[1] = 0x01;									//exception code
		SetRspPDU_Length(0x02);											//cal PDU length
		IsResponsepduCorrect = false;
		return false;
	}
}

/*		read coils function code 01 process										*/
bool CModbus_Server::FC01_Proc(unsigned short addr_start,unsigned short quantity_coils)
{
	//judge if quantity of coils out of range
	if (quantity_coils>=0x0001 && quantity_coils<=0x07D0)
	{
		//if true,ok
		//then,judge if if the address is ok
		if (addr_start>=0x0000 && (addr_start+quantity_coils)<=0xFFFF)
		{
			//if true,ok
			//now read coils,if success,build mb_rsp,if not,build mb_exception_rsp
			unsigned short byte_count = 0x00;		//define byte_count
			if (quantity_coils%8)	 byte_count = quantity_coils/8+1;
			else	byte_count = quantity_coils/8;										//calculate byte_count

			//cal if request discrete inputs is out of range in coils data block
			if (addr_start+quantity_coils<= Coils.size())
			{
				//if true,request process is ok,build mb_rsp
				MB_Response_PDU[0] = 0x01;				//function code
				MB_Response_PDU[1] = byte_count&0x00FF;			//byte count
				for (int k=0;k<byte_count;k++)
				{
					MB_Response_PDU[2+k] = 0x00;
				}
				for (int i=0;i<quantity_coils;i++)		//coil status
				{
					if (Coils[addr_start+i] != 0) MB_Response_PDU[2+i/8] |=1<<(i%8);
					else	MB_Response_PDU[2+i/8] &=~(1<<(i%8));
					
				}
				SetRspPDU_Length(byte_count+2);				//set rsp pdu length
				return true;
			}
			else
			{
				//if false,request process is not ok,build mb_exception_rsp,exception code 04
				MB_Exception_rsp_PDU[0] = 0x81;			//function code
				MB_Exception_rsp_PDU[1] = 0x04;			//exception code 04
				SetRspPDU_Length(0x02);					//cal PDU length
				return false;
			}
			
			
		}
		else
		{
			//if false,not ok,build mb_exception_rsp,exception code 02
			MB_Exception_rsp_PDU[0] = 0x81;			//function code
			MB_Exception_rsp_PDU[1] = 0x02;			//exception code 02
			SetRspPDU_Length(0x02);					//cal PDU length
			return false;
		}
	}
	else
	{
		//if false,not ok,build exception rsp,exception code 03
		MB_Exception_rsp_PDU[0] = 0x81;			//function code
		MB_Exception_rsp_PDU[1] = 0x03;			//exception code 03
		SetRspPDU_Length(0x02);					//cal PDU length
		return false;
	}
}

/*		read discrete inputs function code 02 process							*/
bool CModbus_Server::FC02_Proc(unsigned short addr_start,unsigned short quantity_inputs)
{
	//judge if quantity of discrete inputs out of range
	if (quantity_inputs>=0x0001 && quantity_inputs<=0x07D0)
	{
		//if true,ok
		//then,judge if if the address is ok
		if (addr_start>=0x0000 && (addr_start+quantity_inputs)<=0xFFFF)
		{
			//if true,ok
			//now read discrete inputs,if success,build mb_rsp,if not,build mb_exception_rsp
			unsigned short byte_count =0;		//define byte_count
			if (quantity_inputs%8)	 byte_count = quantity_inputs/8+1;
			else	byte_count = quantity_inputs/8;										//calculate byte_count

			//cal if request discrete inputs is out of range in discrete inputs data block
			if (addr_start+quantity_inputs<= Discretes_Inputs.size())
			{
				//if true,request process is ok,build mb_rsp
				MB_Response_PDU[0] = 0x02;				//function code
				MB_Response_PDU[1] = byte_count&0x00FF;			//byte count
				for (int k=0;k<byte_count;k++)
				{
					MB_Response_PDU[2+k] = 0x00;
				}
				for (int i=0;i<quantity_inputs;i++)		//input status
				{
					if (Discretes_Inputs[addr_start+i] != 0) MB_Response_PDU[2+i/8] |=1<<(i%8);
					else	MB_Response_PDU[2+i/8] &=~(1<<(i%8));

				}
				SetRspPDU_Length(byte_count+2);		//set pdu length
				return true;
			}
			else
			{
				//if false,request process is not ok,build mb_exception_rsp,exception code 04
				MB_Exception_rsp_PDU[0] = 0x82;			//function code
				MB_Exception_rsp_PDU[1] = 0x04;			//exception code 04
				SetRspPDU_Length(0x02);					//cal PDU length
				return false;
			}


		}
		else
		{
			//if false,not ok,build mb_exception_rsp,exception code 02
			MB_Exception_rsp_PDU[0] = 0x82;			//function code
			MB_Exception_rsp_PDU[1] = 0x02;			//exception code 02
			SetRspPDU_Length(0x02);					//cal PDU length
			return false;
		}
	}
	else
	{
		//if false,not ok,build exception rsp,exception code 03
		MB_Exception_rsp_PDU[0] = 0x82;			//function code
		MB_Exception_rsp_PDU[1] = 0x03;			//exception code 03
		SetRspPDU_Length(0x02);					//cal PDU length
		return false;
	}
}

/*		read holding registers function code 03 process							*/
bool CModbus_Server::FC03_Proc(unsigned short addr_start,unsigned short quantity_regs)
{
	//judge if quantity of registers is out of range
	if (quantity_regs>= 0x0001 && quantity_regs<= 0x007D)
	{
		//if true,ok
		//then,judge if addrss is ok,in the range
		if (addr_start>= 0x0000 && addr_start+quantity_regs <= 0xFFFF)
		{
			//if true,ok,request process,read holding registers,if success,build mb_rsp,if not,build mb_exception_rsp
			//judge if the request of addr and quantity is out of range in holding_regs data block
			if (addr_start+quantity_regs <= Holding_regs.size())
			{
				//if true,request process is ok,build mb_rsp
				MB_Response_PDU[0] = 0x03;					//function code
				MB_Response_PDU[1] = 2*quantity_regs;		//byte count
				for (int i=0;i<quantity_regs;i++)			//register value
				{
					MB_Response_PDU[2+i*2] = Holding_regs[addr_start+i]>>8;			//register value Hi
					MB_Response_PDU[2+i*2+1] = Holding_regs[addr_start+i]&0x00FF;	//register value Lo
				}
				SetRspPDU_Length(quantity_regs*2+2);		//set pdu length
				//finish building mb_rsp,return true
				return true;
			}
			else
			{
				//if false,request process is wrong,build mb_exception_rsp,exception code 04
				MB_Exception_rsp_PDU[0] = 0x83;			//function code
				MB_Exception_rsp_PDU[1] = 0x04;			//exception code
				SetRspPDU_Length(0x02);					//cal PDU length
				return false;
			}
		}
		else
		{
			//if false,not ok,build modbus exception_rsp,exception code 02
			MB_Exception_rsp_PDU[0] = 0x83;			//function code
			MB_Exception_rsp_PDU[1] = 0x02;			//exception code
			SetRspPDU_Length(0x02);					//cal PDU length
			return false;
		}
	}
	else
	{
		//if false,not ok,build modbus exception_rsp,exception code 03
		MB_Exception_rsp_PDU[0] = 0x83;			//function code
		MB_Exception_rsp_PDU[1] = 0x03;			//exception code
		SetRspPDU_Length(0x02);					//cal PDU length
		return false;
	}
}

/*		read input registers function code 04 process							*/
bool CModbus_Server::FC04_Proc(unsigned short addr_start,unsigned short quantity_input_regs)
{
	//judge if quantity of input registers is out of range
	if (quantity_input_regs>= 0x0001 && quantity_input_regs<= 0x007D)
	{
		//if true,ok
		//then,judge if address is ok
		if (addr_start>= 0x0000 && addr_start+ quantity_input_regs <=0xFFFF)
		{
			//if true,ok,request processing
			//judge if the request of addr&quantity is in the range of Input_regs data block
			if (addr_start+quantity_input_regs <= Input_regs.size())
			{
				//if true,ok,build mb_response
				MB_Response_PDU[0] = 0x04;							//function code
				MB_Response_PDU[1] = 2*quantity_input_regs;			//byte count
				for (int i=0;i<quantity_input_regs;i++)
				{
					MB_Response_PDU[2+i*2] = Input_regs[addr_start+i]>>8;			//register value Hi
					MB_Response_PDU[2+i*2+1] = Input_regs[addr_start+i]&0x00FF;		//register value Lo
				}
				SetRspPDU_Length(quantity_input_regs*2+2);			//set pdu length
				return true;
			}
			else
			{
				//if false,build mb_exception_rsp,exception code 04
				MB_Exception_rsp_PDU[0] = 0x84;				//function code
				MB_Exception_rsp_PDU[1] = 0x04;				//exception code
				SetRspPDU_Length(0x02);					//cal PDU length
				return false;
			}
		}
		else
		{
			//if false,not ok,build mb_exception_rsp,exception code 02
			MB_Exception_rsp_PDU[0] = 0x84;				//function code
			MB_Exception_rsp_PDU[1] = 0x02;				//exception code
			SetRspPDU_Length(0x02);					//cal PDU length
			return false;
		}
	}
	else
	{
		//if false,not ok,build mb_exception_rsp,exception code 03
		MB_Exception_rsp_PDU[0] = 0x84;				//function code
		MB_Exception_rsp_PDU[1] = 0x03;				//exception code
		SetRspPDU_Length(0x02);					//cal PDU length
		return false;
	}
}

/*		write single coil function code 05 process								*/
bool CModbus_Server::FC05_Proc(unsigned short output_addr,unsigned short output_value)
{
	//judge if output value is 0x0000 or 0xFF00
	if (output_value ==0x0000 || output_value == 0xFF00)
	{
		//if true,ok
		//then judge if output address is ok
		if (output_addr>=0x0000 && output_addr<= 0xFFFF)
		{
			//if true,ok
			//request processing,if ok,build mb_rsp,if not,build mb_exception_rsp
			//judge if the request of addr is in the range of Coils data block
			if (output_addr<Coils.size())
			{
				//if true,request processing and build mb_rsp
				if(output_value == 0xFF00)	Coils[output_addr] = 0x01;			//set coil ON
				else	Coils[output_addr] = 0x00;								//set coil OFF

				MB_Response_PDU[0] = 0x05;										//function code
				MB_Response_PDU[1] = output_addr>>8;							//output address Hi
				MB_Response_PDU[2] = output_addr&0x00FF;						//output address Lo
				if (output_value == 0xFF00)
				{
					MB_Response_PDU[3] = 0xFF;									//output value Hi
					MB_Response_PDU[4] = 0x00;									//output value Lo
				}
				else
				{
					MB_Response_PDU[3] = 0x00;									//output value Hi
					MB_Response_PDU[4] = 0x00;									//output value Lo
				}
				SetRspPDU_Length(0x05);											//set pdu length
				//finish processing and building,return true
				return true;
			}
			else
			{
				//if not,build mb_exception_rsp,exception code 04
				MB_Exception_rsp_PDU[0] = 0x85;				//function code
				MB_Exception_rsp_PDU[1] = 0x04;				//exception code
				SetRspPDU_Length(0x02);						//cal PDU length
				return true;
			}
		}
		else
		{
			//if false,not ok,build mb_exception_rsp,exception code 02
			MB_Exception_rsp_PDU[0] = 0x85;				//function code
			MB_Exception_rsp_PDU[1] = 0x02;				//exception code
			SetRspPDU_Length(0x02);						//cal PDU length
			return true;
		}
	}
	else
	{
		//if false,not ok,build mb_exception_rsp,exception code 03
		MB_Exception_rsp_PDU[0] = 0x85;				//function code
		MB_Exception_rsp_PDU[1] = 0x03;				//exception code
		SetRspPDU_Length(0x02);						//cal PDU length
		return true;
	}
}

/*		write single register function code 06 process							*/
bool CModbus_Server::FC06_Proc(unsigned short register_addr,unsigned short register_value)
{
	//judge if the register value is in the range'
	if (register_value>= 0x0000 &&register_value<= 0xFFFF0)
	{
		//if true,ok
		//then,judge if register address is in the range
		if (register_addr>= 0x0000 &&register_addr<= 0xFFFF)
		{
			//if true,ok
			//request processing,if ok,request processing and build mb_rsp
			//judge if the request of address is out of range in the holding_regs data block
			if (register_addr<Holding_regs.size())
			{
				Holding_regs[register_addr] = register_value;			//write single register
				MB_Response_PDU[0] = 0x06;								//function code
				MB_Response_PDU[1] = register_addr>>8;					//register address Hi
				MB_Response_PDU[2] = register_addr& 0x00FF;				//register address Lo
				MB_Response_PDU[3] = register_value >>8;				//register value Hi
				MB_Response_PDU[4] = register_value& 0x00FF;			//register value Lo
				SetRspPDU_Length(0x05);									//set pdu length
				//finish processing&building,return true
				return true;
			}
			else
			{
				//if false,not ok,build mb_exception_rsp,exception code 04
				MB_Exception_rsp_PDU[0] = 0x86;							//function code
				MB_Exception_rsp_PDU[1] = 0x04;							//exception code
				SetRspPDU_Length(0x02);									//cal PDU length
				return false;
			}
		}
		else
		{
			//if false,not ok,build mb_exception_rsp,exception code 02
			MB_Exception_rsp_PDU[0] = 0x86;							//function code
			MB_Exception_rsp_PDU[1] = 0x02;							//exception code
			SetRspPDU_Length(0x02);									//cal PDU length
			return false;
		}
	}
	else
	{
		//if false,not ok,build mb_exception_rsp,exception code 03
		MB_Exception_rsp_PDU[0] = 0x86;							//function code
		MB_Exception_rsp_PDU[1] = 0x03;							//exception code
		SetRspPDU_Length(0x02);									//cal PDU length
		return false;

	}
}

/*		write multiple coils function code 15(0x0F) process						*/
bool CModbus_Server::FC15_Proc(unsigned short addr_start,unsigned short quantity_outputs,char byte_count,char* outputs_value)
{
	//judge if quantity of outputs is in the range and byte count is correct
	if (quantity_outputs>=0x0001 && quantity_outputs<=0x07B0 && byte_count*8-quantity_outputs>=0 && byte_count*8-quantity_outputs<8)
	{
		//if true,ok
		//then,judge if address is ok
		if (addr_start>=0x0000 && addr_start+quantity_outputs<=0xFFFF)
		{
			//if true,ok
			//request process,if ok,build mb_rsp,if not,build mb_exception_rsp
			//judge if the request of addr and quantity is out of range in the coils data block
			if (addr_start+quantity_outputs< Coils.size())
			{
				//if true,request processing and building mb_rsp
				//request process:write multiple coils
				for (int i=0;i<quantity_outputs;i++)
				{
					if (*(outputs_value+i/8) & 0x01<<(i%8))				//judge coil value bit
						Coils[addr_start+i] = 0x01;						//set one coil ON
					else
						Coils[addr_start+i] = 0x00;						//set one coil OFF
				}
				//build mb_rsp
				MB_Response_PDU[0] = 0x0F;								//function code
				MB_Response_PDU[1] = addr_start>>8;						//starting addrss Hi
				MB_Response_PDU[2] = addr_start & 0x00FF;				//starting address Lo
				MB_Response_PDU[3] = quantity_outputs>>8;				//quantity of outputs Hi
				MB_Response_PDU[4] = quantity_outputs & 0x00FF;			//quantity of outputs Lo
				SetRspPDU_Length(0x05);									//set pdu length
				//finish request processing and building,return true
				return true;
			}
			else
			{
				//if false,build mb_exception_rsp,exception code is 04
				MB_Exception_rsp_PDU[0] = 0x8F;							//function code
				MB_Exception_rsp_PDU[1] = 0x04;							//exception code
				SetRspPDU_Length(0x02);									//cal PDU length
				return false;
			}
		}
		else
		{
			//if false,not ok,build mb_exception_rsp,exception code 02
			MB_Exception_rsp_PDU[0] = 0x8F;							//function code
			MB_Exception_rsp_PDU[1] = 0x02;							//exception code
			SetRspPDU_Length(0x02);									//cal PDU length
			return false;
		}
	}
	else
	{
		//if false,not ok,build mb_exception_rsp,exception code 03
		MB_Exception_rsp_PDU[0] = 0x8F;							//function code
		MB_Exception_rsp_PDU[1] = 0x03;							//exception code
		SetRspPDU_Length(0x02);									//cal PDU length
		return false;
	}
}

/*		write multiple registers function code 16(0x10) process					*/
bool CModbus_Server::FC16_Proc(unsigned short addr_start,unsigned short quantity_regs,char byte_count,char* regs_value)
{
	//judge if quantity of registers is in the range and byte count equals with quantity*2
	if (quantity_regs>= 0x0001 && quantity_regs<= 0x007B && byte_count == quantity_regs*2)
	{
		//if true,ok
		//then, judge if address is ok
		if (addr_start>=0x0000 && addr_start+quantity_regs<=0xFFFF)
		{
			//if true,ok
			//request process,if ok,buid mb_rsp,if not,build mb_exception_rsp
			//judge if the request of address and quantity is in the range of holding_regs data block
			if (addr_start+quantity_regs<Holding_regs.size())
			{
				//if true,request processing and building mb_rsp
				//request processing: write multiple registers
				for (int i=0;i<quantity_regs;i++)
				{
					Holding_regs[addr_start+i] = *(regs_value+2*i)<<8;				//register value Hi
					Holding_regs[addr_start+i] |= *(regs_value+2*i+1);				//register value Lo
				}
				//building mb_rsp
				MB_Response_PDU[0] = 0x10;											//function code
				MB_Response_PDU[1] = addr_start>>8;									//starting address Hi
				MB_Response_PDU[2] = addr_start & 0x00FF;							//starting address Lo
				MB_Response_PDU[3] = quantity_regs >>8;								//quantity of registers Hi
				MB_Response_PDU[4] = quantity_regs & 0x00FF;						//quantity of registers Lo
				SetRspPDU_Length(0x05);												//set pdu length
				//finish request processing and building mb_rsp,return true
				return true;
			}
			else
			{
				//if false,not ok,build mb_exception_rsp,exception code 04
				MB_Exception_rsp_PDU[0] = 0x90;										//function code
				MB_Exception_rsp_PDU[1] = 0x04;										//exception code
				SetRspPDU_Length(0x02);												//cal PDU length
				return false;
			}
		}
		else
		{
			//if false,not ok,build mb_exception_rsp,exception code 02
			MB_Exception_rsp_PDU[0] = 0x90;										//function code
			MB_Exception_rsp_PDU[1] = 0x02;										//exception code
			SetRspPDU_Length(0x02);												//cal PDU length
			return false;
		}
	}
	else
	{
		//if false,not ok,build mb_exception_rsp,exception code 03
		MB_Exception_rsp_PDU[0] = 0x90;										//function code
		MB_Exception_rsp_PDU[1] = 0x03;										//exception code
		SetRspPDU_Length(0x02);												//cal PDU length
		return false;
	}
}

