#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int open (const char *name, int flags);
int open (const char *name, int flags, mode_t mode);

int fd;
fd = open ("/home/kidd/madagascar", O_RDONLY);
if (fd == -1)

