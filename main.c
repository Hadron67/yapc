#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "yapc.h"

typedef struct _YConfig{
    const char *input;
    const char *output;
}YConfig;


static int parseArg(YConfig *config,int agv,const char *ags[]){
    int i;
    memset(config,0,sizeof(YConfig));
    for(i = 1;i < agv;i++){
        if(!strcmp(ags[i],"--output")){
            i++;
            if(i < agv){
                config->output = ags[i];
            }
            else {
                fprintf(stderr,"--output requires one argument.\n");
                return -1;
            }
        }
        else {
            if(config->input != NULL){
                fprintf(stderr,"exactly one input file is required.\n");
                return -1;
            }
            else {
                config->input = ags[i];
            }
        }
    }
    //config->s = (char *)malloc(sizeof(char) * (strlen(config->input) + 6));

    return 0;
}



int main(int agv,const char *ags[]){
    int ret = 0;
    YConfig config;
    if(parseArg(&config,agv,ags)){
        return -1;
    }

    if(config.input == NULL){
        fprintf(stderr,"%s: fatal error: no input file.\n",ags[0]);
        return -1;
    }

    FILE *f = fopen(config.input,"ro");
    
    if(f == NULL){
        fprintf(stderr,"%s: fatal error: %s: %s\n",ags[0],ags[1],strerror(errno));
        return -1;
    }

    void *yctx = yNewContext();

    if(!yParseFile(yctx,f,ags[1],stderr)){
        const char *sourcen,*headern,*outn;
        FILE *out;

        yConvertFileNames(yctx,config.input,&headern,&sourcen,&outn);

        if(config.output != NULL){
            out = fopen(config.output,"wo");
        }
        else {
            out = fopen(outn,"wo");
        }

        yGenerateTable(yctx);
        yPrintGenerationWarnings(yctx,out);
        yPrintResult(yctx,out);
        yPrintTestResult(yctx,out);

        yPrintGenerationWarnings(yctx,stdout);

        FILE *source = fopen(sourcen,"wo");
        FILE *header = fopen(headern,"wo");
        yGenerateCCode(yctx,header,source,headern,sourcen);
        fclose(out);
        fclose(source);
        fclose(header);
    }
    else {
        fprintf(stderr,"%s: error: failed to parse grammar file.\n",ags[0]);
        ret = -1;
    }

    yDestroyContext(yctx);

    fclose(f);

    return ret;
}
