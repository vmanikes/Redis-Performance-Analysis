#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "/usr/include/hiredis/hiredis.h"

int main(){
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

    const char* c1="SET name j";
    const char* c2="GET name";
    const char* c3="SET name chaitanya";

   	redisAppendCommand(c,"set name %s","chaitanya");
   	redisAppendCommand(c,"get name"); 

   	redisGetReply(c,(void**)&reply);
   	printf("PING: %s\n",reply->str);
   	return 0;
}