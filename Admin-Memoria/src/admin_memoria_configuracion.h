/*
 * admin_memoria_configuracion.h
 *
 *  Created on: 13/4/2017
 *      Author: utnso
 */
#include "funciones_generales.h"

#ifndef ADMIN_MEMORIA_CONFIGURACION_H_
#define ADMIN_MEMORIA_CONFIGURACION_H_

typedef struct admin_memoria_configuracion {
	int PUERTO_MEMORIA;
	int MARCOS;
	int MARCO_SIZE;
	int ENTRADAS_CACHE;
	int CACHE_X_PROC;
	char* REEMPLAZO_CACHE;
	int RETARDO_MEMORIA;
	int ID_MEMORIA;
	int SIZE_MEMORIA;
} admin_memoria_configuracion;


admin_memoria_configuracion get_configuracion() {
	printf("Inicializando proceso de Administrador de Memoria...\n");
	admin_memoria_configuracion configuracion;

	// Obtiene el archivo de configuracion
	char* path = "./config-memoria.cfg";
	t_config* admin_memoria_configuracion = config_create(path);

	configuracion.PUERTO_MEMORIA = get_campo_config_int(admin_memoria_configuracion, "PUERTO_MEMORIA");
	configuracion.MARCOS = get_campo_config_int(admin_memoria_configuracion, "MARCOS");
	configuracion.MARCO_SIZE = get_campo_config_int(admin_memoria_configuracion, "MARCO_SIZE");
	configuracion.ENTRADAS_CACHE = get_campo_config_int(admin_memoria_configuracion, "ENTRADAS_CACHE");
	configuracion.CACHE_X_PROC = get_campo_config_int(admin_memoria_configuracion, "CACHE_X_PROC");
	configuracion.REEMPLAZO_CACHE = get_campo_config_char(admin_memoria_configuracion, "REEMPLAZO_CACHE");
	configuracion.RETARDO_MEMORIA = get_campo_config_int(admin_memoria_configuracion, "RETARDO_MEMORIA");
	configuracion.ID_MEMORIA = get_campo_config_int(admin_memoria_configuracion, "ID_MEMORIA");
	configuracion.SIZE_MEMORIA = get_campo_config_int(admin_memoria_configuracion, "SIZE_MEMORIA");

	return configuracion;
}


#endif /* ADMIN_MEMORIA_CONFIGURACION_H_ */
