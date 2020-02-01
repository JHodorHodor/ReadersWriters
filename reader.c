#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

char * wr_connection = "/tmp/wr_connection";

int main(int argc, char * argv[]){

    if(mkfifo(wr_connection, 0644) < 0){
        if(errno != EEXIST){
			fprintf(stderr,"WR fifo creation FAILED.\n");
            exit(1);
        }
    }

    int read_fd = open(wr_connection, O_RDONLY);

    char buf[4];
    char buf2[20];
    while(true){

        int len = read(read_fd, &buf, 4);

        if(len == -1){
			if (errno == EINTR){
                continue;
			} else {
				fprintf(stderr,"Read from wr FAILED.\n");
				exit(1);
			}
        }
        
        else if(len == 0) {
            break;
        }

        else if(len == 4) {
            int val = *(int*)buf;
            int n = sprintf(buf2,"%d\n",val);
            write(1,buf2,n);
        }

        else {
			fprintf(stderr,"Boundary ERROR.\n");
			exit(1);
        }
    }

    close(read_fd);

}
