#include <stdio.h>
#include <time.h> // clock()
void main()
{
  int pose = 1;
  int boyPos = 1;
  int step = 5;
  int x, y;
  clock_t start;//3afsa brk
  char something[] = "...eituc ym teem ot tiaw t'naC ]naa9la9[";
    
  while(1==1){// keep repeating forever *0*
  
    for(x=0;x/10<=666; x++) for(y=0;y/10<=666; y++);// "pause" for idk how long
    //for(start=clock(); (clock()-start)<(0.25*1000); );// hada khir y'pausi for 0.25 seconds

    for(x=0; x<30; x++) printf("\n"); // za3ma new page/screen/view
    //system("cls"); // mi hada khir

    for(x=40; x>=0; x--) printf("%c", something[x]); // write something
    printf("\n\n\n\n\n");
    
    for(x=0; x<boyPos; x++) printf(" ");
    printf(" O\n");

    for(x=0; x<boyPos;x++) printf(" ");
    if(pose%2 != 0)
    printf("/|\\\n");
    else
    printf(" |  \n");

    for(x=0; x<boyPos;x++) printf(" ");
    if(pose == 1)
    printf("/ \\");
    else if(pose == 2)
    printf("/|");
    else if(pose == 3)
    printf(" |");
    else// 4
    printf(" |\\");

    pose = pose%4 + 1;// 1,2,3,4,1,2,3,4,1,2,3,4...
    boyPos = boyPos + step;
    if( (boyPos>70 && step > 0) || (boyPos<5 && step < 0) ) step = -step;     
  }
}
