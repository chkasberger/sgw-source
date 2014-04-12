/*
 * Port.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: ptse
 */

#include "Port.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

//extern bool DEBUG;

using namespace std;
namespace sgw {


int tty_fd = 0;
struct termios stdio,oldStdio;

const unsigned int buf_size = 256;
const char *filter = "\n";
char startIndication[2] = {0x06, 0x02};
char *startSequence = &startIndication[0];
char endIndication = 0x03;
char *endSequence = &endIndication;
char errorSequence = 0x15;
enum State{None, Received, Mess, TimeOut};

char buf[buf_size];
char *bufPointerStart = 0;
char *buf_pointer_tmp = 0;
char *bufPointerEnd = 0;
int maxFrameSize = 8;

int RCV_Flag = 0;
unsigned int frm_chksum = 0;

Port::Port() {
    tcgetattr(STDOUT_FILENO,&oldStdio);
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=CS8|CREAD|CLOCAL;
    stdio.c_cflag |= PARENB;
    stdio.c_cflag &= ~PARODD;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=0;
    stdio.c_cc[VTIME]=1;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

Port::~Port() {
	// TODO Auto-generated destructor stub
}

int Port::openPort(const char *port){
    tty_fd=open(port, O_RDWR | O_NONBLOCK | O_NDELAY);

    fcntl(tty_fd, F_SETFL, 0);

    if(tty_fd != -1){
		cfsetospeed(&stdio,B19200);            // 9600 baud
		cfsetispeed(&stdio,B19200);            // 9600 baud
		tcsetattr(tty_fd,TCSANOW,&stdio);
    }
    return (tty_fd);
}

char *Port::sendPort(char *request, int requestLegth){
	int requestSize = requestLegth;

	calcCRC(request);

	char frameSequence[3] = {(char)0x00, (char)0x00, (char)TimeOut};
	char *result = &frameSequence[0];

	tcflush(tty_fd, TCIOFLUSH);
	int writeSuccess = write(tty_fd,request,requestSize);

	if(DEBUG){
		if(writeSuccess < 0)
			cout << "PORT::sendPort: could not write to serial port\r" << endl;
		else
			cout << "PORT::sendPort: successfully wrote to serial port\r" << endl;
		cout << "frame size is: " << dec << requestSize << "->";
		for(int i = 0; i < requestSize; i++)
			cout << hex << (int)request[i] << " ";
		cout << "\r" << endl;
	}
    bufPointerStart=&buf[0];
    buf_pointer_tmp=&buf[0];
    bufPointerEnd=&buf[0];

	int readSize = 0;

	while((readSize = read(tty_fd, bufPointerEnd, buf_size)) > 0){
		if(DEBUG)
			cout << "Port::sendPort: " << "read from port " << readSize << " bytes\r" << endl;
		frameSequence[2] = (char)Received;
		bufPointerEnd += readSize;
		//readSize = 0;

		if(bufPointerEnd > bufPointerStart){
			if(DEBUG)
				cout << "Port::sendPort: " << "end pointer address (" << hex << (int)*(bufPointerEnd-1) << " " << &bufPointerEnd<< ") "
						"is higher than start pointer address ("  << hex << (int)*(bufPointerStart) << " " << &bufPointerStart<< ")\r" << endl;
			if(DEBUG){
				for(int i = 0; i < bufPointerEnd-bufPointerStart; i++)
					cout << hex << (int)*(bufPointerStart+i);
				cout << "\r" << endl;
			}

			// check if error code is received!
			if((strncmp(bufPointerStart, &errorSequence, 1) == 0)){
				if(DEBUG)
					cout << "Port::sendPort" << "received error frame\r" << endl;
				frameSequence[2] = errorSequence;
				//result = &frameSequence[0];
				break;
			}

			// check if maximum frame size is exceeded!
			if(bufPointerEnd - bufPointerStart > maxFrameSize){

				if(DEBUG)
					cout << "Port::sendPort: " << "got messed frame\r" << endl;

				frameSequence[2] = (char)Mess;
				char * subBufPointerStart;
				char * subBufPointerEnd;
				subBufPointerStart = strstr (&buf[0],startSequence);
				subBufPointerEnd = strstr (&buf[0],endSequence);
				if(
					subBufPointerEnd != NULL &&
					subBufPointerStart != NULL &&
					subBufPointerEnd - subBufPointerStart +1 == maxFrameSize){
					if(DEBUG)
						cout << "Port::sendPort" << "rearranged messed frame\r" << endl;
					bufPointerStart = subBufPointerStart;
					bufPointerEnd = subBufPointerEnd;
			  }
			}

			// filter valid frame out of sequence!
			if(
					(strncmp(bufPointerStart, startSequence, 2) == 0) &&
					(strncmp(bufPointerEnd - 2, endSequence, 1) == 0))
			{
				if(DEBUG)
					cout << "Port::sendPort: " << "got valid frame\r" << endl;
				*bufPointerEnd = 0x00;
				bufPointerEnd = bufPointerStart;

				char buf1 = *(bufPointerStart+2);
				char buf2 = *(bufPointerStart+3);
				char buf3 = *(bufPointerStart+4);
				char buf4 = *(bufPointerStart+5);

				//char receivedValue[3];

				frameSequence[1] = (buf1 & 0x0f) | ((buf2 & 0x0f) << 4);// | buf2;
				frameSequence[0] = (buf3 & 0x0f) | ((buf4 & 0x0f) << 4);// | buf2;

				frameSequence[2] = None;
				//result = &frameSequence[0];
				break;
			}
		}
	}
/*
		while ((nbytes = read(tty_fd, bufptr, buffer + sizeof(buffer) - bufptr - 1)) > 0)
		 {
		   bufptr += nbytes;
		   //if (bufptr[-1] == '\n' || bufptr[-1] == '\r')
			//		break;
		 }

		//if (read(tty_fd,&c,1)>0)        write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
		//if (read(STDIN_FILENO,&c,1)>0)  write(tty_fd,&c,1);                     // if new data is available on the console, send it to the serial port
*/
	return result;
}

int Port::calcCRC(char *frame){

	char checkSum = 0x00;
	//int frameSize = sizeof(frame)/sizeof(frame[0]);
	int frameSize = 8;
	for (int i = 0; i < frameSize - 1; i++) {
		checkSum += frame[i];
	}
	frame[frameSize-1] = ~checkSum + 0x01;

	return frameSize;
}

int Port::closePort(){
	int exitCode = close(tty_fd);
    tcsetattr(STDOUT_FILENO,TCSANOW,&oldStdio);

	return (exitCode);
}
} /* namespace sgw */
