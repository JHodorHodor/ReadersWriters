#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

char * wr_connection = "/tmp/wr_connection";
char * writers_manager = "/tmp/writers_manager";

int main(int argc, char * argv[]){

    int writers_number = atoi(argv[1]);

    if(mkfifo(wr_connection, 0644) < 0){
        if(errno != EEXIST){
			fprintf(stderr,"WR fifo creation FAILED.\n");
            exit(1);
        }
    }

    int write_fd = open(wr_connection, O_WRONLY);

    char buf[4];
    fd_set read_fds;

    bool done3;
    bool done4;

    while(!done3 || !done4){

        FD_ZERO(&read_fds);
        FD_SET(3, &read_fds);
        FD_SET(4, &read_fds);

        int ret = select(50, &read_fds, NULL, NULL, NULL);

        if(ret == 0){
            break;
        }

        if(ret == -1){
            if (errno == EINTR){
                continue;
			} else {
				fprintf(stderr,"Read from producer FAILED.\n");
				exit(1);
			}
        } 
        
        if(ret > 0){
            if(FD_ISSET(3, &read_fds)){
                int len = read(3, &buf, 4);
                if(len == 0) done3 = true;
                if(len > 0) write(write_fd, buf, 4);
            }

            if(FD_ISSET(4, &read_fds)){
                int len = read(4, &buf, 4);
                if(len == 0) done4 = true;
                if(len > 0) write(write_fd, buf, 4);
            }
        }
    }

    bool librarian = false;
    int lib_fd = -1;
    int wr_fd = -1;
    char done[1];
    done[0] = 1;

    if(writers_number > 1){
        if(mkfifo(writers_manager, 0644) < 0){
            if(errno != EEXIST){
                fprintf(stderr,"WM fifo creation FAILED.\n");
                exit(1);
            }
        } else {
            librarian = true;
        }
    }

    if(librarian) {
        lib_fd = open(writers_manager, O_RDONLY);
        writers_number--;

        while(true){

            FD_ZERO(&read_fds);
            FD_SET(lib_fd, &read_fds);
            int ret = select(50, &read_fds, NULL, NULL, NULL);

            if(ret > 0){
                if(FD_ISSET(lib_fd, &read_fds)){
                    int len = read(lib_fd, &done, 1);
                    if(len > 0 && --writers_number == 0) break;
                }
            }

            else {
                if (ret == -1 && errno == EINTR){
                    continue;
                } else {
                    fprintf(stderr,"Read from producer FAILED.\n");
                    exit(1);
                }
            }
        }

        close(lib_fd);
        unlink(writers_manager);
        close(write_fd);
        unlink(wr_connection);
    }

    else {
        close(write_fd);

        if(writers_number > 1){
            wr_fd = open(writers_manager, O_WRONLY);
            write(wr_fd, done, 1);
            close(wr_fd);
        }
    }



}
