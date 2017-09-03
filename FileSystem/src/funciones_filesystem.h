#ifndef FUNCIONES_FILESYSTEM_H_
#define FUNCIONES_FILESYSTEM_H_
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/config.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <commons/bitarray.h>
#include <stdbool.h>
/*#define mensajeAbrirArchivo 9
#define mensajeCerrarArchivo 10
#define mensajeLeerArchivo 11
#define mensajeEscribirArchivo 12*/

void obtenerDatos(char*path,int offset,int size);
void guardarDatos(char*path,int offset,char* buffer);

void asignarBloque(int tamanio);

int cantidadRedondeada(int tamanio);


void liberarBloques(int* bloques);

//////////////////////////////////////////////////////////////////////////

char* obtenerPathCompleto( char* path);


bool validarArchivo(char* path);

void crearArchivo(char* path);


void borrar(char* path);

void recibirPeticiones(int kernel);

int proximoBloqueLibre();

void inicializarBitmap();

int cantidadBloquesArchivo(int longitudArchivo);

void actualizarArchivoBitmap();

#endif /* FUNCIONESFS_H_ */
