#include "types.h"
#include "stat.h"
#include "user.h"

void exec_processes(int pid);
void loop_times(int times);
char * check_p_type(int pid);

int
main(int argc, char *argv[])
{

  int n = atoi(argv[1]), retime, rutime, stime, pid;
  const int main_pid = getpid();
  char *process_type;

  int t_iobound = 0, tretime_iobound = 0, tstime_iobound = 0, tturnaround_iobound = 0;
  int t_scpu = 0, tretime_scpu = 0, tstime_scpu = 0, tturnaround_scpu = 0;
  int t_cpubound = 0, tretime_cpubound = 0, tstime_cpubound = 0, tturnaround_cpubound = 0;

  if(argc < 2){
    printf(2, "Usage: pass the number of process to be created...\n");
    exit();
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < n; j++) {
      // the only process allowed to fork is main process
      if (main_pid == getpid()) {
        pid = fork();
        if (pid == 0) {
          exec_processes(pid);
        } else
        {
          wait2(&retime, &rutime, &stime);

          process_type = check_p_type(pid);
          printf(1, "pid = %d, process type = %s\n", pid, process_type);
          printf(1, "waiting time = %d, borst time = %d, io time = %d\n", retime+stime, rutime, 0);

          if (pid % 3 == 0) {
            t_cpubound += 1;
            tretime_cpubound += retime;
            tstime_cpubound += stime;
            tturnaround_cpubound += retime+stime+rutime;
          }
           else if (pid % 3 == 2) {
            t_iobound += 1;
            tretime_iobound += retime;
            tstime_iobound += stime;
            tturnaround_iobound = retime+stime+rutime;
          }
           else if (pid % 3 == 1) {
            t_scpu += 1;
            tretime_scpu += retime;
            tstime_scpu += stime;
            tturnaround_scpu = retime+stime+rutime;
          }
        } 
      }
    }
  }

  if (main_pid == getpid()) {
    if (t_iobound > 0) {
      printf(1, "IO-BOUND: sleeping = %d, ready = %d, turnaround = %d\n", tstime_iobound/t_iobound, tretime_iobound/t_iobound, tturnaround_iobound/t_iobound);
    }
    if (t_cpubound > 0) {
      printf(1, "CPU-BOUND: sleeping = %d, ready = %d, turnaround = %d\n", tstime_cpubound/t_cpubound, tretime_cpubound/t_cpubound, tturnaround_cpubound/t_cpubound);
    }
    if (t_scpu > 0) {
      printf(1, "S-CPU: sleeping = %d, ready = %d, turnaround = %d\n", tstime_scpu/t_scpu, tretime_scpu/t_scpu, tturnaround_scpu/t_scpu);
    }
  }

  exit();
}

void
exec_processes(int pid)
{
  int cpu_bound_loop = 10000000;
  int s_bound_loop = 1000000;

  if (pid % 3 == 0) {
    for (int i = 0; i < 100; i ++) {
      set_tickets(1000);
      loop_times(cpu_bound_loop);
    }
  }
  else if (pid % 3 == 1) {
    for (int i = 0; i < 100; i ++) {
      loop_times(s_bound_loop);
    }
  } else if (pid % 3 == 2) {
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

char *
check_p_type(int pid)
{
  if (pid % 3 == 0) {
    return "CPU-Bound";
  } else if (pid % 3 == 1) {
    return "S-CPU";
  } else if (pid % 3 == 2) {
    return "IO-BOUND";
  }

  return "";
}

