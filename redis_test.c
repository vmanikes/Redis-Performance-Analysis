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

uint16_t arrLPort[IP_CNT][PORT_CNT];
uint16_t arrRPort[IP_CNT][PORT_CNT];
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
	int ndxIp;
	//Select Random IP
    ndxIp = random() % IP_CNT;
    lip = arrLip[ndxIp];
    ndx = random()%ndxLPort[ndxIp];
    lport = arrLPort[ndxIp][ndx];
    arrLPort[ndxIp][ndx] = arrLPort[ndxIp][--ndxLPort[ndxIp]];
 
    ndxIp = random() % IP_CNT;
    rip = arrRip[ndxIp];
    ndx = random() % ndxRPort[ndxIp];
    rport = arrRPort[ndxIp][ndx];
    arrRPort[ndxIp][ndx] = arrRPort[ndxIp][--ndxRPort[ndxIp]];
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

	for( i=0; i<IP_CNT; i++){
		for(j=0;j<PORT_CNT;j++){
			arrLPort[i][j] = 10000+j;
			arrRPort[i][j] = 10000+j;
		}
		ndxLPort[i] = PORT_CNT;
		ndxRPort[i] = PORT_CNT;
	}

	//50k values are now stored
	begin=clock();
	
	//
	for( j = 0; j < 100000; j++ )
	{
		genkey( key );
		strcpy( data, key );
		

		//send to REDIS
		reply=redisCommand(c,"SET  %s %s",key,data); //Redis set
        freeReplyObject(reply);
	}

	// 
}





