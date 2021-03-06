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

char* Directorio;//="./Cliente/";
pthread_t* hilos;
int clientes=0;

void trapper(int sig)
{

    printf("Recibida la señal: %d\n", sig);
    int i;
    for(i=0;i<clientes;i++)
		{pthread_join (hilos[i], NULL);}
	
	
	//close(SockServ);

    exit(0);
    
}


void* HiloServidorCliente(void* Arg)//Se escuchan Peticiones del Cliente
{
	int x2=(int)Arg,TAM;
	printf("Nuevo Cliente conectado\n"); 
  //printf("Mi canal es: %d :v ",x2);
      
  //zona de comunicaciones  
  char BufferEnviar[50],BufferRecibir[50];

  while(1)//Opciones:
  {

	RecibirMensaje(x2,BufferRecibir,50);
	//printf("Se recibio: %s\n", BufferRecibir);

	if(strcmp(BufferRecibir,"ListarArchivos")==0)
	{
		//printf("Se inicio de ListarArchivos\n");
		ListarArchivos(x2,Directorio);

		printf("Se termino de ListarArchivos\n");
	}

	else if(strcmp(BufferRecibir,"PedirParte")==0)
	{
		FunEnviarParte(x2,Directorio);
	}

	else if(strcmp(BufferRecibir,"CerrarComuicacion")==0)
	{
		break;
	}
	else
	{
		printf("%s No es valida en este punto\n",BufferRecibir );
		getchar();
		getchar();
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
	
	printf("Hilo Servidor Iniciado\n");
	struct Dir ServInf=*((struct Dir*)Arg);

	int sockt=InitSockServ(ServInf.puerto);
	hilos=NULL;
	
	
	int x2;
	struct sockaddr_in conect_cliente;
	int endof=sizeof(conect_cliente); 
	  
	//system("clear");
	printf("Servidor: Al espera de Comunicaciones\n");
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
	  pthread_create (&hilos[clientes-1], NULL,HiloServidorCliente  ,(void*)x2);
	    
	 
	}
	//char BufferEnviar[50],BufferRecibir[50];

}


void Menu(int sock)
{
	int op;
	char BufferEnviar[50];
	do{
		printf("¿Que desea Hacer\n1)Solicitar ListarArchivos\n0)Salir\n");
		
		scanf("%d",&op);
		if(op)
		{
			printf("Nombre de Archivo");
			getchar();
			scanf("%[^\n]",BufferEnviar);
			FunPendirArchivo(sock,BufferEnviar,Directorio);
		}

	}while(op);
}


void HiloCliente(char* puerto,char* ip,char* puertoEscucha)//Con el Que se hacen las peticiones;
{
	//int server2=InitSockClien(ServInf.puerto,"0.0.0.0");
	//getchar();
	int server2=InitSockClien(puerto,ip);
	char BufferEnviar[50],BufferRecibir[50];
	EnviarMensaje(server2,puertoEscucha,50);
	Menu(server2);

	//getchar();
	/*
	//EjemploPedirListado
	EnviarMensaje(server2,"ListarArchivos",50);
	while(1)
  {
	  RecibirMensaje(server2,BufferRecibir,50);
	  printf("recibi %s\n",BufferRecibir );
	  if(strcmp(BufferRecibir,"Listado_Completo")==0)
	  {
	  	printf("Listado terminado\n");
	  	break;
	  }
  }

  //EjemploPedirArchiv
  int sock[1];
  sock[0]=server2;
  DescargarArchivo(sock,1,"Firma.jpg","./Prueba/");


  //CerrarComuicacion
  EnviarMensaje(server2,"CerrarComuicacion",50);
	*/

}


int main(int argc, char const *argv[])
{
		
	signal(2, trapper);
	if(argc<5)
    {
     	printf("faltan argumentos <puerto> <PuertoDestino> <ip> <nomrbecarpeta>\n");
     	exit(1);
    }
    Directorio=argv[4];
    pthread_t Des_HiloServidor;
    struct Dir ServInf;
  	//ServInf.puerto=argv[1];
  	strcpy(ServInf.puerto,argv[1]);

  	pthread_create (&Des_HiloServidor, NULL,HiloServidor  ,(void*)&ServInf);
  	//printf("O:\n");
  	HiloCliente(argv[2],argv[3],argv[1]);
  	pthread_join (Des_HiloServidor, NULL);
  	
     
  //system("./prueba.exec");
  //execl("/bin/ls","ls","-1","/usr",0);

  
  
  
  




	return 0;
}
