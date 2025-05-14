#include "complex_ext.h"
#include "complex.h"
#include "complex_trace.h"
#include "complex_trace.c"
#include "complex_dbg.h"

#ifdef _WIN32
#include <winsock2.h> // for the trace generation
#include <ws2tcpip.h> // for IP_MULTICAST_TTL, IP_ADD_MEMBERSHIP
typedef unsigned long in_addr_t; // Define in_addr_t for Windows
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <net/if.h>
#include <ifaddrs.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function prototype
in_addr_t get_interface_ip(void);

// the following definitions are for the trace generation
#define DEFAULT_PORT 4445
#define MULTICAST_ADDR "239.1.1.1"

#ifdef _WIN32
int rc;
SOCKET s;
WSADATA wsa;
SOCKADDR_IN addr;
unsigned long ulAddr = INADDR_NONE;
#else
int rc;
int s;
struct sockaddr_in addr;
#endif

// end trace defs

COMPLEX_INSTANCEDATA_T instData = COMPLEX_INSTANCEDATA_INIT;

// Function to get interface IP address
in_addr_t get_interface_ip(void) {
    // Use INADDR_ANY to bind to any available interface
    return INADDR_ANY;
}

// Helper function to send a reset message to the simulator
void sendResetToSimulator(){
    char buffer[255];
    sprintf(buffer, "from=guisim_client_console_app;to=__ALL__;msg=__INIT__\n");
#ifdef _WIN32
    rc=sendto(s, buffer, strlen(buffer), 0, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
    if(rc==SOCKET_ERROR)
    {
        printf("Error: Could not send bytes: %d\n",WSAGetLastError());
    }
#else
    rc = sendto(s, buffer, strlen(buffer), 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if(rc < 0) {
        printf("Error: Could not send bytes: %s\n", strerror(errno));
    }
#endif
}

// Called from the state machine
// Used to send a trace message to the GUI simulator via UDP multicast messages
void complexTraceEvent(uint16_t inst_id, uint16_t evt){
    char buffer[255]={0};

    // You must use the complexTraceEvents[] array here to map to string.
    // The complexTraceEvents[] array is defined in complex_trace.c
    sprintf(buffer, "from=guisim_client_console_app;to=__ALL__;msg=%s\n", complexTraceEvents[evt]);

#ifdef _WIN32
    rc=sendto(s, buffer, strlen(buffer), 0, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
    if(rc==SOCKET_ERROR)
    {
        printf("Error: Could not send bytes: %d\n",WSAGetLastError());
    }
#else
    rc = sendto(s, buffer, strlen(buffer), 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if(rc < 0) {
        printf("Error: Could not send bytes: %s\n", strerror(errno));
    }else{
        printf("Sent event: %s\n", buffer);
    }
#endif
}

// Initialize the trace. The code considers the platform (Windows or Linux/MacOS)
int initializeTrace(void){
#ifdef _WIN32
    rc=WSAStartup(MAKEWORD(2,0),&wsa);
    if(rc!=0)
    {
        printf("Error: Could not start Winsock: %d\n",rc);
        return 1;
    } 
      
    s=socket(AF_INET,SOCK_DGRAM,0);
    if(s==INVALID_SOCKET)
    {
        printf("Error: Could not create socket: %d\n",WSAGetLastError());
        return 1;
    }

    // Set up multicast
    int ttl = 1;
    if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl)) < 0) {
        printf("Error: Could not set TTL: %d\n", WSAGetLastError());
        return 1;
    }

    // Allow multiple sockets to use the same port
    int reuse = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0) {
        printf("Error: Could not set SO_REUSEADDR: %d\n", WSAGetLastError());
        return 1;
    }

    // Join multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr = INADDR_ANY;
    if(setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0) {
        printf("Error: Could not join multicast group: %d\n", WSAGetLastError());
        return 1;
    }

    memset(&addr,0,sizeof(SOCKADDR_IN));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(DEFAULT_PORT);
    addr.sin_addr.s_addr=inet_addr(MULTICAST_ADDR);
    return 0;
#else
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s < 0) {
        printf("Error: Could not create socket: %s\n", strerror(errno));
        return 1;
    }

    // Set up multicast
    int ttl = 1;
    if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        printf("Error: Could not set TTL: %s\n", strerror(errno));
        return 1;
    }

    // Allow multiple sockets to use the same port
    int reuse = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        printf("Error: Could not set SO_REUSEADDR: %s\n", strerror(errno));
        return 1;
    }

    // Get interface IP address
    in_addr_t if_ip = get_interface_ip();

    // Join multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr = if_ip;
    if(setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        printf("Error: Could not join multicast group: %s\n", strerror(errno));
        return 1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFAULT_PORT);
    addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    return 0;
#endif
}

// message to state machine
COMPLEX_EVENT_T message[]={e14,e12,e2,e15,e6,e7,e9,e11,e1};
COMPLEX_EVENT_T msg;

int main(int argc, char* argv[]){

    
    // init socket
    if(initializeTrace()){
        exit(1);
    }
	
	complex(&instData, COMPLEX_NO_MSG);
	for(int i=0; i<(sizeof(message)/sizeof(COMPLEX_EVENT_T)); i++){		
		msg=message[i];
		complex(&instData, msg);
#ifdef _WIN32
		Sleep(1000);
#else
        sleep(1);
#endif
		printf("------------\n");
	}
	
#ifdef _WIN32
    closesocket(s);
    WSACleanup();
#else
    close(s);
#endif
	
	return 0;
}
