#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#define SA struct sockaddr

int getAnswerC(char *answer){
	printf("Respuesta:%c\n",answer[0]);
	if(answer[0]== 'y' || answer[0] == 'Y')
		return 30;
	else if(answer[0]== 'n' || answer[0] == 'N')
		return 31;
	else
		return -1;	
}

int main(){
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
	printf("\n===============================================\n"); 
	printf("====B I E N V E N I D O = A = P O K E M O N====\n"); 
	printf("===============================================\n"); 
	printf("Presiona enter para iniciar\n");
	scanf("a%s", buffer); 
	sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));
	//send(sockfd, buffer, sizeof(buffer), 0);//envia 'Cualquier tecla'
	printf("\n---------Comenzamos!---------\n");
	int recv_length= recv(sockfd, &buffer, 1024, 0);//inicio de conexion
	printf("Servidor:  %s\n", buffer);
	//printf("Si quieres salir presiona 'x'");
	
	
	while(sizeof(buffer)>0){
		memset(buffer, 0, sizeof(buffer));
		scanf("%s", buffer); 
		sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));//envia 'Cualquier tecla'
	 
		recv_length= recv(sockfd, &buffer, 1024, 0);
		printf("Servidor:  %s\n", buffer);//Envia el primer pokemon
		memset(buffer, 0, sizeof(buffer));
		//recv_length= recv(sockfd, &buffer, 1024, 0);
		scanf("%s", buffer);
		//printf("Se introduce -%s-\n",buffer);
		int acert=getAnswerC(buffer);
		//printf("acert es %d\n",acert);
		while(acert==-1){//Si no se ingreso la entrada correcta
			//recv_length= recv(sockfd, &buffer, buffer_SIZE, 0);//recibe mensaje de error
			printf("Ingresa nuevamente la entrada\n");
			scanf("%s", buffer); //Ingresa nueva entrada
			//send(sockfd, buffer, buffer_SIZE, 0)//Envia entrada
			acert=getAnswerC(buffer);
		}
		
		printf("Se envia -%s-\n",buffer);
		sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));
		memset(buffer, 0, sizeof(buffer));
		printf("Enseguida se mostrara si lo lograste\n");
		if(acert ==30){
			//printf("Entra al acert\n");
						recv_length= recv(sockfd, &buffer, 29, 0);//recibe mensaje de SUERTE
			printf("Servidor:  -%s-\n", buffer);
			int attempts = (int)buffer[7] - 48;
			printf("El intento fue:-%d-\n",attempts);
			
			while(attempts>0){//Mientras hayan intentos
				memset(buffer, 0, 1024);
				recv_length= recv(sockfd, &buffer, 13, 0);//Mensaje de numero de intento
				printf("Servidor: -%s-\n", buffer);
				printf("Inicia intentos\n");
				//Recibe resultado de si fue o no atrapado
				memset(buffer, 0, 1024);
				recv_length= recv(sockfd, &buffer, 12, 0);//Recibe mensaje de capturado?
				printf("Servidor:  -%s-\n", buffer);
				int caught= (int)buffer[11] - 48;
				printf("Capturado?:  -%d-\n", caught);
				
				if(caught==0){//si no fue atrapado
					memset(buffer, 0, 1024);
					recv_length= recv(sockfd, &buffer, 57, 0);//Recibe mensaje si se quiere volver a intentar o no
					printf("Servidor:  -%s-\n", buffer);
					//memset(buffer, 0, recv_length);
					scanf("%s", buffer); 
					printf("Se envia:  -%s-\n", buffer);
					sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));//Se envia mensaje de si se quiere volver a intentar
					printf("YOH\n");
					recv_length= recv(sockfd, &buffer, sizeof(buffer), 0);//Recibe cualquier respuesta
					printf("Servidor:  %s\n", buffer);
					attempts--;
				}else{//Si fue atrapado
					recv_length= recv(sockfd, &buffer, 12, 0);
					printf("Servidor:  %s\n", buffer);
					
					printf("\n---------Data Received---------\n"); 
					memset(buffer, 0, recv_length);
					int sfd =0, n=0, b,tot;
					char rbuff[1024];
					char sendbuffer[100];
					struct sockaddr_in serv_addr;
					memset(rbuff, '0', sizeof(rbuff));
					sfd = socket(AF_INET, SOCK_STREAM, 0);
					serv_addr.sin_family = AF_INET;
					serv_addr.sin_port = htons(5000);
					serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
					b=connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
					if (b==-1) {
						printf("Connect error\n");
						return 1;
					}
					FILE* fp = fopen( "gato1.png", "wb");
					printf("Abro textito1.txt\n"); 
					tot=0;
					if(fp != NULL){
						while( (b = recv(sfd, rbuff, 1024,0))> 0 ) {
							tot+=b;
							fwrite(rbuff, 1, b, fp);
						}
						printf("Received byte: %d\n",tot);
						if (b<0)
							perror("Receiving");
						fclose(fp);
					}else{
						printf("Error File\n");
					}
					printf("Cerrando\n Presione cualquier otra tecla\n");
					close(sfd);
					break; 
				}
				
			//recv_length= recv(sockfd, &buffer, sizeof(buffer), 0);
			//printf("ServidorMil:  %s\n", buffer);	
			//scanf("%s", buffer); 
			//printf("Se envia:  -%s-\n", buffer);
			//sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));
			}
		}else{//Si no acepta
			recv_length= recv(sockfd, &buffer, sizeof(buffer), 0);
			printf("ServidorMiluno:  %s\n", buffer);
			//scanf("%s", buffer); 
			//printf("Se envia:  -%s-\n", buffer);
			//sendto(sockfd, buffer,  1024, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));
		}

    scanf("%s-", buffer);
	}
}
