/* 
 * File:   main.c
 * Author: Jesús Pérez, Alejandro Niño y Jorge Valero
 *
 * Created on 25 de diciembre de 2015, 17:47
 */

#include <stdio.h>
#include <stdlib.h>
struct proceso
{
    char name;
    int Tejecucion;
    int Tllegada;
    int Tpermanencia;
    int Tespera;
    int Tacceso;
    int accedido; //0=false, 1=true
};

struct cpu
{
    int Tfuncionamiento;
    int quantum;
    struct proceso enprocesador;
};

struct proceso zero;
int numprocesos=0;
void reordenar(struct proceso cola[],int pos);
void insertarCola(struct proceso cola[],struct proceso proces,int pos);
void SJF(struct proceso proces[],struct cpu cpu1, int numproces,char argv[]);
void RR(struct proceso proces[],struct cpu cpus, int numproces,char argv[]);

/*
 * 
 */
int main(int argc, char** argv) {
    if(argc==2)         //Si se le pasa un argumento, comienza la ejecución
    {
    FILE *fp;           //Se crean las variables necesarias para realizar el tratamiento del archivo
    char *ejercicioRR = argv[1];

    fp = fopen(ejercicioRR, "r"); // Se abre el archivo pasado como parámetro
    
    if (fp == NULL) {           //Si el archivo indicado no se encuentra, se para la ejecución
        printf("Error al abrir el fichero \n");
        return EXIT_FAILURE;
    }
    
    zero.name='0';      //Proceso nulo (se explica su utilidad más adelante)
    zero.Tejecucion=0;
    int i=0;            //Variable auxiliar para usar en las sentencias for
    
    struct cpu cpu1;    //Se define el procesador
    cpu1.Tfuncionamiento=0;
    cpu1.quantum;

    
    char ch;            //Variables auxiliares utilizadas en la lectura del fichero
    char buffer[6];
    
    while ((ch = fgetc(fp)) != EOF)     //Contamos el numero de procesos
    {
      if (ch == '\n') 
         numprocesos++;
    }
    
    struct proceso procesos[numprocesos];   //Creamos un array de procesos a partir del numero de procesos que se ha encontrado en el fichero
    rewind(fp);                             //Volvemos a poner el puntero para leer el fichero al inicio
    
    if((fscanf(fp, "%d\n", &cpu1.quantum)) == 1){   //Evaluamos si está el quantum
            }
            else{
                printf("Error, no aparece el quantum");
                return EXIT_FAILURE;
            }
while(!feof (fp)){
        if((fscanf(fp,"\n%c\t",&procesos[i].name)==1)&&(procesos[i].name>64))
        {
            if(fscanf(fp,"\t%d|\t",&procesos[i].Tllegada)==1)
            {
                if(fscanf(fp,"\t%d\n",&procesos[i].Tejecucion)==1)
                {
                    procesos[i].Tacceso=0;
                    procesos[i].Tespera=0;
                    procesos[i].accedido=0;
                    procesos[i].Tpermanencia=0;
                    i++;
                }else{
                    printf("Error, falta el tiempo de ejecucion del proceso en la linea %d\n",i);
                    return EXIT_FAILURE;
                }
            }else{
                printf("Error, falta el tiempo de llegada del proceso en la linea %d\n",i);
                return EXIT_FAILURE;
            }
        }else{
            printf("Error, falta el nombre del proceso en la linea %d\n",i);
            return EXIT_FAILURE;
        }
        }      
    fclose(fp);         //Cerramos el archivo leido
    //Se imprimen los procesos del array para ver que se han definido correctamente
    for(i=0;i<numprocesos;i++)
    {
        printf("Proceso: %c %d %d\n",procesos[i].name,procesos[i].Tllegada,procesos[i].Tejecucion);
    }
    printf("\n\n");
    
    struct proceso procesos2[numprocesos]; //Clonamos el array de procesos para evitar fallos al ejecutar el SJF
    for(i=0;i<numprocesos;i++)
    {
        procesos2[i]=procesos[i];
    }
    
    RR(procesos,cpu1,numprocesos,argv[1]);      //Ejecutamos el algoritmo RR
    printf("----------------------------------\n\n");
    cpu1.Tfuncionamiento=0;                     //Reiniciamos el estado de la cpu
    SJF(procesos2,cpu1,numprocesos,argv[1]);    //Ejecutamos el algoritmo SJF
    }else{
        printf("Numero de parametros inválido. Por favor, pase un fichero como parámetro\n");
        return EXIT_FAILURE;
    }
    return (EXIT_SUCCESS);
}

