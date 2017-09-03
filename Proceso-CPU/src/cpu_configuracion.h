/*
 * cpu_configuracion.h
 *
 *  Created on: 13/4/2017
 *      Author: utnso
 */

#ifndef CPU_CONFIGURACION_H_
#define CPU_CONFIGURACION_H_

typedef struct cpu_configuracion {
	int PUERTO_KERNEL;
	char* IP_KERNEL;
	int PUERTO_MEMORIA;
	char* IP_MEMORIA;
} cpu_configuracion;

#endif /* CPU_CONFIGURACION_H_ */
