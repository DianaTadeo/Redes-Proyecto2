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
	memset(buffer, 0, sizeof(buffer));
	scanf("%s", buffer); 
	while(sizeof(buffer)>0){
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
		
		if(acert ==30){
			//printf("Entra al acert\n");
			//recv_length= recvfrom(sockfd, buffer,1024, 0, (struct sockaddr*)&servaddr,sizeof(struct sockaddr));//recibe mensaje de SUERTE
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
				printf("cachado?:  -%d-\n", caught);
				if(caught==0){//si no fue atrapado
					memset(buffer, 0, 1024);
					recv_length= recv(sockfd, &buffer, 57, 0);//Recibe mensaje si se quiere volver a intentar o no
					printf("Servidor:  -%s-\n", buffer);
					memset(buffer, 0, recv_length);
					scanf("%s", buffer); 
					int acert=getAnswerC(buffer);
					printf("acert fue %d\n", acert);
					while(acert==-1){//Si no se ingreso la entrada correcta
						printf("Ingresa nuevamente la entrada\n");
						scanf("%s", buffer); //Ingresa nueva entrada
						acert=getAnswerC(buffer);
					}
					printf("Se envia:  -%s-\n", buffer);
					sendto(sockfd, "YHOLI",5, 0, (struct sockaddr*)&servaddr,sizeof(servaddr));//Envia si se quiere volver a intentar o no

					printf("YOH\n");
					recv_length= recv(sockfd, &buffer, sizeof(buffer), 0);
					printf("Servidor:  %s\n", buffer);
				}else{//Si fue atrapado
					recv_length= recv(sockfd, &buffer, sizeof(buffer), 0);
					printf("Servidor:  %s\n", buffer);
					
					printf("\n---------Data Received---------\n"); 

					while (1) { 
					// receive 
					memset(buffer, 0, recv_length);
					recv_length= recv(sockfd, &buffer, sizeof(buffer), 0);

					// process 
						//if (recvFile(buffer, sizeof(buffer))) { 
						//	break; 
						//} 
					} 
				}
			}
		}else{
			recv_length= recv(sockfd, &buffer, sizeof(buffer), 0);
			printf("Servidor:  %s\n", buffer);
		}
    //scanf("%s", buffer); 
	//send(sockfd, buffer, sizeof(buffer), 0);//envia 'Cualquier tecla'
    
    //FILE *f;
    //f=fopen("add.txt","r");
    //fscanf(f,"%s",buffer);
    //write(sockfd,buffer,100);
    scanf("%s-", buffer);
	}
    printf("the file was sent successfully");
}
