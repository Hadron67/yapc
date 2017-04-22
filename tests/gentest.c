#include "gentest.h"
static int yytokenCount = 4;
static int yyntCount = 8;
#define YYPUSH_STATE(parser,s) \
   if(parser->sLen >= parser->sSize){ \
       parser->sSize *= 2; \
       parser->state = (int *)parser->rtor(parser->state,sizeof(int) * parser->sSize); \
   } \
   parser->state[parser->sLen++] = (s);

#define YYSTATE() (yyparser->state[yyparser->sLen - 1])
/** shift action table
 * do shift if positive,reduction if negtive,and error of zero.
 * the state should be the number in the table minus one,since zero marks
 * for error.*/
static const int yyshift[] = {
/* state 0 */0,-7,0,-8,
/* state 1 */-1,0,0,0,
/* state 2 */-2,0,4,0,
/* state 3 */-4,-4,-4,-4,
/* state 4 */0,-7,0,-8,
/* state 5 */-3,0,-3,0,
/* state 6 */-3,0,-3,0,
/* state 7 */0,-6,0,0,
/* state 8 */0,10,0,0,
/* state 9 */-5,0,-5,0,
/* state 10 */0,0,0,12,
/* state 11 */-5,0,-5,0,
};
/** goto table,
 * zero iff there is an error*/
static const int yygoto[] = {
/* state 0 */0,1,2,0,6,0,7,10,
/* state 1 */0,0,0,0,0,0,0,0,
/* state 2 */0,0,0,0,0,0,0,0,
/* state 3 */0,0,0,4,0,0,0,0,
/* state 4 */0,0,0,0,5,0,7,10,
/* state 5 */0,0,0,0,0,0,0,0,
/* state 6 */0,0,0,0,0,0,0,0,
/* state 7 */0,0,0,0,0,8,0,0,
/* state 8 */0,0,0,0,0,0,0,0,
/* state 9 */0,0,0,0,0,0,0,0,
/* state 10 */0,0,0,0,0,0,0,0,
/* state 11 */0,0,0,0,0,0,0,0,
};
static const char *yytokenNames[] = {
    "EOF","id","+","num",
};
static const char *yytokenAlias[] = {
    "EOF","ID","PLUS","NUM",
};
static const char *yynonTerminals[] = {
    "(accept)","start","expr","@1","atom","@2",
    "@3","@4",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule,int yytoken){
   if(yyparser->sp - yyparser->pstack >= yyparser->pSize){
       size_t offset = yyparser->sp - yyparser->pstack;
       yyparser->pSize *= 2;
       yyparser->pstack = (int *)yyparser->rtor(yyparser->pstack,sizeof(int) * yyparser->pSize);
       yyparser->sp = yyparser->pstack + offset;
   }
   int yyval;
   switch(yyrule){
       case 0:
           /* (accept) -> start  */
           /* no action. */
           yyparser->sp -= 1;
           yyparser->sLen -= 1;
           *++yyparser->sp = yyval;
           break;
       case 1:
           /* start -> expr  */
           #line 25 "action.y"
           {  printf("accept");yyval = (yyparser->sp[-1]); }
           #line 76 "gentest.c"
           yyparser->sp -= 1;
           yyparser->sLen -= 1;
           *++yyparser->sp = yyval;
           break;
       case 2:
           /* @1 ->  */
           #line 27 "action.y"
           {  printf("i saw a plus");yyval = 0; }
           #line 85 "gentest.c"
           *++yyparser->sp = yyval;
           break;
       case 3:
           /* expr -> expr <+> @1 atom  */
           #line 27 "action.y"
           {  yyval = (yyparser->sp[-4]) + (yyparser->sp[-2]); }
           #line 92 "gentest.c"
           yyparser->sp -= 4;
           yyparser->sLen -= 4;
           *++yyparser->sp = yyval;
           break;
       case 4:
           /* expr -> atom  */
           #line 27 "action.y"
           {  yyval = (yyparser->sp[-1]); }
           #line 101 "gentest.c"
           yyparser->sp -= 1;
           yyparser->sLen -= 1;
           *++yyparser->sp = yyval;
           break;
       case 5:
           /* @2 ->  */
           #line 29 "action.y"
           {  printf("a new node"); }
           #line 110 "gentest.c"
           break;
       case 6:
           /* @3 ->  */
           #line 29 "action.y"
           {  printf("i'm expecting an id"); }
           #line 116 "gentest.c"
           break;
       case 7:
           /* atom -> @3 @2 <id>  */
           #line 29 "action.y"
           {  yyval = yytoken; }
           #line 122 "gentest.c"
           yyparser->sp -= 1;
           yyparser->sLen -= 3;
           *++yyparser->sp = yyval;
           break;
       case 8:
           /* @4 ->  */
           #line 29 "action.y"
           {  printf("i'm expecting a num"); }
           #line 131 "gentest.c"
           break;
       case 9:
           /* atom -> @4 <num>  */
           #line 29 "action.y"
           {  yyval = yytoken; }
           #line 137 "gentest.c"
           yyparser->sp -= 1;
           yyparser->sLen -= 2;
           *++yyparser->sp = yyval;
           break;
   }
   YYPUSH_STATE(yyparser,yygoto[YYSTATE() * yyntCount + yyrule] - 1);
   return 0;
}
int yyParser_init(yyParser *yyparser,yyalloc altor,yyrealloc rtor,yyfree dtor){
   yyparser->altor = altor;
   yyparser->dtor = dtor;
   yyparser->rtor = rtor;
   yyparser->sLen = 0;
   yyparser->sSize = yyparser->pSize = 16;
   yyparser->state = (int *)altor(sizeof(int) * yyparser->sSize);
   yyparser->sp = yyparser->pstack = (int *)altor(sizeof(int) * yyparser->pSize);
}
int yyParser_free(yyParser *yyparser){
   yyparser->dtor(yyparser->state);
   yyparser->dtor(yyparser->pstack);
}
int yyParser_acceptToken(yyParser *yyparser,int yytokenid,int yytoken){
   do{
       int yyaction = yyshift[YYSTATE() * yytokenCount + yytokenid];
       if(yyaction > 0){
           YYPUSH_STATE(yyparser,yyaction - 1);
       }
       else if(yyaction < 0){
           yyParser_reduce(yyparser,1 - yyaction,yytoken);
           continue;
       }
       else {
           return -1;
       }
   }while(0);
   return 0;
}
