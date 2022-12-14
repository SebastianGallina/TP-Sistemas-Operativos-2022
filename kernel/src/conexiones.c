#include "../include/conexiones.h"


void conectar_memoria(){
	conexionAMemoria = crear_conexion(config_values.ip_memoria,config_values.puerto_memoria);
	if(conexionAMemoria == -1){
		error_handler(logger, errorMessageAux, SERVIDOR_AUSENTE, " Memoria", NULL);
		terminar_programa();
		exit(-1);
	}

	if(pthread_create(&thr_memoria, NULL, (void*) manejar_memoria,(void*)conexionAMemoria) != 0){
		log_error(logger, "Error al crear el hilo con la memoria");
	}
	log_debug(logger, "se creo un thread para %s", "Memoria");
	log_info(logger, "Conexión exitosa con la Memoria");
}

void conectar_cpu(){

	// Conexion de Kernel con CPU por puerto Dispatch
	conexionACPU = crear_conexion(config_values.ip_cpu,config_values.puerto_cpu_dispatch);
	if(conexionACPU == -1){
		error_handler(logger, errorMessageAux, SERVIDOR_AUSENTE, " CPU", NULL);
		terminar_programa();
		exit(-1);
	}

	if(pthread_create(&thr_cpu, NULL, (void*) planificacion_cpu,(void*)conexionACPU) != 0){
		log_error(logger, "Error al crear el hilo con el CPU - Dispatch");
	}
	log_debug(logger, "se creo un thread para %s", "CPU-D");
	log_info(logger, "Conexión exitosa con el CPU-Dispatch");

	// Conexion de Kernel con CPU por puerto Interrupt
	conexionACPU_interrupt = crear_conexion(config_values.ip_cpu,config_values.puerto_cpu_interrupt);
	if(conexionACPU_interrupt == -1){
		error_handler(logger, errorMessageAux, SERVIDOR_AUSENTE, " CPU-Interrupt", NULL);
		terminar_programa();
	}

	log_info(logger,"Conexion exitosa con CPU-Interrupt");

}


void servidor_procesos() {
	server_fd = iniciar_servidor(config_values.ip_escucha, config_values.puerto_escucha);
	if (pthread_create(&thr_consolas, NULL, (void*) manejar_consolas,(void*)server_fd) != 0) {
		log_error(logger, "Error al crear el hilo del servidor");
	}
	else{
		log_debug(logger, "se creo un thread para %s", "Procesos");
		log_info(logger,"Servidor iniciado y esperando que lleguen procesos");
	}
}


