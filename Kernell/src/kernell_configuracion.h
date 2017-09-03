/*
 * kernell_configuracion.h
 *
 *  Created on: 10/4/2017
 *      Author: utnso
 */

#ifndef KERNELL_CONFIGURACION_H_
#define KERNELL_CONFIGURACION_H_

typedef struct kernell_configuracion {
	int PUERTO_PROG;
	int PUERTO_CPU;
	char* IP_MEMORIA;
	int PUERTO_MEMORIA;
	char* IP_FS;
	int PUERTO_FS;
	int QUANTUM;
	int QUANTUM_SLEEP;
	char* ALGORITMO;
	int GRADO_MULTIPROG;
	int STACK_SIZE;
	char** SEM_IDS;
	char** SEM_INIT;
	char** SHARED_VARS;
} kernell_configuracion;

/*kernell_configuracion get_configuracion() {
	kernell_configuracion configuracion;

	// Obtiene el archivo de configuracion
	char* path = "./config-kernell.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.PUERTO_PROG = get_campo_config_int(archivo_configuracion, "PUERTO_PROG");
	configuracion.PUERTO_CPU = get_campo_config_int(archivo_configuracion, "PUERTO_CPU");
	configuracion.IP_MEMORIA = get_campo_config_char(archivo_configuracion, "IP_MEMORIA");
	configuracion.PUERTO_MEMORIA = get_campo_config_int(archivo_configuracion, "PUERTO_MEMORIA");
	configuracion.IP_FS = get_campo_config_char(archivo_configuracion, "IP_FS");
	configuracion.PUERTO_FS = get_campo_config_int(archivo_configuracion, "PUERTO_FS");
	configuracion.QUANTUM = get_campo_config_int(archivo_configuracion, "QUANTUM");
	configuracion.QUANTUM_SLEEP = get_campo_config_int(archivo_configuracion, "QUANTUM_SLEEP");
	configuracion.ALGORITMO = get_campo_config_char(archivo_configuracion, "ALGORITMO");
	configuracion.GRADO_MULTIPROG = get_campo_config_int(archivo_configuracion, "GRADO_MULTIPROG");
	configuracion.SEM_IDS = get_campo_config_array(archivo_configuracion, "SEM_IDS");
	configuracion.SEM_INIT = get_campo_config_array(archivo_configuracion, "SEM_INIT");
	configuracion.SHARED_VARS = get_campo_config_array(archivo_configuracion, "SHARED_VARS");
	configuracion.STACK_SIZE = get_campo_config_int(archivo_configuracion, "STACK_SIZE");

	return configuracion;
}*/


#endif /* KERNELL_CONFIGURACION_H_ */
