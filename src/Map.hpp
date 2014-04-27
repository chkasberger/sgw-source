/*
 * Map.hpp
 *
 *  Created on: Mar 10, 2014
 *      Author: root
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include "Register.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

namespace sgw {

class Map {
public:
	Map();
	virtual ~Map();

	list<Register*> m_registerListController;
	list<Register*> m_registerListModule;

	int m_ModuleCount = 0;

private:
	Register DesignCapacity = 		Register("DesignCapacity",	45000,1,0x18,Register::Read,Register::Unsigned,Register::None,Register::Controller,0);
	Register DesignVoltage = 		Register("DesignVoltage",	45001,1,0x19,Register::Read,Register::Unsigned,Register::None,Register::Controller,0);
	Register ManufactureDate = 		Register("ManufactureDate",	45002,1,0x1B,Register::Read,Register::Undefined,Register::None,Register::Controller,0);
	Register SerialNumber = 		Register("SerialNumber",	45003,1,0x1C,Register::Read,Register::Unsigned,Register::None,Register::Controller,0);
	Register ManufactureName = 		Register("ManufactureName",	45004,1,0x20,Register::Read,Register::Undefined,Register::None,Register::Controller,0);
	Register DeviceName = 			Register("DeviceName",		45005,1,0x21,Register::Read,Register::Undefined,Register::None,Register::Controller,0);
	Register DeviceChemistry = 		Register("DeviceChemistry",	45006,1,0x22,Register::Read,Register::Undefined,Register::None,Register::Controller,0);
	Register ModelName = 			Register("ModelName",		45007,1,0x49,Register::Read,Register::Undefined,Register::None,Register::Controller,0);
	Register ConnectedModuleInfo = 	Register("ConnectedModuleInfo",	45008,1,0x4B,Register::Read,Register::Undefined,Register::None,Register::Controller,0);
	Register ControllerInfo = 		Register("ControllerInfo",	45009,1,0x6A,Register::Read,Register::Undefined,Register::None,Register::Controller,0);
	Register FirmwareVersion = 		Register("FirmwareVersion",	45010,1,0x73,Register::Read,Register::Undefined,Register::None,Register::Controller,0);
	Register Status = 				Register("Status",			45011,1,0x00,Register::Read,Register::Undefined,Register::High,Register::Controller,0);
	Register BatteryStatus = 		Register("BatteryStatus",	45012,1,0x16,Register::Read,Register::Undefined,Register::High,Register::Controller,0);
	Register Voltage = 				Register("Voltage",			45013,1,0x09,Register::Read,Register::Unsigned,Register::High,Register::Controller,0);
	Register Current = 				Register("Current",			45014,1,0x0A,Register::Read,Register::Signed,Register::High,Register::Controller,0);
	Register SysWarning = 			Register("SysWarning",		45015,1,0x48,Register::Read,Register::Undefined,Register::Medium,Register::Controller,0);
	Register SysMaxCellVoltage = 	Register("SysMaxCellVoltage",45016,1,0x46,Register::Read,Register::Unsigned,Register::High,Register::Controller,0);
	Register SysMinCellVoltage = 	Register("SysMinCellVoltage",45017,1,0x47,Register::Read,Register::Unsigned,Register::High,Register::Controller,0);
	Register Temperature = 			Register("Temperature",		45018,1,0x08,Register::Read,Register::Unsigned,Register::Medium,Register::Controller,0);
	Register RelativeStateOfCharge = Register("RelativeStateOfCharge",45019,1,0x0D,Register::Read,Register::Unsigned,Register::High,Register::Controller,0);
	Register RemainCapacity = 		Register("RemainCapacity",	45020,1,0x0F,Register::Read,Register::Unsigned,Register::Medium,Register::Controller,0);
	Register FullChargeCapacity = 	Register("FullChargeCapacity",45021,1,0x10,Register::Read,Register::Unsigned,Register::Low,Register::Controller,0);
	Register AverageCurrent = 		Register("AverageCurrent",	45022,1,0x0B,Register::Read,Register::Signed,Register::Low,Register::Controller,0);
	Register ChargingCurrent = 		Register("ChargingCurrent",	45023,1,0x14,Register::Read,Register::Unsigned,Register::Low,Register::Controller,0);
	Register ChargingVoltage = 		Register("ChargingVoltage",	45024,1,0x15,Register::Read,Register::Unsigned,Register::Low,Register::Controller,0);
	Register CycleCount = 			Register("CycleCount",		45025,1,0x17,Register::Read,Register::Unsigned,Register::Low,Register::Controller,0);
	Register HeaterStatus = 		Register("HeaterStatus",	45026,1,0x40,Register::Read,Register::Undefined,Register::Low,Register::Controller,0);
	Register FanStatus = 			Register("FANStatus",		45027,1,0x41,Register::Read,Register::Undefined,Register::Low,Register::Controller,0);
	Register SysMaxCellTemp = 		Register("SysMaxCellTemp",	45028,1,0x44,Register::Read,Register::Unsigned,Register::Low,Register::Controller,0);
	Register SysMinCellTemp = 		Register("SysMinCellTemp",	45029,1,0x45,Register::Read,Register::Unsigned,Register::Low,Register::Controller,0);
	Register HeatsinkTemperature = 	Register("HeatsinkTemperature",45030,1,0x69,Register::Read,Register::Unsigned,Register::Low,Register::Controller,0);
	Register IGBTControl = 			Register("IGBTControl",		45031,1,0x6B,Register::Write,Register::Undefined,Register::Low,Register::Controller,0);
	Register IGBTStatus = 			Register("IGBTStatus",		45032,1,0x6C,Register::Read,Register::Undefined,Register::Low,Register::Controller,0);

	Register ModuleInfo_0 = 		Register("ModuleInfo",	45033,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,0);
	Register MaxCellVoltage_0 = 	Register("MaxCellVoltage",	45034,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,0);
	Register MinCellVoltage_0 = 	Register("MinCellVoltage",	45035,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,0);
	Register ModuleStatus_0 = 		Register("ModuleStatus",	45036,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,0);
	Register Alarm_0 = 				Register("Alarm",	45037,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,0);
	Register ModuleVoltage_0 = 		Register("ModuleVoltage",	45038,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,0);
	Register ModuleCurrent_0 = 		Register("ModuleCurrent",	45039,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,0);
	Register ModuleSOC_0 = 			Register("ModuleSOC",	45040,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,0);
	Register MaxCellTemp_0 = 		Register("MaxCellTemp",	45041,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,0);
	Register MinCellTemp_0 = 		Register("MinCellTemp",	45042,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,0);
	Register ModuleTemperature_0 = 	Register("ModuleTemperature",	45043,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,0);
	Register ModuleCycleCount_0 = 	Register("ModuleCycleCount",	45044,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,0);
	Register ModuleInfol_1 = 		Register("ModuleInfo",	45045,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,1);
	Register MaxCellVoltage_1 =		Register("MaxCellVoltage",	45046,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,1);
	Register MinCellVoltage_1 =		Register("MinCellVoltage",	45047,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,1);
	Register ModuleStatus_1 =		Register("ModuleStatus",	45048,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,1);
	Register Alarm_1 =				Register("Alarm",	45049,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,1);
	Register ModuleVoltage_1 =		Register("ModuleVoltage",	45050,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,1);
	Register ModuleCurrent_1 =		Register("ModuleCurrent",	45051,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,1);
	Register ModuleSOC_1 =			Register("ModuleSOC",		45052,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,1);
	Register MaxCellTemp_1 =		Register("MaxCellTemp",		45053,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,1);
	Register MinCellTemp_1 =		Register("MinCellTemp",		45054,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,1);
	Register ModuleTemperature_1 =	Register("ModuleTemperature",45055,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,1);
	Register ModuleCycleCount_1 =	Register("ModuleCycleCount",45056,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,1);
	Register ModuleInfo_2 =			Register("ModuleInfo",		45057,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,2);
	Register MaxCellVoltage_2 =		Register("MaxCellVoltage",	45058,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,2);
	Register MinCellVoltage_2 =		Register("MinCellVoltage",	45059,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,2);
	Register ModuleStatus_2 =		Register("ModuleStatus",	45060,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,2);
	Register Alarm_2 =				Register("Alarm",			45061,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,2);
	Register ModuleVoltage_2 =		Register("ModuleVoltage",	45062,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,2);
	Register ModuleCurrent_2 =		Register("ModuleCurrent",	45063,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,2);
	Register ModuleSOC_2 =			Register("ModuleSOC",		45064,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,2);
	Register MaxCellTemp_2 =		Register("MaxCellTemp",		45065,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,2);
	Register MinCellTemp_2 =		Register("MinCellTemp",		45066,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,2);
	Register ModuleTemperature_2 =	Register("ModuleTemperature",45067,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,2);
	Register ModuleCycleCount_2 =	Register("ModuleCycleCount",45068,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,2);
	Register ModuleInfo_3 =			Register("ModuleInfo",		45069,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,3);
	Register MaxCellVoltage_3 =		Register("MaxCellVoltage",	45070,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,3);
	Register MinCellVoltage_3 =		Register("MinCellVoltage",	45071,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,3);
	Register ModuleStatus_3 =		Register("ModuleStatus",	45072,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,3);
	Register Alarm_3 =				Register("Alarm",			45073,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,3);
	Register ModuleVoltage_3 =		Register("ModuleVoltage",	45074,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,3);
	Register ModuleCurrent_3 =		Register("ModuleCurrent",	45075,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,3);
	Register ModuleSOC_3 =			Register("ModuleSOC",		45076,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,3);
	Register MaxCellTemp_3 =		Register("MaxCellTemp",		45077,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,3);
	Register MinCellTemp_3 =		Register("MinCellTemp",		45078,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,3);
	Register ModuleTemperature_3 =	Register("ModuleTemperature",45079,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,3);
	Register ModuleCycleCount_3 =	Register("ModuleCycleCount",45080,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,3);
	Register ModuleInfo_4 =			Register("ModuleInfo",		45081,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,4);
	Register MaxCellVoltage_4 =		Register("MaxCellVoltage",	45082,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,4);
	Register MinCellVoltage_4 =		Register("MinCellVoltage",	45083,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,4);
	Register ModuleStatus_4 =		Register("ModuleStatus",	45084,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,4);
	Register Alarm_4 =				Register("Alarm",			45085,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,4);
	Register ModuleVoltage_4 =		Register("ModuleVoltage",	45086,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,4);
	Register ModuleCurrent_4 =		Register("ModuleCurrent",	45087,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,4);
	Register ModuleSOC_4 =			Register("ModuleSOC",		45088,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,4);
	Register MaxCellTemp_4 =		Register("MaxCellTemp",		45089,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,4);
	Register MinCellTemp_4 =		Register("MinCellTemp",		45090,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,4);
	Register ModuleTemperature_4 =	Register("ModuleTemperature",45091,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,4);
	Register ModuleCycleCount_4 =	Register("ModuleCycleCount",45092,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,4);
	Register ModuleInfo_5 =			Register("ModuleInfo",		45093,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,5);
	Register MaxCellVoltage_5 =		Register("MaxCellVoltage",	45094,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,5);
	Register MinCellVoltage_5 =		Register("MinCellVoltage",	45095,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,5);
	Register ModuleStatus_5 =		Register("ModuleStatus",	45096,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,5);
	Register Alarm_5 =				Register("Alarm",			45097,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,5);
	Register ModuleVoltage_5 =		Register("ModuleVoltage",	45098,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,5);
	Register ModuleCurrent_5 =		Register("ModuleCurrent",	45099,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,5);
	Register ModuleSOC_5 =			Register("ModuleSOC",		45100,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,5);
	Register MaxCellTemp_5 =		Register("MaxCellTemp",		45101,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,5);
	Register MinCellTemp_5 =		Register("MinCellTemp",		45102,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,5);
	Register ModuleTemperature_5 =	Register("ModuleTemperature",45103,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,5);
	Register ModuleCycleCount_5 =	Register("ModuleCycleCount",45104,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,5);
	Register ModuleInfo_6 =			Register("ModuleInfo",		45105,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,6);
	Register MaxCellVoltage_6 =		Register("MaxCellVoltage",	45106,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,6);
	Register MinCellVoltage_6 =		Register("MinCellVoltage",	45107,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,6);
	Register ModuleStatus_6 =		Register("ModuleStatus",	45108,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,6);
	Register Alarm_6 =				Register("Alarm",			45109,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,6);
	Register ModuleVoltage_6 =		Register("ModuleVoltage",	45110,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,6);
	Register ModuleCurrent_6 =		Register("ModuleCurrent",	45111,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,6);
	Register ModuleSOC_6 =			Register("ModuleSOC",		45112,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,6);
	Register MaxCellTemp_6 =		Register("MaxCellTemp",		45113,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,6);
	Register MinCellTemp_6 =		Register("MinCellTemp",		45114,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,6);
	Register ModuleTemperature_6 =	Register("ModuleTemperature",45115,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,6);
	Register ModuleCycleCount_6 =	Register("ModuleCycleCount",45116,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,6);
	Register ModuleInfo_7 =			Register("ModuleInfo",		45117,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,7);
	Register MaxCellVoltage_7 =		Register("MaxCellVoltage",	45118,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,7);
	Register MinCellVoltage_7 =		Register("MinCellVoltage",	45119,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,7);
	Register ModuleStatus_7 =		Register("ModuleStatus",	45120,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,7);
	Register Alarm_7 =				Register("Alarm",			45121,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,7);
	Register ModuleVoltage_7 =		Register("ModuleVoltage",	45122,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,7);
	Register ModuleCurrent_7 =		Register("ModuleCurrent",	45123,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,7);
	Register ModuleSOC_7 =			Register("ModuleSOC",		45124,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,7);
	Register MaxCellTemp_7 =		Register("MaxCellTemp",		45125,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,7);
	Register MinCellTemp_7 =		Register("MinCellTemp",		45126,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,7);
	Register ModuleTemperature_7 =	Register("ModuleTemperature",45127,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,7);
	Register ModuleCycleCount_7 =	Register("ModuleCycleCount",45128,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,7);
	Register ModuleInfo_8 =			Register("ModuleInfo",		45129,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,8);
	Register MaxCellVoltage_8 =		Register("MaxCellVoltage",	45130,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,8);
	Register MinCellVoltage_8 =		Register("MinCellVoltage",	45131,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,8);
	Register ModuleStatus_8 =		Register("ModuleStatus",	45132,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,8);
	Register Alarm_8 =				Register("Alarm",			45133,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,8);
	Register ModuleVoltage_8 =		Register("ModuleVoltage",	45134,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,8);
	Register ModuleCurrent_8 =		Register("ModuleCurrent",	45135,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,8);
	Register ModuleSOC_8 =			Register("ModuleSOC",		45136,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,8);
	Register MaxCellTemp_8 =		Register("MaxCellTemp",		45137,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,8);
	Register MinCellTemp_8 =		Register("MinCellTemp",		45138,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,8);
	Register ModuleTemperature_8 =	Register("ModuleTemperature",45139,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,8);
	Register ModuleCycleCount_8 =	Register("ModuleCycleCount",45140,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,8);
	Register ModuleInfo_9 =			Register("ModuleInfo",		45141,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,9);
	Register MaxCellVoltage_9 =		Register("MaxCellVoltage",	45142,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,9);
	Register MinCellVoltage_9 =		Register("MinCellVoltage",	45143,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,9);
	Register ModuleStatus_9 =		Register("ModuleStatus",	45144,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,9);
	Register Alarm_9 =				Register("Alarm",			45145,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,9);
	Register ModuleVoltage_9 =		Register("ModuleVoltage",	45146,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,9);
	Register ModuleCurrent_9 =		Register("ModuleCurrent",	45147,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,9);
	Register ModuleSOC_9 =			Register("ModuleSOC",		45148,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,9);
	Register MaxCellTemp_9 =		Register("MaxCellTemp",		45149,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,9);
	Register MinCellTemp_9 =		Register("MinCellTemp",		45150,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,9);
	Register ModuleTemperature_9 =	Register("ModuleTemperature",45151,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,9);
	Register ModuleCycleCount_9 =	Register("ModuleCycleCount",45152,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,9);
	Register ModuleInfo_10 =		Register("ModuleInfo",		45153,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,10);
	Register MaxCellVoltage_10 =	Register("MaxCellVoltage",	45154,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,10);
	Register MinCellVoltage_10 =	Register("MinCellVoltage",	45155,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,10);
	Register ModuleStatus_10 =		Register("ModuleStatus",	45156,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,10);
	Register Alarm_10 =				Register("Alarm",			45157,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,10);
	Register ModuleVoltage_10 =		Register("ModuleVoltage",	45158,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,10);
	Register ModuleCurrent_10 =		Register("ModuleCurrent",	45159,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,10);
	Register ModuleSOC_10 =			Register("ModuleSOC",		45160,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,10);
	Register MaxCellTemp_10 =		Register("MaxCellTemp",		45161,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,10);
	Register MinCellTemp_10 =		Register("MinCellTemp",		45162,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,10);
	Register ModuleTemperature_10 =	Register("ModuleTemperature",45163,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,10);
	Register ModuleCycleCount_10 =	Register("ModuleCycleCount",45164,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,10);
	Register ModuleInfo_11 =		Register("ModuleInfo",		45165,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,11);
	Register MaxCellVoltage_11 =	Register("MaxCellVoltage",	45166,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,11);
	Register MinCellVoltage_11 =	Register("MinCellVoltage",	45167,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,11);
	Register ModuleStatus_11 =		Register("ModuleStatus",	45168,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,11);
	Register Alarm_11 =				Register("Alarm",			45169,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,11);
	Register ModuleVoltage_11 =		Register("ModuleVoltage",	45170,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,11);
	Register ModuleCurrent_11 =		Register("ModuleCurrent",	45171,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,11);
	Register ModuleSOC_11 =			Register("ModuleSOC",		45172,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,11);
	Register MaxCellTemp_11 =		Register("MaxCellTemp",		45173,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,11);
	Register MinCellTemp_11 =		Register("MinCellTemp",		45174,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,11);
	Register ModuleTemperature_11 =	Register("ModuleTemperature",45175,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,11);
	Register ModuleCycleCount_11 =	Register("ModuleCycleCount",45176,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,11);
	Register ModuleInfo_12 =		Register("ModuleInfo",		45177,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,12);
	Register MaxCellVoltage_12 =	Register("MaxCellVoltage",	45178,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,12);
	Register MinCellVoltage_12 =	Register("MinCellVoltage",	45179,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,12);
	Register ModuleStatus_12 =		Register("ModuleStatus",	45180,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,12);
	Register Alarm_12 =				Register("Alarm",			45181,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,12);
	Register ModuleVoltage_12 =		Register("ModuleVoltage",	45182,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,12);
	Register ModuleCurrent_12 =		Register("ModuleCurrent",	45183,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,12);
	Register ModuleSOC_12 =			Register("ModuleSOC",		45184,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,12);
	Register MaxCellTemp_12 =		Register("MaxCellTemp",		45185,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,12);
	Register MinCellTemp_12 =		Register("MinCellTemp",		45186,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,12);
	Register ModuleTemperature_12 =	Register("ModuleTemperature",45187,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,12);
	Register ModuleCycleCount_12 =	Register("ModuleCycleCount",45188,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,12);
	Register ModuleInfo_13 =		Register("ModuleInfo",		45189,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,13);
	Register MaxCellVoltage_13 =	Register("MaxCellVoltage",	45190,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,13);
	Register MinCellVoltage_13 =	Register("MinCellVoltage",	45191,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,13);
	Register ModuleStatus_13 =		Register("ModuleStatus",	45192,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,13);
	Register Alarm_13 =				Register("Alarm",			45193,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,13);
	Register ModuleVoltage_13 =		Register("ModuleVoltage",	45194,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,13);
	Register ModuleCurrent_13 =		Register("ModuleCurrent",	45195,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,13);
	Register ModuleSOC_13 =			Register("ModuleSOC",		45196,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,13);
	Register MaxCellTemp_13 =		Register("MaxCellTemp",		45197,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,13);
	Register MinCellTemp_13 =		Register("MinCellTemp",		45198,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,13);
	Register ModuleTemperature_13 =	Register("ModuleTemperature",45199,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,13);
	Register ModuleCycleCount_13 =	Register("ModuleCycleCount",45200,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,13);
	Register ModuleInfo_14 =		Register("ModuleInfo",		45201,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,14);
	Register MaxCellVoltage_14 =	Register("MaxCellVoltage",	45202,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,14);
	Register MinCellVoltage_14 =	Register("MinCellVoltage",	45203,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,14);
	Register ModuleStatus_14 =		Register("ModuleStatus",	45204,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,14);
	Register Alarm_14 =				Register("Alarm",			45205,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,14);
	Register ModuleVoltage_14 =		Register("ModuleVoltage",	45206,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,14);
	Register ModuleCurrent_14 =		Register("ModuleCurrent",	45207,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,14);
	Register ModuleSOC_14 =			Register("ModuleSOC",		45208,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,14);
	Register MaxCellTemp_14 =		Register("MaxCellTemp",		45209,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,14);
	Register MinCellTemp_14 =		Register("MinCellTemp",		45210,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,14);
	Register ModuleTemperature_14 =	Register("ModuleTemperature",45211,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,14);
	Register ModuleCycleCount_14 =	Register("ModuleCycleCount",45212,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,14);
	Register ModuleInfo_15 =		Register("ModuleInfo",		45213,1,0x23,Register::Read,Register::Unsigned,Register::None,Register::Module,15);
	Register MaxCellVoltage_15 =	Register("MaxCellVoltage",	45214,1,0x60,Register::Read,Register::Unsigned,Register::High,Register::Module,15);
	Register MinCellVoltage_15 =	Register("MinCellVoltage",	45215,1,0x61,Register::Read,Register::Unsigned,Register::High,Register::Module,15);
	Register ModuleStatus_15 =		Register("ModuleStatus",	45216,1,0x62,Register::Read,Register::Unsigned,Register::High,Register::Module,15);
	Register Alarm_15 =				Register("Alarm",			45217,1,0x68,Register::Read,Register::Undefined,Register::High,Register::Module,15);
	Register ModuleVoltage_15 =		Register("ModuleVoltage",	45218,1,0x64,Register::Read,Register::Unsigned,Register::Medium,Register::Module,15);
	Register ModuleCurrent_15 =		Register("ModuleCurrent",	45219,1,0x65,Register::Read,Register::Signed,Register::Medium,Register::Module,15);
	Register ModuleSOC_15 =			Register("ModuleSOC",		45220,1,0x67,Register::Read,Register::Unsigned,Register::Medium,Register::Module,15);
	Register MaxCellTemp_15 =		Register("MaxCellTemp",		45221,1,0x42,Register::Read,Register::Unsigned,Register::Low,Register::Module,15);
	Register MinCellTemp_15 =		Register("MinCellTemp",		45222,1,0x43,Register::Read,Register::Unsigned,Register::Low,Register::Module,15);
	Register ModuleTemperature_15 =	Register("ModuleTemperature",45223,1,0x63,Register::Read,Register::Unsigned,Register::Low,Register::Module,15);
	Register ModuleCycleCount_15 =	Register("ModuleCycleCount",45224,1,0x6F,Register::Read,Register::Unsigned,Register::Low,Register::Module,15);

};
} /* namespace sgw */

#endif /* MAP_HPP_ */
