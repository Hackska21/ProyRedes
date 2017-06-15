//Estandars 
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


//Sockets
#include <sys/socket.h>
#include <netdb.h>   
#include <netinet/in.h>
//Hilos
#include <pthread.h>

#include <dirent.h>
#include <arpa/inet.h>

#define TamPaquetes 512


int EnviarMensaje(int Socket,char* mensaje,int Tamano);
int RecibirMensaje(int Socket,char* mensaje,int Tamano);





struct Dir
{
  char puerto[50];
  char ip[50];
  //char parte;

};

int InitSockServ(char *Puerto)//Inicializa un socket Servidor. 
{
//paso 1 creando el socket
  int sockt; 
  int bandera; 
  struct sockaddr_in conect_serv,conect_cliente;  // estructura del socket
  sockt = socket(AF_INET, SOCK_STREAM, 0);
  
  if(sockt==-1)
  {
    exit(0);
  }     
       
  int x2,clientes=0,i; //Descriptor de canal
  int endof=sizeof(conect_cliente); //Tamaño de estructura


        
  //paso 2 llenar la estructura 
  memset(&conect_serv,0,sizeof(conect_serv));      // llena con 0 toda la estructura
  conect_serv.sin_family = AF_INET;                
  conect_serv.sin_addr.s_addr = INADDR_ANY; //inet_addr(argv[2]);    // cast requerido para el campo
  conect_serv.sin_port = htons(atoi(Puerto));//Puerto);            // cast requerido para el campo
  setsockopt(sockt, SOL_SOCKET, SO_REUSEADDR, &bandera, sizeof(int));  
      
      
  //printf("Dirección IP: %s\n",inet_ntoa(conect_serv.sin_addr.s_addr));
  /* muestra la dirección IP */
      
  
  //Paso 3 Publicar direccion IP y puerto para ser accesible por cualquiera que se quiera conectar
  if((bind(sockt , (struct sockaddr*)&conect_serv, sizeof(conect_serv) ))==-1)
    {
     	printf("Error al publicar IP\n");
     	exit(-1);
    }
      
  //Paso 4 Escuchar conexiones
  if((listen(sockt,2))==-1)
    {
     	printf("Error al escuchar\n");
     	exit(-1);
    }
    printf("Socket Servidor Iniciado\n");
    return sockt;

}

int InitSockClien(char* Puerto, char* IP)
{
	int x = socket(AF_INET, SOCK_STREAM, 0),TAM;    
	struct sockaddr_in conect_serv;
	printf("Creando Socket Cliente\n");
	if(x == -1)
	{
	        printf("Error al crear el socket");
	        exit(1);
	}
	//paso 2 llenar la estructura 
	memset(&conect_serv,0,sizeof(conect_serv));      // llena con 0 toda la estructura
	conect_serv.sin_family = AF_INET;
	conect_serv.sin_addr.s_addr = inet_addr(IP);    	// cast requerido para el campo
	//printf("yaaa\n");
	conect_serv.sin_port = htons(atoi(Puerto));//Puerto);  // cast requerido para el campo

	printf("Tratando de conectarse\n");
	//paso 3 conectarse al servidor
	while(1){
	if(connect(x,(struct sockaddr*)&conect_serv, sizeof(conect_serv)) == -1)
	{
	        printf("Error al conectar al socket\n\n");
	        //exit(1);
	        sleep(5);
	}
	else
		break;
	}
	return x;
}

int EnviarMensaje(int Socket,char* mensaje,int Tamano)
{
	//Paso  enviar mensaje
	if(send(Socket, mensaje, Tamano, 0) == -1)
	{
	printf("Error al enviar el mensaje");
	exit(1);
	}



}

int RecibirMensaje(int Socket,char* mensaje,int Tamano)
{
	//Paso  recibir mensaje
	int TAM;
	if((TAM=recv(Socket, mensaje, Tamano, 0)) == -1 )
	{
	printf("Error al recibir el mensaje");
	exit(1);        
	}
	mensaje[TAM]='\0';
	return TAM;
}