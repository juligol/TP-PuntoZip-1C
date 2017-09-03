/*
 * FuncionesParser.h
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#ifndef FUNCIONESPARSER_H_
#define FUNCIONESPARSER_H_


static const int contenidoVariable = 15;
static const int posMemoria = 10; //Lo da la memoria
int termino = 0; //0 = false



char *const getDatosMemoria(char *programa, t_puntero_instruccion inicio, t_size tamanio) {
    char *datos = malloc(sizeof(programa));
    memcpy(datos, programa + inicio, tamanio);
    return datos;
}

t_puntero definirVariable(t_nombre_variable variable) {
	printf("Definir la variable %c\n", variable);
	return posMemoria;
}

t_puntero obtenerPosicionVariable(t_nombre_variable variable) {
	printf("Obtener posicion de %c\n", variable);
	return posMemoria;
}

void finalizar(){
	termino = 1;
	printf("Finalizar\n");
}

int terminoElPrograma(){
	return termino;
}

t_valor_variable dereferenciar(t_puntero puntero) {
	printf("Dereferenciar %d y su valor es: %d\n", puntero, contenidoVariable);
	return contenidoVariable;
}

void asignar(t_puntero puntero, t_valor_variable variable) {
	printf("Asignando en %d el valor %d\n", puntero, variable);
}

void llamarSinRetorno(t_nombre_etiqueta etiqueta){
	printf("Lllamando sin retorno a %c\n", etiqueta);
}
void wait_kernel(t_nombre_semaforo identificador_semaforo){
	printf("Se envió un wait a %c\n", identificador_semaforo);
}
void signal_kernel(t_nombre_semaforo identificador_semaforo){
	printf("Se envió un signal a %c\n", identificador_semaforo);
}

AnSISOP_kernel kernel_functions = {
		.AnSISOP_wait = wait_kernel,
		.AnSISOP_signal = signal_kernel };

AnSISOP_funciones functions = {
		.AnSISOP_definirVariable		= definirVariable,
		.AnSISOP_obtenerPosicionVariable= obtenerPosicionVariable,
		.AnSISOP_finalizar 				= finalizar,
		.AnSISOP_dereferenciar			= dereferenciar,
		.AnSISOP_asignar				= asignar,
		.AnSISOP_llamarSinRetorno		= llamarSinRetorno

};
void inicializarFuncionesParser(){
			functions.AnSISOP_definirVariable		= definirVariable,
			functions.AnSISOP_obtenerPosicionVariable= obtenerPosicionVariable,
			functions.AnSISOP_finalizar 				= finalizar,
			functions.AnSISOP_dereferenciar			= dereferenciar,
			functions.AnSISOP_asignar				= asignar;
			printf("Se cargaron las primitivas correctamente \n");
}


#endif /* FUNCIONESPARSER_H_ */
