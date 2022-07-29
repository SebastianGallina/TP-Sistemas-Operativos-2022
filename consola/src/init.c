#include "../include/init.h"
#include "../../shared/include/utils/utils.h"


void inicializar(){
//Logs y Config
	char* log_level;
	config_t config_values;
	char* ip;
	char* puerto;
	char* errorMessageAux[1024];

	char* logger_name = string_new();
	//char* pid_str = string_new();

	//sprintf(pid_str, "%d.log\0", getpid());


	string_append(&logger_name, ARCHIVO_DE_LOG);
	//string_append(&logger_name, pid_str);



	config = iniciar_config(ARCHIVO_DE_CONFIGURACION);
	config_values = leer_config();

	log_level = config_values.log_level;
	logger = iniciar_logger(log_level, logger_name, LOGGER);


	log_info(logger, "Se generó correctamente el logger");
	log_info(logger, "Se generó correctamente la configuracion");

	ip = config_values.ip_kernel;
	puerto = config_values.puerto_kernel;

	// se crea la conexion con el modulo kernel
	conexion = crear_conexion(ip, puerto);
	if(conexion == -1){
			error_handler(logger, errorMessageAux, SERVIDOR_AUSENTE, " Kernel", NULL);
			terminar_programa();
			exit(-1);
	}

	log_info(logger, "Se generó correctamente la conexion con el Kernel");
}


config_t leer_config(){

	config_t config_values;

	config_values.log_level = config_get_string_value(config, "LOG_LEVEL");
	config_values.ip_kernel = config_get_string_value(config, "IP_KERNEL");
    config_values.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

	return config_values;

}
