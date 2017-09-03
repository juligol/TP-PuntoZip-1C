/*
 * Genericas.h
 *
 *  Created on: 4/6/2017
 *      Author: utnso
 */

#ifndef GENERICAS_H_
#define GENERICAS_H_

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

#include "Sockets.h"

struct proceso {
	int id_tipo_proceso;
	int id_proceso;
};

int conectar_con_server(struct in_addr ip_kernell, int puerto_kernell) {
	int server = get_socket();
	struct sockaddr_in direccion_kernell = get_direccion(ip_kernell, puerto_kernell);
	conectar(server, direccion_kernell);
	return server;
}

int iniciar_proceso(int id_tipo_proceso, char* nombre_proceso, int id_proceso , struct in_addr ip_kernell, int puerto_kernell) {
	fprintf(stderr,"Proceso %s iniciado\n", nombre_proceso);
	puts("Conectando con Kernel...");
	int kernell = conectar_con_server(ip_kernell, puerto_kernell);
	char * data_inicial = malloc(snprintf(NULL, 0, "%i|%i|%s", id_tipo_proceso, id_proceso, "nuevo-proceso") + 1);
	sprintf(data_inicial, "%i|%i|%s", id_tipo_proceso, id_proceso, "nuevo-proceso");
	enviar_datos(kernell, data_inicial);
	return kernell;
}

int obtener_id_proceso(int kernell) {
	char* id_recibido = recibir_datos(kernell);
	puts("Conectado");
	fprintf(stderr,"ID: %s\n\n", id_recibido);
	return atoi(id_recibido);
}

char* tipo_proceso(int id_tipo_proceso) {
	switch(id_tipo_proceso) {
		case 0:
			return "Kernell";
		break;
		case 1:
			return "Consola";
		break;
		case 2:
			return "CPU";
		break;
		case 3:
			return "File System";
		break;
		case 4:
			return "Admin. memoria";
		break;
		default:
			return -1;
		break;
	}
}

bool esKernell(int id_tipo_proceso) {
	return id_tipo_proceso == 0;
}

char* generar_mensaje(int id_tipo_proceso, int id_proceso, char* mensaje) {
	char * data = malloc(snprintf(NULL, 0, "%i|%i|%s", id_tipo_proceso,id_proceso, mensaje) + 1);
	sprintf(data, "%i|%i|%s", id_tipo_proceso,id_proceso, mensaje);
	return data;
}

char* leer_archivo(char* path) {
	FILE* file = fopen(string_split(path, "\n")[0], "r"); /* Si toma solo la primer linea */
	char line[256];
	char *buffer = string_new();

	while (fgets(line, sizeof(line), file)) {
		string_append(&buffer, line);
	}
	fclose(file);
	return buffer;
}

#endif /* GENERICAS_H_ */
