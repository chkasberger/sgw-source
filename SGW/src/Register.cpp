/*
 * Register.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: root
 */

#include "Register.hpp"

namespace sgw {

Register::Register() {
	r_Name = "uninitialized";
	r_ModbusAddress = 0xee;
	r_ScsCommand = 0xff;
	r_DataSize = 1;
	r_ReadWrite = Read;
	r_DataType = Undefined;
	r_Priority = None;
	r_Type = Controller;
	r_InternalAddress = 0;
	r_RawData[0] = 0xff;
	r_RawData[1] = 0xff;
}

Register::Register (
		const char *name,
		int modbusAddress,
		int dataSize,
		char scsCommand,
		ReadWrite readWrite,
		DataType dataType,
		Priority priority,
		Type type,
		int internalAddress){
	r_Name = name;
	r_ModbusAddress = modbusAddress;
	r_DataSize = dataSize;
	r_ScsCommand = scsCommand;
	r_ReadWrite = readWrite;
	r_DataType = dataType;
	r_Priority = priority;
	r_Type = type;
	r_InternalAddress = internalAddress;
	r_RawData[0] = 0xff;
	r_RawData[1] = 0xff;
}

Register::~Register() {
	// TODO Auto-generated destructor stub
}

} /* namespace sgw */
