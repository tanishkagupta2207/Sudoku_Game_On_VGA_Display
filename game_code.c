#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/*For Testing Purposes visit: https://cpulator.01xz.net/?sys=nios-de1soc
 * OR
 * USE DE1SOC Board and VGA display
 */

#define N 9
#define VGA_CHAR_BUFFER_BASE 0x09000000
int k=5;

int mat[N][N];
int finalMat[N][N];
int mat2[N][N];
int prevSw=0;
int prevNum=0;
int SRN;
int posx = 0;
int posy = 0;
int mpx[9] = {29,31,34,37,40,43,45,48,51};
int mpy[9] = {31,34,37,39,42,45,48,50,53};
void fillDiagonal();
int unUsedInBox(int rowStart, int colStart, int num);
void fillBox(int row, int col);
int randomGenerator(int num);
int isSafe(int i, int j, int num);
int unUsedInRow(int i, int num);
int unUsedInCol(int j, int num);
int fillRemaining(int i, int j);
void removeKDigits();
void printSudoku();
void clear_screen();
void write_pixel(int x, int y, short colour);
void printStart();
void write_char(int x,int y, char c);
int readNumber();
void setK(int l);
void updateFinal();
void printRules();
void printBase();
int readSwitch();
void gameLogic();
void highlightPos();
int checkComplete();
void colorBox(int x, int y);
void printWin();

int main(){
    srand(time(NULL)); 
    while(1){
        clear_screen();
        printSudoku();
        printStart();
        int b=readNumber();
        while(b!=1 && b!=2 && b!=3){b=readNumber();}
        setK(b);
        SRN = sqrt(N);
        memset(mat, 0, sizeof(mat));
        fillDiagonal();
        fillRemaining(0, SRN);
        updateFinal();
        removeKDigits();
        for(int i=0;i<N;i++){
            for(int j=0;j<9;j++){
                // printf("%d ",finalMat[i][j]);
                mat2[i][j]=mat[i][j];
            }
            // printf("\n");
        }
        clear_screen();
        printSudoku();
        printRules();
        printBase();
        readSwitch();
        highlightPos();
        gameLogic();
    }
    return 0;
}

void write_pixel(int x, int y, short colour) {
    volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
    *vga_addr=colour;
}

void clear_screen() {
    int x, y;
    for (x = 0; x < 320; x++) {
        for (y = 0; y < 240; y++) {
        write_pixel(x,y,0);
        }
    }
    for (x = 0; x < 80; x++) {
        for (y = 0; y < 60; y++) {
            write_char(x, y, ' '); // Clear character buffer by writing space
        }
    }
}

void fillDiagonal(){
    for (int i = 0; i < N; i = i + SRN){
        fillBox(i, i);
    }
}

int unUsedInBox(int rowStart, int colStart, int num){
    for (int i = 0; i < SRN; i++) {
        for (int j = 0; j < SRN; j++) {
            if (mat[rowStart + i][colStart + j] == num) {
                return 0;
            }
        }
    }
    return 1;
}

void fillBox(int row, int col){
    int num;
    for (int i = 0; i < SRN; i++) {
        for (int j = 0; j < SRN; j++) {
            do {
                num = randomGenerator(N);
            } while (!unUsedInBox(row, col, num));
            mat[row + i][col + j] = num;
        }
    }
}

int randomGenerator(int num){
    return (rand() % num) + 1;
}

int isSafe(int i, int j, int num){
    return (unUsedInRow(i, num) && unUsedInCol(j, num) && unUsedInBox(i - i % SRN, j - j % SRN, num));
}

int unUsedInRow(int i, int num){
    for (int j = 0; j < N; j++) {
        if (mat[i][j] == num) {
            return 0;
        }
    }
    return 1;
}

int unUsedInCol(int j, int num){
    for (int i = 0; i < N; i++) {
        if (mat[i][j] == num) {
            return 0;
        }
    }
    return 1;
}

