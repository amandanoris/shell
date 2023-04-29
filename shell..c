#include <signal.h>
#include "ejecutarPrograma.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "prompt.h"
#include <stdio.h>
#include <readline/history.h>
// Variable global para almacenar el PID del proceso en ejecución
pid_t *procesos_en_ejecucion = NULL;
int numero_de_procesos = 0;

extern command_t* bg_processes;

// Manejador de señales para SIGINT
void sigint_handler(int signum) {
    // Si hay un proceso en ejecución
    if (procesos_en_ejecucion != NULL) {
        // Enviar la señal SIGINT a todos los procesos en ejecución
        for (int i = 0; i < numero_de_procesos; i++) {
            kill(procesos_en_ejecucion[i], SIGINT);
        }
    }
}

int main() {
    char *comando;
    
    // Establecer el manejador de señales para SIGINT
    signal(SIGINT, sigint_handler);
    read_history("Mishell_history");
    while (1) {
        comando = Prompt();
        
        if (strcmp(comando, "exit") == 0) {
            
            command_t* current = bg_processes;
            while (current != NULL)
            {
                kill (current->pid, SIGINT);
                current = current->next;
            }
        break;
        }
        
        // Llamar a la función parser con el comando ingresado por el usuario como parámetro
        command_t *comandos = parse_commands(comando);
        
        // Establecer el PID del proceso en ejecución
        procesos_en_ejecucion = ejecutar_programa(comando, comandos, &numero_de_procesos);
        
        // Restablecer el PID del proceso en ejecución
        (procesos_en_ejecucion == NULL) ? NULL : (procesos_en_ejecucion = (free (procesos_en_ejecucion), NULL));
    }
    write_history("Mishell_history");
    return 0;
}