void RR(struct proceso proces[],struct cpu cpu1, int numproces, char argv[])
{   int i=0;                            //Variable auxiliar
    int contador=0;                     //Indicará el tiempo de ejecución en cada instante, para cada proceso
    struct proceso salida[numprocesos]; //Array que contiene los procesos con sus datos finales
    int possalida=0;                    //Numero que controla donde se va a añadir el siguiente elemento en el array "salida"
    struct proceso cola[numprocesos];   //Array que corresponde a la cola de procesos, que se usa cuando el procesador entra en fuencionamiento
    int aejecutar=numprocesos;          //Numero que indica el numero de procesos restantes
    int quantumrestante=0;              //Numero que indica cuanto quantum le queda a un proceso al que se le ha asignado el procesador
    int posicioncola=0;                 //Numero que controla donde se va a añadir el siguiente elemento de la cola
    cpu1.enprocesador=zero;         //Proceso por defecto que aparece cuando ningún proceso se esta ejecutando
    int tlibre=0;                   //Numero de instantes que el procesador se encuentra libre a lo largo de la ejecucion
    
    FILE *fp;                       
    fp = fopen ( "RR.txt", "w" );   //Creamos un archivo para escribir en él (si está ya creado se sobreescribe) 
    
    printf("Resultado de la ejecución del algoritmo RR para el archivo %s\n\n",argv);
    fprintf(fp, "Resultado de la ejecución del algoritmo RR para el archivo %s\n\n",argv);
    fprintf(fp, "Nota: si no hay ningun proceso en el procesador se representara con 0 para el nombre y para el tiempo de ejecucion restante\n\n",argv);
    while(aejecutar!=0) //Mientras que el número de procesos activos sea distinto de 0
    {
        fprintf(fp,"???????????????????????????????????????????????????????\n");
        fprintf(fp,"Instante nº %d",cpu1.Tfuncionamiento);
        printf("Instante nº %d",cpu1.Tfuncionamiento);
        for(i=0;i<numprocesos;i++) //Comprobamos los procesos que llegan en este instante
        {
            if(proces[i].Tllegada==cpu1.Tfuncionamiento) //Si un proceso llega en este instante
            {
                if(cpu1.enprocesador.name=='0') //Si el procesador no se encuentra ocupado
                {
                    if(proces[i].Tejecucion<=cpu1.quantum)   //Asignamos el quantum correspondiente e introducimos el proceso en el procesador
                    {
                       quantumrestante=proces[i].Tejecucion;
                       contador=proces[0].Tejecucion;
                       proces[i].Tejecucion=0;
                    }else{
                       quantumrestante=cpu1.quantum;
                       contador=proces[0].Tejecucion;
                       proces[i].Tejecucion=proces[i].Tejecucion-cpu1.quantum;
                    }
                       cpu1.enprocesador=proces[i];
                       cpu1.enprocesador.accedido=1;
                }
                else    //Si el procesador se encuentra ocupado, el proceso entra en la cola
                {
                    
                    cola[posicioncola]=proces[i];
                    posicioncola++;
                }
            }
        }
                
        if((quantumrestante==0)&&(cpu1.enprocesador.name!='0'))    //Se acaba el quantum para un proceso
        {
            if(cpu1.enprocesador.Tejecucion==0) //El proceso finaliza totalmente
            {
                salida[possalida]=cpu1.enprocesador;
                possalida++;
                aejecutar--;
            }else{  //Si su ejecución no ha acabado, entra en la cola
                if(cola[posicioncola-1].Tllegada==cpu1.Tfuncionamiento){
                    struct proceso aux;
                    aux=cola[posicioncola-1];
                    cola[posicioncola-1]=cpu1.enprocesador;
                    cola[posicioncola]=aux;
                    posicioncola++;
                }else{
                cola[posicioncola]=cpu1.enprocesador;
                posicioncola++;
            }
            }
            
            if(posicioncola!=0) //Entra el proceso que se encuentra en la cabeza de la cola
            {
                if(cola[0].Tejecucion<=cpu1.quantum)   //Asignamos el quantum correspondiente e introducimos el proceso en el procesador
                    {
                       quantumrestante=cola[0].Tejecucion;
                       contador=cola[0].Tejecucion;
                       cola[0].Tejecucion=0;
                    }else{
                       quantumrestante=cpu1.quantum;
                       contador=cola[0].Tejecucion;
                       cola[0].Tejecucion=cola[0].Tejecucion-cpu1.quantum;
                    }
                cpu1.enprocesador=cola[0];
                cpu1.enprocesador.accedido=1;
                posicioncola--;
                for(i=0;i<posicioncola;i++) //Reordenamos la cola
                {
                    cola[i]=cola[i+1];
                }
            }else{
                cpu1.enprocesador=zero;
            }
        }
        //Se incrementa el tiempo de permanencia para el proceso en el procesador y los procesos en cola
        if(cpu1.enprocesador.name!='0')
        {
            cpu1.enprocesador.Tpermanencia++;
            for(i=0;i<posicioncola;i++)
                {
                    cola[i].Tpermanencia++;
                }
        }else{
            tlibre++;
        }
        //Se incrementa el tiempo de espera en los procesos de la cola
        for(i=0;i<posicioncola;i++)
            {
                    cola[i].Tespera++;
            }
        //Se incrementa el tiempo de respuesta en los procesos de la cola que no han accedido todavia al procesador
        for(i=0;i<posicioncola;i++)
            {
            if(cola[i].accedido==0)
                    cola[i].Tacceso++;
            }
        if(cpu1.enprocesador.name!='0'){
            contador--;
        }
        fprintf(fp, "\nProceso en el procesador y su tiempo de ejecucion restante: %c %d\nNumero de procesos a ejecutar: %d\nNumero de procesos en cola: %d \n\n",cpu1.enprocesador.name,contador,aejecutar,posicioncola);
        printf("\nProceso en el procesador y su tiempo de ejecucion restante: %c %d\nNumero de procesos a ejecutar: %d\nNumero de procesos en cola: %d \n\n",cpu1.enprocesador.name,contador,aejecutar,posicioncola);
        fprintf(fp, "Estado de la cola en el instante nº %d : \n",cpu1.Tfuncionamiento);
        printf("Estado de la cola en el instante nº %d : \n",cpu1.Tfuncionamiento);
        for(i=0; i<posicioncola;i++)
        {
            fprintf(fp, "%c %d %d \n",cola[i].name,cola[i].Tllegada,cola[i].Tejecucion);
            printf("%c %d %d \n",cola[i].name,cola[i].Tllegada,cola[i].Tejecucion);
        }
        fprintf(fp,"//////////////////////////////////////////////////////////////\n\n\n");
        printf("//////////////////////////////////////////////////////////////\n\n\n");
        cpu1.Tfuncionamiento++;
        if(quantumrestante>0)
        quantumrestante--;
    }
    fprintf(fp,"????????????????????????????????????????????????????????\n");
    float porcentaje=((cpu1.Tfuncionamiento-tlibre+1)*100)/cpu1.Tfuncionamiento;                //Calculamos el porcentaje de funcionamiento de la CPU
    printf("Porcentaje de utilizacion de la CPU: %f",porcentaje);
    fprintf(fp,"Porcentaje de utilizacion de la CPU: %f",porcentaje);
    fprintf(fp,"\nDatos obtenidos para cada uno de los procesos al finalizar la ejecucion del RR:\n\n");
    printf("\nFin del RR. Datos obtenidos:\n\n");
    for(i=0;i<numprocesos;i++)          //Imprimimos los datos finales de los procesos
    {
        fprintf(fp,"Proceso %c :\n Tiempo de permanencia: %d\n Tiempo de espera: %d\n Tiempo de acceso: %d\n\n",salida[i].name,salida[i].Tpermanencia,salida[i].Tespera,salida[i].Tacceso);
        printf("Proceso %c :\n Tiempo de permanencia: %d\n Tiempo de espera: %d\n Tiempo de acceso: %d\n\n",salida[i].name,salida[i].Tpermanencia,salida[i].Tespera,salida[i].Tacceso);
    }
    fclose (fp);
}

