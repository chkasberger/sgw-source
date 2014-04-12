/*
 * Modbus.hpp
 *
 *  Created on: Mar 23, 2014
 *      Author: root
 */

#ifndef MODBUS_HPP_
#define MODBUS_HPP_

#include "Register.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include </usr/include/modbus/modbus.h>
//#include <modbus/modbus.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <list>
#include <algorithm>
#include <iostream>
#include <iomanip>
//#include <pthread.h>

using namespace std;

namespace sgw {

class Modbus {
public:
	bool DEBUG = true;
	Modbus();
	virtual ~Modbus();
	void openServer(const char *mode, const char *port, pthread_mutex_t *mtx, list<Register*> *l);
private:
	int setupModbusConnection();
	void closeServer();
	int updateModbusMap(modbus_mapping_t *mb_mapping, list<Register*> *l);
	int updateModbusMap(modbus_mapping_t *mb_mapping, list<Register*> *l, uint8_t *subSequence);

	//void *checkSocketHealth(void *socket);
	void listenModbusConnection();
	void listenModbusConnectionTCP();
	void listenModbusConnectionRTU();
};


} /* namespace sgw */

#endif /* MODBUS_HPP_ */
