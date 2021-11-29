#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void q()
{
  printf("Se ha producido la señal SIGQUIT\n");
  //exit(2);
}

void p()
{
  printf("Se ha producido la señal SIGINT\n");
  exit(2);
}

int main(int argc, char *arg[])
{
  //signal(SIGQUIT,q);
  struct sigaction a;
  struct sigaction b;
  a.sa_handler = q;
  sigemptyset(&a.sa_mask);
  a.sa_flags = 0;
  sigaction(SIGQUIT, &a, NULL);

  //signal(SIGINT, p);
  b.sa_handler = p;
  sigemptyset(&b.sa_mask);
  b.sa_flags = 0;
  sigaction(SIGINT, &b, NULL);
  while (1)
  {
    sleep(1);
    printf("Esperando señal y trabajando\n");
  }
}
