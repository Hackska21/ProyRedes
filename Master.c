#include "Funciones_Sockets.c"
void *hilo0 (void *arg); 
void *hilo1 (void *arg);
void *hilo2 (void *arg);
void ListarArchivos(int des_canal);

struct InfoCliente
{
	char IP[16];
	char Puerto[10];
	int des_canal;
};

int sock1;
int sock2;
struct InfoCliente *InInformacionTodos;

 
main(int argc, char *argv[]) // PuertoPropio IP_otroM Puerto_otroM
{
	sock1 = InitSockServ(argv[1]); //Socket Servidor
	sock2 = InitSockClien(argv[3], argv[2]); //socket cliente
	struct InfoCliente A;
	struct sockaddr_in connect_clien;
	char IP[16];
	
	pthread_t *hilos;
    int *des_canal;

    int i = 1;
    int tam;
    char buffer[512];


	hilos = (pthread_t *) malloc((1) * sizeof(pthread_t));
	pthread_create(&hilos, NULL, &hilo2, NULL); //Hilo clienteMaster
	
	


    hilos = (pthread_t *) malloc((1) * sizeof(pthread_t));
	des_canal = accept(sock1, (struct sockaddr*)&connect_clien, &tam); // aceptando primer sol(master)
    strcpy(IP,inet_ntoa(connect_clien.sin_addr)); 
    pthread_create(&hilos, NULL, &hilo1, (void*)des_canal);


	while(1)
    {
    	printf("Creacion canal....\n");
      	hilos = (pthread_t *) malloc((1) * sizeof(pthread_t));
        tam = sizeof(connect_clien);
        des_canal = accept(sock1, (struct sockaddr*)&connect_clien, &tam);
        
        if( des_canal == -1)
        {
            printf("Error al crear el canal");
            exit(1);
        }   
        InInformacionTodos =(struct InfoCliente*)realloc(InInformacionTodos,i*sizeof(struct InfoCliente));
        strcpy(InInformacionTodos[i-1].IP,inet_ntoa(connect_clien.sin_addr)); 
        InInformacionTodos[i-1].des_canal = des_canal;
        strcpy (InInformacionTodos->Puerto[i-1],"0");
        pthread_create(&hilos, NULL, &hilo0, (void*)&InInformacionTodos);
                     
    }
}


void *hilo0 (void *arg) // Atiende a todos los clientes p2p
{
	struct InfoCliente info = *((struct InfoCliente*)arg);
	char buffer[512];
	RecibirMensaje(info.des_canal,buffer,10);  // primer mensaje Puerto
	printf("Recibiendo %s\n",buffer);
	strcpy(info.Puerto,buffer); 

	while(1)
	{
		if(strcmp(buffer,"EnviarArchivo") == 0);
		{
			printf(".----\n");
		}
	}
}

//enviar puerto ip

void *hilo1 (void *arg) // comunicaciones master a master Server
{
	int des_canal = (int) arg;
	char buffer[512];
	RecibirMensaje(des_canal,buffer,512);
	printf("Recibiendo %s\n",buffer);
}


void *hilo2 (void *arg) // comunicaciones master a master Cliente
{
	//int des_canal = (int) arg;
	
	char buffer[512];
	strcpy(buffer,"Master alv");
	EnviarMensaje(sock2,buffer,strlen(buffer));
	printf("Enviando Hola\n");

}

void ListarArchivos(int des_canal)
{
	char BufferRecibir[512];
	while(1)
  	{
		RecibirMensaje(des_canal,BufferRecibir,50);
	  	printf("recibi %s\n",BufferRecibir );
	  	if(strcmp(BufferRecibir,"Listado_Completo")==0)
	  	{
	  		printf("Listado terminado\n");
	  		break;
	  	}
  	}
}

void buscarArchivo (char *archivo)
{
	// enviar cantidad de puñetas que tienen el archivo despues ip y puerto de los respectivos puñetas
	int tam = sizeof(InInformacionTodos);

}
