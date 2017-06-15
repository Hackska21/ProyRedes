int* ConectarHosts(int NoHost ,struct Dir* Hosts);


int VerificarPrefijo (char * Cadena)//Sirve para encontrar Archivos A omitir
{
	int tam=strlen(Cadena);
	//if(strncmp (Cadena,"Err_" , 4)==0)
	if((Cadena[tam-4]=='_')&&(Cadena[tam-3]=='E')&&(Cadena[tam-2]=='r')&&(Cadena[tam-1]=='r'))
		return 1;
	
	if(strncmp (Cadena,"Rec_" , 4)==0)
		return 2;
	
	//_bor
	if((Cadena[tam-4]=='_')&&(Cadena[tam-3]=='b')&&(Cadena[tam-2]=='o')&&(Cadena[tam-1]=='r'))
		{
			printf("Entro\n");
			return 3;
			
		}
	if((Cadena[tam-4]=='_')&&(Cadena[tam-3]=='C')&&(Cadena[tam-2]=='o')&&(Cadena[tam-1]=='m'))
		{
			//printf("Entro\n");
			return 4;
			
		}
	return 0;// :v

}

//Envia Nombre de archivos 
//Termina enviando Listado_Completo
void ListarArchivos(int sockt,char* directorio)
{

	printf("Listando Archios de %s\n",directorio);
	/* Con un puntero a DIR abriremos el directorio */
  DIR *dir;
  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
  struct dirent *ent;
  unsigned numfiles=0;          /* Ficheros en el directorio actual */
  unsigned char tipo;       /* Tipo: fichero /directorio/enlace/etc */
  char *nombrecompleto;     /* Nombre completo del fichero */
  dir = opendir (directorio);
  
  /* Miramos que no haya error */

  if (dir == NULL) 
    error("No puedo abrir el directorio");
  
  while ((ent = readdir (dir)) != NULL) 
    {
      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
    {
      nombrecompleto=getFullName(directorio, ent);
      tipo=getFileType(nombrecompleto, ent);

      if (tipo==DT_REG)
        {
          ++numfiles;
          printf("Leyendo Archivo: %s\n",ent->d_name );
           if(VerificarPrefijo(ent->d_name)==0)//Si no se esta recibiendo
           {
           		EnviarMensaje(sockt,ent->d_name,100);
           }
          //sprintf(result,"%s %d) %s \n",result,numfiles,ent->d_name);


        }
      else if (tipo==DT_DIR)
        {

        }
      free(nombrecompleto);
    }
    }
  closedir (dir);
  
  

  EnviarMensaje(sockt,"Listado_Completo",50);
  
	



}

int FunPendirArchivo(int sockt,char* NombreAr,char* Directorio)
{
	
	int NoHost=0;
	int* Hosts;
	struct Dir* InfoHosts;
	char BufferEnviar[50],BufferRecibir[50];

	EnviarMensaje(sockt,"EnviarArchivo",50);
	EnviarMensaje(sockt,NombreAr,50);

	//PedirNoHost
	RecibirMensaje(sockt,BufferRecibir,50);
	printf("Pedi host%s\nfin de recibido\n",BufferRecibir );
	if(strcmp(BufferRecibir,"SinResultados")==0)
	{
		printf("Archivo %s no encontrado :'v\n",NombreAr);
		return 0;
	}

	NoHost=atoi(BufferRecibir);
	//NoHost;

	printf("Numero de host %d\nFin numero host\n",NoHost );

	//Redimensionamos la estructura
	InfoHosts=(struct Dir*)malloc(NoHost*sizeof(struct Dir));
	//Pedir Host eh IP al server principal
	
	int i=0;
	while(1)
	{
		RecibirMensaje(sockt,BufferRecibir,50);
		printf("%s\n",BufferRecibir );
		getchar();	
		if(strcmp(BufferRecibir,"TerminaenvioIP")==0)
		{
			printf("Salgo con %s %s  \n",InfoHosts[0].puerto,InfoHosts[0].ip);
			break;
		}
		//InfoHosts[i].ip=(char)malloc(sizeof(char)*50);
		//printf("Mamo?\n");
		strcpy(InfoHosts[i].ip,BufferRecibir);
		//printf("NO O:\n");
		//InfoHosts[i].ip=BufferRecibir;
		
		RecibirMensaje(sockt,BufferRecibir,50);
		printf("%s  \n",BufferRecibir );
		
		//InfoHosts[i].ip=(char)malloc(50);
		strcpy(InfoHosts[i].puerto,BufferRecibir);
		//InfoHosts[i].puerto=BufferRecibir;
		
		i++;
	}
	printf("Procedo A conectar :v \n");
	Hosts=ConectarHosts(NoHost,InfoHosts);
	printf("Conectado!\n");
	DescargarArchivo(Hosts,NoHost,NombreAr,Directorio);

	for(i=0;i<NoHost;i++)
	{
		EnviarMensaje(Hosts[i],"CerrarComuicacion",50);
		close(Hosts[i]);	
	}


	//N

}

