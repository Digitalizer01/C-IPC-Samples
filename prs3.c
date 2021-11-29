
//Programa que crea 5 procesos hijos tras mostrar un mensaje identificativo, el padre esperará hasta que todos los hijos finalizam.
//Cada proceso hijo se comunicará con el padre mediante tuberías y sincronización con señales. Los hijos enviarán su identificación de proceso y a continuación realizarán un bucle con 5 iteraciones y en cada iteración enviarán al padre el valor del índice y utilizará SIGUSR2 como notificación.
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
  pipe(tuberia);
  
  printf("Proceso padre: %d voy a crear 5 procesos hijoa y me enviarán algunos datos\n",getpid());
  for (i=0;i<5;i++)
  {
   pid=fork();
   if (pid==0)
   {
    p=getpid();	  
    printf("Proceso hijo: %d comunico a mi padre algunos datos\n",p);
    write(tuberia[1],&p,sizeof(p));
    write(tuberia[1],&n,sizeof(n));
    for (i=0;i<n;i++)
     write(tuberia[1],&i,sizeof(i));
    kill(getppid(),SIGUSR1);

    printf("Finaliza el proceso hijo\n");
    exit(0);
   }
  }
  printf("Proceso padre: %d, continuaré cuando finalicen mis hijos\n",getpid()); 
  for (i=0;i<5;i++) 
    wait(&estado);
  printf("Proceso padre: %d, mis hijos han finalizado y continúo\n",getpid());
  exit(0);
 }

void notifica( ){
 int p,cuantos,i,j;
 int padre=getpid();
 read(tuberia[0],&p,sizeof(p));
 printf("Soy: %d. Comunicación recibida del proceso %d\n",padre,p);
 read(tuberia[0],&cuantos,sizeof(cuantos));
 for (i=0;i<cuantos;i++)
 {
  read(tuberia[0],&j,sizeof(j));
  printf("Soy: %d y he recibido: %d, del proceso: %d\n",padre,j,p);
 } 
}

