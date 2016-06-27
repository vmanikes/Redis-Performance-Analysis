//Make the file to be stored in the directory downloaded for coding steps:- hiredis
//make ./hiredis-<filename>
//./<filename>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "/usr/include/hiredis/hiredis.h"
int random_number();

clock_t begin ,end;
double timesp;
int main(void) {
    
    redisContext *c;
    redisReply *reply;
    int j;
    
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
    
    reply=redisCommand(c,"MULTI");
    printf("PING: %s\n",reply->str);
    freeReplyObject(reply);
    begin=clock();
    for(int i=0;i<1000000;i++){
        reply=redisCommand(c,"SET %s %d","name",1);
        freeReplyObject(reply);

        reply=redisCommand(c,"GET %s","name");
        freeReplyObject(reply);        
    }

    reply=redisCommand(c,"EXEC");
    printf("PING: %s\n",reply->str);
    freeReplyObject(reply);
    end=clock();
    timesp=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("INSERT TIME:%f Seconds\n",timesp);
    printf("----------");
 
  	return 0;
}

int random_number(){
    int i;
    i=(rand()%50)+i*2;   
    return i;
}