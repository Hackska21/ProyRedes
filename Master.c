#include "Funciones_Sockets.c"
void *hilo0 (void *arg); 
void *hilo1 (void *arg);
void *hilo2 (void *arg);
void ListarArchivos(int des_canal);

struct InfoCliente
{
	char IP[40];
	char Puerto[40];
	int numeroID;
	int des_canal;
};

int sock1;
int sock2;
int conectados = 0;
int Bandera = 0;
int SinResultados = 0;
int Elegido = 0;
char Archivo[50];
struct InfoCliente *InInformacionTodos;


 
main(int argc, char *argv[]) // PuertoPropio IP_otroM Puerto_otroM
{
	
	sock1 = InitSockServ(argv[1]); //Socket Servidor
	sock2 = InitSockClien(argv[3], argv[2]); //socket cliente
	struct InfoCliente A;
	struct sockaddr_in connect_clien;
	char IP[16];

	InInformacionTodos = NULL;

	
	pthread_t *hilos;
    	int *des_canal;

    	int i = 1;
    	int tam;
    	char buffer[512];


	hilos = (pthread_t *) malloc((1) * sizeof(pthread_t));
	pthread_create(&hilos, NULL, &hilo2, NULL); //Hilo cliente Master
	
	

	hilos = (pthread_t ) malloc((1) * sizeof(pthread_t));
	des_canal = accept(sock1, (struct sockaddr*)&connect_clien, &tam); // aceptando primer sol(master)
    	strcpy(IP,inet_ntoa(connect_clien.sin_addr)); 
    	pthread_create(&hilos, NULL, &hilo1, (void*)des_canal);


	while(1)
    	{
    		printf("Creacion canal....\n");
      	hilos = (pthread_t *) malloc((1) * sizeof(pthread_t));
	      tam = sizeof(connect_clien);
	      des_canal = accept(sock1, (struct sockaddr*)&connect_clien, &tam);
	      conectados++;
	      if( des_canal == -1)
	      {
	      	printf("Error al crear el canal");
	            exit(1);
	      }   
	      InInformacionTodos =(struct InfoCliente*)realloc(InInformacionTodos,conectados*sizeof(struct InfoCliente));

	      strcpy(InInformacionTodos[conectados-1].IP,inet_ntoa(connect_clien.sin_addr)); //Se copia IP a estructura global
	      InInformacionTodos[conectados-1].des_canal = des_canal;
	      InInformacionTodos[conectados-1].numeroID = conectados-1;

	      strcpy(A.IP,inet_ntoa(connect_clien.sin_addr)); 
	      A.des_canal = des_canal;
	      A.numeroID = conectados-1;
	        
	      strcpy (A.Puerto,"0");
	      i++;
	      pthread_create(&hilos, NULL, &hilo0, (void*)&A);                     
    }
}


void *hilo0 (void *arg) // Atiende a todos los clientes p2p
{
	struct InfoCliente info = *((struct InfoCliente*)arg);
	char buffer[512];
	int x;
	RecibirMensaje(info.des_canal,buffer,10);  // primer mensaje Puerto
	printf("Recibiendo %s\n",buffer);
	strcpy(info.Puerto,buffer); 
	strcpy(InInformacionTodos[info.numeroID].Puerto,buffer);
	//printf("conectados %d\n",conectados );

	// 

	while(1)
	{
		RecibirMensaje(info.des_canal,buffer,100); 
		if(strcmp(buffer,"EnviarArchivo") == 0)
		{
			RecibirMensaje(info.des_canal,buffer,10); // Recibir nombreeee del archivo
			printf("Archivo solicitado: %s\n",buffer);
			x = buscarArchivo(buffer, info.numeroID);
			if(x == 0)
			{
				printf("No se encontro de este lado...\n");
				strcpy(Archivo,buffer);
				Elegido = info.numeroID;
				Bandera = 1; // hilo 1
				
			}
			else
				printf("Envio IP terminado\n");
		}
	}
}

//enviar puerto ip

void *hilo1 (void *arg) // comunicaciones master a master Server
{
	int des_canal = (int) arg;
	char buffer[512];
	int socktemp;
	RecibirMensaje(des_canal,buffer,512);
	printf("Recibiendo %s\n",buffer);
	while(1)
	{
		sleep(2);
		if(Bandera == 1)
		{
			printf("Solicitando al otro Master...\n");
			EnviarMensaje(des_canal,"SolicitudBusqueda",strlen("SolicitudBusqueda"));
			EnviarMensaje(des_canal,Archivo,strlen(Archivo));
			RecibirMensaje(des_canal,buffer,strlen(buffer)); // sin resultados 
			socktemp = InitSockClien(InInformacionTodos[Elegido].IP,InInformacionTodos[Elegido].Puerto);
			if(strcmp(buffer,"SinResultados") == 0)
			{
					
				EnviarMensaje(socktemp,"SinResultados",strlen("SinResultados"));	

			}
			if(strcmp(buffer,"EnvioInfo") == 0)
			{
				
				EnviarMensaje(socktemp,"EnvioInfo",strlen("EnvioInfo"));

				RecibirMensaje(des_canal,buffer,strlen(buffer)); // Cantidad
				EnviarMensaje(socktemp,buffer,strlen(buffer));

				while(1)
				{
					RecibirMensaje(des_canal,buffer,strlen(buffer)); // IP
					if(strcmp(buffer,"TerminaenvioIP") == 0)
					{
						EnviarMensaje(socktemp,buffer,strlen(buffer));	//Envia TerminaEnvio
						break;	
					}
					EnviarMensaje(socktemp,buffer,strlen(buffer));//Envia IP

					RecibirMensaje(des_canal,buffer,strlen(buffer)); // Puerto
					EnviarMensaje(socktemp,buffer,strlen(buffer));//Envia Puerto
					
				}
				
			}
			Bandera = 0;
			close(socktemp);	
		}
	}
}


