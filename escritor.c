
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int des, i;
    char cadena[100];
    sprintf(cadena, "Saludo desde el proceso escritor con pid= %d\n", getpid());
    des = -1;
    do
    {
        des = open("tubo", O_WRONLY);
        if (des == -1)
            sleep(1);
    } while (des == -1);
    for (i = 0; i < 3; i++)
    {
        write(des, cadena, 100);
        sleep(2);
    }
    close(des);
    printf("Acaba el escritor\n ");
}
