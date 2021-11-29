//Programa que crea un proceso hijo tras mostrar un mensaje identificativo, el padre esperará hasta que termine el proceso hijo.
//El proceso hijo se comunicará con el padre mediante tuberías y sincronización con señales. En la primera comunicación el hijo le enviará su identificación de proceso y utilizará la señal SIGUSR1 como notificación. A continuación realizará un bucle con 5 iteraciones y en cada iteración enviará al padre el valor del índice y utilizará SIGUSR2 como notificación.
//Después el hijo finalizará 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int tuberia[2];

void notifica();
void notifica2();

int main(int argc, char *argv[] ) { 
  int pid,p,i,estado;
  int n;
  if (argc>1)
    n=atoi(argv[1]);
  else
   {
    printf("Error, ejecutar: ./prs2 num_iteraciones\n");
    exit(0);
   }
  signal(SIGUSR1,notifica);
  signal(SIGUSR2,notifica2);
  pipe(tuberia);
  
  printf("Proceso padre: %d voy a crear un hijo y me enviará algunos datos\n",getpid());
  pid=fork();
  if (pid==0)
  {
    p=getpid();	  
    printf("Proceso hijo: %d comunico a mi padre algunos datos\n",p);
    write(tuberia[1],&p,sizeof(p));
    kill(getppid(),SIGUSR1);
    sleep(0.5);

    write(tuberia[1],&n,sizeof(n));
    for (i=0;i<n;i++)
     write(tuberia[1],&i,sizeof(i));
    kill(getppid(),SIGUSR2);
    sleep(0.5);

    printf("Finaliza el proceso hijo\n");
    exit(0);
  }
  printf("Proceso padre: %d, continuaré cuando finalice mi hijo\n",getpid()); 
  wait(&estado);
  printf("Proceso padre: %d, mi hijo ha finalizado y continúo\n",getpid());
  exit(0);
 }

void notifica( ){
 int p;
 int padre=getpid();
 read(tuberia[0],&p,sizeof(p));
 printf("Soy: %d. Comunicación recibida del proceso %d\n",padre,p);
}

void notifica2( ){
 int cuantos,i,j;
 int padre=getpid();
 read(tuberia[0],&cuantos,sizeof(cuantos));
 for (i=0;i<cuantos;i++)
 {
  read(tuberia[0],&j,sizeof(j));
  printf("Soy: %d y he recibido: %d\n",padre,j);
 } 
}

