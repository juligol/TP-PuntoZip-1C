/*
 ============================================================================
 Name        : FileSystem.c
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
#include "filesystem_configuracion.h"
#include "funciones_filesystem.h"

#define ID_TIPO_PROCESO 3
#define NOMBRE_PROCESO "File System"

void new_conection_handler(int cliente) {
	puts("\nConexion recibida\n");
	while(1) {
		char* recieved_data = recibir_datos(cliente);
		struct mensaje message = handle_recieved_data(recieved_data);
		if (esKernell(message.id_tipo_proceso)) {
			handle_accion(cliente, message);
		}
		else {
			puts("Intento de conexion de proceso no autorizado\n");
			close(cliente);
		}
	}
}

void handle_accion(int cliente, struct mensaje message) {
	fprintf(stderr,"Kernell: %s \n", message.comando );

	char* comando_ejecutar = message.comando;
	if (string_contains(comando_ejecutar,"borrar-archivo")) {
		fprintf(stderr,"Borrando archivo %s \n", message.params_comando[1] );
		enviar_datos(cliente, "El archivo no pudo borrarse");
	}
}

int main(void) {
	//Datos hardcodeados
	//filesystem_configuracion configuracion = get_configuracion();

	/*struct in_addr ip_fs =  get_direccion_ip();
	int puerto_fs = 5053;
	int backlog = 5;

	//!Datos hardcodeados

	int fs = get_server();
	init_server(fs, ip_fs, puerto_fs, backlog);
	escuchar(fs, &new_conection_handler);*/

	inicializarBitmap();
	//actualizarArchivoBitmap();
	//proximoBloqueLibre();
	//guardarDatos("nuevo.bin",0,"hola cheja");
	crearArchivo("pepe.bin");
	crearArchivo("juan.bin");

	return EXIT_SUCCESS;
}
