


void ListarArchivos(int sockt)
{





}


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