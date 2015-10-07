#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h> // Para usar isspace()
#define NCMD 10

int i, j, k;
char * tmp1, * tmp2;
//Para leer cadena del usuario
char * inshell; 
size_t len;

//Cadenas para identificar los comandos
char * cadena, * comando, * ejecutable, * parametros, * busq1, * busq2;
char * programas[NCMD];
char ** strptr;
int numprogramas, numIO, ultimo;
int igreat, iless;

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
        numprogramas = 0;
        //Se lee la linea del usuario
        getline(&inshell, &len, stdin);
        //Remueve el salto de linea
        inshell[strcspn(inshell, "\n")] = 0; 
        cadena = strdup(inshell);
        //Se verifica si tiene pipes la cadena
        if(strchr(cadena, '|')!=NULL) 
        {
            //Si tiene, se separa la cadena por pipes
            for(strptr = programas;(*strptr = strsep(&cadena, "|")) !=NULL;)
            {
                if(**strptr != '\0')
                {
                    numprogramas++;
                    if(++strptr>= &programas[NCMD]) { break; }
                }
            }
        }
        else
        {
            //Si no tiene, se guarda como unico programa
            numprogramas = 1;
            programas[0] = strdup(cadena);
        }
        //Se analiza cada programa que es conectado por un pipe
        for(i=0; i<numprogramas; i++)
        {
            //Prueba
            printf("\n\n");
            numIO = 0;
            //Se buscan < y > y, de encontrarse, se guardan sus indices
            tmp1 = strchr(programas[i], '<');
            tmp2 = strchr(programas[i], '>');
            if(tmp1!=NULL){
                iless = (int)(tmp1 - programas[i]);
                numIO++;
            }
            else { iless = 0; }
            if(tmp2!=NULL){
                igreat = (int)(tmp2 - programas[i]);
                numIO++;
            }
            else { igreat = 0; }
            //Pruebas
            printf("Indice de <:%i\n", iless);
            printf("Indice de >:%i\n", igreat);
            printf("numIO:%i\n", numIO);

            //Se parte el programa de acuerdo a el numero llaves
            //y se asignan respectivamente a input y output
            switch(numIO)
            {
                case 1:
                    tmp1 = strdup(programas[i]);
                    if(igreat == 0)
                    {
                        comando = strsep(&tmp1, "<");
                        //comandos[i].input = fopen(*programas[i], 'r');
                    }
                    else
                    {
                        comando = strsep(&tmp1, ">");
                        //comandos[i].output = fopen(*programas[i], 'r');
                    }
                    printf("archivo del programa:%s\n",tmp1);
                    break;
                case 2:
                    //Se busca cual es la llave derecha
                    ultimo = (igreat > iless) ? 2 : 1;
                    //Se prepara el orden de las busquedas en comando
                    if(ultimo==1){busq1 = strdup(">");busq2 = strdup("<");}
                    else{ busq1 = strdup("<"); busq2 = strdup(">"); }
                    //Se guardan las tres cadenas de acuerdo a su orden
                    comando = strsep(&programas[i], busq1);
                    if(ultimo==1)
                    {
                        tmp2 = strsep(&programas[i], busq2);
                        tmp1 = programas[i];
                    }
                    else
                    {
                        tmp1 = strsep(&programas[i], busq2);
                        tmp2 = programas[i];
                    }
                    //Se abren los archivos y se guardan sus apuntadores
                    //en comando
                    //comandos[i].input = fopen(tmp1, 'r');
                    //comandos[i].output = fopen(tmp2, 'w');
                    printf("input:%s\n",tmp1);
                    printf("output:%s\n",tmp2);
                    break;
                default:
                    comando = strdup(programas[i]);
                    break;
            }

            //Remueve espacios antes del comando
            while(isspace(*comando)) comando++;
            //Verifica si el comando aun tiene espacios
            if(strchr(comando, ' ')!=NULL) 
            {
                ejecutable = strsep(&comando, " ");
                parametros = strdup(comando);
            }
            else
            {
                ejecutable = strdup(comando);
            }

            //Pruebas
            printf("Programa %i:\n", i+1);
            printf("Ejecutable: %s\n", ejecutable);
            printf("Parametros: %s\n", parametros);
        }


        //Pruebas (borrar)
        printf("Cadena original:%s\n", inshell);
    } while(comandos[0].comando==NULL);
    //} while(strcmp(comandos[0].comando, "exit") != 0);

    

    return 0;
}
        
        
