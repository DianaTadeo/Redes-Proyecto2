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
#include <string.h> 
#include <sys/types.h> 
#include <unistd.h> 


#define IP_PROTOCOL 0 
#define NET_BUF_SIZE 1024
#define cipherKey 'S' 
#define sendrecvflag 0 
#define nofile "File Not Found!" 
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
	printf("Pokerandom es %d\n", r);
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
	printf("Respuesta:%c",answer[0]);
	if(answer[0]== 'y' || answer[0] == 'Y')
		return 30;
	else if(answer[0]== 'n' || answer[0] == 'N')
		return 31;
	else
		return -1;	
}

// function to encrypt 
char Cipher(char ch){ 
    return ch ^ cipherKey; 
} 
  
// function sending file 
int sendFile(FILE* fp, char* buf, int s){ 
    int i, len; 
    if (fp == NULL) { 
        strcpy(buf, nofile); 
        len = strlen(nofile); 
        buf[len] = EOF; 
        for (i = 0; i <= len; i++) 
            buf[i] = Cipher(buf[i]); 
        return 1; 
    } 
  
    char ch, ch2; 
    for (i = 0; i < s; i++) { 
        ch = fgetc(fp); 
        ch2 = Cipher(ch); 
        buf[i] = ch2; 
        if (ch == EOF) 
            return 1; 
    } 
    return 0; 
}

