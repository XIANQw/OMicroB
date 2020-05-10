#include<stdio.h>
#include<string.h>
#include"chs_tab.h"

char ch_tab[5];


char * getCharTab(char c,char *chs_tab)
{   
    memset(ch_tab,0,5*sizeof(char));
    int offset;
    offset = (int)c*5;            /*通过ascii码算出偏移量*/
    //printf("%d",offset);
    for(int i =0;i<5;i++){
        ch_tab[i] = chs_tab[offset+i];
    
    }
    return ch_tab;
}


void print_char(char *ch_tab){
    int i,j,a;
	for(i=0;i<5;i++){

	    for(j=0;j<8;j++)
	        {
				if((ch_tab[i]&(1 << j)) == (1 << j))
					printf("*");
				else
					printf("-");
			}printf("\n");   
        }printf("\n");
}
int main(){

    //char ch_tab[5]	= {6, 9, 17, 31, 17};
while(1){
    char AscC  = getchar();

    //print_char(ch_tab);
    getCharTab(AscC,chs_tab);
    print_char(getCharTab(AscC,chs_tab));
}

        
}   