/*
 ============================================================================
 Name        : Admin-Memoria.c
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
#include "Funciones/Mensajes.h"
#include "admin_memoria_configuracion.h"
#include "Funciones/Structs.h"
#define ID_TIPO_PROCESO 4
#define NOMBRE_PROCESO "Admin. memoria"
int cantidadDeMarcos;
int tamanioMarco;
int tamanioPaginas;
int cantidadEntradasCache;
void * Frame;
t_list * listaDeMarcos;
t_list * tabla_de_paginas;
t_list * tabla_de_paginas_invertidas;
t_list * cache;

int id_proxima_cpu = 0;

char* enviarProximaInstruccion(int pid){
	int i;
	char* contenido;
	registroTablaInvertida * reg;
	for(i=0; i<cantidadDeMarcos-1;i++){
		reg = list_get(tabla_de_paginas_invertidas, i);
		//la violacion es aca
		if(reg->PID == pid){
			contenido = reg->contenido;
		}
	}
	return contenido;
}


struct proceso handle_accion(int cliente, struct mensaje message) {
	struct proceso process;
	process.id_tipo_proceso = message.id_tipo_proceso;
	process.id_proceso = message.id_proceso;

	char* comando_ejecutar = message.comando;
	if (string_contains(comando_ejecutar,"nuevo-proceso")) {
		id_proxima_cpu++;
		process.id_proceso = id_proxima_cpu;
		mensaje_nuevo_proceso(process.id_proceso, process.id_tipo_proceso);
	}
	else {
		mensaje_comando(message.id_tipo_proceso, message.id_proceso, message.comando );
		if (string_contains(comando_ejecutar,"obtener-memoria")) {
			puts("Me estan pidiendo memoria");
			enviar_datos(cliente, "1024mb");
		}else{
			if (string_contains(comando_ejecutar,"solicitar-proxima")) {

				int pid = atoi(message.params_comando[1]);
				puts("Me estan pidiendo proxima instruccion");
				char* proxima = enviarProximaInstruccion(pid);
				enviar_datos(cliente, proxima);

			}
		}
	}

	return process;
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
			process = handle_accion(cliente, message);
		}
	}
}

void new_conection_handler(int cliente) {
	puts("\nConexion recibida\n");
	pthread_t new_thread;
	pthread_create(&(new_thread), NULL, &handle_conection, cliente);
}
void inicializarMarcos() {
	listaDeMarcos = list_create();
		int i;
		for (i = 0; i < cantidadDeMarcos; ++i) {
			marco * unMarco = malloc(sizeof(marco));
			unMarco->disponible = true;
			unMarco->id = i;
			unMarco->numeroDePagina = 0;
			list_add(listaDeMarcos, unMarco);
		}
}

void inicializarTablaPaginasInvertidas(){
	tabla_de_paginas_invertidas = list_create();
	int i;
	for (i = 0; i < cantidadDeMarcos-1; i++) {
		registroTablaInvertida* RegistroTablaInvertida = malloc(sizeof(registroTablaInvertida));
		RegistroTablaInvertida->PID = 0;
		RegistroTablaInvertida->contenido = "Estoy vacio";
		RegistroTablaInvertida->numero_pagina = 0;
		RegistroTablaInvertida->numFrame = i;
		list_add(tabla_de_paginas_invertidas, RegistroTablaInvertida);
	}

}
int main(void) {
	//empezar();//Aca empieza la Memoria

	//Abro el archivo configuracion
	/*admin_memoria_configuracion configuracion = get_configuracion();
	cantidadDeMarcos = configuracion.MARCOS;
	tamanioMarco = configuracion.MARCO_SIZE;*/
	cantidadDeMarcos = 20;
	tamanioMarco = 10;

	//Al iniciar, solicitará un bloque de memoria contigua de tamaño configurable por archivo de configuración
	//int dir_bloque_de_memoria = solicitarBloqueDeMemoria();
	//printf(dir_bloque_de_memoria);

	//Creo los marcos donde se van a guardar las paginas
	inicializarMarcos();
	inicializarTablaPaginasInvertidas();
	agregarFrameATablaInvertida(2, 3, 1, "Hola");
	/*char *cont = enviarProximaInstruccion(2);
		puts(cont);*/
	//hayMarcosDisponibles();

	//agregarFrameACache(int pid,int numPagina);

	struct in_addr ip_memoria =  get_direccion_ip();
	int puerto_memoria = 5054;
	int backlog = 5;

	//!Datos hardcodeados

	int memoria = get_server();
	init_server(memoria, ip_memoria, puerto_memoria, backlog);
	escuchar(memoria, &new_conection_handler);

	return EXIT_SUCCESS;
}

