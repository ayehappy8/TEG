#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

/*Nombres:
  - Sebastián Araus
  - José Velásquez*/

//variables globales
#define C_AUTOS 12
#define BUFFER_SIZE 256

//Recurso compartido representando el tunel
//tunel ED
int tunel_ED[2];
int tunel_DE[2];
//tunel ED
//tunel AC
int tunel_AC[2];
int tunel_CA[2];
//tunel AC
//tunel BC
int tunel_BC[2];
int tunel_CB[2];
//tunel BC
//Recurso compartido representando el tunel

//Monitor
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condicion;
    int ocupado;
    int contador;
} Monitor;

Monitor monitor_tunel[2];
//Monitor
//Inicializacion de monitor
void ini_monitor(Monitor *monitor) {
    pthread_mutex_init(&monitor->mutex, NULL);
    pthread_cond_init(&monitor->condicion, NULL);
    monitor->ocupado = 0;
    monitor->contador = 0;
}
//Inicializacion de monitor
//Tunel
void *Tunel_DE(Monitor *monitor_tunel){
    char tunel_id[]="DE";
while (1) {
    char buffer[BUFFER_SIZE];
    read(tunel_DE[0], buffer, sizeof(buffer));
    printf("Tunel %s : Mensaje recibido desde del Auto: %s\n",tunel_id, buffer);
    //pthread_mutex_lock(&monitor_tunel->mutex);

        if (monitor_tunel->ocupado == 0) {
        monitor_tunel->ocupado = 1;
        printf("Tunel %s: Enviando respuesta al  Auto\n", tunel_id);
        char reply[] = "Esta Libre";
        write(tunel_ED[1], reply, sizeof(reply));
      //  pthread_cond_signal(&monitor_tunel->condicion);
    } else {
        char reply[] = "Esta ocupado";
        write(tunel_ED[1], reply, sizeof(reply));
        // pthread_cond_wait(&monitor_tunel->condicion, &monitor_tunel->mutex);
    }
}

  //  pthread_mutex_unlock(&monitor_tunel->mutex);

     pthread_exit(NULL);
}
void *Tunel_AC(Monitor *monitor_tunel){
    char tunel_id[]="AC";
while (1) {
    char buffer[BUFFER_SIZE];
    read(tunel_AC[0], buffer, sizeof(buffer));
    printf("Tunel %s : Mensaje recibido desde del Auto: %s\n",tunel_id, buffer);
    //pthread_mutex_lock(&monitor_tunel->mutex);

        if (monitor_tunel->ocupado == 0) {
        monitor_tunel->ocupado = 1;
        printf("Tunel %s: Enviando respuesta al  Auto\n", tunel_id);
        char reply[] = "Esta Libre";
        write(tunel_CA[1], reply, sizeof(reply));
      //  pthread_cond_signal(&monitor_tunel->condicion);
    } else {
        char reply[] = "Esta ocupado";
        write(tunel_CA[1], reply, sizeof(reply));
        // pthread_cond_wait(&monitor_tunel->condicion, &monitor_tunel->mutex);
    }
}

  //  pthread_mutex_unlock(&monitor_tunel->mutex);

     pthread_exit(NULL);
}

void *Tunel_BC(Monitor *monitor_tunel){
    char tunel_id[]="BC";
while (1) {
    char buffer[BUFFER_SIZE];
    read(tunel_BC[0], buffer, sizeof(buffer));
    printf("Tunel %s : Mensaje recibido desde del Auto: %s\n",tunel_id, buffer);
    //pthread_mutex_lock(&monitor_tunel->mutex);

        if (monitor_tunel->ocupado == 0) {
        monitor_tunel->ocupado = 1;
        printf("Tunel %s: Enviando respuesta al  Auto\n", tunel_id);
        char reply[] = "Esta Libre";
        write(tunel_CB[1], reply, sizeof(reply));
      //  pthread_cond_signal(&monitor_tunel->condicion);
    } else {
        char reply[] = "Esta ocupado";
        write(tunel_CB[1], reply, sizeof(reply));
        // pthread_cond_wait(&monitor_tunel->condicion, &monitor_tunel->mutex);
    }
}

  //  pthread_mutex_unlock(&monitor_tunel->mutex);

     pthread_exit(NULL);
}

//Tunel
//Acciones autos DE
void *Auto_hilo_DE(Monitor *monitor_tunel) {
    char tunel_id[]="DE";
    pthread_t auto_id = pthread_self();
  //  int auto_id = *((int *)arg);
    //Tunel_ED(&monitor_tunel, auto_id);
    pthread_mutex_lock(&monitor_tunel->mutex);
    char msj[] = "¿Esta Ocupado?";
    printf("El Auto %lu: Enviando mensaje al Tunel %s\n",auto_id, tunel_id );
    write(tunel_DE[1], msj, sizeof(msj));
    // Respuesta desde el Canal B

    char buffer[BUFFER_SIZE];
    read(tunel_ED[0], buffer, sizeof(buffer));
    printf("El Auto:%lu recibio el mensaje que el tunel %s %s\n",auto_id,tunel_id ,buffer);
        printf("-------------------------------------------------------------\n");
        printf("El auto %lu Pasando por el tunel %s\n", auto_id, tunel_id);
        printf("-------------------------------------------------------------\n");
        sleep(1);
        printf("-------------------------------------------------------------\n");
        printf("El auto %lu Saliendo por el tunel %s\n", auto_id, tunel_id);
        printf("-------------------------------------------------------------\n");

    monitor_tunel->ocupado =0;
    monitor_tunel->contador ++;

    pthread_mutex_unlock(&monitor_tunel->mutex);


    pthread_exit(NULL);
}
//Acciones autos AC
void *Auto_hilo_AC(Monitor *monitor_tunel) {
    char tunel_id[]="AC";
    pthread_t auto_id = pthread_self();
  //  int auto_id = *((int *)arg);
    //Tunel_ED(&monitor_tunel, auto_id);

    pthread_mutex_lock(&monitor_tunel->mutex);
    char msj[] = "¿Esta Ocupado?";
    printf("El Auto %lu: Enviando mensaje al Tunel %s\n",  auto_id, tunel_id);
    write(tunel_AC[1], msj, sizeof(msj));
    // Respuesta desde el Canal B

    char buffer[BUFFER_SIZE];
    read(tunel_CA[0], buffer, sizeof(buffer));
    printf("El Auto:%lu recibio el mensaje que el tunel %s %s\n",auto_id,tunel_id ,buffer);
        printf("-------------------------------------------------------------\n");
        printf("El auto %lu Pasando por el tunel %s\n", auto_id, tunel_id);
        printf("-------------------------------------------------------------\n");
        sleep(2);
        printf("-------------------------------------------------------------\n");
        printf("El auto %lu Saliendo por el tunel %s\n", auto_id, tunel_id);
        printf("-------------------------------------------------------------\n");
    printf("\n");


    monitor_tunel->ocupado = 0;
    monitor_tunel->contador ++;

    pthread_mutex_unlock(&monitor_tunel->mutex);


    pthread_exit(NULL);
}
//Acciones autos AC

