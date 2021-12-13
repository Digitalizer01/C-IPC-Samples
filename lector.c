#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int des;
    char cadena[100];

    mkfifo("tubo", 0);
    chmod("tubo", 460);
    do
    {
        des = open("tubo", O_RDONLY);
        if (des == -1)
            sleep(1);
    } while (des == -1);

    while (read(des, cadena, 100) > 0)
    {
        printf("%s \n", cadena);
    }
    close(des);
    printf("Finaliza lector\n");
    unlink("tubo");
}
