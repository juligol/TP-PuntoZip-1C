/*
 * Mensajes.h
 *
 *  Created on: 4/6/2017
 *      Author: utnso
 */

#ifndef MENSAJES_H_
#define MENSAJES_H_

#include "Genericas.h"

void mensaje_desconeccion(int id_tipo_proceso, int id_proceso) {
	if (id_tipo_proceso > -1) {
		fprintf(stderr,"%s %i desconectada \n", tipo_proceso(id_tipo_proceso), id_proceso);
	}
}

void mensaje_comando(int id_tipo_proceso, int id_proceso, char* comando) {
	if (id_tipo_proceso > -1) {
		fprintf(stderr,"%s %i: %s \n", tipo_proceso(id_tipo_proceso), id_proceso, comando );
	}
}

void mensaje_nuevo_proceso(int id_proceso, int id_tipo_proceso) {
	fprintf(stderr,"Nuevo proceso conectado\n ID: %i\n TIPO: %s \n", id_proceso, tipo_proceso(id_tipo_proceso));
}

#endif /* MENSAJES_H_ */
