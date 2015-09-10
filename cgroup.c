#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mount.h>

#include "cgroup.h"
#include "util.h"


char * const cgroup_base_dir="/tmp/unicorn/cgroup";
char* cgroup_subsystems[] = {"cpu", "cpuset", "cpuacct", "memory", "blkio"};
int cgroup_subsystems_lengh = 5;

int cgroup_init(){
    int i;
    for (i=0; i< cgroup_subsystems_lengh; i++){
    	char cmd[100];
		check_result(sprintf(cmd, "mkdir -p %s/%s", cgroup_base_dir, cgroup_subsystems[i]), cmd);
		check_result(system(cmd), "system call mkdir ");

	    check_result(sprintf(cmd, "mount -t cgroup %s -o %s %s/%s", cgroup_subsystems[i], cgroup_subsystems[i], cgroup_base_dir, cgroup_subsystems[i]), cmd);
	    check_result(system(cmd), "system call mount ");
    }


	return 0;
}

int cgroup_add(unicorn_t * u){
	int i;
	for (i=0; i< cgroup_subsystems_lengh; i++){
		char system_path[80];
		check_result(sprintf(system_path, "%s/%s", cgroup_base_dir, cgroup_subsystems[i]),system_path);

		char cmd[100];
		check_result(sprintf(cmd, "mkdir -p %s/%s", system_path, u->unicorn_id),cmd);
		check_result(system(cmd), "system call mkdir ");

		// char file_path[100];
		// check_result(sprintf(file_path, "%s/%s/%s/tasks", cgroup_base_dir, cgroup_subsystems[i], u->unicorn_id),file_path);

  //       char value[10];
  //       check_result(sprintf(value, "%d", u->child_pid), value);
  //       printf("write value %s to %s \n", value, file_path);	
		// FILE * f = fopen(file_path, "w");
		// if(f == NULL){
		// 	printf("open file error %s \n", strerror(errno));
		// 	exit(errno);
		// }
		// int rt = fputs("2345", f);
		// if( rt < 0) {
		// 	printf("write task file error %s ", cgroup_subsystems[i]);
		// 	exit(-1);
		// }
		// printf("write %d chars \n", rt);
        if( strcmp(cgroup_subsystems[i], "cpuset") == 0){
        	check_result(sprintf(cmd, "cat %s/cpuset.mems > %s/%s/cpuset.mems", system_path, system_path, u->unicorn_id),cmd);
        	check_result(system(cmd), "system call add cpuset.mems ");

        	check_result(sprintf(cmd, "cat %s/cpuset.cpus > %s/%s/cpuset.cpus", system_path, system_path, u->unicorn_id),cmd);
        	check_result(system(cmd), "system call add cpuset.cpus ");

        }
		check_result(sprintf(cmd, "echo %d > %s/%s/tasks", u->child_pid, system_path, u->unicorn_id),cmd);
		check_result(system(cmd), "system call add pid ");
    }

	return 0;
}