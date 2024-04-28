#include"utils.h"

t_log* logger;

int iniciar_servidor(void)
{
	int fd_escucha;
	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, PUERTO, &hints, &server_info);

	// Creamos el socket de escucha del servidor
	fd_escucha = socket(server_info->ai_family, 
						server_info->ai_socktype, 
						server_info->ai_protocol);

	// Asociamos el socket a un puerto
	bind(fd_escucha, server_info->ai_addr, server_info->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(fd_escucha, SOMAXCONN);

	freeaddrinfo(server_info);
	log_trace(logger, "Listo para escuchar a mi cliente.");

	return fd_escucha;
}

int esperar_cliente(int fd_escucha)
{
	// Aceptamos un nuevo cliente
	int fd_conexion = accept(fd_escucha, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return fd_conexion;
}

int recibir_operacion(int fd_conexion)
{
	int cod_op;
	if(recv(fd_conexion, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(fd_conexion);
		return -1;
	}
}

void* recibir_buffer(int* size, int fd_conexion)
{
	void * buffer;

	recv(fd_conexion, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(fd_conexion, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int fd_conexion)
{
	int size;
	char* buffer = recibir_buffer(&size, fd_conexion);
	log_info(logger, "Me llego el mensaje: %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int fd_conexion)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, fd_conexion);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
