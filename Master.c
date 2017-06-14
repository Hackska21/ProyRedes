#include "Funciones_Sockets.c"
void *hilo0 (void *arg); 
void *hilo1 (void *arg);
void *hilo2 (void *arg);
void ListarArchivos(int des_canal);

struct InfoCliente
{
	char IP[40];
	char Puerto[40];
	int des_canal;
};

int sock1;
int sock2;
int conectados = 0;
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
	pthread_create(&hilos, NULL, &hilo2, NULL); //Hilo clienteMaster
	
	

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
        InInformacionTodos =(struct InfoCliente*)realloc(InInformacionTodos,i*sizeof(struct InfoCliente));
        ///(InInformacionTodos[i-1]).IP = "\0";
        ///(InInformacionTodos[i-1]).Puerto = "\0";

        strcpy(InInformacionTodos[i-1].IP,inet_ntoa(connect_clien.sin_addr)); 
        strcpy(A.IP,inet_ntoa(connect_clien.sin_addr)); 
        
        InInformacionTodos[i-1].des_canal = des_canal;
        A.des_canal = des_canal;
        
        //strcpy (InInformacionTodos[i-1].Puerto,"0");
        strcpy (A.Puerto,"0");
        
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
	strcpy(InInformacionTodos[conectados-1].Puerto,buffer);
	printf("conectados %d\n",conectados );

	// 

	printf("puto : %s\n", InInformacionTodos[1].Puerto);
	while(1)
	{
		RecibirMensaje(info.des_canal,buffer,100); 
		if(strcmp(buffer,"EnviarArchivo") == 0)
		{
			RecibirMensaje(info.des_canal,buffer,10); // Recibir nombreeee del puto archivo
			printf("%s----\n",buffer);
			printf("%s    %s\n", InInformacionTodos[0].Puerto, InInformacionTodos[0].IP);
			buscarArchivo(buffer);
			getchar();
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



void buscarArchivo (char *archivo)
{
	// enviar cantidad de puñetas que tienen el archivo despues ip y puerto de los respectivos puñetas
	int i;
	int *sockets;
	char buffer[512];
	sockets = (int)malloc(sizeof(int)*conectados);

	for (i = 0; i < conectados; i++)
	{
		printf("%s    %s\n", InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);
		
		sockets[i] = InitSockClien(InInformacionTodos[i].Puerto, InInformacionTodos[i].IP);
		strcpy(buffer,"ListarArchivos");
		EnviarMensaje(sockets[i],"ListarArchivos"r,strlen("ListarArchivos"));
		while(1)
  		{
			RecibirMensaje(sockets[i],buffer,50);
	  		printf("recibi %s\n",buffer );
	  		if(strcmp(buffer,"Listado_Completo")==0)
	  		{
	  			printf("Listado terminado\n");
	  			break;
	  		}
  		}
		close(sockets[i]);
	}

}
