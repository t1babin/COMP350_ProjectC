//Isaiah's Operating System Kernel
//Modified by Tom and Tim
//kernel.c

void handleInterrupt21(int,int,int,int); 
void printChar(char);
void printstring(char*);
void readstring(char*);
void readsector(char*, int);
void makeInterrupt21(int,int,int,int);
void readFile(char*, char*, int*);

void main(){
	char buffer[13312];
	int sectorsread = 0;
	printstring("Hello world\r\n");
	makeInterrupt21();
	//sectorsread = readFile("messag", buffer);
	interrupt(0x21,3,"messag", buffer,&sectorsread);
	if (sectorsread > 0)
		interrupt(0x21,0,buffer,0,0);
	else
		interrupt(0x21,0,"messag not found\r\n",0,0);
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
	else if (AX==3){
	readFile(BX,CX,DX);
	}
	else{
	printstring("Error with interrupt 21");
	}
}
void readFile(char* filename, char* buffer, int* sectorsread){
	char directory[512];
	int filenamematch = 1;
	int i = 0;
	int fileentry = 0;
	int sectorcount = 0;
	readsector(directory, 2);
	for (fileentry = 0; fileentry < 512; fileentry=fileentry+32){
		filenamematch = 1;
		for (i = 0; i < 6; ++i){
			if(directory[fileentry+i] != filename[i]){
				filenamematch = 0;
			}
		}
		if (filenamematch){
			for (i = 0; i < 26; ++i)
				if (directory[fileentry+6+i] != 0x00){{
					readsector(*(&buffer+512*i), directory[fileentry+6+i]);
					sectorcount = sectorcount+1;
				}
			}
			*sectorsread = sectorcount;
			fileentry = 512;
		}
	}
}
