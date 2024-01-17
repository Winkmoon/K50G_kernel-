#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define PROC_FILE "/proc/pid_kprobe_ioctl"
#define DUMMY_IOCTL_CMD 12345

int main() {
  char pid_buffer[16];
  int fd;
  int pid;

  // 输入待监测进程pid
  printf("input target pid:");
  scanf("%d", &pid);
  // 向 /proc/kprobe_pid 写入当前进程的 PID
  fd = open(PROC_FILE, O_WRONLY);
  if (fd < 0) {
    perror("Failed to open " PROC_FILE);
    return 1;
  }

  if (write(fd, pid_buffer, strlen(pid_buffer)) < 0) {
    perror("Failed to write to " PROC_FILE);
    close(fd);
    return 1;
  }

  close(fd);
  printf("Set kprobe to monitor current process PID %d\n", pid);

  return 0;
}
