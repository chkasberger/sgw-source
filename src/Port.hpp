/*
 * Port.hpp
 *
 *  Created on: Mar 7, 2014
 *      Author: ptse
 */

#ifndef PORT_HPP_
#define PORT_HPP_

namespace sgw {

class Port {
public:
	bool DEBUG = false;
	Port();
	virtual ~Port();
	int openPort(const char *port);
	char *sendPort(char *request, int requestLegth);
	int closePort();
	//int p_timeOut = 50;
private:
	int calcCRC(char frame[]);
	enum State{None, Received, Mess, TimeOut};
};

} /* namespace sgw */
#endif /* PORT_HPP_ */
