main(){
while(1){
	char input[80];
	char buffer[13312];
	int sectors = 0;

	syscall(0, "\rSHELL>");
	syscall(1, input);
	if (input[0]=='t' && input[1]=='y' &&input[2]=='p' &&input[3]=='e' &&input[4]==' '){
			syscall(3, input+5, buffer, &sectors);
			if (sectors < 1){
				syscall(0,"file not found\r\n");
			}
			else{
				syscall(0, buffer);
			}
	}
	else if (input[0] =='e' && input[1]=='x' &&input[2]=='e' &&input[3]=='c' &&input[4]==' '){
	syscall(4, input+5);
	}
	else{
		syscall(0, "Command not found\r\n");
		syscall(5);
	}
}
}
