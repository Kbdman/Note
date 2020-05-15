#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
    int fd = open("/dev/chardev", O_RDWR);
    if (fd == -1) {
        perror("Fopen ");
    }
    if (write(fd, "1234", 4) < 0) {
        perror("write");
    }
    sleep(10);
    if (write(fd, "4321", 4) < 0) {
        perror("write");
    }
    sleep(10);
    char buf[1024] = {0};
    int res = read(fd, buf, 10);
    if (res == -1) {

        perror("read ");
    }
    printf("%s\n", buf);
    close(fd);


}