void SJF(struct proceso proces[],struct cpu cpu1, int numproces, char argv[]){
    int contador=0; //Indicará el tiempo de ejecución en cada instante, para cada proceso
    struct proceso salida[numprocesos]; //Array que contiene los procesos con sus datos finales
    int possalida=0;                    //Numero que controla donde se va a añadir el siguiente elemento en el array "salida"
    struct proceso cola[numprocesos];   //Array que corresponde a la cola de procesos, que se usa cuando el procesador entra en fuencionamiento
    int aejecutar=numprocesos;          //Numero que indica el numero de procesos restantes
    int tiemporestante=0;              //Numero que indica cuanto tiempo le queda a un proceso al que se le ha asignado el procesador
    int posicioncola=0;                 //Numero que controla donde se va a añadir el siguiente elemento de la cola
    cpu1.enprocesador=zero;         //Proceso por defecto que aparece cuando ningún proceso se esta ejecutando
    int tlibre=0;                   //Numero de instantes que el procesador se encuentra libre a lo largo de la ejecucion
    int i=0;
    
    FILE *fp;
    fp = fopen ( "SJF.txt", "w" );      //Creamos un archivo para escribir en él (si está ya creado se sobreescribe) 
    
    fprintf(fp,"Resultado de la ejecución del algoritmo SJF para el archivo %s\n\n",argv);
    fprintf(fp, "Nota: si no hay ningun proceso en el procesador se representara con 0 para el nombre y para el tiempo de ejecucion restante\n\n",argv);
    printf("Resultado de la ejecución del algoritmo SJF para el archivo %s\n\n",argv);
    while(aejecutar!=0) //Mientras que el número de procesos activos sea distinto de 0
    {   fprintf(fp,"?????????????????????????????????????????????????\n");
        fprintf(fp,"Instante nº %d",cpu1.Tfuncionamiento);
        printf("Instante nº %d",cpu1.Tfuncionamiento);
        for(i=0;i<numprocesos;i++) //Comprobamos los procesos que llegan en este instante
        {
            if(proces[i].Tllegada==cpu1.Tfuncionamiento) //Si un proceso llega en este instante
            {
                if(cpu1.enprocesador.name=='0') //Si el procesador no se encuentra ocupado
                {
                    
                       tiemporestante=proces[i].Tejecucion;
                       contador=proces[0].Tejecucion;
                       proces[i].Tejecucion=0;
                       cpu1.enprocesador=proces[i];
                       cpu1.enprocesador.accedido=1;
                }
                else    //Si el procesador se encuentra ocupado, el proceso entra en la cola
                {
                    insertarCola(cola,proces[i],posicioncola);  //se inserta en orden en la cola
                    posicioncola++; //se aumenta el puntero de la cola
                    reordenar(cola,posicioncola); //Se reordena el array
                }
            }
        }
                
        if((contador==0)&&(cpu1.enprocesador.name!='0'))    //El proceso finaliza totalmente
        {
            if(contador==0) 
            {
                salida[possalida]=cpu1.enprocesador;
                possalida++;
                aejecutar--;
            }
            
            if(posicioncola!=0) //Entra el proceso que se encuentra en la cabeza de la cola
            {
                contador=cola[0].Tejecucion; //Se le asigna el tiempo de ejecución a un contador que irá decrementando progresivamente hasta que finalice.
                cola[0].Tejecucion=0;   //su tiempo de ejecución se cambia a cero pues, al ser no expulsivo, se finalizará necesariamente, es decir no volvera a la cola de procesos listos.
                cpu1.enprocesador=cola[0];  // se le asigna el procesador
                cpu1.enprocesador.accedido=1;
                posicioncola--;
                for(i=0;i<posicioncola;i++) //Reordenamos la cola
                {
                    cola[i]=cola[i+1];
                }
            }else{
                cpu1.enprocesador=zero;
            }
        }
        //Se incrementa el tiempo de permanencia para el proceso en el procesador y los procesos en cola
        if(cpu1.enprocesador.name!='0')
        {
            cpu1.enprocesador.Tpermanencia++;
            for(i=0;i<posicioncola;i++)
                {
                    cola[i].Tpermanencia++;
                }
        }else{
            tlibre++;
        }
        //Se incrementa el tiempo de espera en los procesos de la cola
        for(i=0;i<posicioncola;i++)
            {
                    cola[i].Tespera++;
            }
        //Se incrementa el tiempo de respuesta en los procesos de la cola que no han accedido todavia al procesador
        for(i=0;i<posicioncola;i++)
            {
            if(cola[i].accedido==0)
                    cola[i].Tacceso++;
            }
        if(cpu1.enprocesador.name!='0'){
            contador--;
        }
        fprintf(fp,"\nProceso en el procesador y su tiempo de ejecucion restante: %c %d\nNumero de procesos a ejecutar: %d\nNumero de procesos en cola: %d \n\n",cpu1.enprocesador.name,contador,aejecutar,posicioncola);
        fprintf(fp,"Estado de la cola en el instante nº %d : \n",cpu1.Tfuncionamiento);
        printf("\nProceso en el procesador y su tiempo de ejecucion restante: %c %d\nNumero de procesos a ejecutar: %d\nNumero de procesos en cola: %d \n\n",cpu1.enprocesador.name,contador,aejecutar,posicioncola);
        printf("Estado de la cola en el instante nº %d : \n",cpu1.Tfuncionamiento);
        for(i=0; i<posicioncola;i++)
        {
            fprintf(fp,"%c %d %d \n",cola[i].name,cola[i].Tllegada,cola[i].Tejecucion);
            printf("%c %d %d \n",cola[i].name,cola[i].Tllegada,cola[i].Tejecucion);
        }
        fprintf(fp,"//////////////////////////////////////////////////////////////\n\n\n");
        printf("//////////////////////////////////////////////////////////////\n\n\n");
        cpu1.Tfuncionamiento++;
        if(tiemporestante>0)
        tiemporestante--;
    }
    fprintf(fp,"?????????????????????????????????????????????????\n");
    float porcentaje=((cpu1.Tfuncionamiento-tlibre+1)*100)/cpu1.Tfuncionamiento;
    printf("Porcentaje de utilizacion de la CPU: %f",porcentaje);
    fprintf(fp,"Porcentaje de utilizacion de la CPU: %f",porcentaje);
    fprintf(fp,"\nDatos obtenidos para cada uno de los procesos al finalizar la ejecucion del SJF:\n\n");
    printf("\nFin del SJF. Datos obtenidos:\n\n");
    for(i=0;i<numprocesos;i++)
    {
        fprintf(fp,"Proceso %c :\n Tiempo de permanencia: %d\n Tiempo de espera: %d\n Tiempo de acceso: %d\n\n",salida[i].name,salida[i].Tpermanencia,salida[i].Tespera,salida[i].Tacceso);
        printf("Proceso %c :\n Tiempo de permanencia: %d\n Tiempo de espera: %d\n Tiempo de acceso: %d\n\n",salida[i].name,salida[i].Tpermanencia,salida[i].Tespera,salida[i].Tacceso);
    }
    fclose (fp);
}

void insertarCola(struct proceso cola[],struct proceso proces,int pos){ //Inserta un elemento en la cola de forma ordenada para el algoritmo SJF
    int i=0;
    struct proceso aux;
    int ordenado=0;
    if(pos==0){
        cola[0]=proces;
    }else{
    
    for(i=0;i<pos;i++){
        
        if((cola[i].Tejecucion>proces.Tejecucion)&&ordenado==0){
            aux=cola[i];
            cola[i]=proces;
            ordenado=1;
        }
        
    }
    if(ordenado==0){
       cola[pos]=proces; 
    }else{
        cola[pos]=aux;
    }
    }
}

void reordenar(struct proceso cola[],int pos){  //Reordena la cola de menor a mayor tiempo de ejecución para el algoritmo SJF
    int i=0;
    struct proceso temp;
    for(i=0;i<pos-1;i++){ 
        int j;
        for(j=i+1;j<pos;j++){
            if(cola[j].Tejecucion<cola[i].Tejecucion){
                temp=cola[j];
                cola[j]=cola[i];
                cola[i]=temp;
            }
        }
    }
}