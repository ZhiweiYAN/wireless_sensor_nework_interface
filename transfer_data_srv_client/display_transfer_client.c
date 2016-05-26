#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "192.168.95.10"
#define SERVER_PORT 38002
#define MAX_SIZE_BUFFER_RECV 512
#define MAX_SIZE_BUFFER_SEND 512
#define BUFFER_SIZE 512
#define BACKLOG 20

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  send_msg_to_server
 *  Description:
 * =====================================================================================
 */
int send_msg_to_server(char* pay_load, int pay_load_size)
{
    int ret = 0;
    int count = 0;
    int msg_len = 0;

    int sd = 0;
    struct sockaddr_in sa;
    bzero(&sa, sizeof(sa));

    char srv_ip_address[16]=SERVER_IP;
    int srv_port = SERVER_PORT;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(srv_port);
    sa.sin_addr.s_addr = inet_addr(srv_ip_address);

    //create socket for communication
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket initialization in Communicate_parse_server, failed.");
        return -1;
    }

    //connect to the parse server with ip and port.
    ret = connect(sd, (struct sockaddr*)&sa, sizeof(sa));
    if(ret<0) {
        printf("Failed, to connect the parse server: %s:%d.\n", srv_ip_address, srv_port);
        close(sd);
        return -1;
    } else {
        printf("Success, to connect the parse server: %s:%d.\n", srv_ip_address, srv_port);
    }

    //send the original packet
    char *send_buffer = NULL;

    send_buffer = (char*)malloc(BUFFER_SIZE);
    if(NULL==send_buffer) {
        printf("Memeory mallloc, NULL");
        ret = -1;
        goto LABEL_C_P;
    }
    bzero(send_buffer, BUFFER_SIZE);

    memcpy(send_buffer, pay_load, pay_load_size);
    //copy data from standard IO
    //scanf();
    msg_len = pay_load_size;

    //send the inner packet header and raw packet to the parse server.
    count = send(sd, send_buffer, msg_len, 0);
    if (count < msg_len) {
        printf("Failed to send to parse server %s with %d bytes: |%s|.\n", srv_ip_address,count, send_buffer);
        ret = -1;
        goto LABEL_C_P;
    } else {
        printf("Success to send to parse server %s with %d bytes:\n", srv_ip_address,count);
    }

    //colse socket.
LABEL_C_P:

    if(NULL!=send_buffer) {
        free(send_buffer);
        send_buffer = NULL;
    }
    close(sd);

    return ret;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  recv_msg_from_client
 *  Description:
 * =====================================================================================
 */

int recv_msg_from_client(void)
{
    int connection_sd = 0;
    int reuse = -1;
    socklen_t len = 0;
    int count = 0;
    char buf_recv[MAX_SIZE_BUFFER_RECV];

    int port = SERVER_PORT;
    int welcome_sd = 0;
    struct sockaddr_in sa;

    /* Bind the socket to the port, and allow all IP connect with it */
    bzero(&sa,sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    welcome_sd = socket(AF_INET, SOCK_STREAM, 0);

    /* Elimitate the message "Address already in use" */
    if (setsockopt(welcome_sd,SOL_SOCKET,SO_REUSEADDR, &reuse, sizeof(int))<0) {
        printf("setsockopt,  error, IP address already in use.");
    }
    do {
        if (0==bind(welcome_sd,(struct sockaddr *)&sa,sizeof(struct sockaddr_in))) {
            /* success for bind operation */
            break;
        } else {
            printf("\nDatabase program attempts to bind port after 5 seconds.\n");
            sleep(5);
            continue;
        }
    } while (1);

    listen(welcome_sd,BACKLOG);

    while(1) {

        if (( connection_sd = accept(welcome_sd,(struct sockaddr*)&sa,&len))<0) {
            printf("Error happens when socket function <accept> is running. We will close the socket at once and delay 2 ms and restart.");
            close(connection_sd);
            sleep(2);
            continue;
        }

        memset(buf_recv, 0, MAX_SIZE_BUFFER_RECV);
        count = recv(connection_sd,buf_recv,MAX_SIZE_BUFFER_RECV,0);
        if(0>count) {
            printf("recv data, error.\n");
        } else {
            printf("%s\n", buf_recv);
        }

        close(connection_sd);
    }
    return 1;
}

int main(int argc, char* argv[])
{
    int ret = 0;
    
    char prefix;

    int index;

    int trans_addr;
    float trans_pysical_value;
    int trans_pysical_unit;

    int wnic_addr;
    float wnic_battery_capacity;

    char payload[MAX_SIZE_BUFFER_SEND];

    ret = strncmp("send", argv[1], 4);


    if(0==ret) {
        while(1) {
            //send msg
            scanf("%c,%d,%d,%f,%d, \
                    %d,%f",
                  &prefix,
                  &index,
                  &trans_addr,&trans_pysical_value,&trans_pysical_unit,
                  &wnic_addr, &wnic_battery_capacity);
            if('~'!=prefix){
                continue;
            }

            memset(payload, 0, MAX_SIZE_BUFFER_SEND);
            sprintf(payload, "%d,%d,%f,%d,%d,%f",
                    index,
                    trans_addr,trans_pysical_value,
                    trans_pysical_unit,
                    wnic_addr,
                    wnic_battery_capacity);
            printf("payload: |%s|\n", payload);
            ret = send_msg_to_server(payload, strlen(payload));
        }
    } else {
        //recv msg
        ret = recv_msg_from_client();
    }
    return 1;
}
