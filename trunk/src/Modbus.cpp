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
				modbus_set_error_recovery(ctx,(modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK));
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
			//modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
			modbus_set_slave(ctx, 20);

			if(DEBUG) cout << "Waiting for RTU connection on Port: " << m_Port << "\r" << endl;
			returnValue = modbus_connect(ctx);
			if(DEBUG) cout << "Serial connection started!\r" << endl;
			break;
		default:
			break;
	}

	modbus_set_debug(ctx, DEBUG);
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
			int nToShow = 10;//(query[4]<<8 || query[5]);

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
		switch (subSequence[1]) {
			case 0x06:
				int modbusRegisterAddress;// = (subSequence[2]) | (subSequence[3] << 8);
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
								currentRegister->r_RawData[0] = (char)(mb_mapping->tab_registers[address] & 0x00ff);
								currentRegister->r_RawData[1] = (char)((mb_mapping->tab_registers[address] & 0xff00) >> 8);
							}

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
				returnValue = 0;
				break;
			case 0x10:
				pthread_mutex_lock(m_mtx);
				{
					returnValue = -1;
				}
				pthread_mutex_unlock(m_mtx);
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

