#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define C_AUTOS 8
//Recursos compartidos


typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condicion;
    int ocupado;
    int recurso[2];
    
} Monitor;

Monitor tuneles[3];

void ini_monitor(Monitor *monitor){
    pthread_mutex_init(&monitor->mutex,NULL);
    pthread_cond_init(&monitor->condicion, NULL);
    monitor->ocupado = 0;

    if (pipe(monitor->recurso) == -1) {
        perror("Error al crear al pipe");
        exit(EXIT_FAILURE);
    }
    
}
void* monitor_funcion(){
    
    printf("el Auto solicitante es \n");

}

void entrado_tunel(tunel_id, auto_id){
    

    pthread_mutex_lock(&tuneles[tunel_id].mutex);
    
    printf("El auto %d esta entrando al tunel %d\n", auto_id, tunel_id);
    pthread_mutex_unlock(&tuneles[tunel_id].mutex);


}

void saliendo_tunel(tunel_id, auto_id){
    char buffer[BUFFER_SIZE];
    printf("Saliendo recive el mensaje que %s", buffer);
}

void *car_thread(void *arg) {
    int auto_id = *((int *)arg);
    if(auto_id < 4){
        entrado_tunel(0, auto_id);
    }else if(auto_id >4 && auto_id % 2 ==0){
        entrado_tunel(1, auto_id);
    }else{
        entrado_tunel(2, auto_id);
    }

    pthread_exit(NULL);
}
void hilo_canalAC()
{


}


int main()
{
pthread_t autos[C_AUTOS];
int autos_id[C_AUTOS];

for (int i = 0; i < 3; i++)
{
    ini_monitor(&tuneles[i]);
}

for (int i = 0; i < C_AUTOS; i++)
{
    autos_id[i] = i + 1;
    pthread_create(&autos[i], NULL, *car_thread, (void *)&autos_id[i]);
}
for (int i = 0; i < C_AUTOS; ++i) {
    pthread_join(autos[i], NULL);
}


}





