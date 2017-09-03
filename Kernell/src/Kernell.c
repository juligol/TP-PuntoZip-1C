/*
 ============================================================================
 Name        : Kernell.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

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
#include "manejo_programas.h"

#define ID_TIPO_PROCESO 0
#define PROCESO_CONSOLA 1
#define PROCESO_CPU 2
#define MAX_CPUS 10

//Aca estan las 3 colas
t_queue* cola_new;
t_queue* cola_exec;
t_queue* cola_ready;

pthread_mutex_t mcola_new,mcola_exec,mcola_ready;
/*	PARA USAR LAS COLAS Y LOS SEMAFOROS SERIA ALGO ASI
 *
 	proceso = queue_pop(colas_semaforos[i]);
	pthread_mutex_lock(&mcola_ready);
	queue_push(cola_ready, proceso);
	pthread_mutex_unlock(&mcola_ready);
	sem_post(&sem_ready);
 */

int id_ultima_consola = 0;
int id_ultima_cpu = 0;

int memoria_socket = -1;
int fs_socket = -1;

bool esConsola(int id_tipo_proceso) {
	return id_tipo_proceso == 1;
}

int nuevo_proceso(int id_tipo_proceso) {
	if (esConsola(id_tipo_proceso)) {
		id_ultima_consola++;
		return id_ultima_consola;
	}
	else {
		id_ultima_cpu++;
		return id_ultima_cpu;
	}
}

char* request_a_proceso(int socket_proceso, char* mensaje) {
	char* data = generar_mensaje(ID_TIPO_PROCESO, 1,  mensaje);
	char* response = request(socket_proceso, data);
	return response;
}

char* request_a_fs(char* mensaje) {
	char* response = request_a_proceso(fs_socket, mensaje);
	return response;
}

char* request_a_memoria(char* mensaje) {
	char* response = request_a_proceso(memoria_socket, mensaje);
	return response;
}

struct cpu cpus[MAX_CPUS];
int cant_cpus = 0;

void nueva_cpu(int socket) {
	int i = 0;
	bool found_disponible = false;
	while(i < MAX_CPUS && !found_disponible) {
		if(cpus[i].socket == 0) {
			cpus[i].socket = socket;
			cant_cpus++;
			found_disponible = true;
		}
		i++;
	}
}

struct cpu get_cpu_disponible() {
	int i = 0;
	bool found_disponible = false;
	while(i < MAX_CPUS && !found_disponible) {
		if(!cpus[i].ocupada) {
			found_disponible = true;
			return cpus[i];
		}
		i++;
	}
}

struct proceso handle_accion(int cliente, struct mensaje message) {
	struct proceso process;
	process.id_tipo_proceso = message.id_tipo_proceso;
	process.id_proceso = message.id_proceso;

	char* comando_ejecutar = message.comando;
	if (string_contains(comando_ejecutar,"nuevo-proceso")) {
		int id = nuevo_proceso(message.id_tipo_proceso);
		process.id_proceso = id;
		char data[2];
		sprintf(data, "%d", process.id_proceso);
		enviar_datos(cliente, data);
		mensaje_nuevo_proceso(id, message.id_tipo_proceso);
	}
	else {
		char* response;
		mensaje_comando(message.id_tipo_proceso, message.id_proceso, message.comando );
		if (string_contains(comando_ejecutar,"pedir-memoria")) {
			response = request_a_memoria("obtener-memoria");
			enviar_datos(cliente, response);
		}

		if (string_contains(comando_ejecutar,"borrar-archivo")) {
			response = request_a_fs(message.comando);
			enviar_datos(cliente, response);
		}

		if (string_contains(comando_ejecutar,"ejecutar-programa")) {
			if(cant_cpus > 0) {
				struct cpu cpu_disponible = get_cpu_disponible();
				cpu_disponible.ocupada = true;
				//nuevo_programa(cliente, message);
				char* messageGenerado = generar_mensaje(ID_TIPO_PROCESO, 1, comando_ejecutar);
				enviar_datos(cpu_disponible.socket, messageGenerado);
			}
			else {
				enviar_datos(cliente, "No hay cpus disponibles");
			}
		}
	}

	return process;
}

void nuevo_programa(int socket_consola, struct mensaje message) {
	Programa programa = crear_programa(socket_consola, message.params_comando[1]);
	if(1) { //Valida si puede agregarlo a la cola de nuevos
		queue_push(cola_new, &programa);
		void* p = queue_pop(cola_new);
		Programa* prog = p;
		puts(prog->codigo);
	}
}



void handle_conection(int cliente) {
	int conection = 1;
	struct proceso process;
	while(conection) {
		char* recieved_data = recibir_datos(cliente);
		if (recieved_data == NULL) {
			mensaje_desconeccion(process.id_tipo_proceso, process.id_proceso);
			close(cliente);
			conection = 0;
		}
		else {
			struct mensaje message = handle_recieved_data(recieved_data);
			if(!esConsola(message.id_tipo_proceso)) {
				nueva_cpu(cliente);
			}
			process = handle_accion(cliente, message);
		}
	}
}

void new_conection_handler(int cliente) {
	puts("\nConexion recibida\n");
	pthread_t new_thread;
	pthread_create(&(new_thread), NULL, &handle_conection, cliente);
}

void iniciar_consola_kernell() {
	char comando[50];
	while(1) {
		fgets(comando, 50, stdin);
		puts(comando);
	}
}

int main(void) {
	//Datos hardcodeados
	struct in_addr ip_kernell =  get_direccion_ip();
	int puerto_kernell = 5052;
	int backlog = 5;

	struct in_addr ip_fs =  get_direccion_ip();
	int puerto_fs = 5053;

	struct in_addr ip_memoria =  get_direccion_ip();
	int puerto_memoria = 5054;
	//!Datos hardcodeados

	fs_socket = conectar_con_server(ip_fs, puerto_fs);
	memoria_socket = conectar_con_server(ip_memoria, puerto_memoria);
	int kernell = get_server();
	init_server(kernell, ip_kernell, puerto_kernell, backlog);

	cola_new = queue_create();
	cola_exec = queue_create();
	cola_ready = queue_create();

	pthread_t thread_consola_kernell;
	pthread_create(&(thread_consola_kernell), NULL, &iniciar_consola_kernell, NULL);
	escuchar(kernell, &new_conection_handler);



	/*pthread_mutex_init(&mcola_new, NULL);
	pthread_mutex_init(&mcola_ready, NULL);
	pthread_mutex_init(&mcola_exec, NULL)*/;
}

/*t_proceso* dameProceso(t_queue *cola, int sock ) {
	int a = 0, t;
	t_proceso *w;
	while (w = (t_proceso*)list_get(cola->elements, a)) {
		if (w->socket_CPU == sock) return (t_proceso*)list_remove(cola->elements, a);
		a++;
	}
	printf("NO HAY PROCESO\n"); exit(0);
	return NULL;
}*/


