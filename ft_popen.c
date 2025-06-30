#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int ft_popen(const char *file, char const *argv[], char type)
{
    if ((type != 'r' && type != 'w') || !file || !argv)
        return -1;

    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;

    pid_t pid = fork();
    if (pid < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }
    if (pid == 0)
    { // Proceso hijo
        if (type == 'r')   // Redirige stdout al extremo de escritura del pipe
        {
            close(pipefd[0]); // Cierra el extremo de lectura
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        }

        else  // Redirige stdin al extremo de lectura del pipe
        {
            close(pipefd[1]); // Cierra el extremo de escritura
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
        }
        execvp(file, (char *const *)argv);
        _exit(1); // Si exec falla, termina el hijo
    }
    
    if (type == 'r') // Proceso padre
    {
        close(pipefd[1]); // Cierra el extremo de escritura
        return pipefd[0]; // Devuelve el extremo de lectura
    }
    else
    {
        close(pipefd[0]); // Cierra el extremo de lectura
        return pipefd[1]; // Retorna el extremo de escritura
    }
}
