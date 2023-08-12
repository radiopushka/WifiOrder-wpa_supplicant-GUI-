#include <stdlib.h>
#include <string.h>

void copy_to(char* dest,char* src, int start,int end){
int i;
for(i=0;i<(end-start);i++){
 dest[i]=src[i+start];
}
dest[i]=0;
}

void append_char(char c,char** string){
 if(*string==NULL){
  char* np=malloc(sizeof(char)*2);
  np[0]=c;
  np[1]=0;
  *string=np;
 }else{
  int size=strlen(*string);
  char* newst=malloc(sizeof(char)*size+(2*sizeof(char)));
  int i;
  for(i=0;i<size;i++){
   newst[i]=(*string)[i];
  }
  newst[i]=c;
  newst[i+1]=0;
  free(*string);
  *string=newst;
 }
}


int find_w(int start,char* in,char* word){
 int len=strlen(in);
 int wlen=strlen(word);
 int i;
 int stp=-1;
 int pos=0;
 for(i=start;i<len;i++){
  if(in[i]==word[0]){
   if(stp==-1){
    stp=i;
    pos=0;
   }
  }
  if(stp!=-1){
    if(pos>=wlen){
     return stp;
    }
    if(in[i]!=word[pos]){
     stp=-1;
    }
    pos++;
  }
 }
 return -1;
}
int find_w_begin(char* in,char* word){return find_w(0,in,word);}
