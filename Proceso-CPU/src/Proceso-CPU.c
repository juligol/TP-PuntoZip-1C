/*
 ============================================================================
 Name        : Proceso-CPU.c
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
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <parser/parser.h>
#include <commons/config.h>

#include "Funciones/Structs.h"
#include "Funciones/Sockets.h"
#include "Funciones/Genericas.h"
#include "cpu_configuracion.h"
#include "FuncionesParser.h"

#define ID_TIPO_PROCESO 2
#define NOMBRE_PROCESO "CPU"

int id_proceso = 0;

int puertoMemoria = 5054;
char* ipMemoria = "127.0.0.1";

/*cpu_configuracion get_configuracion() {
	cpu_configuracion configuracion;

	// Obtiene el archivo de configuracion
	char* path = "./config-cpu.cfg";
	t_config* cpu_configuracion = config_create(path);

	configuracion.PUERTO_KERNEL = get_campo_config_int(cpu_configuracion, "PUERTO_KERNEL");
	configuracion.IP_KERNEL = get_campo_config_char(cpu_configuracion, "IP_KERNEL");
	configuracion.PUERTO_MEMORIA = get_campo_config_int(cpu_configuracion, "PUERTO_MEMORIA");
	configuracion.IP_MEMORIA = get_campo_config_char(cpu_configuracion, "IP_MEMORIA");

	return configuracion;
}*/
int memoria_socket = -1;
void conectar_con_memoria(struct in_addr ip_memoria, int puerto_memoria) {
	memoria_socket = conectar_con_server(ip_memoria, puerto_memoria);
}



char* request_a_memoria(char* mensaje) {
	char* data = generar_mensaje(ID_TIPO_PROCESO, id_proceso,  mensaje);
	char* response = request(memoria_socket, data);
	puts(response);
	return response;

}

char* solicitarProximaInstruccion(char* pid){
	char * data = malloc(snprintf(NULL, 0, "solicitar-proxima %s", pid) + 1);
	sprintf(data, "solicitar-proxima %s", pid);
	char* respuesta = request_a_memoria(data);
	return respuesta;
}
void handle_accion(struct mensaje message) {
	char* comando_ejecutar = message.comando;
	if (string_contains(comando_ejecutar,"ejecutar-programa")) {
		fprintf(stderr,"Ejecutando %s \n", message.params_comando[1] );
		solicitarProximaInstruccion(message.params_comando[1]);
	}
}
void ejecutarProceso(Pcb* pcbActual){

    while (1){
         // Obtengo la próxima instrucción a ejecutar:
        char* proximaInstruccion = solicitarProximaInstruccion(pcbActual->idProceso);

        if (proximaInstruccion != NULL){ // Llegó una instrucción, analizo si es o no end:

            //limpiarInstruccion(proximaInstruccion);

            if ( pcbActual->programCounter >= (pcbActual->cantidadInstrucciones -1)
            	&& string_starts_with(proximaInstruccion, "end") ){
            	//Es end. Finalizo ejecución por exit:
            	printf("El programa actual ha finalizado con éxito.\n");
            	//int longitudPcb = calcularTamanioPcb(pcbActual);
            	// Mando solamente el pid, porque al kernel ya no le sirve el PCB.
            	printf("Empaqueto\n");
            	//empaquetar(kernel, mensajePcbEnd,longitudPcb, &(pcbActual->pid));
            	free(proximaInstruccion);
            	proximaInstruccion = NULL;
            	return;
            }
            // Si no es end. Ejecuto la próxima instrucción:
            printf("Instrucción recibida: %s", proximaInstruccion);

            analizadorLinea(proximaInstruccion, &functions, &kernel_functions);

            (pcbActual->programCounter)++; // Incremento Program Counter del PCB
			}
		}
}

int main(void) {
	//Datos hardcodeados
		//cpu_configuracion configuracion = get_configuracion();
		struct in_addr ip_kernell =  get_direccion_ip();
		int puerto_kernell = 5052;

		struct in_addr ip_memoria =  get_direccion_ip();
		int puerto_memoria = 5054;

	//!Datos hardcodeados

	conectar_con_memoria(ip_memoria, puerto_memoria);
	int kernell = iniciar_proceso(ID_TIPO_PROCESO, NOMBRE_PROCESO, -1, ip_kernell, puerto_kernell);
	id_proceso = obtener_id_proceso(kernell);

	while(1) {
		char* datos = recibir_datos(kernell);
		//puts(datos);
		struct mensaje message = handle_recieved_data(datos);
		handle_accion(message);
	}


	return EXIT_SUCCESS;
}
/*void limpiarInstruccion(char* instruccion){

    char *aux = instruccion;
    int j = 0;
    while (*instruccion != '\0') {
        if (*instruccion != '\t' && *instruccion != '\n' && !iscntrl(*instruccion)) {
            if (j == 0 && isdigit((int )*instruccion)) {
                ++instruccion;
            } else {
                *aux++ = *instruccion++;
                j++;
            }
        } else {
            ++instruccion;
        }
    }
    *aux = '\0';
}*/

