#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mount.h>


#include "mount.h"
#include "util.h"

extern int pivot_root(const char *new_root, const char *put_old);

int mount_init(char * mount_base){ 
    check_mkdir(mkdir(mount_base,0755), "mkdir mount_base");
    return 0;
}

int prepare_mount(char * mount_base, char * unicorn_id, char * rootfs_base){	
    //use aufs to create union target rootfs
    char rootfs_path[100];
    check_result(sprintf(rootfs_path,"%s/%s",mount_base,unicorn_id), rootfs_path);
    check_mkdir(mkdir(rootfs_path, 0755), "mkdir root");
    check_result(sprintf(rootfs_path,"%s/rootfs",rootfs_path), rootfs_path);
    check_mkdir(mkdir(rootfs_path,0755), "mkdir rootfs");

    char mount_copy_on_write_dst[100];
    check_result(sprintf(mount_copy_on_write_dst,"%s/%s-init",mount_base,unicorn_id),mount_copy_on_write_dst);
    check_mkdir(mkdir(mount_copy_on_write_dst,0755),"mkdir copy_on_write dir");

    // mount -n -t aufs -o br:$mount_copy_on_write_dst=rw:$rootfs_base=ro+wh none rootfs_path
    char mount_data[100];
    check_result(sprintf(mount_data,"br:%s=rw:%s=ro+wh",mount_copy_on_write_dst, rootfs_base), mount_data);
    printf("rootfs_path:: %s mount_opt:: %s \n", rootfs_path, mount_data);
    check_result(mount("none",rootfs_path,"aufs",0, mount_data),"mount rootfs_base to rootfs dir");

    return 0;

}

int pivot_move(char * mount_base, char * unicorn_id){
    char rootfs_path[100];
    check_result(sprintf(rootfs_path,"%s/%s/rootfs",mount_base,unicorn_id), rootfs_path);
    check_mkdir(mkdir(rootfs_path, 0755), "mkdir root");


	char pivot_old_dir[120];
    check_result(sprintf(pivot_old_dir,"%s/%s",rootfs_path,".pivot_old"),pivot_old_dir);
    check_mkdir(mkdir(pivot_old_dir,0755),"mkdir .pivot_old");
    check_result(chdir(rootfs_path),"chdir to rootfs_path");
    //printf("rootfs::%s pivot_old::%s \n", rootfs_path, pivot_old_dir);
    check_result(pivot_root(".", ".pivot_old"),"pivot_root");  
    check_result(chdir("/"),"chdir to /");
    check_result(umount2("/.pivot_old",MNT_DETACH), "umount /.pivot_old");
    check_result(rmdir(".pivot_old"),"remove .pivot_old");

    // mkdir -p /dev/pts
    //mount -t devpts -o newinstance,ptmxmode=0666 devpts /dev/pts
    //ln -sf pts/ptmx /dev/ptmx 
    check_mkdir(mkdir("/dev",0755),"mkdir /dev");
    check_mkdir(mkdir("/dev/pts",0755),"mkdir /dev/pts");
    check_result(mount("devpts","/dev/pts","devpts",0, "newinstance,ptmxmode=0666"),"mount /dev/pts");
    check_result(symlink("pts/ptmx","/dev/ptmx"),"ln -sf pts/ptmx /dev/ptmx ");

    //mkdir -p /dev/shm
    //mount -t tmpfs tmpfs /dev/shm
    //system("mkdir -p /dev/shm");
    check_mkdir(mkdir("/dev/shm",0755),"mkdir /dev/shm");
    check_result(mount("tmpfs","/dev/shm","tmpfs",0, NULL),"mount /dev/shm");

    //for top tool: mount -t proc proc /proc
	check_mkdir(mkdir("/proc",0755),"mkdir /proc");
    check_result(mount("proc","/proc","proc",0, NULL),"mount /proc");

	//for device data: mount -t proc proc /proc
    check_mkdir(mkdir("/sys",0755),"mkdir /sys");
    check_result(mount("sysfs","/sys","sysfs",0, NULL),"mount /sys");

    //export PS1="root@@\h:\w\$"

    return 0;
}