int* ConectarHosts(int NoHost ,struct Dir* Hosts)//Conecta a los host Especificados y regresa los descriptores
{
	int* Sockets=(int)malloc(sizeof(int)*NoHost);
	int i;
	for (i = 0; i < NoHost; ++i)
	{
		printf("Conectando a %s %s \n",Hosts[i].puerto,Hosts[i].ip );
		Sockets[i]=InitSockClien(Hosts[i].puerto,Hosts[i].ip);
	}
	return Sockets;
}


int DescargarArchivo(int* sockts,int NoSockts,char* NombreAr,char* Directorio)
{
	int i=0;
	char BufferEnviar[50],BufferRecibir[50];
	int NoParte=0;
	char buffAr[TamPaquetes];
	char Nombre[50],NombreTmp[50];
	sprintf(Nombre,"%s/%s",Directorio,NombreAr);
	sprintf(NombreTmp,"%s/Rec_%s",Directorio,NombreAr);
	FILE* Archivo;
	int TamRec;
	Archivo=fopen(NombreTmp,"wb");


	while(1)//Pediremos Parte
	{
		EnviarMensaje(sockts[i],"PedirParte",50);
		EnviarMensaje(sockts[i],NombreAr,50);
		RecibirMensaje(sockts[i],BufferRecibir,50);
		if(strcmp(BufferRecibir,"Parte?")==0)
		{
			sprintf(BufferEnviar,"%d",NoParte);
			EnviarMensaje(sockts[i],BufferEnviar,50);
			//Aqui recibir Si ah acabado o no
			TamRec=RecibirMensaje(sockts[i],buffAr,TamPaquetes);
			//printf("Recibi: %s\n",buffAr);
			if(strcmp(buffAr,"ArchivoTerminado"))
			{
				fwrite(buffAr,1,TamRec,Archivo);
				NoParte++;
			}
			else
			{
				break;
			}

		}
		else
		{
			printf("Server no tiene archivo? error?\n");
		}

	if(i==NoSockts-1)
		i=0;
	else
		i++;
	}

	fclose(Archivo);

	if(rename(NombreTmp,Nombre)==0)
	{
		printf("Archivo renombrado\n");
	}
}





int EnviarParte(int sockt,char* NombreAr, int NoParte)
{
	FILE* Archivo;
	char Buff[TamPaquetes];
	int err;
	//Abrirmos Archivo
	Archivo=fopen(NombreAr,"rb");
	long Desplazamiento=NoParte*TamPaquetes;

	//Nos movemos
	err=fseek(Archivo,Desplazamiento,SEEK_SET);
	if(err!=0)
		printf("Mamo!\n");

	//Leemos
	int i=fread(Buff,1,TamPaquetes,Archivo);
	//printf("Enviare %d,\n%s\n",i,Buff );
	//Mandamos
	if(i)
	EnviarMensaje(sockt,Buff,i);
	else
	EnviarMensaje(sockt,"ArchivoTerminado",50);
	//Terminamos

	//Cerramos
	fclose(Archivo);
	//printf("Termine de enviar Parte\n");

}

int FunEnviarParte(int sockt,char* Directorio)
{
	//Debe llegar el nombre del Archivo
	char BufferEnviar[50],BufferRecibir[50];
	RecibirMensaje(sockt,BufferRecibir,50);
	//Verificamos que esta el archivo (En teoria no debe existir nunca problema)
	char* Nombre=BuscaNombre (Directorio,BufferRecibir);
	//printf("Voy a enviar %s\n",Nombre);
	if(BuscaNombre!=NULL)
	{
		EnviarMensaje(sockt,"Parte?",50);
		RecibirMensaje(sockt,BufferRecibir,50);
		int parte=atoi(BufferRecibir);
		//printf("Procedo a enviar parte %d\n", parte);
		EnviarParte(sockt,Nombre,parte);

	}
	else
	{
		EnviarMensaje(sockt,"EnviandoRehusado",50);
	}
}
