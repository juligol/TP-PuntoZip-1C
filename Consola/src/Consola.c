/*
 ============================================================================
 Name        : Consola.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Funciones/Sockets.h"
#include "Funciones/Genericas.h"
#include "consola_configuracion.h"

#define ID_TIPO_PROCESO 1
#define NOMBRE_PROCESO "Consola"

int id_proceso = 0;


/*consola_configuracion get_configuracion() {
	consola_configuracion configuracion;

	// Obtiene el archivo de configuracion
	char* path = "./config-consola.cfg";
	t_config* consola_configuracion = config_create(path);

	configuracion.IP_KERNELL = get_campo_config_char(consola_configuracion, "IP_KERNELL");
	configuracion.PUERTO_KERNELL = get_campo_config_int(consola_configuracion, "PUERTO_KERNELL");

	return configuracion;
}*/

void enviar_comando(int kernell, char* comando) {
	char * mensaje = generar_mensaje(ID_TIPO_PROCESO, id_proceso, comando);
	enviar_datos(kernell, mensaje);
}
void limpiar_consola() {
	printf("\033[H\033[J");
}

int main(void) {
	//Datos hardcodeados
		//consola_configuracion configuracion = get_configuracion();
		struct in_addr ip_kernell =  get_direccion_ip();
		int puerto_kernell = 5052;

	//!Datos hardcodeados
	int kernell = iniciar_proceso(ID_TIPO_PROCESO, NOMBRE_PROCESO, -1, ip_kernell, puerto_kernell);
	id_proceso = obtener_id_proceso(kernell);
	pthread_t thread_listener, thread_sender;

	char comando[500];
	while(1) {
		fgets(comando, 50, stdin);
		if (string_contains(comando,"limpiar")) {
			limpiar_consola();
		}
		else {
			enviar_comando(kernell, comando);
			char* recieved_data = recibir_datos(kernell);
			if(strlen(recieved_data) > 1) {
				puts(recieved_data);
			}
		}
	}
	return EXIT_SUCCESS;
}
