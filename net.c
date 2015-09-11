#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#include "util.h"

int net_add(unicorn_t * u){
	//ip link add name u-${unicorn-id}-0 type veth peer name u-${unicorn-id}-1
    //ip link set u-${unicorn-id}-0 netns 1
    //ip link set u-${unicorn-id}-1 netns $PID
    //brctl addif ${bridge} u-${unicorn-id}-0	
	char cmd[100];
	check_result(sprintf(cmd, "ip link add name u-%s-0 type veth peer name u-%s-1", u->unicorn_id, u->unicorn_id),cmd);
	check_result(system(cmd), "system call ip link add ");

	check_result(sprintf(cmd, "ip link set u-%s-0 netns 1", u->unicorn_id),cmd);
	check_result(system(cmd), "system call ip link set ");


	check_result(sprintf(cmd, "ip link set u-%s-1 netns %d", u->unicorn_id, u->child_pid),cmd);
	check_result(system(cmd), "system call ip link set ");

	check_result(sprintf(cmd, "brctl addif br0 u-%s-0", u->unicorn_id),cmd);
	check_result(system(cmd), "system call addif br0 ");

    check_result(sprintf(cmd, "ifconfig u-%s-0 up", u->unicorn_id),cmd);
	check_result(system(cmd), "system call inconfig up ");


    return 0;
	
}

int net_conf(net_t * n){
	//ifconfig u-${unicorn-id}-1 $ip netmask $netmask mtu $mtu
	char cmd[100];
	check_result(sprintf(cmd, "ifconfig u-%s-1 %s netmask %s mtu %d", n->unicorn_id, n->ip, n->netmask, n->mtu),cmd);
	check_result(system(cmd), "system call ifconfig  ");
	
	return 0;
}