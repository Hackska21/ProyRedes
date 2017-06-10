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
  
  

  EnviarMensaje(sockt,"Listado_Completo",strlen(ent->d_name));
  
	



}


