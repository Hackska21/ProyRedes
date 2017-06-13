#include "Funciones_Sockets.c"
void *hilo0 (void *arg); 
void *hilo1 (void *arg);
void *hilo2 (void *arg);
int sock1;
int sock2;
char IP_otroM[16];

struct InfoClien
{
	char IP[16];
	char Archivos[512];
};
 
main(int argc, char *argv[]) // PuertoPropio IP_otroM Puerto_otroM
{
	sock1 = InitSockServ(argv[1]);
	sock2 = InitSockClien(argv[3], argv[2]); //socket cliente
	//sock1 = InitSockServ(argv[1]);
	struct sockaddr_in connect_clien;
	char IP[16];
	
	pthread_t *hilos;
    int *des_canal;
    struct InfoClien *x;

    int tam;
    char buffer[512];
	
	int puerto = atoi(argv[3]); // puerto del otro master

	pthread_create(&hilos, NULL, &hilo2, (void*)des_canal);
	strcpy(IP,"\0");
	strcpy(IP_otroM,argv[2]);
	


	printf("Creacion canal....\n");
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
        strcpy(IP,inet_ntoa(connect_clien.sin_addr)); 
        printf("IP %s\n", IP );
        if( des_canal == -1)
        {
            printf("Error al crear el canal");
            exit(1);
        }   
        pthread_create(&hilos, NULL, &hilo0, (void*)des_canal);	
        
             
        //if(strcmp(IP,IP_otroM) != 0) // ULtima ip que recibe diferente al otro master
        if(puerto == ntohs(connect_clien.sin_port))
        {
        	printf("Agregar IP\n");
        	
        	RecibirMensaje(des_canal,buffer,512);
        	//Reservar 1+ memoria Dinamica para Estructura
        	//strcpy(x.IP, IP)						MandarIP
        	//strcpy(x.Archivos, buffer)			MadarArchuvos
        	pthread_create(&hilos, NULL, &hilo0, (void*)des_canal);	
        }
        else
        {

        	pthread_create(&hilos, NULL, &hilo1, (void*)des_canal);
        	//int a = htons(connect_clien.sin_port);
        	int a = ntohs(ntohs(connect_clien.sin_port));
        	printf("Puerto %d otro puerto %d\n", ntohs(htons(atoi(argv[3]))), a);
        }
                     
    }
}


void *hilo0 (void *arg) // Atiende a todos los clientes p2p
{
	int des_canal = (int) arg;
	char buffer[512];
	RecibirMensaje(des_canal,buffer,512);  // primer mensaje (CadenaArchivos)
	printf("Recibiendo %s\n",buffer);

}

void *hilo1 (void *arg) // comunicaciones master a master Server
{
	int des_canal = (int) arg;
	char buffer[512];
	RecibirMensaje(des_canal,buffer,512);
	printf("Recibiendo %s\n",buffer);

}


void *hilo2 (void *arg) // comunicaciones master a master Cliente
{
	char buffer[512];
	strcpy(buffer,"Hola");
	int des_canal = (int) arg;
	EnviarMensaje(sock2,buffer,strlen(buffer));
	printf("Enviando Hola\n");

}

int Analizar(char *campo, char *palabra)
{

	int i = 0;
	char temp[40];
	char temchar[2];
	temchar[1] = '\0';
	strcpy(temp,'\0')
	while(1)
	{
		if (campo[i]='\0')
		{
			return 0;
		}
		else if(campo[i]=',')
		{
			if(strcmp(campo, palabra))
			{
				return 1;
			}
			strcpy(temp, '\0');
		}
		else
		{
			temchar[0] = campo[i];
			strcat(temp, temchar);
		}
	}
}
