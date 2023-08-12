
#include <unistd.h>

#include "interface.c"
#include "strings.c"


struct Network* tmplst=NULL;
int wifi_count=0;

struct Network** heldarray;

void process_input(char* input){
//printf("%s\n",input);
int pssid=find_w_begin(input,"ssid=\"");
pssid=pssid+6;
int pssidend=find_w(pssid,input,"\"");
int start=0;
int priority=find_w_begin(input,"priority=");
if(priority!=-1){
 start=find_w(priority,input,"\n");
}
/*int pass=find_w(0,input,"#psk=\"");
pass=pass+6;
int passend=find_w(pass,input,"\"");
int key=find_w(0,input,"\npsk=");
key=key+5;
int keyend=strlen(input);*/
char* ssid=malloc(sizeof(char)*((pssidend-pssid)+1));
char* datacpy=malloc(sizeof(char)*strlen(input)+1*sizeof(char));
strcpy(datacpy,input+start);
//char* passkey=malloc(sizeof(char)*(passend-pass+1));
//char* hash=malloc(sizeof(char)*(keyend-key+1));
copy_to(ssid,input,pssid,pssidend);
/*copy_to(passkey,input,pass,passend);
copy_to(hash,input,key,keyend);
int loc;
if((loc=find_w(0,hash,"\n"))!=-1){ 
 int len=strlen(hash);
 int i;
 for(i=loc;i<len;i++){
  hash[i]=0;
 }
}
if(hash[strlen(hash)-1]=='\n'){
hash[strlen(hash)-1]=0;
}*/
//printf("ssid:%s\n%s\n",ssid,datacpy);
struct Network* newfi=(struct Network*)malloc(sizeof(struct Network));
 newfi->data=datacpy;
 newfi->ssid=ssid;
 newfi->next=tmplst;
tmplst=newfi;
wifi_count++;
}

void convert_to_array(){
heldarray=(struct Network**)malloc(sizeof(struct Network*)*(wifi_count));
int startindex=wifi_count-1;
 while(tmplst!=NULL){
  heldarray[startindex]=tmplst;
  startindex--;
  tmplst=tmplst->next;
 }
}
void printf_data(int index){
  printf("ssid:%s\n%s\n",heldarray[index]->ssid,heldarray[index]->data);

}
void free_data(){
 if(heldarray==NULL){return;}
 int i;
 for(i=0;i<wifi_count;i++){
  if(heldarray[i]!=NULL){
   if(heldarray[i]->ssid!=NULL){
    free(heldarray[i]->ssid);
    heldarray[i]->ssid=NULL;
   }
   if(heldarray[i]->data!=NULL){
    free(heldarray[i]->data);
    heldarray[i]->data=NULL;
   }
//   free(heldarray[i]);
   heldarray[i]=NULL;
  }
 }
wifi_count=0;
free(heldarray);
heldarray=NULL;
}
void write_back_to_file(){
 FILE* f=fopen("/etc/wpa_supplicant.conf","w");
 if(f==NULL){
  printf("oops, couldn't save changes!\n");
  return;
 }
 int i;
 for(i=0;i<wifi_count;i++){
  if(heldarray[i]!=NULL){
   fputs("network={\n	priority=",f);
   char num[50];
   sprintf(num,"%d",wifi_count-i-1);
   fputs(num,f);
   fputs(heldarray[i]->data,f);
   fputs("}\n",f);
  }
 }
 fclose(f);
}

int main(){
 int ncurses=0;
 while(1==1){
 if(geteuid()!=0){
  printf("please run as sudo\n");
  return 0;
 }
 FILE* f=fopen("/etc/wpa_supplicant.conf","rw");
 if(f==NULL){
  printf("cannot open /etc/wpa_supplicant.conf, exiting\n");
  return 0;
 }
 char rd=0;
 int reading=0;
// int quotes=0;
 char* contents=NULL;
 while(rd!=EOF){
   rd=fgetc(f);
   if(rd!=EOF&&rd!=0){
    if(rd=='}'){
     reading=0;
     if(contents!=NULL){
      process_input(contents);
      free(contents);
      contents=NULL;
     }
    }
    if(reading==1){
     append_char(rd,&contents);
    }
    if(rd=='{'){
     reading=1;
    }
   }
 }
 fclose(f);
 convert_to_array();
 if(ncurses==0){
  start_ncurses();
 }
 ncurses=1;
 int choice=sel_menu(heldarray,wifi_count);
 wifi_count=true_a_size;
 if(choice==1){
  write_back_to_file();
 }
 free_data();
 if(choice!=3){
  break;
 }else{
  prompt_new_wifi(); 
 }
 }
 curs_set(1);
 endwin();
 return 0;

}
