/*
 * CPU.h
 *
 *  Created on: 5/6/2017
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

#include <parser/metadata_program.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/config.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


struct cpu {
	int socket;
	bool ocupada;
};

#endif /* CPU_H_ */
