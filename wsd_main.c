/*

 * =====================================================================================

 *

 *       Filename:  wsd_main.c

 *

 *    Description:

 *

 *        Version:  1.0

 *        Created:  8/2/2013 8:27:42 AM

 *       Revision:  none

 *       Compiler:  gcc

 *

 *         Author:  Zhi-wei YAN (Jerod YAN), jerod.yan@gmail.com

 *        Company:  DrumTm

 *

 * =====================================================================================

 */



#include	<stdlib.h>

#include	<stdio.h>

#include	<assert.h>

#include	<pthread.h>

#include	"mxml.h"

#include	"SDL.h"

#include	"SDL_ttf.h"

#include	"SDL_draw.h"

#include 	"init_display.h"

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



#define MAX_SIZE 256

#define Y_VERTICAL 80

#define X_HORIZON 245

#define Y_STEP 30

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

static int exit_flag = 0;

void * _Do_event(void)

{



    SDL_Event g_Event;

    exit_flag = 0;

    do {



        //adjust the sampling rate

        if (SDL_PollEvent(&g_Event) == 0) {

            SDL_Delay(200);

        }



        if (g_Event.type== SDL_QUIT) {

            exit_flag++;

            break;

        }



        if (g_Event.type==SDL_KEYDOWN && SDLK_ESCAPE == g_Event.key.keysym.sym) {

            exit_flag++;

            break;

        }



    } while (1);



}



int main (int argc, char *argv[] )

