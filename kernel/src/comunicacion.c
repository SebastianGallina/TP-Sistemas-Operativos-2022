#include "../include/init.h"
#include "../include/comunicacion.h"
#include "../../shared/include/utils/utils.h"
#include "../include/shortTerm.h"

int manejarConexion(void* void_args){

	t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* server_name = args->server_name;
    free(args);

	t_list* lista;
	pcb* nodo_pcb;
	while (socket_cliente != -1) {
		op_code cod_op;
		cod_op = recibir_operacion(socket_cliente);

		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, LECTURA_DE_VALORES);
			list_iterate(lista, (void*) iterator);
			break;
		case PAQUETE_CONSOLA:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, RECEPCION_PAQUETE_CONSOLA);
			nodo_pcb = armar_PCB(lista);
			movePCBtoReady(nodo_pcb);
//			imprimir_PCB(readyQueue->pcb);
			log_info(logger, "Se armó un PCB correctamente");

			t_paquete* paquete = generar_paquete_pcb(*nodo_pcb);

			enviar_paquete(paquete, conexionACPU);
			eliminar_paquete(paquete);

			log_info(logger, "Se enviará un PCB a CPU");

			break;
		case -1:
			log_warning(logger, SERVIDOR_DESCONEXION);
			return EXIT_FAILURE;
		default:
			log_warning(logger,OPERACION_DESCONOCIDA);
			break;
		}
	}
	log_warning(logger, "El cliente se desconecto de %s server", server_name);

	return 0;

}

int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) manejarConexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}


