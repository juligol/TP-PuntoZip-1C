/*
 * filesystem_configuracion.h
 *
 *  Created on: 13/4/2017
 *      Author: utnso
 */

#ifndef filesystem_CONFIGURACION_H_
#define filesystem_CONFIGURACION_H_

#include "Funciones/Configuracion.h"

typedef struct filesystem_configuracion {
	int PUERTO;
	int PUNTO_MONTAJE;
} filesystem_configuracion;

filesystem_configuracion get_configuracion() {
	printf("Inicializando proceso FileSystem\n");
	filesystem_configuracion configuracion;

	// Obtiene el archivo de configuracion
	char* path = "./config-file-system.cfg";
	t_config* filesystem_configuracion = config_create(path);

	configuracion.PUERTO = get_campo_config_int(filesystem_configuracion, "PUERTO");
	configuracion.PUNTO_MONTAJE = get_campo_config_char(filesystem_configuracion, "PUNTO_MONTAJE");

	return configuracion;
}

#endif /* filesystem_CONFIGURACION_H_ */
