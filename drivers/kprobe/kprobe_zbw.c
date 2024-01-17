#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/ptrace.h>
#include <linux/sched.h>

#define PROCFS_NAME "pid_kprobe_ioctl"

static int pid_kprobe_ioctl = 0;
static struct proc_dir_entry *proc_file;

// proc文件的写入处理函数
ssize_t procfile_write(struct file *file, const char __user *buffer,
                       size_t count, loff_t *offp) {
  char pid_buffer[16];
  if (count > 16)
    count = 16;

  if (copy_from_user(pid_buffer, buffer, count))
    return -EFAULT;

  pid_buffer[count] = '\0';
  pid_kprobe_ioctl = simple_strtol(pid_buffer, NULL, 10);
  printk(KERN_INFO "kprobe_zbw:kprobe target PID set to %d\n",
         pid_kprobe_ioctl);
  return count;
}

// kprobe的预处理函数
static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
  int i = 0;
  if (current->pid == pid_kprobe_ioctl) {
    // unsigned int fd = regs->regs[0];
    // unsigned int cmd = regs->regs[1];

    // printk(KERN_INFO "ioctl called by target process - fd: %u, cmd: %u\n",
    // fd,
    //        cmd);

    // 这个循环加上会死机，不知道为啥，估计碰了不该碰的
    for (i = 0; i < 9; i++) {
      printk(KERN_INFO "kprobe_zbw: reg%d= %lx", i, regs->regs[i]);
    }
  }
  return 0;
}

// kprobe数据结构
static struct kprobe kp = {
    .symbol_name = "__arch_copy_from_user",
    .pre_handler = handler_pre,
};

static struct proc_ops proc_file_ops = {
    .proc_write = procfile_write,
};

// 初始化函数
static int __init kprobe_init(void) {
  int ret;

  ret = register_kprobe(&kp);
  if (ret < 0) {
    printk(KERN_INFO "kprobe_zbw: register_kprobe failed, returned %d\n", ret);
    return ret;
  }
  printk(KERN_INFO "kprobe_zbw: Planted kprobe at %p\n", kp.addr);

  // 创建 proc 文件
  proc_file = proc_create(PROCFS_NAME, 0666, NULL, &proc_file_ops);
  if (proc_file == NULL) {
    remove_proc_entry(PROCFS_NAME, NULL);
    printk(KERN_ALERT "kprobe_zbw: Error: Could not initialize /proc/%s\n",
           PROCFS_NAME);
    return -ENOMEM;
  }

  printk(KERN_INFO "kprobe_zbw: /proc/%s created\n", PROCFS_NAME);

  return 0;
}

// 清理函数
static void __exit kprobe_exit(void) {
  unregister_kprobe(&kp);
  printk(KERN_INFO "kprobe_zbw: kprobe at %p unregistered\n", kp.addr);
  // 移除 proc 文件
  remove_proc_entry(PROCFS_NAME, NULL);
  printk(KERN_INFO "kprobe_zbw: /proc/%s removed\n", PROCFS_NAME);
}

module_init(kprobe_init) module_exit(kprobe_exit)

    MODULE_LICENSE("GPL");
