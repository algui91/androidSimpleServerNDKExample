#include <jni.h>

#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#include "hacking.h"

#define PORT 7890	// the port users will be connecting to
const char* TAG = "SimpleServer.c";
int command_log_pos = 0;

void addToLog(char log[], char *new_entry) {
	/* Almacenar todo lo transmitido en la conexión */
	strcpy(log + command_log_pos, new_entry);
	command_log_pos += strlen(new_entry);
}

char*
startServer(void) {
	int sockfd = 0, new_sockfd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in host_addr, client_addr;	// my address information
	socklen_t sin_size;
	int recv_length = 1, yes = 1;
	char buffer[1024];
	char command_log[16384];
	command_log_pos = 0;

	__android_log_write(ANDROID_LOG_ERROR, TAG, "Iniciando servidor");

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		__android_log_write(ANDROID_LOG_ERROR, TAG, "Fatal en socket");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		__android_log_write(ANDROID_LOG_ERROR, TAG, "Fatal en setsockopt");

	host_addr.sin_family = AF_INET;		 // host byte order
	host_addr.sin_port = htons(PORT);	 // short, network byte order
	host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct

	if (bind(sockfd, (struct sockaddr *) &host_addr,
			sizeof(struct sockaddr)) == -1)
		__android_log_write(ANDROID_LOG_ERROR, TAG, "Fatal en bind");

	if (listen(sockfd, 5) == -1)
		__android_log_write(ANDROID_LOG_ERROR, TAG, "fatal en listen");

	while (1) {    // Accept loop
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *) &client_addr,
				&sin_size);
		if (new_sockfd == -1)
			__android_log_write(ANDROID_LOG_ERROR, TAG, "Fatal en accpct");

		__android_log_print(ANDROID_LOG_ERROR, TAG,
				"server: conexión de %s en puerto %d\n",
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		send(new_sockfd, "Bienvenido!\n", 12, 0);
		recv_length = recv(new_sockfd, &buffer, 1024, 0);

		while (recv_length > 0) {
			buffer[recv_length] = 0;
			addToLog(command_log, buffer);

			if (strncasecmp(buffer, "adios", 5) == 0) {
				send(new_sockfd, "Adios!\n", 7, 0);
				__android_log_print(ANDROID_LOG_ERROR, TAG,
						"Conexión finalizada con %s:%d",
						inet_ntoa(client_addr.sin_addr),
						ntohs(client_addr.sin_port));
				close(new_sockfd);
				close(sockfd);
				return command_log;
			} else if (strncasecmp(buffer, "ip_de ", 6) == 0) {
				struct hostent *host_info;
				struct in_addr *address;
				char *url = buffer + 6; // quito ip_de

				memset(url + (strlen(url) - 2), '\0', 2); // quito \r\n
				host_info = gethostbyname(url);

				if (host_info == NULL) {
					__android_log_print(ANDROID_LOG_ERROR, TAG,
							"Couldn't lookup %s\n",
							url);
					char *fail = "No se pudo resolver el nombre de dominio de ";
					char *errorMessage = (char*)
							ec_malloc(strlen(url) + strlen(fail));

					strcat(errorMessage, fail);
					strcat(errorMessage, url);
					addToLog(command_log, errorMessage);

					//free(errorMessage);
				} else {
					address = (struct in_addr *) (host_info->h_addr);
					__android_log_print(ANDROID_LOG_ERROR, TAG,
							"%s has address %s \n",
							host_info->h_name, inet_ntoa(*address));
					char *host = strcat(url, " has address ");
					strcat(host, inet_ntoa(*address));
					addToLog(command_log, strcat(host, "\n"));
				}
			}
			__android_log_print(ANDROID_LOG_ERROR, TAG,
					"Recibidos %d bytes mensaje: %s",
					recv_length, buffer);
			recv_length = recv(new_sockfd, &buffer, 1024, 0);
		}
		close(new_sockfd);
		close(sockfd);
		__android_log_write(ANDROID_LOG_ERROR, TAG, "closed");

	}
	return 0;
}

jstring Java_com_elbauldelprogramador_simpleserver_MainActivity_stringFromJNI(
		JNIEnv* env, jobject thiz) {
	char *bf = startServer();
	__android_log_write(ANDROID_LOG_ERROR, TAG, bf);
	return (*env)->NewStringUTF(env, bf);
}
