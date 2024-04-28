#include "server.h"

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int fd_escucha = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente...");
	int fd_conexion = esperar_cliente(fd_escucha);

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(fd_conexion);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(fd_conexion);
			break;
		case PAQUETE:
			lista = recibir_paquete(fd_conexion);
			log_info(logger, "Me llegaron los siguientes valores:");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto. Terminando servidor...\n");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata.");
			break;
		}
	}
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
