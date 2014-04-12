//============================================================================
// Name        : SGW.cpp
// Author      : Christian Kasberger
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Port.hpp"
#include "Map.hpp"
#include "Register.hpp"
#include "Modbus.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

bool DEBUG = false;

using namespace std;
using namespace sgw;

Port p;
Map m_Map;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *p_mtx = &mtx;
stringstream screenBuffer;

struct Arguments {
	    const int   *argCount;
	    const char *argSonyPort;
	    const char *argModbusType;
	    const char *argModbusPort;
	    bool DEBUG;
	  };


char *updateRegister(Register *r){
	if(r != NULL){
		bool readRegister = false;
		time_t timeStamp = time(NULL);
		char resultValues[3];
		char *result = &resultValues[0];

		if(r->r_ReadWrite == Register::Read){
			switch (r->r_Priority) {
				case Register::High:
					if((timeStamp - r->r_TimeStamp) >= 1)
						readRegister = true;
					break;
				case Register::Medium:
					if((timeStamp - r->r_TimeStamp) >= 60)
						readRegister = true;
					break;
				case Register::Low:
					if((timeStamp - r->r_TimeStamp) >= 600)
						readRegister = true;
					break;
				case Register::None:
					if((timeStamp - r->r_TimeStamp) >= 86400) //one day
						readRegister = true;
					break;
				default:
					break;
			}
			if(readRegister || r->r_Count <= 0){
				char readFrame[]{0x02, 0x10, 0x20, 0x30, 0x40, 0x50, 0x03, 0x00};
				char *readPntr = &readFrame[0];
				char scsCommand = r->r_ScsCommand;

				if(r->r_Type == Register::Module){
					readFrame[1] = 0x11;
				}
				//read[2] = (char)r->r_InternalAddress | 0x20;
				readFrame[3] = (char)r->r_InternalAddress | 0x30;
				readFrame[4] = ((scsCommand & 0x0F) | 0x40);
				scsCommand = scsCommand >> 4;
				readFrame[5] = ((scsCommand & 0x0F) | 0x50);

				result = p.sendPort(readPntr, 8);
				if(result != NULL){
					pthread_mutex_lock(p_mtx);
					{
						r->r_RawData[0] = result[0];
						r->r_RawData[1] = result[1];
						r->r_TimeStamp = time(NULL);
						if(result[2] == 0x00)
							r->r_Count++;
					}
					pthread_mutex_unlock(p_mtx);
				}
			}
		}
		if(r->r_ReadWrite == Register::Write && r->r_Count < 0){
			char writeFrame[]{0x02, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x03, 0x00};
			char *writePntr = &writeFrame[0];
			char scsCommand = r->r_ScsCommand;

			if(r->r_Type == Register::Module){
				writeFrame[1] = 0x11;
			}
			//read[2] = (char)r->r_InternalAddress | 0x20;
			writeFrame[3] = (char)r->r_InternalAddress | 0x30;
			writeFrame[4] = ((scsCommand & 0x0F) | 0x40);
			scsCommand = scsCommand >> 4;
			writeFrame[5] = ((scsCommand & 0x0F) | 0x50);

			writeFrame[6] = r->r_RawData[0];
			writeFrame[7] = r->r_RawData[1];
			writeFrame[8] = 0x00;
			writeFrame[9] = 0x00;

			result = p.sendPort(writePntr, 12);
			if(result != NULL){
				pthread_mutex_lock(p_mtx);
				{
					r->r_TimeStamp = time(NULL);
					if(result[0] == 0x06)
						r->r_Count = 0;
					else
						r->r_Count = -1;
				}
				pthread_mutex_unlock(p_mtx);
			}

		}

		if(DEBUG){
			short intVal = 0;
			unsigned short uIntVal = 0;

			screenBuffer << dec << r->r_ModbusAddress << "\t";
			const char* g;
			g = (const char*)r->r_Name;

			std :: string str;
			str.append (g);
			int strLength = str.length();
			for(int i = 0; i <= 25-strLength; i++)
				str.append(" ");

			screenBuffer << str;

			for (int i = 0; i < 2; i++) {
			  screenBuffer << setw(2) << setfill('0') << hex << ((short) r->r_RawData[i] & 0x00ff);
			}

			intVal +=  + r->r_RawData[1] & 0x00ff;
			intVal += (r->r_RawData[0] << 8);

			uIntVal += r->r_RawData[1] & 0x00ff;
			uIntVal += (r->r_RawData[0] << 8);

			screenBuffer << dec << "\t" << intVal << "\t" << uIntVal << "\t";

			int errorCode = 0;
			if(result != NULL)
				errorCode = (int)result[2];

			screenBuffer << r->r_Count << "\t" << hex << errorCode << "\r\n";

		}
	} else{
		cout << "got invalid register pointer!\r" << endl;
	}
	return r->r_RawData;
}

