#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>



void* doSomeThing(void *arg)
{
	char *arguments = (char*)arg;
    unsigned long i = 0;
	printf("\n First thread processing\n");

    for(i=0; i<(0xFFFFFFFF);i++);

    return NULL;
}



int main(void)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

	
// 	pid_t pid=fork();
	// if (pid==0) { /* child process */
     //   static char *argv[]={"echo","Foo is my name.",NULL};
      //  execv("/bin/echo",argv);
       // exit(127); /* only if execv fails */
//    }
 //   else { /* pid!=0; parent process */
   //     waitpid(pid,0,0); /* wait for child to exit */
    //}
	
	

	 int err;
	 static char *argv[]={"ABC",NULL};
	 pthread_t tid; // thread id
	 
	err = pthread_create(&(tid), NULL, &doSomeThing, (void *)argv);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));
	else
		printf("\n Thread created successfully\n");


    
	
	
    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // close connection
    close(client);
    close(s);
    return 0;
}