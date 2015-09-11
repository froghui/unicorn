#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#include "util.h"
#include "cgroup.h"
#include "mount.h"
#include "net.h"

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

char* const child_args[] = {
	"/bin/bash",
	NULL
};


char * const rootfs_base="/tmp/rootfs";
char * const mount_base="/tmp/unicorn";
char * const pivot_old=".pivot_old";


int pipes[2];
int child_main(void* arg)
{
	net_t * n = (net_t *)arg;
    //printf("in child main\n");
	char c;
	close(pipes[1]);   
	printf(" child pid from cloned child process:: [%5d] unicorn_id:: [%s] \n", getpid(), n->unicorn_id);

	pivot_move(mount_base, n->unicorn_id);
		
	//wait for the net peer to be setup
    read(pipes[0],&c,1);
    sethostname(n->hostname, 20);
    net_conf(n);

	check_result(execv(child_args[0], child_args), "execv");
	printf("return from chinld_main \n");
	return 1;
}

int main()
{
	cgroup_init();
	mount_init(mount_base);
        
	pipe(pipes);
	printf(" parent pid:: [%5d] \n",getpid());

	//unicorn id for the container
	char * unicorn_id = malloc(11);
    random_string(unicorn_id, 10);
	
    //net configuration
	net_t *n = calloc(1, sizeof(net_t));;
    assert(n != NULL);
    n->unicorn_id = unicorn_id;
    char * hostname = malloc(20);
    sprintf(hostname,"unicorn-%s",unicorn_id);
    n->hostname = hostname;
    n->mtu=1500;
    n->ip="192.168.3.223";
    n->netmask="255.255.255.0";
    n->gateway="192.168.3.1";

    //prepare auts and then clone
    prepare_mount(mount_base, unicorn_id, rootfs_base);
	int child_pid = clone(child_main, child_stack+STACK_SIZE, CLONE_NEWIPC | CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWNET | SIGCHLD, n);

	
	//prepare unicorn info
	unicorn_t *u = calloc(1, sizeof(unicorn_t));;
    assert(u != NULL);
    u->child_pid = child_pid;
    u->parent_pid = getpid();
    u->unicorn_id = unicorn_id;

    //cgroup
	cgroup_add(u);

	//net
	net_add(u);

	printf(" child pid from parent:: [%5d] \n",child_pid);
	sleep(1);

	//call child to prepare the inner ethenet
	close(pipes[1]);

	//wait to die
	waitpid(child_pid, NULL, 0);
	return 0;
}
