/*
 * Sockets.h
 *
 *  Created on: 4/6/2017
 *      Author: utnso
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

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

int MAXIMO_TAMANO_DATOS = 2000;

int get_socket() {
	int mi_socket;
	mi_socket = socket(AF_INET, SOCK_STREAM, 0);
	return mi_socket;
}

int get_server() {
	int server = get_socket();
	int yes=1;
	setsockopt(server, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	return server;
}

struct sockaddr_in get_direccion(struct in_addr dir_ip, int puerto) {
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(puerto);
	my_addr.sin_addr = dir_ip;
	memset(&(my_addr.sin_zero), '\0', 8);
	return my_addr;
}

void init_server(int server, struct in_addr ip, int puerto, int backlog) {
	struct sockaddr_in direccion_kernell  = get_direccion(ip, puerto);
	bind(server, (struct sockaddr *)&direccion_kernell, sizeof(struct sockaddr));
	if (listen(server, backlog) == -1) {
		perror("listen");
		exit(1);
	}
}

void enviar_datos(int destinatario, char* mensaje) {
	if (send(destinatario, mensaje, strlen(mensaje), 0) == -1) {
		error("send");
	}else{

	}
}

char* recibir_datos(int socket) {
	char buffer[MAXIMO_TAMANO_DATOS];
	int numero_bytes_recibidos;
	if ((numero_bytes_recibidos=recv(socket, buffer, MAXIMO_TAMANO_DATOS - 1, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	if(numero_bytes_recibidos > 0) {
		buffer[numero_bytes_recibidos] = '\0';
		return buffer;
	}
	return NULL;
}

void conectar(int socket, struct sockaddr_in direccion) {
	if (connect(socket, (struct sockaddr *)&direccion, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}

char* request(int socket, char* mensaje) {
	enviar_datos(socket, mensaje);
	//puts(mensaje);
	char* response = recibir_datos(socket);
	return response;
}

struct in_addr get_direccion_ip() {
	char hostname[1024];
	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	struct hostent *h = gethostbyname(hostname);
	return *((struct in_addr *)h->h_addr);
}

struct mensaje {
	int id_tipo_proceso;
	int id_proceso;
	char* comando;
	char** params_comando;
};

struct mensaje handle_recieved_data(char* recieved_data) {
	struct mensaje message;
	char** params_data = string_split(recieved_data, "|");
	message.id_tipo_proceso = atoi(params_data[0]);
	message.id_proceso = atoi(params_data[1]);
	message.params_comando = string_split(params_data[2], " ");
	message.comando = params_data[2];
	return message;
}

void escuchar(int server, void (*new_conection_handler)(int client)) {
	struct sockaddr_in direccion_cliente;
	int size_direccion_cliente = sizeof(struct sockaddr_in);
	int cliente;

	puts("Escuchando..");
	while (1) {
		if ((cliente = accept(server, (struct sockaddr *)&direccion_cliente, &size_direccion_cliente)) != -1) {
			(*new_conection_handler)(cliente);
		}
	}
}


#endif /* SOCKETS_H_ */
