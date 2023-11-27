#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_TUNEL 3
#define NUM_AUTOS 5

// Definición de la estructura del monitor
typedef struct {
    int tunel_disponible[NUM_TUNEL];
    pthread_mutex_t mutex;
} Monitor;

// Inicialización del monitor
void initMonitor(Monitor *monitor) {
    for (int i = 0; i < NUM_TUNEL; ++i) {
        monitor->tunel_disponible[i] = 1; // Todos los túneles disponibles inicialmente
    }
    pthread_mutex_init(&monitor->mutex, NULL);
}

// Operación del monitor: ingresar a un túnel
int ingresarTunel(Monitor *monitor, int id_auto, int num_tunel) {
    pthread_mutex_lock(&monitor->mutex);

    if (monitor->tunel_disponible[num_tunel]) {
        monitor->tunel_disponible[num_tunel] = 0; // Ocupar el túnel
        printf("Auto %d ingresó al túnel %d\n", id_auto, num_tunel);
        pthread_mutex_unlock(&monitor->mutex);
        return 1; // Éxito al ingresar al túnel
    }

    pthread_mutex_unlock(&monitor->mutex);
    return 0; // El túnel está ocupado, no se puede ingresar
}

// Operación del monitor: salir del túnel
void salirTunel(Monitor *monitor, int id_auto, int num_tunel) {
    pthread_mutex_lock(&monitor->mutex);

    monitor->tunel_disponible[num_tunel] = 1; // Liberar el túnel
    printf("Auto %d salió del túnel %d\n", id_auto, num_tunel);

    pthread_mutex_unlock(&monitor->mutex);
}

// Función para el hilo del auto
void *autoFuncion(void *arg) {
    Monitor *monitor = (Monitor *)arg;
    int id_auto = pthread_self(); // Obtener el ID del hilo como identificación del auto

    // Intentar ingresar a los túneles
    for (int i = 0; i < NUM_TUNEL; ++i) {
        if (ingresarTunel(monitor, id_auto, i)) {
            // Simular tiempo en el túnel
            sleep(1);
            salirTunel(monitor, id_auto, i);
            break;
        }
    }

    pthread_exit(NULL);
}

int main() {
    // Crear e inicializar el monitor
    Monitor miMonitor;
    initMonitor(&miMonitor);

    // Crear hilos para los autos
    pthread_t autos[NUM_AUTOS];
    for (int i = 0; i < NUM_AUTOS; ++i) {
        pthread_create(&autos[i], NULL, autoFuncion, (void *)&miMonitor);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_AUTOS; ++i) {
        pthread_join(autos[i], NULL);
    }

    // Destruir el mutex del monitor
    pthread_mutex_destroy(&miMonitor.mutex);

    return 0;
}
