#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int fd_conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'clave'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	clave = config_get_string_value(config, "CLAVE");

	

	// Loggeamos el valor de config
	log_info(logger, "Lei la IP:%s, el PUERTO:%s y la CLAVE:%s", ip, puerto, clave);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	fd_conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(clave , fd_conexion);

	// Armamos y enviamos el paquete
	paquete(fd_conexion);

	terminar_programa(fd_conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL) {
		printf("No se pudo crear el logger. \n");
		exit(1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL) {
		printf("No se pudo crear la config. \n");
		exit(1);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	while (1) {
        leido = readline("> ");

        if (strcmp(leido, "") == 0) {	// Otras opciones: string_is_empty(leido)) / strlen(leido) == 0 / leido == NULL no me funciono.
            free(leido);
			break;
        }
		log_info(logger, leido);
        free(leido);
    }

}

void paquete(int fd_conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	printf("Ingrese las palabras a enviar (dentro de un paquete): \n");

	// Leemos y esta vez agregamos las lineas al paquete
	while (1) {
        leido = readline(">");

        if (strcmp(leido, "") == 0) {	// Otras opciones: string_is_empty(leido)) / strlen(leido) == 0 / leido == NULL no me funciono.
            break;
        }
		int tamanio = string_length(leido) + 1;
		agregar_a_paquete(paquete, leido, tamanio);
        free(leido);
    }
	enviar_paquete(paquete, fd_conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);

}

void terminar_programa(int fd_conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (fd_conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(fd_conexion);
	log_destroy(logger);
	config_destroy(config);
}