int main(void) {
  int sockfd, new_sockfd;  // descriptores de archivo
  //Estructura para guardar los datos necesarios para el socket
  struct sockaddr_in host_addr, client_addr;  // Informacion de las direcciones IP
  int addrlen = sizeof(host_addr); 
  //tamano para conexiones de red
  socklen_t sin_size;
  int recv_length=1, yes=1;
  host_addr.sin_family = AF_INET;//Establece la familia correspondiente al protocolo
  host_addr.sin_port = htons(PORT);
  host_addr.sin_addr.s_addr = INADDR_ANY; // Asigno mi IPP
  memset(&(host_addr.sin_zero), '\0', 8);
  FILE* fp;
  char buffer[1024];
  char instr[8];
  
  //SOCK_STREAM define que vamos a trabajar con un socket de flujo
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    perror("Error al crear el socket");
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    perror("Error al agregar la opcion SO_REUSEADDR en setsockopt");
  //escucha las peticiones.
  if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
    perror("Error haciendo el bind");
  if (listen(sockfd, 5) == -1)
    perror("Error al escuchar en el socket");

  while(1) {    // Accept loop
	sin_size = sizeof(struct sockaddr_in);
    new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
    if(new_sockfd == -1)
      perror("Error al aceptar la conexion");
    recv_length = recv(new_sockfd, &buffer, 1024, 0);
    printf("server: Conexion aceptada desde %s desde  %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    send(new_sockfd, "Presiona cualquier tecla para comenzar a atraparlos\n", 76, 0);
    while(recv_length > 0) {
	  
	  recv_length = recv(new_sockfd, &buffer, 1024, 0);
	  printf("Se recibio algo de longitud %d\n", recv_length);
	  /*obtenemos el pokemon*/
	  char * pokemon=getPokemon(buffer); 
	  //mensaje del pokemon
	  memset(buffer, 0, recv_length);
	  sprintf(buffer, "El pokemon que se encontro fue: %s\nDeseas capturarlo?[Y/N]\n", pokemon);
      send(new_sockfd,buffer,sizeof(buffer), 0);
      memset(buffer, 0, sizeof(buffer));
      recv_length = recv(new_sockfd, &buffer, 3, 0);//Recibe la respuesta del cliente
      printf("RECV: %d bytes\nENTRADA: -%s-\n", recv_length, buffer);
      
      /*El usuario quiere atrapar al pokemon?*/
      int result= getAnswerC(buffer);
      printf("Entrada %s\n",buffer);
      //Revisando la entrada
      while(result==-1){//entrada incorrecta
		memset(buffer, 0, recv_length);
		//char incorrect[54];
		//strcpy(str, "Entrada incorrecta. Vuelve a ingresar una respuesta \n");
		send(new_sockfd,"Entrada Incorrecta, vuelve e intentarlo\n",40, 0);
		recv_length = recv(new_sockfd, buffer, 1, 0);
		printf("RECV: %d bytes\nENTRADA: -%s-\n", recv_length, buffer);
		result= getAnswerC(buffer);
	  }
	  
	  /*El usuario quiere atrapar al pokemon*/
	  if(result==30){
		int attempts= rand() % 5; //Obtenemos un numero de intentos entre 1 y 5
		memset(buffer, 0, recv_length);
		//char a[30];
		sprintf(buffer, "Tienes %d intentos. SUERTE!!!\n", attempts);
		send(new_sockfd,buffer,29, 0);//Envia mensaje de suerte
		printf("Se envia -%s-\n",buffer);
		memset(buffer, 0, sizeof(buffer));
		int caught=0;//Nos permite saber si fue atrapado
		int counter=1;//Nos permite saber el numero de intento
		while(attempts>0){//Mientras no se acaben los intentos
			caught=rand() % 100;
			printf("\nCachado  es %d y ese (mod 2) es %d\n",caught, caught%2);
			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer, "Intento %d...", counter++);
			printf("Se envia el primero -%s-\n",buffer);
			send(new_sockfd,buffer,13, 0);//Envia mensaje con numero de intento
			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer, "Capturado? %d", caught%2);
			send(new_sockfd,buffer,12, 0);//Envia mensaje de si se logro
			printf("Se envia -%s-\n",buffer);
			/*Si no se captura*/
			if(caught%2 ==0){ //Probabilidad 50/50 de ser atrapado (Si aumenta el modulo, hay mas probabilidad, si se niega, menos)
				memset(buffer, 0, 1024);
				sprintf(buffer, "No se ha podido capturar. Desea volverlo a intentar?[Y/N]");
				send(new_sockfd,buffer,57, 0);//Envia mensaje de repetir
				memset(buffer, 0, 1024);
				printf("Imprimiendo...\n");
				recv_length = recv(new_sockfd, &buffer, 1024, 0);//Recibe si se quiere atrapar
				recv_length = recv(new_sockfd, &buffer, 1024, 0);
				
				printf("RECV: %d bytes\nENTRADA: -%s-\n", recv_length, buffer);
				result= getAnswerC(buffer);
				
				/*Si no ingresa una entrada correcta*/
				while(result==-1){
					memset(buffer, 0, recv_length);
					strcpy(buffer, "Entrada incorrecta. Vuelve a ingresar una respuesta \n");
					send(new_sockfd,buffer,54, 0);
					recv_length = recv(new_sockfd, buffer, 1, 0);
					printf("RECV: %d bytes\nENTRADA: -%s-\n", recv_length, buffer);
					result= getAnswerC(buffer);
				}
				/*Si se acaban los intentos sin lograr atraparlo*/
				char cd[25];
				send(new_sockfd,"Ya no te quedan intentos. Suerte para la proxima!\n",50, 0);
			/*Si se captura*/
			}else{
				int fd =0, confd = 0,b,tot;
				struct sockaddr_in serv_addr;
				char buff[1025];
				int num;
				fd = socket(AF_INET, SOCK_STREAM, 0);
				printf("Socket created\n");
				memset(&serv_addr, '0', sizeof(serv_addr));
				memset(buff, '0', sizeof(buff));
				serv_addr.sin_family = AF_INET;
				serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
				serv_addr.sin_port = htons(5000);
				bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
				listen(fd, 10);
				sprintf(buffer, "Lo lograste!");
				send(new_sockfd,buffer,12, 0);//Envia mensaje de si se logro
				memset(buffer, 0, 1024);
				confd = accept(fd, (struct sockaddr*)NULL, NULL);
				if (confd==-1) {
						perror("Accept");
					continue;
				}
				int sfd =0, n=0;
				FILE *fp = fopen("gato.png", "rb");
				if(fp == NULL){
					perror("File");
					return 2;
				}
				while( (b = fread(buff, 1, sizeof(buff), fp))>0 ){
					send(confd, buff, b, 0);
				}
				fclose(fp);
				total++;//Aumenta el numero de pokemon atrapados
				close(confd);
				break;
			}
			
			attempts--;
		}  

		
	/*El usuario no quiere atrapar al pokemon*/
	  }else{
		char end[50];
		sprintf(end, "El total de pokemons capturdos es: %d \nPresiona cualquier tecla\n", total);
		send(new_sockfd,end,50, 0);
		memset(buffer, 0, recv_length);
		//recv_length = recv(new_sockfd, &buffer, 1024, 0);
		//close(new_sockfd);
	  }
      /*Se limpia el buffer para su uso posterior*/
      
    }
    close(new_sockfd);
  }
  return 0;
}
