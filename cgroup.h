#ifndef CGROUP_H
#define CGROUP_H

int cgroup_init(char * cgroup_base_dir);
int cgroup_add(int pid);


#endif
