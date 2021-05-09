#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int rotatebits(int num, int times, int bits);

int main (){
	unsigned int hex;
	int indx, indx2, pc, skip;
	unsigned char reg[16], mem[256], input[0x20], temp, pos, temp2;
	FILE *f;
	
	f = fopen("a5.txt", "r");
	
	if (f == NULL){
		printf("File a5.txt not found.");
		exit(1);
	}
	
	for (indx=0; indx<16; indx++){
		reg[indx] = 0x00;
	}
	for (indx=0; indx<256; indx++){
		mem[indx] = 0x00;
	}
	
	indx = 0;
	pc = 0x00;
	
	while (fgets(input, 0x20, f)!=NULL){
		sscanf(input, "%X", &hex);
		mem[indx] = hex >> 8;
		indx++;
		mem[indx] = hex & 0xff;
		indx++;
	}
	fclose(f);
	
	skip = 0;
	indx = 0;
	indx2 = 0;
	hex = 1;
	while (hex==1){ 
		printf("%02X %02X%02X - [", pc, mem[indx], mem[indx+1]);
		for (indx2=0; indx2<16; indx2++){
			printf("%02X ", reg[indx2]);
		}
		printf("%02X]\n", reg[15]);

		switch (mem[indx]>>4){
			case 1:
				pos = mem[indx]&0xf;		//Gets just the value of the Reg
				temp = mem[indx+1];		//Loads the XY value in temp
				reg[pos] = mem[temp];	//Puts the value at mem location XY into reg R
				break;
			case 2:
				pos = mem[indx]&0xf;
				reg[pos] = mem[indx+1];
				break;
			case 3:
				pos = mem[indx]&0xf;   //Gets the position of the Reg
				temp = mem[indx+1];	  //Gets the value of XY
				mem[temp] = reg[pos];  //Stores value of the register at pos XY
				break;
			case 4:
				pos = mem[indx+1]>>4;
				temp = mem[indx+1]&0xf;
				reg[temp] = reg[pos];
				break;
			case 5:
				pos = mem[indx]&0xf;
				temp = mem[indx+1]>>4;
				temp2 = mem[indx+1]&0xf;
				reg[temp2] = reg[pos] + reg[temp];
				break;
			case 6:
				break;
			case 7:
				pos = mem[indx]&0xf;
				temp = mem[indx+1]>>4;
				temp2 = mem[indx+1]&0xf;
				reg[temp2] = reg[pos] | reg[temp];
				break;
			case 8:
				pos = mem[indx]&0xf;
				temp = mem[indx+1]>>4;
				temp2 = mem[indx+1]&0xf;
				reg[temp2] = reg[pos] & reg[temp];
				break;
			case 9:
				pos = mem[indx]&0xf;
				temp = mem[indx+1]>>4;
				temp2 = mem[indx+1]&0xf;
				reg[temp2] = reg[pos] ^ reg[temp];
				break;
			case 0XA:
				pos = mem[indx]&0xf;
				temp = mem[indx+1]&0xf;
				temp = rotatebits(reg[pos], temp, 8);
				reg[pos] = temp;
				break;
			case 0XB:
				pos = mem[indx]&0xf;
				if (reg[pos] == reg[0]){
					indx = mem[indx+1];
					skip = 1;
					pc = indx;
				}
				//This needs to reset the indx to cause a loop
				break;
			case 0XC:
				hex = 0;
				exit(13);
		}
		if (skip!=1){
			indx = indx+2;
			pc = indx;
		}else{
			skip =0;
		}
	}
}

int rotatebits(int num, int times, int bits){
	int temp1, temp2, result;
	
	temp1 = num >> times;
	temp2 = num << (bits - times);
	result = temp1 | temp2;
	return result;
}



