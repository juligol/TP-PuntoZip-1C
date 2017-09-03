/*
 * manejo_programas.h
 *
 *  Created on: 18/6/2017
 *      Author: utnso
 */

#ifndef MANEJO_PROGRAMAS_H_
#define MANEJO_PROGRAMAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <Funciones/Cpu.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <commons/collections/queue.h> //Para las colas
#include <semaphore.h>


#include "Funciones/Sockets.h"
#include "Funciones/Genericas.h"
#include "Funciones/Mensajes.h"
#include "Funciones/Cpu.h"
#include "Funciones/Structs.h"
#include "kernell_configuracion.h"

/*typedef struct {
	int pid;
	int program_counter;
	int exit_code;
} Pcb;*/

typedef struct {
  int socket_cpu;
  int socket_consola;
  Pcb pcb;
  char* codigo;
} Programa;

int pid = 1;
Programa crear_programa(int socket_consola, char* path) {
	Programa result;
	result.socket_consola = socket_consola;
	result.codigo = leer_archivo(path);
	result.pcb.idProceso = pid;
	result.pcb.programCounter = 1;
	pid++;
	return result;
}

Programa* pop_programa(t_queue * queue) {
	void* p = queue_pop(queue);
	return (struct Programa*) p;
}


#endif /* MANEJO_PROGRAMAS_H_ */
