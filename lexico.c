#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#define NCMD 10

int i, j, k;
//Cadena leida de teclado y su tamaño
char * inshell;
size_t len;
//Cadenas temporales para analizar el comando
char * tmpipe;      //Cadena temporal
char * strpipe[10]; //Almacena los comandos unidos por pipes
int numcmds;        //Num de comandos guardados
char ** apcmd;      //Apuntador iterador

int iless, igreat;  //Index de '<' y '>' en la cadena
char * busqueda;
char * tmpcmd;

char * tmparams;


//Struct donde se almacena lo que se entregara a exec
typedef struct com com;
struct com
{
    char * comando;
    char * parametros;
    FILE * input;
    FILE * output;
    char outmode;
};
//Se pueden almacenar hasta 10 comandos
struct com comandos[NCMD];

int main()
{
    do
    {
        //Se le la linea del usuario
        getline(&inshell, &len, stdin);
        //Remueve el salto de linea
        inshell[strcspn(inshell, "\n")] = 0; 
        tmpipe = strdup(inshell);
        //Separa los comandos basado en los pipes
        numcmds =0;
        for(apcmd = strpipe; (*apcmd = strsep(&tmpipe, "|")) != NULL; )
        {
            if(**apcmd  != '\0')
            { 
                numcmds++;
                if(++apcmd>= &strpipe[NCMD]) { break; }
            }
        }
        //Extrae de cada comando sus redireccionamientos

        for(k=0;k<numcmds;k++)
        {
            printf("Comando %i\n", k+1);
            igreat = 0; iless = 0; j=0;
            //Obtiene el indice de <
            tmpipe = strchr(strpipe[k], '<');
            if(tmpipe != NULL) { iless  = (int)(tmpipe - inshell); }
            else { iless = 0;}
            //Obtiene el indice de >
            tmpipe = strchr(strpipe[k], '>');
            if(tmpipe!= NULL) { igreat = (int)(tmpipe - inshell); }
            else { igreat = 0;}
            // Se manejan los casos de < y >
            if(igreat < iless)
            { 
                busqueda = strdup(">");
                j = 1;
            }
            else if(iless < igreat)
            { 
                busqueda = strdup("<");
                j = -1;
            }

            //Separa los archivos de entrada y salida y los asigna en coms
            if(j != 0)
            {
                tmpipe = strdup(strpipe[k]);
                tmpcmd = strsep(&tmpipe, busqueda);
                tmparams = strdup(tmpcmd);
                if(j == 1)
                { 
                    busqueda = strdup("<"); 
                    tmpcmd = strsep(&tmpipe, busqueda);
                    printf("Abriendo archivo para input:%s\n", tmpcmd);
                    comandos[k].input = fopen(tmpcmd, "r");
                    tmpcmd = strsep(&tmpipe, busqueda);
                    printf("Abriendo archivo para output:%s\n", tmpcmd);
                    comandos[k].output = fopen(tmpcmd, "w");
                }
                else
                { 
                    busqueda = strdup(">"); 
                    tmpcmd = strsep(&tmpipe, busqueda);
                    printf("Abriendo archivo para output:%s\n", tmpcmd);
                    comandos[k].output = fopen(tmpcmd, "w");
                    tmpcmd = strsep(&tmpipe, busqueda);
                    printf("Abriendo archivo para input:%s\n", tmpcmd);
                    comandos[k].input = fopen(tmpcmd, "r");
                }
            }
            //tmpipe = strdup(tmparams);
            do
            {
                tmpcmd = strsep(&tmparams, " ");
                comandos[k].comando = strdup(tmpcmd);
                if(tmparams != NULL)
                    comandos[k].parametros = strdup(tmparams);
            }while(strcmp(tmpcmd, "\0") == 0);
            //if(tmpcmd == NULL) tmpcmd = strdup(tmpipe);
            printf("Comando ingresado: %s\n",tmpcmd);
            printf("Parametros del comando: %s\n",tmparams);
        }

    } while(strcmp(comandos[0].comando, "exit") != 0);

    

    return 0;
}
        
        