//Acciones autos BC
void *Auto_hilo_BC(Monitor *monitor_tunel) {
    char tunel_id[]="BC";
    pthread_t auto_id = pthread_self();
  //  int auto_id = *((int *)arg);
    //Tunel_ED(&monitor_tunel, auto_id);

    pthread_mutex_lock(&monitor_tunel->mutex);
    char msj[] = "¿Esta Ocupado?";
    printf("El Auto %lu: Enviando mensaje al Tunel %s\n",  auto_id, tunel_id);
    write(tunel_BC[1], msj, sizeof(msj));
    // Respuesta desde el Canal B

    char buffer[BUFFER_SIZE];
    read(tunel_CB[0], buffer, sizeof(buffer));
    printf("El Auto:%lu recibio el mensaje que el tunel %s %s\n",auto_id,tunel_id ,buffer);
    printf("-------------------------------------------------------------\n");
        printf("El auto %lu Pasando por el tunel %s\n", auto_id, tunel_id);
    printf("-------------------------------------------------------------\n");
        sleep(5);
    printf("-------------------------------------------------------------\n");    
        printf("El auto %lu Saliendo por el tunel %s\n", auto_id, tunel_id);
    printf("-------------------------------------------------------------\n");    
    monitor_tunel->ocupado = 0;
    monitor_tunel->contador ++;

    pthread_mutex_unlock(&monitor_tunel->mutex);


    pthread_exit(NULL);
}
//Acciones autos BC

int main() {
    if (pipe(tunel_ED) == -1 || pipe(tunel_DE) == -1 || pipe(tunel_AC) == -1 || pipe(tunel_CA) == -1 || pipe(tunel_BC) == -1 || pipe(tunel_CB) == -1) {
        perror("Error al crear el pipe");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++)
    {
        ini_monitor(&monitor_tunel[i]);
    }


    pthread_t autos[C_AUTOS];
   // int autos_id[C_AUTOS];

    for (int i = 0; i < C_AUTOS; i++) {
      //  autos_id[i] = i + 1;
        if(i<=4){
        pthread_create(&autos[i], NULL, Auto_hilo_DE, (void *)&monitor_tunel[0]);
        }else if (i >4 && i %2 == 0){
            pthread_create(&autos[i], NULL, Auto_hilo_AC, (void *)&monitor_tunel[1]);
        }else{
            pthread_create(&autos[i], NULL, Auto_hilo_BC, (void *)&monitor_tunel[2]);
        }
    }

    pthread_t tunel_DE, tunel_AC, tunel_BC;

        pthread_create(&tunel_DE, NULL, Tunel_DE, (void *)&monitor_tunel[0]);
        pthread_create(&tunel_AC, NULL, Tunel_AC, (void *)&monitor_tunel[1]);
        pthread_create(&tunel_BC, NULL, Tunel_BC, (void *)&monitor_tunel[2]);

      //  pthread_join(tunel_ED, NULL);

    for (int i = 0; i < C_AUTOS; ++i) {
        pthread_join(autos[i], NULL);
    }
    printf("La cantidad de autos que pasasaron por el tunel DE fueron %d\n", monitor_tunel[0].contador);
    printf("La cantidad de autos que pasasaron por el tunel AC fueron %d\n", monitor_tunel[1].contador);
    printf("La cantidad de autos que pasasaron por el tunel BC fueron %d\n", monitor_tunel[2].contador);

  //datos estadisticos
  float totalautos = monitor_tunel[0].contador+monitor_tunel[1].contador+monitor_tunel[2].contador;
    printf("Tunel DE: %.2f%\nTunel AC: %.2f %\nTunel BC: %.2f %\nEl total de autos que pasaron fue: %.0f\n", monitor_tunel[0].contador/totalautos*100,(monitor_tunel[1].contador/totalautos*100),(monitor_tunel[2].contador/totalautos*100),totalautos);


    pthread_mutex_destroy(&monitor_tunel[0].mutex);
    pthread_cond_destroy(&monitor_tunel[0].condicion);
    pthread_mutex_destroy(&monitor_tunel[1].mutex);
    pthread_cond_destroy(&monitor_tunel[1].condicion);
    pthread_mutex_destroy(&monitor_tunel[2].mutex);
    pthread_cond_destroy(&monitor_tunel[2].condicion);

   // close(tunel_ED[0]);
   // close(tunel_ED[1]);

    return 0;
}