void *hilo2 (void *arg) // comunicaciones master a master Cliente
{
	//int des_canal = (int) arg;
	
	char buffer[512];
	strcpy(buffer,"Conectado");
	int x;
	EnviarMensaje(sock2,buffer,strlen(buffer));
	printf("Enviando Conectado\n");
	while(1)
	{
		sleep(2);
		RecibirMensaje(sock2,buffer,strlen(buffer));
		if(strcmp(buffer,"SolicitudBusqueda"))
		{
			RecibirMensaje(sock2, buffer,strlen(buffer));
			printf("Archivo solicitado del otro master: %s\n",buffer);
			x = buscarArchivoTodos(buffer);
		}
	}

}



int buscarArchivo (char *archivo, int numeroIDsol) // Archivo que se busca, ID de cliente que solicita
{
	int i;
	int *sockets;
	char buffer[512];
	int encontrados = 0; 
	int socktemp;
	int Ip_ubicaciones[conectados];
	
	sockets = (int)malloc(sizeof(int)*conectados);
	for (i = 0; i < conectados; i++)
	{
		Ip_ubicaciones[i] = 0; //inicia 0
		if(strcmp(InInformacionTodos[numeroIDsol].IP, InInformacionTodos[i].IP) != 0) //strcmp(puertosol, puerto[i]) == 0
		{
			printf("%s    %s\n", InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);		
			sockets[i] = InitSockClien(InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);
			EnviarMensaje(sockets[i],"ListarArchivos",strlen("ListarArchivos"));
			while(1)
	  		{
				RecibirMensaje(sockets[i],buffer,100);
		  		printf("recibi %s\n",buffer );
		  		if(strcmp(buffer,archivo) == 0);
		  		{
		  			Ip_ubicaciones[i] = 1; //  cuando hay coincidencia
		  			encontrados++;
		  		}
		  		if(strcmp(buffer,"Listado_Completo")==0)
		  		{
		  			printf("Listado terminado\n");
		  			break;
		  		}
	  		}
			close(sockets[i]);
		}
	}
	if (encontrados == 0)
	{
		return 0;
	}



	socktemp = InitSockClien(InInformacionTodos[numeroIDsol].IP, InInformacionTodos[numeroIDsol].Puerto);
	EnviarMensaje(socktemp,"EnvioInfo",strlen("EnvioInfo")); // envia mensaje para enviar ip y puertos
	sprintf(buffer,"%d",encontrados); 
	EnviarMensaje(socktemp,buffer,strlen(buffer));  //	cantidad de encontrados
	for (i = 0; i < conectados; i++)
	{
		if(Ip_ubicaciones[i] == 1)
		{
			EnviarMensaje(socktemp, InInformacionTodos[i].IP, strlen(InInformacionTodos[i].IP));
			EnviarMensaje(socktemp, InInformacionTodos[i].Puerto, strlen(InInformacionTodos[i].Puerto));
		}		
	}
	EnviarMensaje(socktemp, "TerminaenvioIP", strlen("TerminaenvioIP"));
	close(socktemp);
	return encontrados;
}


int buscarArchivoTodos (char *archivo)
{
	int i;
	int *sockets;
	char buffer[512];
	int encontrados = 0; 
	int Ip_ubicaciones[conectados];
	
	sockets = (int)malloc(sizeof(int)*conectados);
	for (i = 0; i < conectados; i++)
	{
		Ip_ubicaciones[i] = 0; //inicia 0
		printf("%s    %s\n", InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);		
		sockets[i] = InitSockClien(InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);
		strcpy(buffer,"ListarArchivos");
		EnviarMensaje(sockets[i],"ListarArchivos",strlen("ListarArchivos"));
		while(1)
	  	{
			RecibirMensaje(sockets[i],buffer,50);
			printf("recibi %s\n",buffer );
			if(strcmp(buffer,archivo) == 0);
			{
				Ip_ubicaciones[i] = 1; //  cuando hay coincidencia
				encontrados++;
			}
			if(strcmp(buffer,"Listado_Completo")==0)
			{
				printf("Listado terminado\n");
				break;
			}
	  	}
		close(sockets[i]);
	}
	
	if (encontrados == 0)
	{
		EnviarMensaje(sock2,"SinResultados",strlen("SinResultados"));
		return 0;
	}


	EnviarMensaje(sock2,"EnvioInfo",strlen("EnvioInfo"));  // envia mensaje para enviar ip y puertos
	sprintf(buffer,"%d",encontrados); 
	EnviarMensaje(sock2,buffer,strlen(buffer));  //	cantidad de encontrados
	for (i = 0; i < conectados; i++)
	{
		if(Ip_ubicaciones[i] == 1)
		{
			EnviarMensaje(sock2, InInformacionTodos[i].IP, strlen(InInformacionTodos[i].IP));
			EnviarMensaje(sock2, InInformacionTodos[i].Puerto, strlen(InInformacionTodos[i].Puerto));
		}		
	}
	EnviarMensaje(sock2, "TerminaenvioIP", strlen("TerminaenvioIP"));
	return encontrados;

} 