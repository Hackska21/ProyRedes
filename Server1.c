

struct Dir
{
  char* puerto;
  char* ip;
  char parte;

};


void Hilo(void* Arg){
 
  int x2=(int)Arg,TAM; 
  //printf("Mi canal es: %d :v ",x2);
      
  //zona de comunicaciones

  //int TAM;
  char BufferEnviar[50],BufferRecibir[50];

  while(1){
  getchar();
  printf("Introdusca un mensaje\n");
  scanf("%[^\n]",BufferEnviar);

  
  EnviarMensaje(x2,BufferEnviar,50);
  //RecibirMensaje(x2,BufferRecibir,50); 
  //printf("recibi %s\n",BufferRecibir );
  }
  close(x2);
       
    
}

void HiloRecibir(void* Arg)
{
  printf("Hilo");
  struct Dir ServInf=*((struct Dir*)Arg);  
  int server2=InitSockClien(ServInf.puerto,"0.0.0.0");
  char BufferEnviar[50],BufferRecibir[50];
  
  while(1)
  {
  RecibirMensaje(server2,BufferRecibir,50);
  printf("recibi %s\n",BufferRecibir );
  }

  
}



main(int argc, char *argv[])
{

	
	if(argc<3)
    {
     	printf("faltan argumentos <puerto> <PuertoDestino>\n");
     	exit(1);
    }
  int sockt=InitSockServ(argv[1]);  
  //system("./prueba.exec");
  //execl("/bin/ls","ls","-1","/usr",0);

  pthread_t Des_HiloRecibir;
  struct Dir ServInf;
  ServInf.puerto=argv[2];
  
  pthread_create (&Des_HiloRecibir, NULL,HiloRecibir  ,(void*)&ServInf);

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
	
	close(sockt); 
                 
}