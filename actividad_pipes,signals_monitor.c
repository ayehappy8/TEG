#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define C_AUTOS 8
#define BUFFER_SIZE 256 // tamaño del Buffer
#define OCUPADO "¿Esta ocupado?"
#define RESPUESTA_1 "Si esta ocupado"
#define RESPUESTA_2 "Esta libre puede pasar"

    int tunelED[2];
    int tunelAC[2];
    int tunelCB[2];
// Recursos compartidos
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condicion;
    int ocupadoED;
    int ocupadoAC;
    int ocupadoCB;


} Monitor;

Monitor monitor_tunel;

void ini_monitor(Monitor *monitor_tunel) {
    pthread_mutex_init(&monitor_tunel->mutex, NULL);
    pthread_cond_init(&monitor_tunel->condicion, NULL);
    monitor_tunel->ocupadoED = 0;
    monitor_tunel->ocupadoAC = 0;
    monitor_tunel->ocupadoCB = 0;
}

void *car_thread(void *arg) {
    int auto_id = *((int *)arg);


    if (auto_id < 4) {
        char respuesta[sizeof(BUFFER_SIZE)];
        write(tunelED[1], OCUPADO, sizeof(OCUPADO));
        
        read(tunelED[0], &respuesta, sizeof(BUFFER_SIZE));
             while (monitor_tunel.ocupadoED != 0) {
        // Espera hasta que el túnel esté libre
        pthread_cond_wait(&monitor_tunel.condicion, &monitor_tunel.mutex);
    }
        printf("El auto %d pregunta si el tunel%s: %s\n",auto_id, "ED",respuesta);
        Tunel(&monitor_tunel, auto_id, "ED");

    } else if (auto_id > 4 && auto_id % 2 == 0) {

        Tunel(&monitor_tunel, auto_id,"AC");

    } else {
        Tunel(&monitor_tunel, auto_id, "CB");
    }

    close(tunelED[0]);
    close(tunelED[1]);

    pthread_exit(NULL);
}

void Tunel(Monitor *monitor_tunel, int auto_id, char tunel_id[2]) {
    char pregunta[sizeof(OCUPADO)];
    char respuesta[sizeof(RESPUESTA_2)];
    read(tunelED[0], &pregunta, sizeof(pregunta)); 
    printf("La pregunta que se recivio es %s\n", pregunta);

    pthread_mutex_lock(&monitor_tunel->mutex);

    if (monitor_tunel->ocupadoED == 0) {
        monitor_tunel->ocupadoED = auto_id;
        strcpy(respuesta, RESPUESTA_2);
    } else {
        strcpy(respuesta, RESPUESTA_1);
    }

    write(tunelED[1], &respuesta, sizeof(respuesta)); 

    pthread_mutex_unlock(&monitor_tunel->mutex);

    printf("Auto %d entrando al túnel.\n", auto_id);

    // Simular algún tipo de operación en el túnel
    sleep(2);

    printf("Auto %d saliendo del túnel.\n", auto_id);

    pthread_mutex_lock(&monitor_tunel->mutex);

    monitor_tunel->ocupadoED = 0;
    pthread_cond_signal(&monitor_tunel->condicion);

    pthread_mutex_unlock(&monitor_tunel->mutex);
}
int main() {
    ini_monitor(&monitor_tunel);
    
    if (pipe(tunelED) == -1 || pipe(tunelAC) == -1 || pipe(tunelCB) == -1) {
        perror("Error al crear al pipe");
        exit(EXIT_FAILURE);
    }

    pthread_t autos[C_AUTOS];
    int autos_id[C_AUTOS];

    for (int i = 0; i < C_AUTOS; i++) {
        autos_id[i] = i + 1;
        pthread_create(&autos[i], NULL, car_thread, (void *)&autos_id[i]);
    }

    for (int i = 0; i < C_AUTOS; ++i) {
        pthread_join(autos[i], NULL);
    }

    pthread_mutex_destroy(&monitor_tunel.mutex);
    pthread_cond_destroy(&monitor_tunel.condicion);

    return 0;
}
