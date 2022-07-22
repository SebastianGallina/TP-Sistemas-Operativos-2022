#include "../include/gestion_estructuras_mem.h"
#include <math.h>


void inicializar_estructuras(){
	/*
		Esta funcion genera:
		 Un espacio contiguo de memoria en donde los procesos podrán leer y/o escribir.
		 bitmap de marcos ocupados
		 las listas de paginas de primer y segundo nivel
	*/

	log_info(logger, "-------------- Inicializacion de estructuras --------------");

	reservar_espacio_lectura_escritura();

	armar_bitmap_marcos();

	inicializar_tablas_de_entradas();

	inicializar_lista_procesos_en_memoria();

	log_info(logger, "-------------- Finalizacion de Inicializacion de estructuras --------------");
}

// Reserva cierta cantidad de espacio en memoria para poder ser escrita o leida por los distintos procesos
void reservar_espacio_lectura_escritura(){

	log_debug(logger, "reservar_espacio_lectura_escritura()");

	log_info(logger, "------- Reservar espacio de memoria contiguo");

	log_info(logger, "Se reservarán %d cantidad de bytes en memoria" , config_values.tam_memoria);

	// La cantidad de espacio (en bytes) que se va a reservar viene ya por archivo de configuracion
	// config_values->tam_memoria

	espacio_lectura_escritura_procesos = malloc(config_values.tam_memoria);

	log_info(logger, "------- Finalizacion de Reservar espacio de memoria contiguo");
}

// Armar un array de bits que representen si los marcos están ocupados o no
void armar_bitmap_marcos(){
	log_debug(logger, "armar_bitmap_marcos()");

	log_info(logger, "------- Armar bitmap de marcos disponibles y ocupados");

	// En estos calculos usamos la funcion ceil() para no quedarnos con cantidades menores a las que necesitamos

	double cantidadMarcos_f = config_values.tam_memoria / config_values.tam_pagina;

	int cantidadMarcos = ceil(cantidadMarcos_f);

	log_info(logger, "La memoria tendrá %d marcos en total" , cantidadMarcos);

	double bytesNecesarios_f = cantidadMarcos / 8;

	int bytesNecesarios = ceil(bytesNecesarios_f);
	log_info(logger, "Se necesitarán %d (%d bits) para representar a todos los marcos de la memoria" , bytesNecesarios , cantidadMarcos);

	// Reservamos en memoria el espacio donde se almacenarán los bits representando los marcos
	char* bitsMarcos = malloc(bytesNecesarios);

	// bitmap - el bloque de memoria que contiene los bits a leer/escribir
	char* bitmap_real = malloc(bytesNecesarios);

	// bitarray_create retorna un struct t_bitarray*
	bitmap_marcos = bitarray_create(bitmap_real, bitsMarcos);

	limpiar_bitmap(cantidadMarcos);

	log_info(logger, "------- Finalizacion de Armado bitmap de marcos disponibles y ocupados");

}

void inicializar_tablas_de_entradas(){
	log_debug(logger, "inicializar_tablas_de_entradas()");

	log_info(logger, "------- Inicializar listas de entradas a ambas tablas de niveles");

	entradas_tabla_primer_nivel = list_create();
	entradas_tabla_segundo_nivel = list_create();

	log_info(logger, "------- Finalizacion de inicializacion de listas de entradas a ambas tablas de niveles");
}

void inicializar_lista_procesos_en_memoria() {

	log_debug(logger, "inicializar_lista_procesos_en_memoria()");

	log_info(logger, "------- Inicializar lista de procesos en memoria");

	procesos_en_memoria = list_create();

	log_info(logger, "------- Finalizacion de inicializacion de lista de procesos en memoria");
}



void limpiar_bitmap(int cantidadMarcos){

	log_info(logger, "------- Limpiando bitmap de marcos");

	for(int i = 0; i < cantidadMarcos; i++)
	{
		// Usaremos el bit 0 como desocupado
		bitarray_clean_bit(bitmap_marcos, i);
	}
	log_info(logger, "------- Bitmap limpio");
}


void liberar_estructuras(){

	log_debug(logger, "liberar_estructuras()");

	log_info(logger, "------- Eliminando estructuras de memoria");

	// Liberar memoria ocupada por espacio en memoria para poder ser escrita o leida por los distintos procesos
	free(espacio_lectura_escritura_procesos);

	// Liberar memoria ocupada por bitmap de marcos
	free(bitmap_marcos->bitarray);
	free(bitmap_marcos);

	// Liberar memoria ocupada por las entradas de ambas tablas de paginas
	list_destroy(entradas_tabla_primer_nivel);
	list_destroy(entradas_tabla_segundo_nivel);

	// Liberar memoria ocupada por la lista de procesos en memoria
	list_destroy(procesos_en_memoria);

	log_info(logger, "------- Finalizacion de eliminacion de estructuras de memoria");

}

void asignar_nuevas_paginas(pcb* pcb){

	// Generamos el elemento del proceso en la lista de procesos en memoria
	proceso_en_memoria* proceso_nuevo = malloc(sizeof(proceso_en_memoria));

	proceso_nuevo->id_proceso = pcb->id;

	// Apunta a las entradas de primer nivel en las que se encuentran los marcos asignados al proceso
	proceso_nuevo->entradas_N1 = list_create();

	proceso_nuevo->entradas_N1 = reservar_paginas(pcb->id, pcb->tamanio);


}

t_list* reservar_paginas(int idProceso, int tamanioProceso){

	// Total de marcos que ocupara el proceso en memoria
	int paginasNecesarias = cantidad_paginas_necesarias(tamanioProceso);

	int entradasNecesariasN1 = ceil(paginasNecesarias / config_values.entradas_por_tabla);

	log_info(logger, "Reservando paginas para nuevo proceso ID: %d",idProceso);

	log_info(logger, "Tamaño proceso: %d", tamanioProceso);
	log_info(logger, "Tamaño pagina: %d", config_values.tam_pagina);
	log_info(logger, "Cantidad necesaria de marcos (tamaño proceso / tamaño pagina) : %d", paginasNecesarias);
	log_info(logger, "Cantidad necesaria de entradas de N1 (cant necesaria entradas N1 / cantidad entradas por configuracion) : %d", entradasNecesariasN1);

	// Lista para ir guardando las entradas de primer nivel que vayamos generando
	t_list* entradas_N1_aux = list_create();

	// cantidad de paginas en N2 reservadas
	int paginasReservadas;

	for(paginasReservadas = 0 ; paginasReservadas < paginasNecesarias ; paginasReservadas++ ){


		if(se_necesita_generar_otra_entrada_N1()){
			log_debug(logger, "entre al IF");
		}

	}


	return entradas_N1_aux;

}

int se_necesita_generar_otra_entrada_N1(int paginasReservadas){
	// TODO: revisar esto
	// Si la cantidad de paginas reservadas ocupó "toda" una entrada?
	return (paginasReservadas % config_values.entradas_por_tabla) == 0;
}

int cantidad_paginas_necesarias(int tamanioProceso){
    float cantidad = tamanioProceso / config_values.tam_pagina;
    return ceil(cantidad);
}

