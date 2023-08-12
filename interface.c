#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers.h"

WINDOW* win;

int true_a_size=0;

void refresh_window(){
 //refresh();
 wrefresh(win);
 box(win,0,0);
 mvwprintw(win,0,1,"Networks /etc/wpa_supplicant.conf");
}

void start_ncurses(){
 initscr();
 cbreak();
 noecho();
 curs_set(0);
 win=newwin(LINES,COLS,0,0);
 refresh();
 box(win,0,0);
 keypad(win,TRUE);
 refresh_window();
}
void print_items(struct Network** items, int selected,int size){
 //refresh_window();
 wclear(win);
 int i;
 int start=0;
 if(selected>LINES-2){
  start=selected-(LINES-2);
 }
 for(i=start;i<size&&(i-start)<LINES-2;i++){
  if(i==selected){wattron(win,A_STANDOUT);}
  if(items[i]!=NULL){
  mvwprintw(win,i-start+1,1,items[i]->ssid);
  }
  if(i==selected){wattroff(win,A_STANDOUT);}
 }
 refresh_window();
}
void inter_change(struct Network** array, int size, int src,int dest){
 if(src>size-1||src<0){return;}
 if(dest>size-1||dest<0){return;}
 struct Network* tmp=array[src];
 array[src]=array[dest];
 array[dest]=tmp;
}
void delete_wifi(struct Network** array, int size,int index){
if(index<0||index>size-1){return;}
free(array[index]->ssid);
free(array[index]->data);
free(array[index]);
int i;
for(i=index+1;i<size;i++){
 array[i-1]=array[i];
}
array[i]=NULL;
true_a_size--;
}
void view(char* text){
 WINDOW* wt=newwin(LINES/2+2,COLS/2+2,LINES/2-LINES/4-1,COLS/2-COLS/4-1);
 WINDOW* popup=newwin(LINES/2,COLS/2,LINES/2-LINES/4,COLS/2-COLS/4);
 refresh();
 box(wt,0,0);
 wrefresh(wt);
 mvwprintw(popup,1,1,text);
 wrefresh(popup);
 getch();
 wclear(popup);
 wrefresh(popup);
 delwin(popup);
 wclear(wt);
 delwin(wt);
}
int sel_menu(struct Network** input_array,int size){
 int select=0;
 int key=0;
 true_a_size=size;
 while(key!='q'&&key!='s'&&key!='\n'&&key!='n'){
  print_items(input_array,select,true_a_size);
  key=wgetch(win);
  switch(key){
   case 'w':
   case KEY_UP:
    if(select>0){select--;}
    break;
   case 'e':
   case KEY_DOWN:
    if(select<true_a_size-1){select++;}
    break;
   case 'p':
   case KEY_LEFT:
    inter_change(input_array,true_a_size,select,select-1);
    break;
   case 'l':
   case KEY_RIGHT:
    inter_change(input_array,true_a_size,select,select+1);
    break;
   case 'v':
    view(input_array[select]->data);
    break;
   case 'd':
    delete_wifi(input_array,true_a_size,select);
    break;
   case '/':
   case '?':
   case 'h':
    view("v(key)-view network details d(key)-delete n(key)-new network q(key)-quit Enter-save");
    break;
  }
 }
 if(key!='n'){
 wclear(win);
 wrefresh(win);
 delwin(win);
 }
 if(key=='q'){
  return -1;
 }
 if(key=='\n'||key=='s'){
  return 1;
 }
 if(key=='n'){
  return 3;
 }
 return 0;
}


void prompt_new_wifi(){
 WINDOW* wt=newwin(LINES/2+2,COLS/2+2,LINES/2-LINES/4-1,COLS/2-COLS/4-1);
 WINDOW* popup=newwin(LINES/2,COLS/2,LINES/2-LINES/4,COLS/2-COLS/4);
 refresh();
 keypad(popup,TRUE);
 box(wt,0,0);
 char ssid[33];
 char psk[64];
 for(int i=0;i<33;i++){ssid[i]=0;}
 for(int i=0;i<64;i++){psk[i]=0;}
 mvwprintw(wt,0,1,"New Network");
 wrefresh(wt);
 wrefresh(popup);
 int c=0;
 char targ=0;
 char ssidp=0;
 char pskp=0;
 //curs_set(1);
 while(c!='\n'&&c!='q'){
  wclear(popup);
  mvwprintw(popup,0,0,"SSID:(");
  wprintw(popup,"%d/2)",ssidp);
  mvwprintw(popup,1,0,ssid);
  if(targ==0){
   wattron(popup,A_STANDOUT);
   wprintw(popup," ");
   wattroff(popup,A_STANDOUT);
  }
  mvwprintw(popup,3,0,"PSK:(");
  wprintw(popup,"%d/8)",pskp);
  mvwprintw(popup,4,0,psk);
  if(targ==1){ 
   wattron(popup,A_STANDOUT);
   wprintw(popup," ");
   wattroff(popup,A_STANDOUT);
  }

  wrefresh(popup);
  c=wgetch(popup);
  if(c!='\n'&&c!='q'){
  if(c!=0&&c!=9&&c!=KEY_UP&&c!=KEY_DOWN&&c!=KEY_LEFT&&c!=KEY_RIGHT){
   if(targ==0){
     if(ssidp<33){
      if(c==KEY_BACKSPACE){
       if(ssidp>0){
        ssidp--;
        ssid[ssidp]=0;
       } 
      }else{
       ssid[ssidp]=(char)c;
       ssidp++;
      }
     }
   }else{
     if(pskp<64){
      if(c==KEY_BACKSPACE){
       if(pskp>0){
        pskp--;
        psk[pskp]=0;
       }

      }else{
       psk[pskp]=(char)c;
       pskp++;
      }
     }
   }
  }else if(c!=0){
   if(targ==0){targ=1;}else{targ=0;}
  }
  }
 }
 //curs_set(0);
 wclear(popup);
 wrefresh(popup);
 delwin(popup);
 wclear(wt);
 delwin(wt);
 if(c==10){
  if(pskp==0&&ssidp>=2){
   char commande[150+ssidp];
   sprintf(commande,"echo \"network{\\n	ssid=\\\"%s\\\"\\n	key_mgmt=NONE\\n}\\n\" >> /etc/wpa_supplicant.conf",ssid);
   system(commande);
  }
  if(ssidp<2||pskp<8){return;}
  char command[150+ssidp+pskp];
  sprintf(command,"wpa_passphrase \"%s\" \"%s\" >> /etc/wpa_supplicant.conf",ssid,psk);
  view(command);
  system(command);
 }
}
