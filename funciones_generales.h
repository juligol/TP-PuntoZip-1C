/*
 * FuncionesGenerales.h
 *
 *  Created on: 10/4/2017
 *      Author: utnso
 */

#ifndef FUNCIONESGENERALES_H_
#define FUNCIONESGENERALES_H_

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

char get_campo_config_char(t_config* archivo_configuracion, char* nombre_campo) {
	char* valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_string_value(archivo_configuracion, nombre_campo);
		//printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

int get_campo_config_int(t_config* archivo_configuracion, char* nombre_campo) {
	int valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_int_value(archivo_configuracion, nombre_campo);
		//printf("El %s es: %i\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

int get_campo_config_array(t_config* archivo_configuracion, char* nombre_campo) {
	char** valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_array_value(archivo_configuracion, nombre_campo);
		//printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}


#endif /* FUNCIONESGENERALES_H_ */