void updateAllRegister(list<Register*> *l){
	if(DEBUG)
		screenBuffer << "Address\tName\t\t\t  hex\tsig\tusig\tcnt\r\n";

	for_each(l->begin(), l->end(), updateRegister);
	if(DEBUG){
		cout << "\033[2J\033[1;1H";
		cout << screenBuffer.str() << endl;
		screenBuffer.str( string() );
		screenBuffer.clear();
	}
}

void *startModbusConnection(void *arguments){

	Arguments *args = static_cast<Arguments*>(arguments);

	if(true){
		cout << "arg count: " << *args->argCount << "\r" << endl;
		cout << "arg sony port: " << args->argSonyPort << "\r" << endl;
		cout << "arg modbus: " << args->argModbusType << "\r" << endl;
		cout << "arg port: " << args->argModbusPort << "\r" << endl;
		//cout << "arg port2: " << args->argModbusPort2 << "\r" << endl;
		//cout << "arg debug: " << args->DEBUG << "\r" << endl;
	}

	if(*args->argCount >= 1){
		cout << "initiate modbus connection(s)!\r" << endl;
			Modbus modbus = Modbus();
			modbus.openServer(args->argModbusType,args->argModbusPort,&mtx,&m_Map.m_registerListController);
	}

	return (0);
}

int main(int argc, char *argv[]) {
	cout << "!!!Passing Arguments!!!\r" << endl; // prints !!!Hello World!!!

	Arguments arguments, argsAlternative;
	//*arguments.argCount = 5;
	arguments.argSonyPort = "/dev/ttyUSB0";
	arguments.argModbusType = "tcp";
	arguments.argModbusPort = "502";
	//arguments.argModbusPort2 = "/dev/ttyUSB1";
	arguments.DEBUG = false;

	argsAlternative.argCount = &argc;
	argsAlternative.argSonyPort = NULL;
	argsAlternative.argModbusType = "rtu";
	argsAlternative.argModbusPort = "/dev/ttyUSB1";
	argsAlternative.DEBUG = false;


	if(argc > 0){

		arguments.argCount = &argc;
		if(argc > 1)
			arguments.argSonyPort = argv[1];
		if(argc > 2)
			arguments.argModbusType = argv[2];
		if(argc > 3)
			arguments.argModbusPort = argv[3];

		if(argc > 4 && (strcmp(argv[2], "both") == 0)){
			arguments.argModbusType = "tcp";
			argsAlternative.argModbusPort = argv[4];
		}
		else{
			if(argc > 4 && atoi(argv[4]) == 1){
				DEBUG = true;
			}
	  }
	  if(argc > 5 && atoi(argv[4]) == 1){
		  DEBUG = true;
	  }
	}

	cout << "!!!Starting Gateway!!!\r" << endl; // prints !!!Hello World!!!
	pthread_t threadModbusTCP, threadModbusRTU;
	//pthread_t threadArray[]{threadModbusTCP, threadModbusRTU};

	if(strncmp(argv[2],"both",1) == 0){
			pthread_create(&threadModbusRTU,NULL, startModbusConnection, (void *)&argsAlternative);
			pthread_create(&threadModbusTCP,NULL, startModbusConnection, (void *)&arguments);
	}
	else if(strcmp(arguments.argModbusType,"tcp") == 0){
			pthread_create(&threadModbusTCP,NULL, startModbusConnection, (void *)&arguments);
	}
	else if(strcmp(arguments.argModbusType,"rtu") == 0){
			pthread_create(&threadModbusRTU,NULL, startModbusConnection, (void *)&arguments);
	}

	p = Port();
	if(p.openPort(arguments.argSonyPort) < 0){
		cout << "failed to open port\r" << endl;
	}

	 unsigned char c='D';
	 while(c != 'q'){
		read(STDIN_FILENO,&c,1);

		if(c == 'd')
			DEBUG = true;
		if(c == 'n'){
			DEBUG = false;
		}
		usleep(1000000);
		updateAllRegister(&m_Map.m_registerListController);
		//usleep(500000);
		//updateAllRegister(&m_Map.m_registerListModule);

		/*for(unsigned int i = 0; i < 2; i++)
		if (pthread_kill(threadArray[i], 0) == ESRCH){
			cout << "threadModbus is not alive any more; try to restart!\r" << endl;
			if(int rc = pthread_create(&threadArray[i],NULL, startModbusConnection, (void *)&arguments))
				cout << "Error:unable to create thread, " << rc << endl;
		}
		*/
	//int rc = pthread_create(&threads[1],NULL, startModbusConnection, (void *)1);
		//usleep(300000);
		//updateAllRegister(&m_Map.m_registerListModule);
		//system("stty sane");

	}

	p.closePort();
	//pthread_exit(&threadModbus);
	//pthread_cancel(threadModbus);
	system("stty sane");
	return (0);
}
