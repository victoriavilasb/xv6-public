#include "types.h"
#include "stat.h"
#include "user.h"

void exec_processes(int pid);
void loop_times(int times);

int
main(int argc, char *argv[])
{

  int n = atoi(argv[1]), retime, rutime, stime, pid;
  uint ctime;
  // uint ftime;
  // int total_process = 0, t_retime = 0, t_stime = 0, t_turnaround = 0;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < n; j++) {
      pid = fork();
      if (pid == 0) {
        exec_processes(pid);
      } else
      {
        wait2(&ctime, &retime, &rutime, &stime);

        printf(1, "ctime %d, retime %d, rutime %d stime %d\n", ctime, retime, rutime, stime);
        // printf(1, "waiting time = %d, borst time = %d, io time = %d\n", retime+stime, rutime, 0);

        // ftime = uptime();

        // t_turnaround += ftime - ctime;
        // t_retime += retime;
        // t_stime += stime;
      }
    }
  }


  // total_process = n * 3;
  // printf(1, "\nsleeping = %d, ready = %d, turnaround = %d\n", t_stime/total_process, t_retime/total_process, t_turnaround/total_process);

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
    // printf(1, "pid = %d, process type = CPU-Bound\n", pid);
    for (int i = 0; i < 100; i ++) {
      loop_times(cpu_bound_loop);
    }
  }
  else if (pid % 3 == 1) {
    // printf(1, "pid = %d, process type = S-CPU\n", pid);
    for (int i = 0; i < 100; i ++) {
      loop_times(io_bound_loop);
    }
  } else if (pid % 3 == 2) {
    // printf(1, "pid = %d, process type = IO-Bound\n", pid);
    for (int i = 0; i < 100; i++) {
      sleep(1);
    }
  }
}

void
loop_times(int times) 
{
  for (int i = 0; i < times; i++);
}