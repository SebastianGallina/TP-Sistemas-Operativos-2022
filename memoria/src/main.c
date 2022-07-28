#include "../include/main.h"

//Finalizacion por interrupción ctrl + c
void sighandler(int s){
	if(clienteMemoria){
		liberar_conexion(&clienteMemoria);
	}
	terminar_programa();
	exit(0);
}

int main(void) {

	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal

	// Leer config y arrancar logger
	inicializar();

	inicializar_estructuras();

	// Hilo puerto de escucha (Kernel y CPU)
	pthread_create(&thr_memoria, NULL, (void*) &iniciar_servidor_memoria, NULL);
	// Hilo SWAP
    pthread_create(&thr_swap, NULL, (void*) &gestionar_solicitudes_swap, NULL);

	terminar_programa();
	return EXIT_SUCCESS;
}

void terminar_programa()
{

	pthread_join(thr_memoria, NULL);
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	if(clienteMemoria){
		liberar_conexion(&clienteMemoria);
	}
	close(serverMemoria);
	log_debug(logger,TERMINANDO_EL_LOG);
	liberar_estructuras();
}
