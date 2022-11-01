//Isaiah's Operating System Kernel
//Modified by Tom and Tim
//kernel.c

void handleInterrupt21(int,int,int,int); 
void printChar(char);
void printstring(char*);
void readstring(char*);
void readsector(char*, int);
void makeInterrupt21(int,int,int,int);

void main(){
	char line[80];
	char line2[80];
	char buffer[512];
	char* interruptstring = "the interrupt for printstring";
	char buffer2[512];
	printstring("Hello world\n");
	printstring("Enter a line: ");
	readstring(line);
	printstring(line);
	readsector(buffer, 30);
	printstring(buffer);
	makeInterrupt21();
	interrupt(0x21,0,interruptstring,0,0);
	interrupt(0x21,1,line2,0,0);
	interrupt(0x21,2,buffer,30,0);
	interrupt(0x21,5,0,0,0);
        while(1);
}

void printChar(char c){
	interrupt(0x10,0xe*256+c,0,0,0);
}

void printstring(char* letters){

	while(*letters != 0x0) {
		interrupt(0x10,0xe*256+*letters,0,0,0);
		//advance letters pointer
		letters++;
	}
}

void readsector(char* buffer, int sector){
	interrupt(0x13, 2*256+1, buffer, 0*256+(sector+1),0*256+0x80);
}

void readstring(char output[]){
	int i=0;
	output[i] = interrupt(0x16,0,0,0,0);
	interrupt(0x10,0xe*256+output[i],0,0,0);
	while(output[i] != 0xd){
		i++;
		output[i] = interrupt(0x16,0,0,0,0);
		if(output[i] == 0x8 && i>=1){
			i--;
			output[i] = 0x20;
			printChar(0x8);
			printChar(output[i]);
			printChar(0x8);
			i--;
		}
		else if (output[i] != 0x8){
			interrupt(0x10,0xe*256+output[i],0,0,0);
		}
	}
	i++;
	output[i] = 0xa;
	interrupt(0x10,0xe*256+output[i],0,0,0);
	i++;
	output[i] = 0x0;
	interrupt(0x10,0xe*256+output[i],0,0,0);
}

void handleInterrupt21(int AX, int BX, int CX, int DX){
	if (AX==0){
	printstring(BX);
	}
	else if (AX==1){
	readstring(BX);
	}
	else if (AX==2){
	readsector(BX,CX);
	}
	else{
	printstring("Error with interrupt 21");
	}
}
