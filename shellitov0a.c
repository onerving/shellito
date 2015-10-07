#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

//Funciones para hilos
void *bitacora(void *arg); //trabaja la bitacora 
void *scriptf(void *arg); //trabaja el script

//Funciones para hora y fecha
void obtenerhf(void); //obtencion
void imprimirhf(void *arch); //impresion

//Funciones para cambiar prompt
void * caritaf(void);
void * carital(void);
void * caritas(void);

char * inshell = NULL;
size_t len;
ssize_t longcad = 0;
char * carita;
char strhf[128];
int leido = 1;
int registrado = 1;
int ejecutado = 1;
int pends = 0;
int sends = 0;

int main(void)
{
    
    //Bienvenida a Shellito
    printf("Bienvenido!\n\t\tShellito iniciado con pid %d\n\n", getpid());
    obtenerhf();//hora y fecha en que se ingresa a Shellito
   
    //Prompt predeterminado del sistema
    carita=":3";

    //Captura de señales para cambiar prompt
    signal( 29, caritaf);
    signal( 30, carital);
    signal( 31, caritas);

    //Creacion del hilo que guardara el historial
    pthread_t delCapitan;
    pthread_create( &delCapitan, NULL, bitacora, NULL);

    pthread_t grabar;//creacion de un hilo aun no llamado 
    int sactivo = 0;//indicador de script aun no llamado

    //Ciclo infinito de Shellito
    int n = 0;
    while(n!=5)
    {
        //Ciclos de espera de guardado
        while(leido==0){} //para la bitacora
        if(sactivo==1) //para el script
        {
            while(registrado==0){} //comando
            while(ejecutado==0){} //salida estandar
        }

        //Prueba de script
        if(n==2)
        {
            pthread_create( &grabar, NULL, scriptf, NULL);
            sactivo = 1;
        }

        //Shellito para el usuario
        printf("%s ", carita);
        longcad = getline(&inshell, &len, stdin);
        n++; 

        //Prueba cierre de script
        if(n==4)
        {
            sends = 1;//Terminar script
            sactivo = 2;//Indicar que ya se termino script
        }

        //Indicadores de que se ingreso una nueva cadena
        leido=0;//indicador a bitacora
        registrado=0;//indicador a scrip (comando) 
        ejecutado=0;//indicador a script (salida estandar)
    }

    pends = 1;//Se indica que Shellito finalizo
    pthread_join(delCapitan, NULL);//Se espera el cierre del historial

    //Si al solicitar salir de Shellito sigue en uso script se termina
    if(sactivo == 1)
    {
        sends = 1;
        sactivo = 2;
    }
    //Si existio un script se espera
    else if(sactivo==2)
    {
        pthread_join(grabar, NULL);
    }
    
    return 0;
}

void *bitacora(void *arg)
{

    //Abriendo el historial
    FILE *bitCapitan;
    bitCapitan = fopen ("historial.txt", "a");
    imprimirhf(bitCapitan);
    
    //Ciclo para guardar cada comando utilizado
    while(pends == 0)
    {
        while(leido==1){}//Ciclo de espera para lectura
        fwrite(inshell, longcad, 1, bitCapitan);//Guardado
        leido=1;//Se indica que ya fue leido por la bitacora
    }

    fclose(bitCapitan);

    return NULL;
    
}


void *scriptf(void *arg)
{
    //Creando o sobre escribiendo el archivo grabadora con hora y fecha 
    FILE *grabadora;
    grabadora = fopen ("grabadora.txt", "w");
    imprimirhf(grabadora);

    //Variables para leer la salida
    char *lout = NULL;
    size_t tamlout = 0;
    ssize_t longout;
    
    //Ciclo para guardar en grabadora
    while(sends == 0)
    {    
        //Guardando cada comando llamado
        while(registrado==1){}//ciclo de espera para leer entrada
        fwrite(inshell, longcad, 1, grabadora);//guardado
        registrado=1;//se indica que ya fue leido por la bitacora

        //Guardando cada salida estandar
        while(ejecutado==1){}//ciclo de espera para leer salida
        while( (longout = getline(&lout, &tamlout, stdout)) != -1)
        {
            fwrite(lout,longout, 1, grabadora);
        }    
        ejecutado=1;
    }

    fclose(grabadora);

    return NULL;
}


void obtenerhf(void)
{
    //Obtención de hora y fecha
    time_t thf = time(NULL);
    struct tm *hflocal = localtime(&thf);
    strftime(strhf,128,"%d/%m/%y %H:%M:%S",hflocal);
}

void imprimirhf(void *arch)
{
    //Insertando en el la hora y fecha
    char * imphfb = "\n\n\n\t";
    char * imphfa = "\n\n";
    fwrite( imphfb, (int)sizeof(imphfb), 1, arch);
    fwrite( strhf, (int)sizeof(strhf), 1, arch);
    fwrite( imphfa, (int)sizeof(imphfa), 1, arch);
}


void * caritaf(void)
{
    carita="=D";
}


void * carital(void)
{
    carita=":P";
}

void * caritas(void)
{
    carita="O.o";
}

