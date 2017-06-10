/*
//Estandars 
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

//Sockets
#include <sys/socket.h>
#include <netdb.h>   
#include <netinet/in.h>
//Hilos
#include <pthread.h>*/

#include "Funciones_Sockets.c"
#include "FuncionesDirectorios.c"
#include "EnviarRecibirArchivos.c"



void* HiloServidorCliente(void* Arg)//Se escuchan Peticiones del Cliente
{
	int x2=(int)Arg,TAM; 
  //printf("Mi canal es: %d :v ",x2);
      
  //zona de comunicaciones  
  char BufferEnviar[50],BufferRecibir[50];

  while(1)//Opciones:
  {

	RecibirMensaje(BufferRecibir,BufferRecibir,50);

	if(strcmp(BufferRecibir,"ListarArchivos"))
	{
		void ListarArchivos(x2);
		printf("Se termino de ListarArchivos\n");
	}

	else if(strcmp(BufferRecibir,"PedirParte"))
	{

	}

	else if(strcmp(BufferRecibir,"CerrarComuicacion"))
	{
		break;
	}
	else
	{
		printf("%s No es valida en este punto\n",BufferRecibir );
	}

  }

  printf("Cliente Termino comunicaciones\n");
  close(x2);
/*
  while(1){
  getchar();
  printf("Introdusca un mensaje\n");
  scanf("%[^\n]",BufferEnviar);

  
  EnviarMensaje(x2,BufferEnviar,50);
  //RecibirMensaje(x2,BufferRecibir,50); 
  //printf("recibi %s\n",BufferRecibir );
  }*/
  
}


void* HiloServidor(void* Arg)//Al Escucha de Peticiones
{
	
	printf("Hilo");
	struct Dir ServInf=*((struct Dir*)Arg);

	int sockt=InitSockServ(ServInf.puerto);
	pthread_t* hilos=NULL;
	
	int clientes;
	int x2;
	struct sockaddr_in conect_cliente;
	int endof=sizeof(conect_cliente); 
	  
	//system("clear");

	while(1){
	x2=(int)malloc(sizeof(int));
	//x2=(int*)malloc(sizeof(int));
	//Paso 5 aceptamos el descriptor del canal en el que se van a comunicar, este solo existe del lado del serividor 
	if((x2=accept(sockt ,(struct sockaddr *)&conect_cliente, &endof))==-1)
	  {
	    printf("Error al aceptar");
	    exit(-1);
	  }
	  
	  clientes++;
	  hilos=(pthread_t*)realloc(hilos,clientes*sizeof(pthread_t));
	  pthread_create (&hilos[clientes-1], NULL,Hilo  ,(void*)x2);
	    
	 
	}
	//char BufferEnviar[50],BufferRecibir[50];

}


void Menu()
{
	int op;
	char BufferEnviar[50];
	do{
		printf("¿Que desea Hacer\n
			1)Solicitar ListarArchivos\n 
			2)Salir\n");
		scanf("%d",&op);
		if(op)
		{
			printf("Nombre de Archivo");
			scanf("%[^\n]",BufferEnviar);
		}

	}while(op);
}


void HiloCliente()//Con el Que se hacen las peticiones;
{
	int server2=InitSockClien(ServInf.puerto,"0.0.0.0");

}


int main(int argc, char const *argv[])
{
		
	if(argc<4)
    {
     	printf("faltan argumentos <puerto> <PuertoDestino> <ip>\n");
     	exit(1);
    }

    pthread_t Des_HiloServidor;
    struct Dir ServInf;
  	ServInf.puerto=argv[1];
  	pthread_create (&Des_HiloRecibir, NULL,HiloServidor  ,(void*)&ServInf);

  	HiloCliente();
     
  //system("./prueba.exec");
  //execl("/bin/ls","ls","-1","/usr",0);

  
  
  
  




	return 0;
}
