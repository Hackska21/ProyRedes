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
	//printf("sock2 %d\n",sock2 );
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
    		printf("Creacion canal....");
      	hilos = (pthread_t *) malloc((1) * sizeof(pthread_t));
	      tam = sizeof(connect_clien);
	      des_canal = accept(sock1, (struct sockaddr*)&connect_clien, &tam);
	      conectados++;
	      printf(" %d \n", conectados);
	      
	      if( des_canal == -1)
	      {
	      	printf("Error al crear el canal");
	            exit(1);
	      }   
	      InInformacionTodos =(struct InfoCliente*)realloc(InInformacionTodos,conectados*sizeof(struct InfoCliente));

	      strcpy(InInformacionTodos[conectados-1].IP,inet_ntoa(connect_clien.sin_addr)); //Se copia IP a estructura global
	      InInformacionTodos[conectados-1].des_canal = des_canal;
	      InInformacionTodos[conectados-1].numeroID = conectados-1;

	      printf("IP:%s\n",inet_ntoa(connect_clien.sin_addr) );

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
	RecibirMensaje(info.des_canal,buffer,10);  // primer mensaje Puerto
	printf("Recibiendo %s\n",buffer);
	strcpy(info.Puerto,buffer); 
	strcpy(InInformacionTodos[info.numeroID].Puerto,buffer);
	//printf("conectados %d\n",conectados );
	int x;
	// 

	while(1)
	{
		RecibirMensaje(info.des_canal,buffer,50); 
		if(strcmp(buffer,"EnviarArchivo") == 0)
		{
			RecibirMensaje(info.des_canal,buffer,50); // Recibir nombreeee del archivo
			printf("Archivo solicitado: %s\n",buffer);
			x = buscarArchivo(buffer, info.numeroID);
			if(x == 0)
			{
				printf("No se encontro de este lado...\n");
				strcpy(Archivo,buffer);
				printf("Archivo   %s\n", Archivo);
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
	char IP[10][50];
	char Puerto[10][50];
	int cantidad;
	int i = 0;
	
	RecibirMensaje(des_canal,buffer,512);
	printf("Recibiendo %s\n",buffer);
	while(1)
	{
		//printf(":v---\n");
		sleep(2);
		if(Bandera == 1)
		{
			printf("Solicitando al otro Master...%s\n", Archivo);
			EnviarMensaje(des_canal,"SolicitudBusqueda",512);
			EnviarMensaje(des_canal,Archivo,512);


			RecibirMensaje(des_canal,buffer,512); // sin resultados 
			if(strcmp(buffer,"SinResultados") == 0)
			{
				printf("Resultados otro SinResultados\n");		
				Bandera = 0;
				EnviarMensaje(InInformacionTodos[Elegido].des_canal,"SinResultados",strlen("SinResultados"));	

			}
			
				printf("Resultados otro IP\n");
				printf("Cantidad %s\n",buffer );
				cantidad = atoi(buffer);
				

				while(1)
				{
					RecibirMensaje(des_canal,buffer,512); // IP
					printf("buffer %s\n",buffer);
					

					if(strcmp(buffer,"TerminaenvioIP") == 0)
					{
						break;	
					}

					strcpy(IP[i], buffer);
					//sleep(1);
					
					RecibirMensaje(des_canal,buffer,512); // Puerto
					strcpy(Puerto[i], buffer);
					printf("bufferP\n",buffer);
					i++;				
					
				}
				printf("Enviando...\n");
				sprintf(buffer,"%d",cantidad);
				EnviarMensaje(InInformacionTodos[Elegido].des_canal,buffer,50); // cantidad
				for ( i = 0; i < cantidad; i++)
				{
					printf("IP %s\n",IP[i] );
					EnviarMensaje(InInformacionTodos[Elegido].des_canal,IP[i],50);//Envia IP	
					sleep(1);
					printf("Puerto %s\n",Puerto[i] );
					EnviarMensaje(InInformacionTodos[Elegido].des_canal,Puerto[i],50);//Envia Puerto
				}
				EnviarMensaje(InInformacionTodos[Elegido].des_canal,"TerminaenvioIP",50);//Envia Fin

				
			Bandera = 0;
			printf("Bandera = %d\n",Bandera);
			
		}
	}
}


void *hilo2 (void *arg) // comunicaciones master a master Cliente
{
	//int des_canal = (int) arg;
	
	char buffer[512];
	strcpy(buffer,"Conectado");
	int x;
	EnviarMensaje(sock2,buffer,512);
	printf("Enviando Conectado\n");
	while(1)
	{
		sleep(2);
		RecibirMensaje(sock2,buffer,512);
		printf("%s\n",buffer );
		if(strcmp(buffer,"SolicitudBusqueda")==0)
		{
			printf("Solicitud\n");
			RecibirMensaje(sock2, buffer,512);
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
	char buffer2[512];
	int encontrados = 0; 
	int Ip_ubicaciones[conectados];
	
	//sockets = (int)malloc(sizeof(int)*conectados);
	//printf("------Conectados %d\n",conectados );
	for (i = 0; i < conectados; i++)
	{
		strcpy(buffer,"\0");
		Ip_ubicaciones[i] = 0; //inicia 0
		//if(strcmp(InInformacionTodos[numeroIDsol].IP, InInformacionTodos[i].IP) != 0 && strcmp(InInformacionTodos[numeroIDsol].Puerto, InInformacionTodos[i].Puerto) != 0) //strcmp(puertosol, puerto[i]) == 0
		//{
			//printf("%s    %s\n", InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);		
			int j = InitSockClien(InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);
			//getchar();
			//printf("Antes de enviar ListarArchivos\n");
			EnviarMensaje(j,"ListarArchivos",50);
			///printf("Despues de enviar ListarArchivos\n");
			while(1)
	  		{
				RecibirMensaje(j,buffer,100);
		  		//printf("comparando %s con %s\n",archivo, buffer );
		  		if(strcmp(buffer,archivo) == 0)
		  		{
		  			Ip_ubicaciones[i] = 1; //  cuando hay coincidencia
		  			//printf("coincidencia\n");
		  			encontrados++;
		  		}
		  		if(strcmp(buffer,"Listado_Completo")==0)
		  		{
		  			//printf("Listado terminado\n");
		  			break;
		  		}
	  		}
	  		EnviarMensaje(j,"CerrarComuicacion",50);
			close(j);

	}
	//}
	if (encontrados == 0)
	{
		//printf("Sin resultados funcion\n");
		return 0;
	}


	strcpy(buffer2,"\0");
	sprintf(buffer2,"%d",encontrados); 
	//printf("encontrados %d, buffer %s\n",encontrados, buffer );
	
	EnviarMensaje(InInformacionTodos[numeroIDsol].des_canal,buffer2,50);  //	cantidad de encontrados
	for (i = 0; i < conectados; i++)
	{
		if(Ip_ubicaciones[i] == 1)
		{
			EnviarMensaje(InInformacionTodos[numeroIDsol].des_canal, InInformacionTodos[i].IP, 50);
			EnviarMensaje(InInformacionTodos[numeroIDsol].des_canal, InInformacionTodos[i].Puerto,50);
		}		
	}
	//printf("Termino envio ip\n");
	EnviarMensaje(InInformacionTodos[numeroIDsol].des_canal, "TerminaenvioIP", 50);
	
	return encontrados;
}


int buscarArchivoTodos (char *archivo)
{
	int i;
	char buffer[512];
	char buffer2[512];
	int encontrados = 0; 
	int Ip_ubicaciones[conectados];
	//int socktemp = sock2;
	
	for (i = 0; i < conectados; i++)
	{
		strcpy(buffer,"\0");
		Ip_ubicaciones[i] = 0; //inicia 0
		//printf("%s    %s\n", InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);		
		int j= InitSockClien(InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);
		EnviarMensaje(j,"ListarArchivos",50);
		while(1)
	  	{
			RecibirMensaje(j,buffer,50);
			//printf("recibi %s\n",buffer );
			if(strcmp(buffer,archivo) == 0)
			{
				//printf("coincidencia\n");
				Ip_ubicaciones[i] = 1; //  cuando hay coincidencia
				encontrados++;
			}
			if(strcmp(buffer,"Listado_Completo")==0)
			{
				//printf("Listado terminado\n");
				break;
			}
	  	}
	  	EnviarMensaje(j,"CerrarComuicacion",50);
		close(j);
	}
	
	if (encontrados == 0)
	{
		//printf("Sin Resultados\n");
		EnviarMensaje(sock2,"SinResultados",strlen("SinResultados"));
		return 0;
	}


	//EnviarMensaje(sock2,"EnvioInfo",512);  // envia mensaje para enviar ip y puertos
	strcmp(buffer2,"\0");
	sprintf(buffer2,"%d",encontrados); 
	EnviarMensaje(sock2,buffer2,50);  //	cantidad de encontrados
	for (i = 0; i < conectados; i++)
	{
		//printf("Sock2 %d---Des_canal%d \n",sock2, InInformacionTodos[i].des_canal );
		if(Ip_ubicaciones[i] == 1)
		{
			printf("Enviando por sock2(%d) IP y puerto\n",sock2);
			sleep(1);
			EnviarMensaje(sock2, InInformacionTodos[i].IP, 50);
			sleep(1);
			EnviarMensaje(sock2, InInformacionTodos[i].Puerto, 50);

		}		
	}
	//close(socktemp);
	printf("Termino envio ip\n");
	sleep(2);
	EnviarMensaje(sock2, "TerminaenvioIP\0", 512);
	//EnviarMensaje(sock2, "TerminaenvioIP\0", 512);
	
	return encontrados;

} 