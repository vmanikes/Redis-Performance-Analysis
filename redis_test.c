#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "/usr/include/hiredis/hiredis.h"

int32_t convert_binary(int);
int32_t convert_hex(long);

#define  PORT_CNT   50000
#define  IP_CNT     10

uint16_t arrLPort[PORT_CNT][IP_CNT];
uint16_t arrRPort[PORT_CNT][IP_CNT];
uint32_t ndxLPort[IP_CNT];
uint32_t ndxRPort[IP_CNT];
uint32_t arrLip[IP_CNT]={0x0A0A0A0A,0x0A0A0A0B,0x0A0A0B0B,0x0A0B0B0B,0x0B0B0B0B,0x0B0B0B0C,0x0B0B0C0C,0x0B0C0C0C,0x0C0C0C0C,0x0C0C0C0D};
uint32_t arrRip[IP_CNT]={0x0B0A0A0A,0x0B0B0A0A,0x0B0B0B0A,0x0B0B0B0B,0x0C0B0B0B,0x0C0C0B0C,0x0C0C0C0B,0x0C0C0C0C,0x0D0C0C0C,0x0D0D0C0D};

clock_t begin;
clock_t end;
double timesp;

int genkey(char* key)
{
	uint32_t  lip;
	uint32_t  rip;
	uint16_t  lport;
	uint16_t  rport;
	int ndx;
	//Select Random IP
    ndx = random()%10;
    lip = arrLip[ndx];
    ndx = random()%10;
    rip = arrRip[ndx];
    ndx = random()%ndxLPort;
    lport = arrLPort[ndx];
    arrLPort[ndx] = arrLPort[--ndxLPort];
    ndx = random()%ndxRPort;
    rport = arrRPort[ndx];
    arrRPort[ndx] = arrRPort[--ndxRPort];
    snprintf(key, 25, "%08lX%08lX%04hX%04hX", lip, rip, lport, rport);  
    return 0;
}


void main(){
	char  key[25];
	char  data[25];
	int j = 0;
	int ndx;
	int i;
	int holder=10000;
	FILE *f=fopen("/home/kchaitanya/file.txt","w");

	redisContext *c;
    redisReply *reply;

    //Connection with the redis databas
	c = redisConnect("127.0.0.1", 6379);
	if (c->err) {
        	printf("Error: %s\n", c->errstr);
    }else{
        	printf("Connection Made! \n");
    }

    //PING THE SERVER
    reply=redisCommand(c,"PING");
    printf("PING: %s\n",reply->str);
    freeReplyObject(reply);

	for( i=0; i<PORT_CNT; i++){
		for(j=0;j<IP_CNT;j++){
			arrLPort[i][j] = holder;
			arrRPort[i][j] = holder;
		}
		holder++;
	}

	for( i=0; i<PORT_CNT; i++){
		for(j=0;j<IP_CNT;j++){
			printf("%d\t",arrLPort[i][j]);
		}
		printf("\n");
	}
	ndxLPort = PORT_CNT;
	ndxRPort = PORT_CNT;
	//50k values are now stored
	begin=clock();
	for( j = 0; j < 100; j++ )
	{
		genkey( key );
		strcpy( data, key );
		printf("%s\n", key );

		//send to REDIS
		reply=redisCommand(c,"SET  %s %s",key,data); //Redis set
        freeReplyObject(reply);
       reply=redisCommand(c,"GET  %s",key); //Redis get

        if( strcmp(key,reply->str) == 0 ){
        	fprintf(f,"success %s: %s\n",key,reply->str);
        }
        else{
          	printf("failure");
        	perror("err");      	
        }
	}
}





