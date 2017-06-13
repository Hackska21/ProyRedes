long fileSize(char *fname)
{
  int fich;
  long ftam=0;
  char c;
  fich=open(fname,O_RDONLY);
  //fich=fopen(fname, "r");
  //if (fich)
  while(read(fich,&c,sizeof(c)!=0))
    {
      //fseek(fich, 0L, SEEK_END);
      //ftam=ftell(fich);
      //fclose(fich);
      ftam++;
    }
  //else
  //  printf("ERRNO:\n"); //%d - %s\n", errno, strerror(errno));
  close(fich);
  return ftam;
}
unsigned char statFileType(char *fname)
{
  struct stat sdata;

  /* Intentamos el stat() si no funciona, devolvemos tipo desconocido */
  if (stat(fname, &sdata)==-1)
    {
      return DT_UNKNOWN;
    }

  switch (sdata.st_mode & S_IFMT) 
    {
    case S_IFBLK:  return DT_BLK;
    case S_IFCHR:  return DT_CHR;
    case S_IFDIR:  return DT_DIR;
    case S_IFIFO:  return DT_FIFO;
    case S_IFLNK:  return DT_LNK;
    case S_IFREG:  return DT_REG;
    case S_IFSOCK: return DT_SOCK;
    default:       return DT_UNKNOWN;
    }
}

char *getFullName(char *ruta, struct dirent *ent)
{
  char *nombrecompleto;
  int tmp;

  tmp=strlen(ruta);
  nombrecompleto=malloc(tmp+strlen(ent->d_name)+2); /* Sumamos 2, por el \0 y la barra de directorios (/) no sabemos si falta */
  if (ruta[tmp-1]=='/')
    sprintf(nombrecompleto,"%s%s", ruta, ent->d_name);
  else
    sprintf(nombrecompleto,"%s/%s", ruta, ent->d_name);
  
  return nombrecompleto;
}

unsigned char getFileType(char *nombre, struct dirent *ent)
{
  unsigned char tipo;

  tipo=ent->d_type;
  if (tipo==DT_UNKNOWN)
    {
      tipo=statFileType(nombre);
    }

  return tipo;
}


char* BuscaNombre (char *ruta,char* NombreArchivo)
{

  //Con un puntero a DIR abriremos el directorio
    DIR *dir;

  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
  struct dirent *ent;
  unsigned char tipo;       /* Tipo: fichero /directorio/enlace/etc */
  char *nombrecompleto;     /* Nombre completo del fichero */
  char *posstr;         /* Cadena usada para posicionarnos horizontalmente */

    //Abrimos directorio
    dir = opendir (ruta);
  /* Miramos que no haya error */
  if (dir == NULL) 
  error("No puedo abrir el directorio");
  
    //printf("Archivos en Carpeta!\n\n");
  /* Leyendo uno a uno todos los archivos que hay */
  while ((ent = readdir (dir)) != NULL) 
  {
  
  /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
  if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
  {
    nombrecompleto=getFullName(ruta, ent);
    tipo=getFileType(nombrecompleto, ent);
    
    
      if (tipo==DT_REG)
      {
        
        //printf("%d) %s\ttamaño %.3lf mb\ttipo %d\n",NumeroArchivo,ent->d_name,((double)fileSize(ent->d_name))/1000,tipo);
        
        if(strcmp(NombreArchivo,ent->d_name)==0)
        {
          
          return nombrecompleto;
        }
        
      }

      else if (tipo==DT_DIR)
      {

      }
      free(nombrecompleto);
  }
  }
  closedir (dir);
  return NULL;
}





char*  BuscaArchivos(char *ruta,int numero)
{
  /* Con un puntero a DIR abriremos el directorio */
  DIR *dir;
  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
  struct dirent *ent;
  unsigned numfiles=0;          /* Ficheros en el directorio actual */
  unsigned char tipo;       /* Tipo: fichero /directorio/enlace/etc */
  char *nombrecompleto;     /* Nombre completo del fichero */
  char *posstr;         /* Cadena usada para posicionarnos horizontalmente */
  dir = opendir (ruta);

  /* Miramos que no haya error */
  if (dir == NULL) 
    error("No puedo abrir el directorio");
  
  while ((ent = readdir (dir)) != NULL) 
    {
      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
    {
      nombrecompleto=getFullName(ruta, ent);
      tipo=getFileType(nombrecompleto, ent);
      if (tipo==DT_REG)
        {
          ++numfiles;
          if(numero==numfiles)
          {
          	printf("Lo encontre en %s\n",nombrecompleto );
          	return ent->d_name;
          }
        }
      else if (tipo==DT_DIR)
        {
          
        }
      free(nombrecompleto);
    }
    }
  closedir (dir);
  return NULL;
  
}
