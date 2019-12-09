#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SA struct sockaddr
int total=0;

/*Funcion auxiliar que devuelve el valor de una respuesta*/
int getAnswerC(char *answer){
	//printf("Respuesta:%c\n",answer[0]);
	if(answer[0]== 'y' || answer[0] == 'Y')
		return 30;
	else if(answer[0]== 'n' || answer[0] == 'N')
		return 31;
	else
		return -1;	
}

int main(){
	/*Creo el socket principal de la comunicacion*/
    int sockfd;
    char fname[25];
    int len;
    struct sockaddr_in servaddr,cliaddr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(6789);
    char el[4];
	sprintf(el, "%d", 6789);
    inet_pton(AF_INET,el,&servaddr.sin_addr);
    connect(sockfd,(SA*)&servaddr,sizeof(servaddr));
    
    
   
					
    char buffer[1024];
	printf("\n=============================================================================================================================================\n"); 
	printf("=============================================================================================================================================\n"); 
	printf (" .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .-----------------.\n");
	printf ("| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |\n");
	printf ("| |   ______     | || |     ____     | || |  ___  ____   | || |  _________   | || | ____    ____ | || |     ____     | || | ____  _____  | |\n");
	printf ("| |  |_   __ \\   | || |   .'    `.   | || | |_  ||_  _|  | || | |_   ___  |  | || ||_   \\  /   _|| || |   .'    `.   | || ||_   \\|_   _| | |\n");
	printf ("| |    | |__) |  | || |  /  .--.  \\  | || |   | |_/ /    | || |   | |_  \\_|  | || |  |   \\/   |  | || |  /  .--.  \\  | || |  |   \\ | |   | |\n");
	printf ("| |    |  ___/   | || |  | |    | |  | || |   |  __'.    | || |   |  _|  _   | || |  | |\\  /| |  | || |  | |    | |  | || |  | |\\ \\| |   | |\n");
	printf ("| |   _| |_      | || |  \\  `--'  /  | || |  _| |  \\ \\_  | || |  _| |___/ |  | || | _| |_\\/_| |_ | || |  \\  `--'  /  | || | _| |_\\   |_  | |\n");
	printf ("| |  |_____|     | || |   `.____.'   | || | |____||____| | || | |_________|  | || ||_____||_____|| || |   `.____.'   | || ||_____|\\____| | |\n");
	printf ("| |              | || |              | || |              | || |              | || |              | || |              | || |              | |\n");
	printf ("| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |\n");
	printf (" '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------' \n");
	printf("=============================================================================================================================================\n"); 
	printf("=============================================================================================================================================\n"); 
	printf("Presiona enter para iniciar\n");
	scanf("a%s", buffer); 
	sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));
	printf("\n---------Comenzamos!---------\n");
	int recv_length= recv(sockfd, &buffer, 1024, 0);//inicio de conexion
	printf("Servidor:  %s\n", buffer);
	//printf("Si quieres salir presiona 'x'");
	
	
	
	
	while(sizeof(buffer)>0){
		memset(buffer, 0, 1024);
		scanf("%s", buffer);
		printf("buffer: %s\n",buffer); 
		sendto(sockfd, buffer,  1, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));//envia 'Cualquier tecla'
		memset(buffer, 0, 1024);
		recv_length= recv(sockfd, &buffer, 1024, 0);//Recibe el pokemon
		printf("=============================================================================================================================================\n"); 
		printf("========Servidor:  %s\n", buffer);
		printf("=============================================================================================================================================\n"); 

		memset(buffer, 0, sizeof(buffer));
		//recv_length= recv(sockfd, &buffer, 1024, 0);
		scanf("%s", buffer);
		
		int acert=getAnswerC(buffer);
		
		//printf("acert es %d\n",acert);
		while(acert==-1){//Si no se ingreso la entrada correcta
			printf("Ingresa nuevamente la entrada\n");
			scanf("%s", buffer); //Ingresa nueva entrada
			acert=getAnswerC(buffer);
		}
		
		
		sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));
		memset(buffer, 0, sizeof(buffer));
		printf("Enseguida se mostrara una respuesta\n");
		if(acert ==30){
			recv_length= recv(sockfd, &buffer, 29, 0);//recibe mensaje de SUERTE
			printf("=============================================================================================================================================\n"); 
			printf("========Servidor:  %s\n", buffer);
			printf("=============================================================================================================================================\n"); 
			int attempts = (int)buffer[7] - 48;
			
			
			while(attempts>0){//Mientras hayan intentos
				memset(buffer, 0, 1024);
				recv_length= recv(sockfd, &buffer, 13, 0);//Mensaje de numero de intento
				printf("=============================================================================================================================================\n"); 
				printf("========Servidor:  %s\n", buffer);
				printf("=============================================================================================================================================\n"); 
				//Recibe resultado de si fue o no atrapado
				memset(buffer, 0, 1024);
				recv_length= recv(sockfd, &buffer, 12, 0);//Recibe mensaje de capturado?
				
				int caught= (int)buffer[11] - 48;
				
				if(caught==0){//si no fue atrapado
					printf("No fue atrapado :(\n", caught);
					memset(buffer, 0, 1024);
					recv_length= recv(sockfd, &buffer, 57, 0);//Recibe mensaje si se quiere volver a intentar o no
					printf("=============================================================================================================================================\n"); 
					printf("========Servidor:  %s\n", buffer);
					printf("=============================================================================================================================================\n"); 
					scanf("%s", buffer); 
					sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));//Se envia mensaje de si se quiere volver a intentar
					recv_length= recv(sockfd, &buffer, sizeof(buffer), 0);//Recibe cualquier respuesta
					printf("=============================================================================================================================================\n"); 
					printf("========Servidor:  %s\n", buffer);
					printf("=============================================================================================================================================\n"); 

					attempts--;
				}else{//Si fue atrapado
					total++;
					int sfd =0, n=0, b,tot;
					char rbuff[60000];
					char sendbuffer[100];
					struct sockaddr_in serv_addr;
					memset(rbuff, '0', sizeof(rbuff));
					sfd = socket(AF_INET, SOCK_STREAM, 0);
					serv_addr.sin_family = AF_INET;
					serv_addr.sin_port = htons(5000);
					serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
					recv_length= recv(sockfd, &buffer, 12, 0);
					printf("=============================================================================================================================================\n"); 
					printf("========Servidor:  %s\n", buffer);
					printf("=============================================================================================================================================\n"); 

					memset(buffer, 0, recv_length);
					printf("Ingresa cuelquier tecla para recibirlo\n");
					scanf("%s", buffer); 
					printf("\n---------Recibiendo Pokemon---------\n"); 
					
					b=connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
					if (b==-1) {
						printf("Connect error\n");
						return 1;
					}
					int dir=mkdir("Capturados",0777);
					char nombre[3];
					sprintf(nombre, "Capturados/%d.png",total);
					FILE* fp = fopen( nombre, "wb");
					tot=0;
					if(fp != NULL){
						while( (b = recv(sfd, rbuff, 60000,0))> 0 ) {
							tot+=b;
							fwrite(rbuff, 1, b, fp);
							//break;
						}
						printf("Received byte: %d\n",tot);
						if (b<0)
							perror("Receiving");
						fclose(fp);
					}else{
						printf("Error File\n");
					}
					printf("=============================================================================================================================================\n"); 

			
					printf("TIENES UN NUEVO POKEMON!\n Revisa la carpeta de 'Atrapados' para verlo\n");
					printf("=============================================================================================================================================\n\n"); 

					printf("Envia 2 veces una tecla para continuar\n");
					close(sfd);
					break; 
				}
			printf("Cerrando\n Envia cualquier otra tecla\n");
			}
		}else{//Si no acepta
			recv_length= recv(sockfd, &buffer, 72,0);
			printf("=============================================================================================================================================\n"); 
			printf("========Servidor:  %s\n", buffer);
			printf("=============================================================================================================================================\n"); 
		}

    scanf("%s-", buffer);
	}
}
