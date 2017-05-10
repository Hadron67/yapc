/*  
    YAPC - Yet Another Parser Compiler - An LR(1) parser generater

    Copyright (C) 2017  Chen FeiYu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "yapc.h"

typedef struct _YConfig{
    const char *input;
    const char *output;
    int showLah;
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
        else if(!strcmp(ags[i],"--show-lah")){
            config->showLah = 1;
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
    YContext *yctx = NULL;
    FILE *gram = NULL,*out = NULL,*source = NULL,*header = NULL;

    if(parseArg(&config,agv,ags)){
        return -1;
    }

    if(config.input == NULL){
        fprintf(stderr,YFATAL_ERROR_COLORED "no input file.\n");
        return -1;
    }

    gram = fopen(config.input,"ro");
    
    if(gram == NULL){
        fprintf(stderr,YFATAL_ERROR_COLORED "%s: %s\n",config.input,strerror(errno));
        ret = -1;
        goto ret;
    }

    yctx = yNewContext();

    if(!yParseFile(yctx,gram,config.input,stderr)){
        const char *sourcen,*headern,*outn;

        yConvertFileNames(yctx,config.input,&headern,&sourcen,&outn);

        outn = config.output == NULL ? outn : config.output;
        out = fopen(outn,"wo");
        if(out == NULL){
            fprintf(stderr,YERROR_COLORED "cannot create output file %s: %s\n",outn,strerror(errno));
            ret = -1;
            goto ret;
        }

        yGenerateTable(yctx);
        yPrintGenerationWarnings(yctx,out);
        yPrintItemSets(yctx,config.showLah,out);
        yPrintTestResult(yctx,out);

        yPrintGenerationWarnings(yctx,stdout);

        source = fopen(sourcen,"wo");
        header = fopen(headern,"wo");
        if(source == NULL){
            fprintf(stderr,YERROR_COLORED "cannot create source file %s: %s\n",sourcen,strerror(errno));
            ret = -1;
            goto ret;
        }
        if(header == NULL){
            fprintf(stderr,YERROR_COLORED "cannot create header file %s: %s\n",headern,strerror(errno));
            ret = -1;
            goto ret;
        }

        yGenerateCCode(yctx,header,source,headern,sourcen);
    }
    else {
        fprintf(stderr,YFATAL_ERROR_COLORED "failed to parse grammar file.\n");
        ret = -1;
        goto ret;
    }

    ret:
    if(yctx != NULL) yDestroyContext(yctx);
    if(gram != NULL) fclose(gram);
    if(out != NULL) fclose(out);
    if(source != NULL) fclose(source);
    if(header != NULL) fclose(header);
    return ret;
}