int fillRemaining(int i, int j){
    if (j >= N && i < N - 1) {
        i = i + 1;
        j = 0;
    }
    if (i >= N && j >= N) {
        return 1;
    }
    if (i < SRN) {
        if (j < SRN) {
            j = SRN;
        }
    }
    else if (i < N - SRN) {
        if (j == (int)(i / SRN) * SRN) {
            j = j + SRN;
        }
    }
    else {
        if (j == N - SRN) {
            i = i + 1;
            j = 0;
            if (i >= N) {
                return 1;
            }
        }
    }
    for (int num = 1; num <= N; num++) {
        if (isSafe(i, j, num)) {
            mat[i][j] = num;
            if (fillRemaining(i, j + 1)) {
                return 1;
            }
            mat[i][j] = 0;
        }
    }
    return 0;
}

void removeKDigits(){
    int count = k;
    while (count != 0) {
        int cellId = randomGenerator(N * N) - 1;
        int i = (cellId / N);
        int j = cellId % N;
        if (j != 0) {
            j = j - 1;
        }
        if (mat[i][j] != 0) {
            count--;
            mat[i][j] = 0;
        }
    }
}

void printSudoku(){
    short sud[60][240]={0};
    for(int i=0;i<30;i++){
        for(int j=0;j<10;j++){
            sud[j][i]=65535;
        }
    }
	for(int i=0;i<30;i++){
        for(int j=0;j<10;j++){
            sud[j+25][i]=65535;
        }
    }
	for(int i=0;i<30;i++){
        for(int j=0;j<10;j++){
            sud[j+50][i]=65535;
        }
    }
	for(int i=0;i<15;i++){
        for(int j=0;j<10;j++){
            sud[i+10][j]=65535;
        }
    }
	for(int i=0;i<15;i++){
        for(int j=0;j<10;j++){
            sud[i+35][j+20]=65535;
        }
    }  //S
	
	for(int i=0;i<30;i++){
		for(int j=0;j<10;j++){
			sud[j+50][i+40]=65535;
		}
	}
	for(int i=0;i<50;i++){
        for(int j=0;j<10;j++){
            sud[i][j+40]=65535;
        }
    }
	for(int i=0;i<50;i++){
        for(int j=0;j<10;j++){
            sud[i][j+60]=65535;
        }
    }   //U
	for(int i=0;i<60;i++){
        for(int j=0;j<10;j++){
            sud[i][j+80]=65535;
        }
    }
	for(int i=0;i<40;i++){
        for(int j=0;j<10;j++){
            sud[i+10][j+100]=65535;
        }
    }
	for(int i=0;i<20;i++){
		for(int j=0;j<10;j++){
			sud[j+50][i+80]=65535;
		}
	}
	for(int i=0;i<20;i++){
		for(int j=0;j<10;j++){
			sud[j][i+80]=65535;
		}
	}   //D
	
	for(int i=0;i<60;i++){
        for(int j=0;j<10;j++){
            sud[i][j+120]=65535;
        }
    }
	for(int i=0;i<60;i++){
        for(int j=0;j<10;j++){
            sud[i][j+140]=65535;
        }
    }
	for(int i=0;i<30;i++){
		for(int j=0;j<10;j++){
			sud[j+50][i+120]=65535;
		}
	}
	for(int i=0;i<30;i++){
		for(int j=0;j<10;j++){
			sud[j][i+120]=65535;
		}
	} //O
	
	for(int i=0;i<60;i++){
        for(int j=0;j<10;j++){
            sud[i][j+160]=65535;
        }
    }
	for(int i=0;i<10;i++){
		for(int j=0;j<30;j++){
			sud[30-j][j+160+i]=65535;
        }
	}
	for(int i=0;i<10;i++){
		for(int j=0;j<30;j++){
			sud[30+j][j+160+i]=65535;
        }
	}	
	//K
	for(int i=0;i<30;i++){
		for(int j=0;j<10;j++){
			sud[j+50][i+210]=65535;
		}
	}
	for(int i=0;i<50;i++){
        for(int j=0;j<10;j++){
            sud[i][j+210]=65535;
        }
    }
	for(int i=0;i<50;i++){
        for(int j=0;j<10;j++){
            sud[i][j+230]=65535;
        }
    } //U
	
	for(int i=0;i<60;i++){
		for(int j=0;j<240;j++){
			write_pixel(j+40,i+20,sud[i][j]);
		}
	}
}

