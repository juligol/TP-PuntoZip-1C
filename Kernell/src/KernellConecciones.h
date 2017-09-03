/*
 * FuncionesKernell.h
 *
 *  Created on: 8/6/2017
 *      Author: utnso
 */

#ifndef KERNELLCONECCIONES_H_
#define KERNELLCONECCIONES_H_

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


#define ID_TIPO_PROCESO 0

char* request_a_proceso(int socket_proceso, char* mensaje) {
	char* data = generar_mensaje(ID_TIPO_PROCESO, 1,  mensaje);
	char* response = request(socket_proceso, data);
	return response;
}




#endif /* KERNELLCONECCIONES_H_ */
