/*
 * Modbus.cpp
 *
 *  Created on: Mar 23, 2014
 *      Author: root
 */

#include "Modbus.hpp"
#include <string>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <arpa/inet.h>

//extern bool DEBUG;
namespace sgw {

enum ModbusMode {TCP, RTU};
enum SocketState {CLOSED=0, OPEN};
int slaveAddress = 20;
int socket = -1;
int socketState = CLOSED;
int m_Connections = 3;
int use_backend = TCP;
int max_adu_length = 0;
const char *m_Port = 0;
const char *m_Mode = 0;
modbus_t *ctx = 0;
modbus_mapping_t *mb_mapping = NULL;
static list<Register*> *m_RegisterList = 0;
pthread_mutex_t *m_mtx = NULL;

Modbus::Modbus(){

}

void Modbus::openServer(const char *mode, const char *port, pthread_mutex_t *mtx, list<Register*> *l){
	m_Mode = mode;
	m_Port = port;
    m_RegisterList = l;
    m_mtx = mtx;

	setupModbusConnection();
	listenModbusConnection();
}

void Modbus::closeServer(){
	if(DEBUG) cout << "Modbus::closeServer: " << modbus_strerror(errno) << "\r" << endl;

	if (use_backend == TCP){
		if(DEBUG) cout << "Modbus::closeServer: " << "Close server\r" << endl;
		close(socket);
		socketState = CLOSED;
	}

	modbus_close(ctx); // close
	modbus_free(ctx);

	ctx = NULL;
}

int Modbus::setupModbusConnection(){
	if(DEBUG) cout << "try to set up modbus connection in " << m_Mode << " mode; with port " << m_Port << "\r" << endl;
	int returnValue = -1;

	if (mb_mapping == NULL) {

		mb_mapping = modbus_mapping_new(0, 0, 50000, 0);
		mb_mapping->tab_registers[0] = 0;

		if (mb_mapping == NULL) {
			if(DEBUG) cout << "Failed to allocate the mapping: " << modbus_strerror(errno) << endl;
			modbus_free(ctx);
			return (-1);
		}
	}

	/* Select Modbus mode TCP|RTU; default TCP */
	if (strcmp(m_Mode, "tcp") == 0) {
		use_backend = TCP;
	} else if (strcmp(m_Mode, "rtu") == 0) {
		use_backend = RTU;
	} else {
		use_backend = TCP;
	}
	switch (use_backend) {
		case TCP:
	    	max_adu_length = MODBUS_TCP_MAX_ADU_LENGTH;
			ctx = modbus_new_tcp("NULL", atoi(m_Port));
			if(ctx == NULL)
				cout << "could not allocate ctx\r" << endl;
			else{
				//modbus_set_slave(ctx, 3);
				//modbus_set_error_recovery(ctx,(modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK));
				if(socketState == CLOSED){
					socket = modbus_tcp_listen(ctx, m_Connections);
					if(socket <= 0)
						cout << "could not allocate socket\r" << endl;
					else{
						int optval = 0;
						socklen_t optlen = sizeof(optval);

						socketState = OPEN;
					   /* Set TCP KEEPALIVE active */
					   optval = 1;
					   optlen = sizeof(optval);
					   if(setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
						  perror("setsockopt()");
						  close(socket);
						  exit(EXIT_FAILURE);
					   }else
						   cout << "SO_KEEPALIVE set on socket\r" << endl;
					}
				}
			}
			break;
		case RTU:
			max_adu_length = MODBUS_RTU_MAX_ADU_LENGTH;
			ctx = modbus_new_rtu(m_Port, 9600, 'N', 8, 1);
			modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
			modbus_set_slave(ctx, slaveAddress);

			if(DEBUG) cout << "Waiting for RTU connection on Port: " << m_Port << "\r" << endl;
			returnValue = modbus_connect(ctx);
			if(DEBUG) cout << "Serial connection started!\r" << endl;
			break;
		default:
			break;
	}

	//if(DEBUG)
		//modbus_set_debug(ctx, DEBUG);
    return (returnValue);
}

void Modbus::listenModbusConnectionTCP(){
	int rc;

	fd_set refset;
	fd_set rdset;
	int fdmax;
	int master_socket;
	FD_ZERO(&refset);
	FD_SET(socket, &refset);
	fdmax = socket;

    while(true){
    	usleep(100);
    	rdset = refset;
    	if(select(fdmax+1, &rdset, NULL, NULL, NULL) == -1){
    		exit(-1);
    	}

    	for (master_socket = 0; master_socket <= fdmax; master_socket++) {
			if (!FD_ISSET(master_socket, &rdset)) {
				continue;
			}

			if (master_socket == socket) {
				/* A client is asking a new connection */
				socklen_t addrlen;
				struct sockaddr_in clientaddr;
				int newfd;

				/* Handle new connections */
				addrlen = sizeof(clientaddr);
				memset(&clientaddr, 0, sizeof(clientaddr));
				newfd = accept(socket, (struct sockaddr *)&clientaddr, &addrlen);
				if (newfd == -1) {
					perror("Server accept() error");
				} else {
					FD_SET(newfd, &refset);

					if (newfd > fdmax) {
						/* Keep track of the maximum */
						fdmax = newfd;
					}
					cout << "New connection from " << inet_ntoa(clientaddr.sin_addr) << ":" << dec << clientaddr.sin_port << " on socket " << newfd << "\r" << endl;
					//printf("New connection from %s:%d on socket %d\n", inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
				}
			} else {
				modbus_set_socket(ctx, master_socket);
				uint8_t query[max_adu_length];
				uint8_t *subSequence;

				rc = modbus_receive(ctx, query);
				if (rc > 0) {
					if(query[7] == 0x03 || 0x06 || 0x10){
						subSequence = &query[06];
						/*if(dummy){
							mb_mapping->tab_registers[45000] = 0x0000;	// 	DesignCapacity		0	0
							mb_mapping->tab_registers[45001] = 0x0c00;	// 	DesignVoltage		3072	3072
							mb_mapping->tab_registers[45002] = 0x435f;	// 	ManufactureDate		17247	17247
							mb_mapping->tab_registers[45003] = 0x0012;	// 	SerialNumber		18	18
							mb_mapping->tab_registers[45004] = 0x534f;	// 	ManufactureName		21327	21327
							mb_mapping->tab_registers[45005] = 0x4335;	// 	DeviceName		17205	17205
							mb_mapping->tab_registers[45006] = 0x4c46;	// 	DeviceChemistry		19526	19526
							mb_mapping->tab_registers[45007] = 0x5001;	// 	ModelName		20481	20481
							mb_mapping->tab_registers[45008] = 0x003f;	// 	ConnectedModuleInfo		63	63
							mb_mapping->tab_registers[45009] = 0x1217;	// 	ControllerInfo		4631	4631
							mb_mapping->tab_registers[45010] = 0x1701;	// 	FirmwareVersion		5889	5889
							mb_mapping->tab_registers[45011] = 0x0010;	// 	Status		16	16
							mb_mapping->tab_registers[45012] = 0x00c0;	// 	BatteryStatus		192	192
							mb_mapping->tab_registers[45013] = 0x0c48;	// 	Voltage		3144	3144
							mb_mapping->tab_registers[45014] = 0xffac;	// 	Current		-84	65452
							mb_mapping->tab_registers[45015] = 0x0000;	// 	SysWarning		0	0
							mb_mapping->tab_registers[45016] = 0x0cce;	// 	SysMaxCellVoltage		3278	3278
							mb_mapping->tab_registers[45017] = 0x0cc8;	// 	SysMinCellVoltage		3272	3272
							mb_mapping->tab_registers[45018] = 0x0b8e;	// 	Temperature		2958	2958
							mb_mapping->tab_registers[45019] = 0x0027;	// 	RelativeStateOfCharge		39	39
							mb_mapping->tab_registers[45020] = 0x03a2;	// 	RemainCapacity		930	930
							mb_mapping->tab_registers[45021] = 0x0960;	// 	FullChargeCapacity		2400	2400
							mb_mapping->tab_registers[45022] = 0xffc1;	// 	AverageCurrent		-63	65473
							mb_mapping->tab_registers[45023] = 0x0960;	// 	ChargingCurrent		2400	2400
							mb_mapping->tab_registers[45024] = 0x0d80;	// 	ChargingVoltage		3456	3456
							mb_mapping->tab_registers[45025] = 0x002d;	// 	CycleCount		45	45
							mb_mapping->tab_registers[45026] = 0x0000;	// 	HeaterStatus		0	0
							mb_mapping->tab_registers[45027] = 0x0000;	// 	FANStatus		0	0
							mb_mapping->tab_registers[45028] = 0x0ba9;	// 	SysMaxCellTemp		2985	2985
							mb_mapping->tab_registers[45029] = 0x0b6a;	// 	SysMinCellTemp		2922	2922
							mb_mapping->tab_registers[45030] = 0x0c00;	// 	HeatsinkTemperature		3072	3072
							mb_mapping->tab_registers[45031] = 0xffff;	// 	IGBTControl		-1	65535
							mb_mapping->tab_registers[45032] = 0x030f;	// 	IGBTStatus		783	783
						}
						else{
							updateModbusMap(mb_mapping, m_RegisterList, subSequence); // every request we want a new set of cards
						}*/
						updateModbusMap(mb_mapping, m_RegisterList, subSequence); // every request we want a new set of cards
					}
					modbus_reply(ctx, query, rc, mb_mapping);

				} else if (rc == -1) {
					/* This example server in ended on connection closing or
					 * any errors. */
					cout << "Connection closed on socket " << master_socket << " due to " << modbus_strerror(errno) << "\r" << endl;
					close(master_socket);

					/* Remove from reference set */
					FD_CLR(master_socket, &refset);

					if (master_socket == fdmax) {
						fdmax--;
					}
				}
			}
    	}
    }
}

void Modbus::listenModbusConnectionRTU(){
	while(true){
		if(DEBUG) cout << "MODBUS::listenModbusConnectionRTU: entered while loop!\r" << endl;
		usleep(100);
		uint8_t query[MODBUS_RTU_MAX_ADU_LENGTH];
		int rc = modbus_receive(ctx, query);
		if(DEBUG) cout << "MODBUS::listenModbusConnectionRTU: got request!\r" << endl;

		if (rc >= 0)
		{
			if(query[0] == slaveAddress){
				if(DEBUG) cout << "Modbus::listenModbusConnection: " << "got frame with my slave address!\r" << endl;
				int nToShow = 10;//(query[4]<<8 || query[5]);

				/*if(dummy){
					mb_mapping->tab_registers[45000] = 0x0000;	// 	DesignCapacity		0	0
					mb_mapping->tab_registers[45001] = 0x0c00;	// 	DesignVoltage		3072	3072
					mb_mapping->tab_registers[45002] = 0x435f;	// 	ManufactureDate		17247	17247
					mb_mapping->tab_registers[45003] = 0x0012;	// 	SerialNumber		18	18
					mb_mapping->tab_registers[45004] = 0x534f;	// 	ManufactureName		21327	21327
					mb_mapping->tab_registers[45005] = 0x4335;	// 	DeviceName		17205	17205
					mb_mapping->tab_registers[45006] = 0x4c46;	// 	DeviceChemistry		19526	19526
					mb_mapping->tab_registers[45007] = 0x5001;	// 	ModelName		20481	20481
					mb_mapping->tab_registers[45008] = 0x003f;	// 	ConnectedModuleInfo		63	63
					mb_mapping->tab_registers[45009] = 0x1217;	// 	ControllerInfo		4631	4631
					mb_mapping->tab_registers[45010] = 0x1701;	// 	FirmwareVersion		5889	5889
					mb_mapping->tab_registers[45011] = 0x0010;	// 	Status		16	16
					mb_mapping->tab_registers[45012] = 0x00c0;	// 	BatteryStatus		192	192
					mb_mapping->tab_registers[45013] = 0x0c48;	// 	Voltage		3144	3144
					mb_mapping->tab_registers[45014] = 0xffac;	// 	Current		-84	65452
					mb_mapping->tab_registers[45015] = 0x0000;	// 	SysWarning		0	0
					mb_mapping->tab_registers[45016] = 0x0cce;	// 	SysMaxCellVoltage		3278	3278
					mb_mapping->tab_registers[45017] = 0x0cc8;	// 	SysMinCellVoltage		3272	3272
					mb_mapping->tab_registers[45018] = 0x0b8e;	// 	Temperature		2958	2958
					mb_mapping->tab_registers[45019] = 0x0027;	// 	RelativeStateOfCharge		39	39
					mb_mapping->tab_registers[45020] = 0x03a2;	// 	RemainCapacity		930	930
					mb_mapping->tab_registers[45021] = 0x0960;	// 	FullChargeCapacity		2400	2400
					mb_mapping->tab_registers[45022] = 0xffc1;	// 	AverageCurrent		-63	65473
					mb_mapping->tab_registers[45023] = 0x0960;	// 	ChargingCurrent		2400	2400
					mb_mapping->tab_registers[45024] = 0x0d80;	// 	ChargingVoltage		3456	3456
					mb_mapping->tab_registers[45025] = 0x002d;	// 	CycleCount		45	45
					mb_mapping->tab_registers[45026] = 0x0000;	// 	HeaterStatus		0	0
					mb_mapping->tab_registers[45027] = 0x0000;	// 	FANStatus		0	0
					mb_mapping->tab_registers[45028] = 0x0ba9;	// 	SysMaxCellTemp		2985	2985
					mb_mapping->tab_registers[45029] = 0x0b6a;	// 	SysMinCellTemp		2922	2922
					mb_mapping->tab_registers[45030] = 0x0c00;	// 	HeatsinkTemperature		3072	3072
					mb_mapping->tab_registers[45031] = 0xffff;	// 	IGBTControl		-1	65535
					mb_mapping->tab_registers[45032] = 0x030f;	// 	IGBTStatus		783	783
				}
				else{
					updateModbusMap(mb_mapping, m_RegisterList, query); // every request we want a new set of cards
				}
				*/

				updateModbusMap(mb_mapping, m_RegisterList, query); // every request we want a new set of cards

				if(DEBUG){
					cout << "Replying to request num bytes= " << hex << rc << ": ";
					for(int i=0;i<rc;i++)
					  cout << setw(2) << setfill('0') << hex << (short)query[i];
					cout << "\r" << endl;
				}

				int received = modbus_reply(ctx, query, rc, mb_mapping);
				if(DEBUG){
					if(received < 0) cout << "Modbus::listenModbusConnection: " << modbus_strerror(errno) << "\r" << endl;

					cout << "tab_registers =";
					for(int i=45000;i<nToShow;i++)
						cout << " " << hex << mb_mapping->tab_registers[i];
					cout << "\r" << endl;
				}
			}
			else
				if(DEBUG) cout << "Modbus::listenModbusConnection: " << "got frame that does not belong to me!\r" << endl;
		} else {
			if(DEBUG) cout << "Modbus::listenModbusConnection: " << modbus_strerror(errno) << "\r" << endl;

			//if(DEBUG) cout << "Modbus::listenModbusConnection: lost connection; try to setup a new one!\r" << endl;
			//closeServer();
			//setupModbusConnection();
			//pthread_exit(NULL);
		}
	}
}

void Modbus::listenModbusConnection(){
	switch (use_backend) {
		case TCP:
			listenModbusConnectionTCP();
			break;
		case RTU:
			listenModbusConnectionRTU();
			break;
		default:
			break;
	}
}

int Modbus::updateModbusMap(modbus_mapping_t *mb_mapping, list<Register*> *l, uint8_t *subSequence){
	int returnValue = -1;
	if(mb_mapping != NULL && subSequence != NULL){
		mb_mapping->tab_registers[44999]++; // increment the holding reg 0 for each read
		int modbusRegisterAddress, modbusRegisterAddressEnd, modbusRegisterLegth;
		switch (subSequence[1]) {
			case 0x06:
				modbusRegisterAddress = (subSequence[2] << 8) | (subSequence[3]);
				pthread_mutex_lock(m_mtx);
				{
					for_each(l->begin(),l->end(),[mb_mapping, modbusRegisterAddress](Register* currentRegister)
					{
						if(currentRegister != NULL){
							//cout << "Modbus::updateModbusConnection: copy register " << currentRegister->r_ModbusAddress;
							int address = currentRegister->r_ModbusAddress;
							if(address == modbusRegisterAddress){
								//short value = 0x0000;
								currentRegister->r_RawData[1] = (char)(mb_mapping->tab_registers[address] & 0x00ff);
								currentRegister->r_RawData[0] = (char)((mb_mapping->tab_registers[address] & 0xff00) >> 8);
							}
						}
					});
				}
				pthread_mutex_unlock(m_mtx);
				returnValue = 0;
				break;
			case 0x10:
				modbusRegisterAddress = (subSequence[2] << 8) | (subSequence[3]);
				modbusRegisterLegth = (subSequence[4] << 8) | (subSequence[5]);
				modbusRegisterAddressEnd = modbusRegisterAddress + modbusRegisterLegth - 1;

				pthread_mutex_lock(m_mtx);
				{
					for_each(l->begin(),l->end(),[mb_mapping, modbusRegisterAddressEnd](Register* currentRegister)
						{
							if(currentRegister != NULL){
								//cout << "Modbus::updateModbusConnection: copy register " << currentRegister->r_ModbusAddress;
								int address = currentRegister->r_ModbusAddress;
								if(address <= modbusRegisterAddressEnd){
									//short value = 0x0000;
									cout << mb_mapping->tab_registers[address] << "\r" << endl;
									currentRegister->r_RawData[1] = (char)(mb_mapping->tab_registers[address] & 0x00ff);
									currentRegister->r_RawData[0] = (char)((mb_mapping->tab_registers[address] & 0xff00) >> 8);
								}
							}
						});

				}
				pthread_mutex_unlock(m_mtx);
				returnValue = -1;
				break;
			case 0x03:
				//if(DEBUG) cout << "Modbus::updateModbusConnection: increment byte 0 of mb_mapping!\r" << endl;
				pthread_mutex_lock(m_mtx);
				{
					//if(DEBUG) cout << "Modbus::updateModbusConnection: mutex locked!\r" << endl;
					for_each(l->begin(),l->end(),[mb_mapping](Register* currentRegister)
					{
						if(currentRegister != NULL){
							//cout << "Modbus::updateModbusConnection: copy register " << currentRegister->r_ModbusAddress;
							int address = currentRegister->r_ModbusAddress;
							short value = 0x0000;
							value = currentRegister->r_RawData[1] & 0x00ff;
							value += (currentRegister->r_RawData[0] *0x100);
							//if(DEBUG) cout << "Modbus::updateModbusConnection: with value " << value << "\r" << endl;
							mb_mapping->tab_registers[address] = value;
							//if(DEBUG) cout << "Modbus::updateModbusConnection: now value " << dec << mb_mapping->tab_registers[address] << "\r" << endl;
						}
					});
				}
				pthread_mutex_unlock(m_mtx);
				//if(DEBUG) cout << "Modbus::updateModbusConnection: mutex unlocked!\r" << endl;
				returnValue = 0;

				break;
			default:
				break;
		}
	}
	return (returnValue);
}

Modbus::~Modbus() {
	// TODO Auto-generated destructor stub
	cout << "Modbus::~Modbus: called distructor!\r" << endl;
}

} /* namespace sgw */

