#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{

  int n = atoi(argv[1]), retime, rutime, stime, pid;

  int t_iobound = 0, tretime_iobound = 0, tstime_iobound = 0, tturnaround_iobound = 0;
  int t_scpu = 0, tretime_scpu = 0, tstime_scpu = 0, tturnaround_scpu = 0;
  int t_cpubound = 0, tretime_cpubound = 0, tstime_cpubound = 0, tturnaround_cpubound = 0;
  int cpu_bound_loop = 10000000;
  int s_bound_loop = 1000000;
  int r;

  if(argc < 2){
    printf(2, "Usage: pass the number of process to be created...\n");
    exit();
  }

  // preciso de dois for de 3*n, um que vai criar os processos e outro que vai calcular as estatísticas chamando o wait
  for (int i = 0; i < 3*n; i++) {
    // the only process allowed to fork is main process
    pid = fork();
    if (pid == 0) {
      r = getpid() % 3;

      if (r == 0) {
        for (int i = 0; i < 100; i ++) {
          for (int i = 0; i < cpu_bound_loop; i++){}
        }
      }
      else if (r == 1) {
        for (int i = 0; i < 100; i ++) {
          for (int i = 0; i < s_bound_loop; i++){}
          yield();
        }
      } else if (r == 2) {
        for (int i = 0; i < 100; i++) {
          sleep(1);
        }
      }

      exit(); // todos os processos exit vão entrar no estado ZOMBIE
    } 

    continue;
  }

  for (int i = 0; i< 3*n; i++) {
    pid = wait2(&retime, &rutime, &stime); // PROCURO POR PROCESSOS ZOMBIE

    r = pid % 3;
    if (r % 3 == 0) {
      printf(1, "CPU-BOUND: pid = %d, waiting time = %d, borst time = %d, io time = %d\n", pid, retime+stime, rutime, stime);
      t_cpubound += 1;
      tretime_cpubound += retime;
      tstime_cpubound += stime;
      tturnaround_cpubound += retime+stime+rutime;
    }
    else if (r % 3 == 2) {
      printf(1, "IO-BOUND: pid = %d, waiting time = %d, borst time = %d, io time = %d\n", pid, retime+stime, rutime, stime);
      t_iobound += 1;
      tretime_iobound += retime;
      tstime_iobound += stime;
      tturnaround_iobound += retime+stime+rutime;
    }
    else if (r % 3 == 1) {
      printf(1, "S-CPU: pid = %d, waiting time = %d, borst time = %d, io time = %d\n", pid, retime+stime, rutime, stime);
      t_scpu += 1;
      tretime_scpu += retime;
      tstime_scpu += stime;
      tturnaround_scpu += retime+stime+rutime;
    }
  }


  printf(1, "\n\n");
  if (t_iobound > 0) {
    printf(1, "IO-BOUND: sleeping = %d, ready = %d, turnaround = %d\n", tstime_iobound/t_iobound, tretime_iobound/t_iobound, tturnaround_iobound/t_iobound);
  }
  if (t_cpubound > 0) {
    printf(1, "CPU-BOUND: sleeping = %d, ready = %d, turnaround = %d\n", tstime_cpubound/t_cpubound, tretime_cpubound/t_cpubound, tturnaround_cpubound/t_cpubound);
  }
  if (t_scpu > 0) {
    printf(1, "S-CPU: sleeping = %d, ready = %d, turnaround = %d\n", tstime_scpu/t_scpu, tretime_scpu/t_scpu, tturnaround_scpu/t_scpu);
  }

  exit();
}





