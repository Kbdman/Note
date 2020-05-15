#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#define DEVICENAME "lchardev"
struct file_operations op;
static int oc = 0;
struct lDevStruct {
    int count;
    struct cdev *mydev;
} tar;
struct tbuf {
    int oc;
    int count;
    char data[0];
};
inline int get_data_offset(void)
{
    return (int) & (((struct tbuf *)0)->data);
}
static int bufsize = 1024;
module_param(bufsize, int, 0644);

struct lDevStruct *getstruct(struct cdev *p)
{
    struct lDevStruct *pz = (struct lDevStruct *)0;
    return (struct lDevStruct *)((char *)p) + (((char *)(&(pz->mydev))) - ((char *)p));
}
ssize_t mread(struct file *f, char __user *buf, size_t count, loff_t *n)
{
    struct tbuf *da = f->private_data;

    printk("start read %d\n", da->oc);
    if (count >= bufsize - get_data_offset()) {
        return -EINVAL;
    }
    ssize_t res = copy_to_user(buf, da->data, count);
    //*n += res;
    printk("read %d\n", count - res);
    return count - res;
}
ssize_t mwrite(struct file *f, const char __user *buf, size_t count, loff_t *n)
{

    struct tbuf *da = f->private_data;
    printk("start write %d\n", da->oc);
    if (count >= bufsize - get_data_offset()) {
        return -EINVAL;
    }
    ssize_t res = copy_from_user(da->data, buf, count);

    da->data[count - res] = '\0';
    da->count = count - res;
    //*n += res;
    printk("write end,%d,%s\n", da->count, da->data);
    return count;
}

int mopen(struct inode *i, struct file *f)
{
    printk(KERN_ALERT "in mopen\n");
    if (f->private_data == NULL) {
        struct tbuf *data = (struct tbuf *)vmalloc(bufsize);
        if (data == NULL)
            return -ENOMEM;
        data->count = 0;
        data->oc = oc;
        oc++;
        f->private_data = data;
    }
    return 0;
}

int mrelease(struct inode *i, struct file *f)
{
    printk(KERN_ALERT, "in release\n");
    struct tbuf *da = f->private_data;

    printk(KERN_ALERT, "count %d\n%s\n", da->count, da->data);
    if (f->private_data == NULL) {
        vfree(f->private_data);
    }
    return 0;
}
dev_t major = 0;
char *buf = NULL;
int  module_setup(void)
{
    op.read = mread;
    op.write = mwrite;
    op.open = mopen;
    op.release = mrelease;
    printk(KERN_ALERT "module_setup\n");
    tar.mydev = cdev_alloc();
    int res = alloc_chrdev_region(&major, 0, 1, DEVICENAME);
    if (res != 0) {
        printk(KERN_ALERT "alloc_chrdev_region failed\n");
        return res;
    }
    printk(KERN_ALERT "alloc_chrdev_region %d,%d\n", MAJOR(major), MINOR(major));
    cdev_init(tar.mydev, &op);
    res = cdev_add(tar.mydev, major, 1);
    if (res != 0) {
        unregister_chrdev_region(major, 1);
        printk(KERN_ALERT"cdev_add failed");
        return res;
    }
    printk(KERN_ALERT"cdev_add done");
    return 0;
}
void module_cleanup(void)
{
    cdev_del(tar.mydev);
    unregister_chrdev_region(major, 1);
    printk(KERN_ALERT "unregister %d\n", MAJOR(major));
}
module_init(module_setup);
module_exit(module_cleanup);
MODULE_LICENSE("GPL");