void printStart(){
    char* hw = "Welcome To Ultimate Sudoku Game!!!";
   	int x = 23;
	int y = 30;
	int count = 0;
	while(count<34){
		write_char(x,y,*hw);
		hw++;
		x++;
		count++;
	}
	x=28;
    y=40;
    count= 0;
    char *level = "Choose Difficulty Level";
    while(count<23){
		write_char(x,y,*level);
		level++;
		x++;
		count++;
	}
	char *one = "Easy(1)";
	char *two = "Medium(2)";
	char *three = "Hard(3)";
	x=37;
    y=44;
    count= 0;
    while(count<7){
		write_char(x,y,*one);
		one++;
		x++;
		count++;
	}
	x=36;
    y=46;
    count= 0;
    while(count<9){
		write_char(x,y,*two);
		two++;
		x++;
		count++;
	}
	x=37;
    y=48;
    count= 0;
    while(count<7){
		write_char(x,y,*three);
		three++;
		x++;
		count++;
	}

}

void write_char(int x, int y, char c) {
    volatile char *char_buffer = (char *)(VGA_CHAR_BUFFER_BASE + (y << 7) + x);
    *char_buffer = c;       // Write the character to the character buffer
}

int readNumber(){
    volatile int *memory_address = (volatile int *)0xFF200040;
    int value = *memory_address;
	if(value == 1){
        if(prevNum!=1){
            prevNum=1;
            return 1;
        }
        return 0;
    }
    if(value == 2){
        if(prevNum!=2){
            prevNum=2;
            return 2;
        }
        return 0;
    }
    if(value == 4){
        if(prevNum!=3){
            prevNum=3;
            return 3;
        }
        return 0;
    }
    if(value == 8){
        if(prevNum!=4){
            prevNum=4;
            return 4;
        }
        return 0;
    }
    if(value == 16){
        if(prevNum!=5){
            prevNum=5;
            return 5;
        }
        return 0;
    }
    if(value == 32){
        if(prevNum!=6){
            prevNum=6;
            return 6;
        }
        return 0;
    }
    if(value == 64){
        if(prevNum!=7){
            prevNum=7;
            return 7;
        }
        return 0;
    }
    if(value == 128){
        if(prevNum!=8){
            prevNum=8;
            return 8;
        }
        return 0;
    }
    if(value == 256){
        if(prevNum!=9){
            prevNum=9;
            return 9;
        }
        return 0;
    }
    if(value == 512){
        if(prevNum!=-1){
            prevNum=-1;
            return -1;
        }
        return 0;
    }
    prevNum=0;
	return 0;
}

void setK(int l){
    if(l==1){             //easy
        k=20;
    }
    else if(l==2){          //medium
        k=40;
    }
    else{                   //hard level
        k=55;
    }
}

void updateFinal(){
    for(int i=0;i<N;i++){
        for(int j=0;j<9;j++){
            finalMat[i][j]=mat[i][j];
        }
    }
}

void printRules(){
    char* hw = "Use Push Buttons for navigation: 1-Right, 2-Left, 3-Up, 4-Down, All-Restart";
   	int x = 2;
	int y = 23;
	int count = 0;
	while(count<75){
		write_char(x,y,*hw);
		hw++;
		x++;
		count++;
	}
    char* h = "10th Switch to remove";
   	int xa = 29;
	int ya = 25;
	int counta = 0;
	while(counta<21){
		write_char(xa,ya,*h);
		h++;
		xa++;
		counta++;
	}

}

