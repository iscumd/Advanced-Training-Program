#include <stdio.h>
#include <unistd.h>
// bluetooth headers
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
// wiringPi headers
#include <wiringPi.h>
#include <wiringPiI2C.h>
//
#include <JHPWMPCA9685_RPI.h>


#define slaveaddress 0x40

int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (s == -1)
        printf("socket error \r\n");
    // bind socket to port 1 of the first available
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = (bdaddr_t) {0, 0, 0, 0, 0, 0};
    loc_addr.rc_channel = (uint8_t) 1;
    int b;
    b = bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    if (b == -1)
        printf("bind error \r\n");
    // put socket into listening mode
    int l;
    l = listen(s, 1);
    if (l == -1)
        printf("listen error \r\n");
    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    if (client == -1)
        printf("accept error \r\n");

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
	// Motor controller setup
	wiringPiSetup () ;
    PCA9685 *pca9685 = new PCA9685(slaveaddress) ;
    int err = pca9685->openPCA9685();
    if (err < 0){
        printf("Error: %d", pca9685->error);
    } else {
        printf("PCA9685 Device Address: 0x%02X\n",pca9685->kI2CAddress) ;
        pca9685->setAllPWM(0,0) ;
        pca9685->reset() ;
        pca9685->setPWMFrequency(1500) ;
        // 27 is the ESC key
      //  printf("Hit ESC key to exit\n");
      }


	unsigned char inputchar = 0;
	int i = 1100; // turn
	int j = 1300; // straight
    // read data from the client
    do {
		bytes_read = read(client, buf, sizeof(buf));

		if( bytes_read > 0 ) {
			inputchar = buf[0];
			switch (inputchar) {
				case 'l' : // left
					pca9685->setPWM(12, i,4095 - i) ;
					pca9685->setPWM(13,4095 , 0) ;
					pca9685->setPWM(14, i,4095 - i) ;
					pca9685->setPWM(15,4095 , 0) ;
				break;
				case 'r': // right
					pca9685->setPWM(12, 4095 , 0);
					pca9685->setPWM(13, i,4095 - i) ;
					pca9685->setPWM(14, 4095 , 0);
					pca9685->setPWM(15, i,4095 - i) ;
				break;
				case 't': // straight
					pca9685->setPWM(12, j, 4095 - j) ;
					pca9685->setPWM(13,4095 , 0) ;
					pca9685->setPWM(14,4095 , 0) ;
					pca9685->setPWM(15, j, 4095 - j) ;
				break;
				case 'b': // backwards
					pca9685->setPWM(12, 4095 , 0) ;
					pca9685->setPWM(13, j, 4095 - j) ;
					pca9685->setPWM(14, j, 4095 - j) ;
					pca9685->setPWM(15, 4095 , 0) ;
				break;
				case 's': // stop
					pca9685->setPWM(12,0,0) ;
					pca9685->setPWM(13,0,0) ;
					pca9685->setPWM(14,0,0) ;
					pca9685->setPWM(15,0,0) ;
				break;
			}
		}
	}
    while (inputchar != 'q');
    pca9685->setPWM(12,0,0) ;
    pca9685->setPWM(13,0,0) ;
    pca9685->setPWM(14,0,0) ;
    pca9685->setPWM(15,0,0) ;

    // close connection
    close(client);
    close(s);
    return 0;
}
