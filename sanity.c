#include "types.h"
#include "stat.h"
#include "user.h"

void exec_processes(int pid);
int loop_times(int times);

int
main(int argc, char *argv[])
{
  int n, retime, rutime, stime, pid;
  int total, t_retime, t_rutime, t_stime;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < n; j++) {
      pid = fork();
      if (pid > 0) {
        exec_processes(pid);
      } else if (pid == getpid()) // processo pai
      {
        wait2(&retime, &rutime, &stime);

        t_retime += retime;
        t_rutime += rutime;
        t_stime += stime;
      }
    }
  }

  if(argc < 2){
    printf(2, "Usage: pass the number of process to be created...\n");
    exit();
  }

  exit();
}

void
exec_processes(int pid)
{
  int cpu_bound_loop = 10000000;
  int io_bound_loop = 1000000;

  if (pid % 3 == 0) {
    for (int i = 0; i < 100; i ++) {
      loop_times(cpu_bound_loop);
    }
  }
  else if (pid % 3 == 1) {
    for (int i = 0; i < 100; i ++) {
      loop_times(io_bound_loop);
    }
  } else if (pid % 3 == 2) {
    for (int i = 0; i < 100; i++) {
      sleep(1);
    }
  }
}

int
loop_times(int times) 
{
  for (int i = 0; i < times; i++);
}