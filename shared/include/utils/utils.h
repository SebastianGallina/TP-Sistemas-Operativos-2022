#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
// 
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>
#include "messages.h"
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>


// Nuevos tipos y estructuras de datos

typedef enum op_code
{
	MENSAJE,
	PAQUETE,
	PAQUETE_CONSOLA,
	PAQUETE_PCB,
	PAQUETE_PCB_IO,
	PAQUETE_PCB_EXIT,
	PAQUETE_KERNEL_EXIT,
	INTERRUPCION,
	// Operaciones en Memoria
	NUEVO_PROCESO,
	HANDSHAKE_INICIAL,
	SOLICITUD_NUEVO_PROCESO,
	SOLICITUD_TABLA_PAGINAS,
	SOLICITUD_MARCO,
	// En CPU
	RESPUESTA_HANDSHAKE_INICIAL
}op_code;


typedef struct t_buffer
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;



// Estructuras de datos para el PCB

typedef struct nodo_parametro
{
	int parametro;
} nodo_parametro;

typedef struct instruccion
{
	char identificador[10];
    t_list* parametros;
} instruccion;

typedef struct nodo_instruccion
{
	instruccion instruccion;
} nodo_instruccion;

typedef enum status
{
	NEW,
	READY,
	EXECUTION,
	BLOCKED,
	SUSPENDED_READY,
	SUSPENDED_BLOCKED,
	EXIT
}status;

typedef struct pcb
{
	int id;
	int tamanio;
	t_list* instrucciones;
	int program_counter;
	int tabla_paginas;
	double estimacion_rafaga;
	status status;
	int tiempo_a_bloquearse;
} pcb;

typedef struct nodo_pcb
{
	pcb pcb;
	struct nodo_pcb* sig;
} nodo_pcb;

//Variables globales
int conexion;
t_log* logger;
t_config* config;

// Definicion de Comportamientos Logger & Config

t_log* iniciar_logger(char* log_level, char* logger_path, char* logger);
t_config* iniciar_config(char* config_path);

// Definicion de Otras Utilidades
int msleep(unsigned int tms);
void liberarStringArray(char** stringArray);
int tamanioStringArray(char** a);
FILE* abrir_archivo_lectura(char* path_pseudocodigo);
void error_handler(t_log* logger, char* message_handler,char *message, char *var1, char *var2);


// Estructuras paginacion memoria

typedef struct {
	int num_pag;
	int dir; //MARCO
	int bit_presencia;
	int bit_uso;
	int bit_modificacion;
} entrada_tabla_N2;

typedef struct {
	int num_tabla; //id tabla2
	int dir; //index en lista general de tablasN2
} entrada_tabla_N1;

typedef t_list t_tablaN2; // elementos tipo entrada_tabla_N2
typedef t_list t_tablaN1; // elementos tipo entrada_tabla_N1

#endif /* UTILS_H_ */
