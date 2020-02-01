#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define READER "reader"
#define WRITER "writer"
#define TEST_WRITER "test_writer"

void produce(int n);

void producer(int fd[], int n);
void waitfor(int n);

void eexit(int errN, char * str){
	fprintf(stderr, "%s", str);
	exit(errN);
}

int main(int argc, char * argv[]){
	int i,r;    

	int readersN, writersN;
	r= scanf("%u", &readersN);
	if (r!=1) eexit(1, "Input format error.\n");
	r= scanf("%u", &writersN);
	if (r!=1) eexit(1, "Input format error.\n");

	if ((readersN<=0) || (writersN<=0)){
		eexit(1, "Invalid numbers.\n");
	}

	int wdelays[writersN];
	for (i=0; i<writersN; i++){
		r= scanf("%u", &(wdelays[i])); 
		if (r!=1){
		        eexit(1, "Input format error.\n");
		}
	}

    
	int test=0;
	if ((argc==2)&&(strncmp("--test", argv[1],7)==0)){
		test=1;
	}

	if (!test){
		// generate readers
		char buf[20];
		int o=    sprintf(buf,"%s.out.",READER);
		for (i=0; i< readersN; i++){
			if (!fork()){
				sprintf(buf+o, "%d",i);
				close(1);
				open( buf, O_CREAT|O_TRUNC|O_WRONLY,S_IWUSR|S_IRUSR);
		        
				close(0);
				execl(READER,READER,NULL);
				exit(1);
			}        
		}
	}


	// generate producers and writers

	close(3); close(4);
	fcntl(0,F_DUPFD,3);
	fcntl(0,F_DUPFD,4);

	struct timespec ts;
	char buf[10];
	sprintf(buf,"%d", writersN);

	for (i=0; i< writersN; i++){
		int fd1[2];
		int fd2[2];

		ts.tv_sec= wdelays[i]/1000;
		ts.tv_nsec= (wdelays[i]%1000)*1000000;
		nanosleep(&ts, NULL);

		if (pipe(fd1)==-1) eexit(1,"Pipe creation failed.\n");
		if (pipe(fd2)==-1) eexit(1,"Pipe creation failed.\n");

		if (!fork()){
			close(fd2[0]); close(fd2[1]);
			producer(fd1,2*i);
		}

		if (!fork()){
			close(fd1[0]); close(fd1[1]);
			producer(fd2,2*i+1);
		}

		if (!fork()){
			close(3);
			close(4);
			fcntl(fd1[0],F_DUPFD,3);
			fcntl(fd2[0],F_DUPFD,4);
			close(0); 
			close(fd1[0]); close(fd1[1]);
			close(fd2[0]); close(fd2[1]);
			if (!test){
				close(1); 
				execl(WRITER, WRITER, buf, NULL);
			} else {
				execl(TEST_WRITER, TEST_WRITER, NULL);
			}

			exit(1);
		}    
		close(fd1[0]); close(fd1[1]);
		close(fd2[0]); close(fd2[1]);        
	}

	if (test) waitfor(3*writersN);
	else waitfor(3*writersN+readersN);
	return 0;
}

void waitfor(int n){
	while (n>0){
		if (wait(NULL)>0) n--;
	}
}

void producer(int fd[], int n){
	close(0);
	close(1);
	close(fd[0]);
	fcntl(fd[1],F_DUPFD,1);
	close(fd[1]);
	produce(n);
	exit(0);
}
