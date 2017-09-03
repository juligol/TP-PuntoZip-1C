/*
 * Structs.h
 *
 *  Created on: 8/6/2017
 *      Author: utnso
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef int boolean;
#define true 1
#define false 0

typedef struct {
	int idProceso;
	int programCounter;
	int indiceCodigo[2];
	int exitCode;
	int cantidadInstrucciones;
}Pcb;

typedef struct{
	int pagina;
	int offset;
	int size;
}direccion;
/*
typedef struct {
  int socket_CPU;
  int socket_CONSOLA;
  Pcb *pcb;
  int unidadesBloqueado; //Borrar estos?
  char semaforoBloqueado;
  bool abortado;
} t_proceso;*/


/*typedef struct{
	char inicio[512];
}frame;*/

typedef struct{
	uint32_t size;
	boolean isFree;
}HeapMetadata;

typedef struct{
	int id;
	boolean disponible;
	int numeroDePagina;
}marco;

typedef struct{
	int PID;
	int numero_pagina;
	int numFrame;
	char* contenido;
}registroTablaInvertida;

typedef struct{
	struct registroHash *Anterior;
	int llave;
	int valor;
	struct registroHash *Siguiente;
}registroHash;

typedef struct{
	int pid;
	int enUso;
	int numeroPagina;
	char* contenido;
}memoriaCache;

typedef struct {
	int pid;
	int pagina;
	int marco;
	boolean uso;
	boolean modificado;
	boolean puntero;
	boolean presencia;
	int ultima_vez_usado;
} t_entrada_tabla_de_paginas;

typedef struct {
	boolean disponible;
	int numero;
} t_control_marco;

typedef struct {
	int numeroFrame;
	int PID;
	int numeroPagina;
} t_frame_pagina_invertida;

#endif /* STRUCTS_H_ */
