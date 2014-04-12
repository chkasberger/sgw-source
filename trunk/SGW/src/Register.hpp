/*
 * Register.hpp
 *
 *  Created on: Mar 10, 2014
 *      Author: root
 */

#ifndef REGISTER_HPP_
#define REGISTER_HPP_

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

namespace sgw {

class Register {
public:
	enum Priority{High, Medium, Low, None};
	enum DataType{Signed, Unsigned, Undefined};
	enum ReadWrite{Read, Write};
	enum Type{Controller, Module};

	const char *r_Name;
	int r_ModbusAddress;
	int r_DataSize;
	char r_ScsCommand;
	ReadWrite r_ReadWrite;
	DataType r_DataType;
	Priority r_Priority;
	Type r_Type;
	char r_RawData[2];
	int r_InternalAddress;
	unsigned int r_Count = 0;
	time_t r_TimeStamp = time(NULL);
    bool operator< (const Register &other) const {
        return r_ModbusAddress < other.r_ModbusAddress;
    }

	Register();

	Register(
			const char *name,
			int modbusAddress,
			char scsCommand
			);

	Register(
			const char *name,
			int modbusAddress,
			int dataSize,
			char scsCommand,
			ReadWrite readWrite,
			DataType dataType,
			Priority priority,
			Type type,
			int internalAddress
	);

	virtual ~Register();

};

} /* namespace sgw */

#endif /* REGISTER_HPP_ */