void empezar() {

	printf("====== INICIO ======\n");
}

int solicitarBloqueDeMemoria() {
	Frame = malloc(tamanioMarco * cantidadDeMarcos);

	tabla_de_paginas = list_create();

	if (Frame == NULL) {
		error_show("No se pudo otorgar la memoria solicitada!!\n");
		exit(EXIT_FAILURE);
	}
	return Frame;
}


boolean verDisponibilidadMarco(marco * unMarco) {
		return unMarco->disponible;
}

int cantidadDeMarcosDisponibles(){
	//Trae la cantidad de elementos de una lista que cumplan con una funcion
	return list_count_satisfying(listaDeMarcos,verDisponibilidadMarco);
}

boolean hayMarcosDisponibles(){
	return cantidadDeMarcosDisponibles > 0;
}

void ocuparMarco(marco * unMarco){
	unMarco->disponible = false;
}

void liberarMarco(marco * unMarco){
	unMarco->disponible = true;
}

void escribir_marco(int marco, int offset, int tamanio, void * contenido) {

	int desplazamiento = marco * tamanioMarco;

	memcpy(desplazamiento + offset, contenido, tamanio);

	printf("El marco se escribe con exito.\n");

}

void agregarFrameATablaInvertida(int pid, int numeroPagina, int numFrame, char* contenido){
	registroTablaInvertida* reg = malloc(sizeof(registroTablaInvertida));
	reg = list_get(tabla_de_paginas_invertidas,numFrame);
	reg->PID = pid;
	reg->numero_pagina = numeroPagina;
	reg->contenido = contenido;
	reg->numFrame = numFrame;
}

int funcionHashBuscar(int pid, int num_pagina) {
	char str1[20];
	char str2[20];
	sprintf(str1, "%d", pid);
	sprintf(str2, "%d", num_pagina);
	strcat(str1, str2);
	unsigned int indice = atoi(str1) % cantidadDeMarcos;
	return indice;
}

void inicializarMemoriaCache(){
	cache = list_create();
	memoriaCache * registroInicial = malloc(sizeof(memoriaCache));
	registroInicial->numeroPagina =-1;
	registroInicial->pid =-1;
	registroInicial->enUso=0;
	registroInicial->contenido = (char*) malloc(tamanioPaginas);
	strcpy(registroInicial->contenido,"");

	int i;
	for(i=0;i<cantidadEntradasCache;i++){
		list_add(cache,registroInicial);
	}
}

void agregarFrameAMemoriaCache(int pid,int numPagina){
	memoriaCache * nuevaEntrada = malloc(sizeof(memoriaCache));
	nuevaEntrada->pid = pid;
	nuevaEntrada->enUso =1;

	//int cantEntradasProc = obtenerEntradasProcesoCache(pid);
	//nuevaEntrada.numeroPagina = cantEntradasProc++;

	//funcionHashBuscar(pid, num_pagina) --> devuelve el id del frame donde esta la pagina
	int numeroDeFrame = funcionHashBuscar(pid,numPagina);
	registroTablaInvertida* reg = malloc(sizeof(registroTablaInvertida));
	reg = list_get(tabla_de_paginas_invertidas,numeroDeFrame);
	strcpy(nuevaEntrada->contenido, reg->contenido);

	/*int entradaCache = obtenerEntradaDeCache();
	memoriaCache * entrada = malloc(sizeof(memoriaCache));
	entrada = list_get(cache, entradaCache);
	entrada = nuevaEntrada;*/
}

/*int obtenerEntradaDeCache(){
	int i;
	for(i=0;i<cantidadEntradasCache;i++){
		if(cache[i].enUso == 0){
			return i;
		}
	}

	//Si encuentra una entrada de cache que no este en uso la devuelve
	//sino utiliza LRU para ver quien reemplazar

	int entradaAReemplazar = aplicar_LRU_De_Reemplazo_De_Cache();

	return entradaAReemplazar;
}

int aplicar_LRU_De_Reemplazo_De_Cache(){
	int i;
	struct memoriaCache minimo = cache[0];
	for(i=1;i<cantidadEntradasCache;i++){
		if(cache[i].enUso < minimo.enUso){
			minimo = cache[i];
		}
		cache[i].enUso = 0;
	}
	return minimo.pid;
}*/


