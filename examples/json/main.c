#include <stdio.h>
#include "json.h"

int main(int agv,const char *ags[]){
    if(agv != 3){
        fprintf(stderr,"invalid arguments.\n");
        fprintf(stderr,"Usage: %s <json file> <pattern> \n",ags[0]);
        return -1;
    }
    int ret = 0;
    FILE *json = fopen(ags[1],"ro");
    jsonval val;
    if(!JSON_parse(json,&val,stderr)){
        jsonval *matched = JSON_match(&val,ags[2],stderr);
        if(matched != NULL){        
            JSON_stringify(matched,0,stdout);
            printf("\n");
        }
    }
    else {
        ret = -1;
    }
    JSON_free(&val);
    fclose(json);
    
    return ret;
}