{

    int err = 0;

    err = 0;



    char buff[MAX_SIZE];





//    FILE * ctt = fopen("CON", "w" );

//    if (ctt) {

//        freopen("CON", "w", stdout);

//        freopen("con", "w", stderr);

//    } else {

//        fclose( ctt );

//    }



    //SDL libary

    SDL_Surface *screen = NULL;

    screen = InitXmlScr(".//cfg_files//cfg.xml");

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



    //welcome_sd = socket(AF_INET, SOCK_STREAM, 0);
    connection_sd = socket(AF_INET, SOCK_DGRAM, 0);



    /* Elimitate the message "Address already in use" */

    if (setsockopt(welcome_sd,SOL_SOCKET,SO_REUSEADDR, &reuse, sizeof(int))<0) {

        printf("setsockopt,  error, IP address already in use.");

    }

    do {

        if (0==bind(connection_sd,(struct sockaddr *)&sa,sizeof(struct sockaddr_in))) {
//        if (0==bind(welcome_sd,(struct sockaddr *)&sa,sizeof(struct sockaddr_in))) {

            /* success for bind operation */

            break;

        } else {

            printf("\nDatabase program attempts to bind port after 5 seconds.\n");

            sleep(5);

            continue;

        }

    } while (1);



    listen(welcome_sd,BACKLOG);



    SDL_Event g_Event;



    T_Text bg_green_text_info;

    T_Text bg_brown_text_info;



    memset(&bg_green_text_info, 0, sizeof(T_Screen));

    memset(&bg_brown_text_info, 0, sizeof(T_Screen));





    // for background green

    bg_green_text_info.size = 13;

    bg_green_text_info.pos_mid = 0;

    bg_green_text_info.bg_color = 0x2D6E32;

    bg_green_text_info.fg_color = 0xFCFCFC;

    strcpy(bg_green_text_info.font, "font_2.ttf");

    strcpy(bg_green_text_info.path, ".//fonts//");



    // for background brown

    bg_brown_text_info.size = 13;

    bg_brown_text_info.pos_mid = 0;

    bg_brown_text_info.bg_color = 0x2D2200;

    bg_brown_text_info.fg_color = 0xFCFCFC;

    strcpy(bg_brown_text_info.font, "font_2.ttf");

    strcpy(bg_brown_text_info.path, ".//fonts//");







    T_Screen scr_info;

    memset(&scr_info, 0, sizeof(T_Screen));



    scr_info.width = 975;

    scr_info.height = 580;

    scr_info.bg_color = 0xFFFFFF;

    scr_info.full_scr = 0;

    scr_info.bpp = 32;

    long cnt = 0;

    do {



        //adjust the sampling rate

        

        if (SDL_PollEvent(&g_Event) == 0) {

            SDL_Delay(200);

            SDL_Flip(screen);

        }



        if (g_Event.type== SDL_QUIT) {

            break;

        }



        if (g_Event.type==SDL_KEYDOWN && SDLK_ESCAPE == g_Event.key.keysym.sym) {

            break;

        }

         

        int index;



        int trans_addr;

        float trans_pysical_value;

        int trans_pysical_unit;



        int wnic_addr;

        float wnic_battery_capacity;


/* 
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
*/
       
    fd_set readfd;
    int recv_flag = 0;
    struct timeval timeout;
    struct sockaddr_in from_addr;

    while(1) {
        FD_ZERO(&readfd);
        FD_SET(connection_sd, &readfd);

        timeout.tv_sec = 2; //sec
        timeout.tv_usec = 0; //mic sec

        int ret = select(connection_sd+1, &readfd, NULL, NULL, &timeout);
        switch(ret) {
        case -1:
            printf("select error!\n");
            break;
        case 0:
            printf(".");
            fflush(NULL);
            break;
        default:
            if( FD_ISSET(connection_sd, &readfd) ) {

                memset(buf_recv,0,MAX_SIZE_BUFFER_RECV);
                ret = recvfrom(connection_sd, buf_recv, MAX_SIZE_BUFFER_RECV-1, 0, (struct sockaddr *)&from_addr, &len);
                if(0 > ret) {
                    perror("Recvfrom UDP broadcast.");
                    break;
                } else {
                    printf("\nMsg[%ld]: |%s|, from IP %s.\n", cnt++, buf_recv, inet_ntoa(from_addr.sin_addr));
                    recv_flag = 1;
                }
            }
            break;
        }
        if (1==recv_flag){
            recv_flag = 0;
            break;
        }
    }

        
       sscanf(buf_recv, "%d,%d,%f,%d,%d,%f",

                  &index,

                  &trans_addr,&trans_pysical_value,&trans_pysical_unit,

                  &wnic_addr, &wnic_battery_capacity);

       //output to the computer display

       //trans_addr

        //index--;

        if(0>=index){

            index =0;

        }

        bg_green_text_info.start_point.x = X_HORIZON+10;

        bg_green_text_info.start_point.y = Y_VERTICAL + index*Y_STEP;



        memset(buff, 0, MAX_SIZE);

        sprintf(buff, "[%d] %10d", index, trans_addr);

        printf( "%d %d\n", index, trans_addr);

        strncpy(bg_green_text_info.content, buff, strlen(buff)+1 );

        err = Draw_text(screen, &bg_green_text_info, &scr_info);



        //physical value

        bg_brown_text_info.start_point.x = X_HORIZON + 220;

        bg_brown_text_info.start_point.y = Y_VERTICAL + index*Y_STEP;



        memset(buff, 0, MAX_SIZE);

        sprintf(buff, "%f  ", trans_pysical_value);

        strncpy(bg_brown_text_info.content, buff, strlen(buff)+1 );

        err = Draw_text(screen, &bg_brown_text_info, &scr_info);

        

         

        //physical value unit

        bg_brown_text_info.start_point.x = X_HORIZON + 370;

        bg_brown_text_info.start_point.y = Y_VERTICAL + index*Y_STEP;

        memset(buff, 0, MAX_SIZE);

        if(11==trans_pysical_unit){

            sprintf(buff, "%s", "Pa \0");

        }

        else if(254==trans_pysical_unit){

            sprintf(buff, "%s", "KPa\0");

        }

        else{

            sprintf(buff, "%s", "N/A\0");

        }

        strncpy(bg_brown_text_info.content, buff, strlen(buff)+1 );

        err = Draw_text(screen, &bg_brown_text_info, &scr_info);

        SDL_Flip(screen);

        



        //Net address

        bg_green_text_info.start_point.x = X_HORIZON+420;

        bg_green_text_info.start_point.y = Y_VERTICAL + index*Y_STEP;



        memset(buff, 0, MAX_SIZE);

        sprintf(buff, "%d ", wnic_addr);

        printf("%d", wnic_addr);

        strncpy(bg_green_text_info.content, buff, strlen(buff)+1 );

        err = Draw_text(screen, &bg_green_text_info, &scr_info);



        //Battery Capacity

        bg_brown_text_info.start_point.x = X_HORIZON + 610;

        bg_brown_text_info.start_point.y = Y_VERTICAL + index*Y_STEP;



        memset(buff, 0, MAX_SIZE);

        sprintf(buff, "%.2f%%", wnic_battery_capacity);

        strncpy(bg_brown_text_info.content, buff, strlen(buff)+1 );

        err = Draw_text(screen, &bg_brown_text_info, &scr_info);

        SDL_Flip(screen);



    } while (1);





    printf("==Press any key to end!\n");



    return 0;

}

/* ----------  end of function main  ---------- */