void printBase(){
	for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(mat[i][j]!=0){
                if(mat2[i][j]!=0){ 
                    colorBox(i,j);
                }
                write_char(mpx[i],mpy[j],mat[i][j]+48);
            }
        }
    }
	for(int i=0;i<100;i++){
        write_pixel(121,i+120,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(132,i+120,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(154,i+120,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(165,i+120,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(187,i+120,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(198,i+120,31759);
    }
	
	for(int i=0;i<100;i++){
        write_pixel(i+110,131,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(i+110,142,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(i+110,164,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(i+110,175,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(i+110,197,31759);
    }
	for(int i=0;i<100;i++){
        write_pixel(i+110,208,31759);
    }
	
    for(int i=0;i<100;i++){
        write_pixel(143,i+120,65535);
    }
	for(int i=0;i<100;i++){
        write_pixel(176,i+120,65535);
    }
	for(int i=0;i<100;i++){
        write_pixel(i+110,153,65535);
    }
	for(int i=0;i<100;i++){
        write_pixel(110+i,186,65535);
    }
	for(int i=0;i<100;i++){
        write_pixel(110+i,120,65535);
    }
	for(int i=0;i<100;i++){
        write_pixel(110+i,219,65535);
    }
	for(int i=0;i<100;i++){
        write_pixel(110,i+120,65535);
    }
	for(int i=0;i<100;i++){
        write_pixel(209,i+120,65535);
    }
    
}

void colorBox(int x, int y){
    for(int i=0;i<11;i++){
        for(int j=0;j<11;j++){
            write_pixel(110+(x*11)+j,120+(y*11)+i,2016);
        }
    }
}

int readSwitch(){
    volatile int *memory_address = (volatile int *)0xFF200050;
    int value = *memory_address;
    if(value == 1){
        if(prevSw!=1){
            prevSw=1;
            return 1;
        }
        return 0;
    }
    if(value == 2){
        if(prevSw!=2){
            prevSw=2;
            return 2;
        }
        return 0;
    }
    if(value == 4){
        if(prevSw!=3){
            prevSw=3;
            return 3;
        }
        return 0;
    }
    if(value == 8){
        if(prevSw!=4){
            prevSw=4;
            return 4;
        }
        return 0;
    }
    if(value == 15){
        if(prevSw!=-1){
            prevSw=-1;
            return -1;
        }
        return 0;
    }
    prevSw=0;
    return 0;
}

void gameLogic(){
    while(1){
        int a = readNumber();
        int s = readSwitch();
        
        if(s==-1){
            break;
        }
        if(s!=0){
            if(s==1){
                if(posx!=8){
                    posx++;
                    printBase();
                    highlightPos();
                    continue;
                }
            }
            if(s==2){
                if(posx>0){
                    posx--;
                    printBase();
                    highlightPos();
                    continue;
                }
            }
            if(s==3){
                if(posy>0){
                    posy--;
                    printBase();
                    highlightPos();
                    continue;
                }
            }
            if(s==4){
                if(posy!=8){
                    posy++;
                    printBase();
                    highlightPos();
                    continue;
                }
            }
        }        
        if(a!=0 && mat2[posx][posy]==0){
            if(a==-1){
                mat[posx][posy]=0;
                write_char(mpx[posx],mpy[posy],' ');
            }
            else{
                mat[posx][posy]=a;
                write_char(mpx[posx],mpy[posy],a+48);
            }
        }
        int c = checkComplete();
        if(c==1){
            printWin();
            break;
        }
    }
}

void highlightPos(){
    for(int i=0;i<11;i++){
        write_pixel(110+(posx*11),120+(posy*11)+i,61440);
        write_pixel(110+(posx*11)+11,120+(posy*11)+i,61440);
        write_pixel(110+(posx*11)+i,120+(posy*11),61440);
        write_pixel(110+(posx*11)+i,120+(posy*11)+11,61440);
    }
}

int checkComplete(){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(mat[i][j]!=finalMat[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

void printWin(){    
    for(int i=0;i<320;i++){
        for(int j=0;j<100;j++){
            write_pixel(i,j+120,0);
        }
    }
    for(int i=0;i<80;i++){
        for(int j=0;j<25;j++){
            write_char(i,j+30,' ');
        }
    }
    char* hw = "YOU WON !!!";
   	int x = 34;
	int y = 39;
	int count = 0;
	while(count<11){
		write_char(x,y,*hw);
		hw++;
		x++;
		count++;
	}
    
	for(int i=0;i<100000000;i++){} //delay
}