#include <stdio.h>

int main() {
    //Ingreso de valores
    int c_procesos, c_bloques, i;
    printf("Ingrese el numero de procesos:");
    scanf("%d", &c_procesos);

    int procesos[c_procesos];
    printf("\nIngrese el tamano de los procesos:\n");
    for (i = 0; i < c_procesos; i++) {
        scanf("%d", &procesos[i]);
    }
    printf("Ingrese el numero de bloques de memoria:");
    scanf("%d", &c_bloques);

    int bloques[c_bloques];
    printf("\nIngrese el tamano de los bloques de memoria:\n");
    
    int total = 0;
    for (i = 0; i < c_bloques; i++) {
        scanf("%d", &bloques[i]);
        total = total + bloques[i];
    }
    //Ingreso de valores
    //Inicialización de variables
    int proceso1[c_bloques];
    int job[c_bloques];
    int fragmentacion[c_bloques];
    int asignacion[c_bloques];

    for (i = 0; i < c_bloques; i++) {
        asignacion[i] = 0;
    }
    //Inicialización de variables
    //Algoritmo
    int j, memoria_usada = 0;
    i = 0;

    while (i < c_procesos) {
        //-1 puede considerarse como un valor "no válido" o "sin asignar" inicialmente.
        int max = -1, indice = -1, k = -1, max1;
        for (j = 0; j < c_bloques; j++) {
            max1 = bloques[j];
        //Donde se encuentra el bloque de memoria mas grande donde se pueda acomodar el procesos actual
            if (max1 >= max && asignacion[j] == 0 && max1 >= procesos[i]) {
                max = max1;
                indice = j;
            } else {
                //el bloque actual no es adecuado para asignar el proceso actual
                if (asignacion[j] == 0) {
                    proceso1[j] = 0;
                    job[j] = 0;
                    fragmentacion[j] = bloques[j];
                }
            }
        }
        // cuando se ha encontrado un bloque de memoria adecuado para asignar el proceso actua
        if (k != indice) {
            proceso1[indice] = procesos[i];
            job[indice] = i + 1;
            fragmentacion[indice] = bloques[indice] - procesos[i];
            memoria_usada = memoria_usada + procesos[i];
            asignacion[indice] = 1;
            int l;
        }
        //actualizar las estructuras de datos y variables para reflejar la asignación del proceso actual al bloque de memoria seleccionado
        i++;
    }
    //Algoritmo
    //Impresión de datos
    printf("Tamano del bloque\tTamano del proceso\tNumero de proceso\tfragmentacion\n");
    for (i = 0; i < c_bloques; i++) {
        printf("%d\t\t\t%d\t\t\t%d\t\t\t%d\n", bloques[i], proceso1[i], job[i], fragmentacion[i]);
    }

    printf("Total de asignacion de memoria:%d\n", total);
    printf("Memoria utilizada:%d\n", memoria_usada);
    //Impresión de datos
    return 0;
}
