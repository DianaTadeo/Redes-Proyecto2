/*														*
 * Programa que realiza la conexion de un socket con el *
 * protocolo de instructor para saber de que instructor	*
 * se trata a traves de su frase celebre.				*
 * 														*
 * Autor: Diana Tadeo									*
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>

#define PORT 6789  // puerto de conexion
int total=0;
/*
 * Funcion que muestra al instructor de acuerdo
 * a su frase celebre.
 * 
 * frase: la frase celebre de acuerdo al instructor
 */
char* getPokemon(char *frase){
	
	int r = rand() % 10;
	if(r==0)
		return "Bulbasaur";
	else if(r==1)
		return "Ivysaur   ";
	else if(r==2)
		return "Venusaur  ";
	else if(r==3)
		return "Charmander";
	else if(r==4)
		return "Charmeleon";
	else if(r==5)
		return "Charizard  ";
	else if(r==6)
		return "Squirtle  ";
	else if(r==7)
		return "Wartortle";
	else if(r==8)
		return "Blastoise";
	else if(r==9)
		return "Caterpie";


}

int getAnswerC(char *answer){
	printf("bla =%c=",answer[0]);
	if(answer[0]== 'y' || answer[0] == 'Y')
		return 30;
	else if(answer[0]== 'n' || answer[0] == 'N')
		return 31;
	else
		return -1;	
	/*
	if(strcmp(answer, "y\n" )== 0 ||strcmp(answer,"Y\n") == 0)
		return 30;
	else if(strcmp(answer, "n\n") == 0||strcmp(answer,"N\n")== 0)
		return 31;
	else
		return -1;
	*/
}

int main(void) {
  int sockfd, new_sockfd;  // descriptores de archivo
  //Estructura para guardar los datos necesarios para el socket
  struct sockaddr_in host_addr, client_addr;  // Informacion de las direcciones IP
  //tamano para conexiones de red
  socklen_t sin_size;
  int recv_length=1, yes=1;

  char buffer[1024];
  char instr[8];
  //PF_INET es una familia de protocolos (macros) ipv4, x25, etc
  //SOCK_STREAM define que vamos a trabajar con un socket de flujo
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    perror("Error al crear el socket");

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    perror("Error al agregar la opcion SO_REUSEADDR en setsockopt");
  
  //Establece la familia correspondiente al protocolo
  host_addr.sin_family = AF_INET;     // 
  //big endian 
  host_addr.sin_port = htons(PORT);   //
  //Macro o constante que define que es nuestra ip
  host_addr.sin_addr.s_addr = INADDR_ANY; // Asigno mi IPP
  memset(&(host_addr.sin_zero), '\0', 8); // El resto de la estructura en 0s
  //escucha las peticiones.
  if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
    perror("Error haciendo el bind");

	//
  if (listen(sockfd, 5) == -1)
    perror("Error al escuchar en el socket");

  while(1) {    // Accept loop
	sin_size = sizeof(struct sockaddr_in);
    new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
    if(new_sockfd == -1)
      perror("Error al aceptar la conexion");
    printf("server: Conexion aceptada desde %s desde  %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    send(new_sockfd, "Conexion aceptada. Comienza a teclear\n", 38, 0);
    recv_length = recv(new_sockfd, &buffer, 1024, 0);
    while(recv_length > 0) {
	  
	  char * pokemon=getPokemon(buffer);
	  char str[65];
	  strcpy(str, "El pokemon que se encontro fue: ");
	  strcat(str,pokemon);
	  strcat(str, "\nDeseas capturarlo?[Y/N]\n");
      send(new_sockfd,str,65, 0);
      recv_length = recv(new_sockfd, &buffer, 1, 0);
      printf("RECV: %d bytes\nENTRADA: -%s-\n", recv_length, buffer);
      int result= getAnswerC(buffer);
      /*Si no ingresa una entrada correcta*/
      while(result==-1){
		memset(buffer, 0, recv_length);
		char str[54];
		strcpy(str, "Entrada incorrecta. Vuelve a ingresar una respuesta \n");
		send(new_sockfd,str,54, 0);
		recv_length = recv(new_sockfd, buffer, 1, 0);
		printf("RECV: %d bytes\nENTRADA: -%s-\n", recv_length, buffer);
		result= getAnswerC(buffer);
	  }
	  
	  
	  if(result==30){
		  
		int attempts= rand() % 5;
		int caught=0;
		while(attempts>0){
			caught=rand() % 1;
			if(caught==0){
				memset(buffer, 0, recv_length);
				send(new_sockfd,"No se ha podido capturar. Desea volverlo a intentar?[Y/N]\n",58, 0);
				recv_length = recv(new_sockfd, &buffer, 2, 0);
				printf("RECV: %d bytes\nENTRADA: -%s-\n", recv_length, buffer);
				result= getAnswerC(buffer);
				
				/*Si no ingresa una entrada correcta*/
				while(result==-1){
					memset(buffer, 0, recv_length);
					char str[54];
					strcpy(str, "Entrada incorrecta. Vuelve a ingresar una respuesta \n");
					send(new_sockfd,str,54, 0);
					recv_length = recv(new_sockfd, buffer, 1, 0);
					printf("RECV: %d bytes\nENTRADA: -%s-\n", recv_length, buffer);
					result= getAnswerC(buffer);
				}
				
				char cd[25];
				sprintf(cd, "Aun te quedan %d intentos\n", attempts-1);
				send(new_sockfd,cd,25, 0);
				
			}else{
				send(new_sockfd,"Se logro capturar.\n",19, 0);
				total++;
				break;
			}
			
			attempts--;
		}  

		
		
	  }else{
		char end[65];
		//strcpy(end, "El total de pokemons capturdos es: ");
		sprintf(end, "El total de pokemons capturdos es: %d \nSesion terminada\n ", total);
		//strcat(end, "");
		send(new_sockfd,end,65, 0);
		close(new_sockfd);
	  }
	  
 
      /*Se limpia el buffer para su uso posterior*/
      memset(buffer, 0, recv_length);
      recv_length = recv(new_sockfd, &buffer, 1024, 0);
    }
    close(new_sockfd);
  }
  return 0;
}