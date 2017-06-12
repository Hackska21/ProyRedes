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
           		EnviarMensaje(sockt,ent->d_name,strlen(ent->d_name));
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


int DescargarArchivo(int sockts*,int NoSockts,char* NombreAr,char* Directorio)
{
	int i=0;
	char BufferEnviar[50],BufferRecibir[50];
	

	while(1)//Pediremos Parte
	{
		EnviarMensaje(sockts[i],"PedirParte",50);
		EnviarMensaje(sockts[i],NombreAr,50);
		RecibirMensaje()









	if(i==NoSockts)
		i=0;
	else
		i++;
	}
}






int EnviarParte(int sockt,char* NombreAr, int NoParte)
{
	FILE* Archivo;
	char Buff[TamPaquetes];
	//Abrirmos Archivo
	Archivo=fopen(NombreAr,"rb");
	long Desplazamiento=NoParte*TamPaquetes;

	//Nos movemos
	fseek(Archivo,Desplazamiento,SEEK_END);

	//Leemos
	int i=fread(Buff,1,TamPaquetes,Archivo);

	//Mandamos
	EnviarMensaje(sockt,Buff,i);

	//Terminamos

	//Cerramos
	fclose(Archivo);


}

int FunEnviarParte(int sockt,char* Directorio)
{
	//Debe llegar el nombre del Archivo
	char BufferEnviar[50],BufferRecibir[50];
	RecibirMensaje(sockt,BufferRecibir,50);
	//Verificamos que esta el archivo (En teoria no debe existir nunca problema)
	char* Nombre=BuscaNombre (Directorio,BufferRecibir);
	if(BuscaNombre!=NULL)
	{
		EnviarMensaje(sockt,"Parte?",50);
		RecibirMensaje(sockt,BufferRecibir,50);
		int parte=atoi(BufferRecibir);
		EnviarParte(sockt,Nombre,parte);

	}
	else
	{
		EnviarMensaje(sockt,"EnviandoRehusado",50);
	}
}