//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////



///////////////////Set the ip address of the UDP Server in SEVER #define below ///////////////////////////////

////////////////////Un-comment "localCommunication" #define below if you are communicating with in windows

//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////





#include<stdio.h>

#include<winsock2.h>



/*#pragma comment(lib,"ws2_32.lib") //Winsock Library*/

#pragma comment(lib,"ws2_32.dll")

//#define SERVER "192.168.52.101"  //ip address of udp server

//#define SERVER "10.53.77.10"
//#define SERVER "10.10.71.159"

#define BUFLEN 512  //Max length of buffer

#define PORT 8888   //The port on which to listen for incoming data

//#define PORT 61557	//for LabView

#define localCommunication



int main(void)

{

    struct sockaddr_in si_other;

    int s, slen=sizeof(si_other);

    char buf[BUFLEN];

    char message[BUFLEN];

    WSADATA wsa;



    //Initialise winsock

    printf("\nInitialising Winsock...");

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)

    {

        printf("Failed. Error Code : %d",WSAGetLastError());

        exit(EXIT_FAILURE);

    }

    printf("Initialised.\n");



    //create socket

    if ( (s=socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP)) == SOCKET_ERROR)

    {

        printf("socket() failed with error code : %d" , WSAGetLastError());

        exit(EXIT_FAILURE);

    }
	int nOpt = 1;
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&nOpt, sizeof(int));


    //setup address structure

    memset((char *) &si_other, 0, sizeof(si_other));

    si_other.sin_family = AF_INET;

    si_other.sin_port = htons(PORT);

    //si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
	si_other.sin_addr.S_un.S_addr = inet_addr(INADDR_ANY);
	//si_other.sin_addr.s_addr = INADDR_ANY;
	si_other.sin_addr.s_addr = INADDR_BROADCAST;



    //start communication

    while(1)

    {

        printf("Enter message : ");

        gets(message);



        //send the message

        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)

        {

            printf("sendto() failed with error code : %d" , WSAGetLastError());

            exit(EXIT_FAILURE);

        }



#ifdef localCommunication

        //receive a reply and print it

        //clear the buffer by filling null, it might have previously received data

        memset(buf,'\0', BUFLEN);

        //try to receive some data, this is a blocking call

        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)

        {

            printf("recvfrom() failed with error code : %d" , WSAGetLastError());

            exit(EXIT_FAILURE);

        }



        puts(buf);

#endif



    /*Sleep(1000);*/

    }





    closesocket(s);

    WSACleanup();



    return 0;

}