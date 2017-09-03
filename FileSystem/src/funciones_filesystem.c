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
#include "funciones_filesystem.h"

int cantBloques = 5192;
int tamanioBloques = 64;
int bitmap[5192];
char* pathBitmap = "/home/utnso/workspace/tp-2017-1c-PuntoZip/mnt/SADICA_FS/Metadata/Bitmap.bin";

//FUNCION VALIDAR ARCHIVO
bool validarArchivo(char* pathArchivo){
	FILE* archivo;
	char* pathCompleto = obtenerPathCompleto(pathArchivo);
	archivo = fopen(pathCompleto,"r");
	if (archivo == NULL){
		//printf("No existe el archivo en el FileSystem\n");
			//fclose(archivo);
			return 0;
	}else{
			//printf("SI!!, Existe el archivo en el FileSystem\n");
			fclose(archivo);
			return 1;
	}
}

char* obtenerPathCompleto( char* pathArchivo){
	char* pathCompleto = malloc(150);
	strcpy(pathCompleto,"/home/utnso/workspace/tp-2017-1c-PuntoZip/mnt/SADICA_FS/Archivos/");
	strcat(pathCompleto,pathArchivo);

	return pathCompleto;
}
//FUNCION VALIDAR ARCHIVO

//FUNCION BORRAR ARCHIVO (SIN PARTE BLOQUES)

void borrar(char* path){
	char* pathCompleto = obtenerPathCompleto(path);
	FILE* archivo;
	archivo = fopen(pathCompleto,"r");
	fclose(archivo);
	int archivoBorrado = remove(pathCompleto);
		if (archivoBorrado == 0){
			printf("El archivo se eliminó correctamente\n");
			//ACA DEBERIA liberarBloques(bloques);
		}else{
			printf("No se pudo eliminar el archivo\n");
		}
}

//FUNCION BORRAR ARCHIVO (SIN PARTE BLOQUES)

//FUNCION CREAR ARCHIVO

void crearArchivo(char* pathArchivo){

	int existe = validarArchivo(pathArchivo);

	if (existe == 0){
		FILE* archivo;

		char* pathCompleto = obtenerPathCompleto(pathArchivo);
		//ARRAY CON LOS VALORES INICIALES CON CAPACIDAD DE UN MULTIPLO DE 64
		char* texto = malloc(sizeof(char) * 64);
		//NUMERO DE BLOQUE "CONVERTIDOOO" DISPONIBLE QUE VA A TENER HASTA 4 DIGITOS

		int idBloqueLibre;

		archivo = fopen(pathCompleto,"ab+");
		fclose(archivo);

		archivo = fopen(pathCompleto,"w");

		idBloqueLibre = proximoBloqueLibre();
		actualizarArchivoBitmap();

		//CONCATENACION DEL TAMANIO= CON EL TAMANIO DEL ARCHIVO
		char* contenido = malloc(snprintf(NULL, 0, "TAMANIO=0\nBLOQUES=[%i]", idBloqueLibre) + 1);
		sprintf(contenido, "TAMANIO=0\nBLOQUES=[%i]", idBloqueLibre);
		fwrite(contenido, strlen(contenido), 1, archivo);

	}

}
//FUNCION CREAR ARCHIVO

char** bloquesArchivo(char* pathArchivo){
	FILE* archivo;
	char* pathCompleto = obtenerPathCompleto(pathArchivo);
	archivo = fopen(pathCompleto,"r");

	char buf[3000];
	char* arrayBloques;

	while (fgets(buf, sizeof(buf), archivo) != NULL)
	{
		buf[strlen(buf)] = '\0';
		char** line = string_split(buf, "=");
		if(string_equals_ignore_case(line[0], "BLOQUES")){
			arrayBloques = line[1];
		}
	}

	arrayBloques = string_substring(arrayBloques, 1, strlen(arrayBloques)- 2);
	fclose(archivo);
	return string_split(arrayBloques, ",");

}

void guardarDatos(char*pathArchivo,int offset,char* buffer){
	char** bloques = bloquesArchivo(pathArchivo);

}


//FUNCION ACTUALIZAR ARCHIVO BITMAP (SIN PARTE BLOQUES)

void actualizarArchivoBitmap() {
	FILE* arch;
	arch = fopen(pathBitmap,"r");
	fseek(arch,0L, SEEK_END);
	int tamanoArchivo = ftell(arch);
	fclose(arch);

	FILE* archivoBitmap;
	archivoBitmap = fopen(pathBitmap,"w");

	int i;
	for (i = 0; i < tamanoArchivo -1; i++) {
		char* buffer;
		sprintf(buffer, "%d", bitmap[i]);

		fwrite(buffer, 1, 1, archivoBitmap);
	}

	fclose(archivoBitmap);
}

//FUNCION ACTUALIZAR ARCHIVO BITMAP (SIN PARTE BLOQUES)

int proximoBloqueLibre() {
	FILE* archivoBitmap;
	archivoBitmap = fopen(pathBitmap,"rb");

	int i;

	for (i = 0; i < cantBloques -1; i++) {
		if (bitmap[i] == 0) {
			bitmap[i] = 1;
			return i;
		}
	}
	return 0;
}

void crearBitmap() {
	FILE* archivoBitmap;
	archivoBitmap = fopen(pathBitmap,"w");

	int i;

	for (i = 0; i < cantBloques; i++) {
		fwrite("0", 1, 1, archivoBitmap);
		bitmap[i] = 0;
	}
	fclose(archivoBitmap);
}

void inicializarBitmap() {
	if(validarArchivo(pathBitmap) == 1){
		FILE* archivoBitmap;
		archivoBitmap = fopen(pathBitmap,"r");

		fgets(bitmap, sizeof(bitmap), archivoBitmap);
		bitmap[strlen(bitmap)] = '\0';
	}else{
		crearBitmap();
	}

}
