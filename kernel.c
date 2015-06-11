void printString(char*);
void readString(char*);
int mod(int, int);
int div(int, int );
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
void executeProgram(char*, int);
void terminate();
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char*, char*, int);

main() {

char line[80];
//task 1 
//writeSector(buffer, 30);
//printString(buffer);

//task 2
/*char buffer[13312];
makeInterrupt21();
interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer
*/

//task 3
/*int i=0;
char buffer1[13312];
char buffer2[13312];
buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l'; buffer2[3]='l';
buffer2[4]='o';
for(i=5; i<13312; i++) buffer2[i]=0x0;
makeInterrupt21();
interrupt(0x21,8, "testW1\0", buffer2, 1); //write file testW
interrupt(0x21,3, "testW1\0", buffer1, 0); //read file testW
interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW
*/

makeInterrupt21();
interrupt(0x21, 4, "shell\0", 0x2000, 0);
while(1);
}

int Match(char* compareFile, char* fileName,int size)
{
    int same=0;
    int i;
    for (i = 0; i<size; i++)
    {
        if (compareFile[i] == fileName[i])
        {
            same = 1;
	   
        }
        else
        {
            same = 0;
            break;
        }
    }
    return same;
}


void printString(char* chars){
int i = 0;
while (chars[i] != '\0') {
interrupt(0x10, 0xE*256 + chars[i], 0, 0,0);
i++;
}
}

void readString(char* chars) {
int i = 2;
chars[0] = 0xa;
chars[1] = 0xd;
while(1) {
int c = interrupt (0x16,0,0, 0, 0);
if (c == 0x8 && i>1) {
chars[i] = 0x0;
interrupt(0x10, 0xE*256 + 0x8, 0, 0,0);
interrupt(0x10, 0xE*256 + 0x0, 0, 0,0);
interrupt(0x10, 0xE*256 + 0x8, 0, 0,0);
i--;
} else
if (c == 0xd) {
chars[i] = 0xd;
chars[i+1] = 0xa;
chars[i+2] = 0x0;
return;
} else {
chars[i] = c;
interrupt(0x10, 0xE*256 + chars[i], 0, 0,0);
i++;
}
}
}

int mod(int x, int y) {
int result = x;
while (result >= y) {
result = result - y;
}
return result;
}

int div(int x, int y) {
int result = 0;
while (x >= y) {
x = x -y;
result ++;
}
return result;
}

void readSector(char* buffer, int sector) {

int rsector = (mod(sector,18)) + 1;
int head = mod(div(sector, 18) , 2);
int track = div(sector, 36);
interrupt(0x13, 2*256 + 1, buffer, track*256 + rsector, head*256 + 0);
}

void writeSector(char* buffer, int sector) {
int rsector = (mod(sector,18)) +1;
int head = mod(div(sector, 18) , 2);
int track = div(sector, 36);
interrupt(0x13, 3*256 + 1, buffer, track*256 + rsector, head*256 + 0);
}

void readFile(char* fileName, char* buffer) {

char directory[512];
//typedef int notFound = 1;
int notFound = 1;
char compareFile[7];
int file,begin,end,k;
int j;
int x;
int length = 0;


readSector (directory, 2);

        //beginning of 1st file is 0 and end is 6 
        //beginning of 2nd file is 32 and end is 38 
        //beginning of 3rd file is 64 and end is 70 
        //beginning of 4th file is 96 and end is 102 
        //beginning of 5th file is 128 and end is 134
        //beginning of 6th file is 160 and end is 166 
        //beginning of 7th file is 192 and end is 198
        //beginning of 8th file is 224 and end is 230 
        //beginning of 9th file is 256 and end is 262
        //beginning of 10th file is 288 and end is 294
        //beginning of 11th file is 320 and end is 326 
        //beginning of 12th file i1s 352 and end is 358
        //beginning of 13thfile is 384 and end is 390
        //beginning of 14th file is 416 and end is 422
        //beginning of 15th file is 448 and end is 454
        //beginning of 16th file is 480 and end is 486

/*while(fileName[length] != '\0'){
	length++;
		}
*/
for (file = 0; file < 16; file++) {
    begin = file * 32;
    
	end = begin + 6;
	//end = begin + length; //we choose begin to be plus length so that it would handle a file that is no 6 characters long    
		k = 0;
        for (j = begin; j < end; j++) 
        {
        compareFile[k] = directory[j];
        k++;
        }
        compareFile[6]='\0';
        
      if (Match(compareFile, fileName, 6)) {
            notFound = 0;
            for (x = end; directory[x] != 0x0; x++) {
                readSector(buffer,directory[x]);
                buffer = buffer + 512;
            }
        }
    }
    if (notFound) {
       printString("ERROR! File not found.");
	//printString(n);    
	}
}

void executeProgram(char* name, int segment)
{
  int i = 0;
  char buffer[13312];
//1. Call readFile to load the file into a buffer.
readFile(name,buffer);
//2. In a loop, transfer the file from the buffer into the bottom (0000) of memory
//at the segment in the parameter. You should use putInMemory to do this.
/* Function putInMemory can be called and it takes 3 parameters:
 The ﬁrst hexadecimal digit of the address, times 0x1000 (segment)
 The remaining four hexadecimal digits of the address.
 The character to be written
 */
while(i < 13312)  {
putInMemory(segment, i, buffer[i]);
i++;
}
//3. Call the assembly function void launchProgram(int segment), which takes
//the segment number as a parameter. This is because setting the registers
//cannot be done in C. The assembly function will set up the registers and
//jump to the program. The computer will never return from this function.

launchProgram(segment);
}

void terminate() {
    interrupt(0x21,4,"shell\0",0x2000,0);     
}

void deleteFile(char* name){
	char map[512];
	char directory[512];
	//char sectors[27];
	char compareFile[7];
	int file,begin,end,k;
	int j;
	int x;
	//int s;
	//int sectorNumber = 0;
	// Load the Directory and Map to 512 byte character arrays.
	readSector(map,1);
	readSector(directory,2);
	//2. Search through the directory and try to find the file name.
	for (file = 0; file < 16; file++) {
    begin = file * 32;
    end = begin + 6;
    k = 0;
        for (j = begin; j < end; j++) 
        {
        compareFile[k] = directory[j];
        k++;
        }
        compareFile[6]='\0';    
	//3. Set the first byte of the file name to 0x00.
              if (Match(compareFile, name, 6)) {
              		directory[begin]=0x0;


	//4. Step through the sectors numbers listed as belonging to the file. For each
	//sector, set the corresponding Map byte to 0x00. For example, if sector 0x07
	//belongs to the file, set the 8th Map byte to 0x00 (you should set the 8th byte
	//not the 7th since the Map starts with sector 0).

              for (x = end ; directory[x] != 0x0; x++){ 
              		int correspondingMap = directory[x];
					map[correspondingMap] = 0x00;
              }
        }
    }
	//5. Write the character arrays holding the Directory and Map back to their
	//appropriate sectors.
	writeSector (directory, 2);
	writeSector (map,1);
}

void writeFile(char* name, char* buffer, int secNum) {
	char map[512];
	char directory[512];
	int file, begin, end;
	int length = 0;
	int t,y,z,k,i, m;
	int start;
	int sectorNumber = 0;
	char hold[512];
        //printString("here");
//1. Load the Map and Directory sectors into buffers.
	readSector(map,1);
	readSector(directory,2);
//2. Find a free directory entry (one that begins with 0x00).
	for (file = 0; file < 16; file++) {
    		begin = file * 32;
    		//end = begin + 6;
 if(directory[begin] == 0x00)
	{
		//3. Copy the name to that directory entry.
		//while the given name is still not finished, increment length of name 
			while(name[length] != '\0'){
				length++;
			}
			end=begin+length;
		//write the name in the directory[0,1,2,3,4,5] , [32,33,34,35,36,37], ......
			for(t =0; t<length; t++){
				directory[begin+t] = name[t];
			}
		//printString(name);
		//If the name is less than 6 bytes, fill in the remaining bytes with 0x00.
		//if length is 2; [0,1] so loop on 2,3,4,5 and let them = 0x00 
		//start = 6-2 =4 ; loop on each of the 2,3,4,5 bytes
			if (length < 6){
				start = 6- length;
				for(y=0; y<start; y++){
				z = y+length;
				directory[begin+z] = 0x00;
				//printString("1");
				}
				}	
	break;
	}
}
	printString(map);
//4. For each sector making up the file:
//• Find a free sector by searching through the Map for a 0x00.
	for (k = 0; k<512; k++) {
       // interrupt(0x10,0xe*256+'c',0,0,0);
		if(map[k] == 0x00)
		{
			//• Set that sector to 0xFF in the Map.
			map[k] = 0xFF;
   			//• Add that sector number to the file’s directory entry.
			
			directory[end +sectorNumber] = k;
				
			for(i=0; i<512;i++)
			{
				hold[i]=buffer[i+(sectorNumber*512)];	
			}
			sectorNumber++;
			writeSector(hold, k);	
		}
			if(sectorNumber==secNum)
			{	
				break;	
			}	
		
	}
	
//5. Fill in the remaining bytes in the directory entry to 0x00.
for(m = end + sectorNumber; m  < 32; m ++) {
directory[m] = 0x00;
}
//6. Write the Map and Directory sectors back to the disk.
	writeSector(directory,2);
	writeSector(map,1);
}



void handleInterrupt21(int ax, int bx, int cx, int dx){

//printString("Task 4 works! Wohoo!!\0");
if (ax == 0) {
printString(bx);
} else if (ax == 1) {
readString(bx);
} else if (ax == 2) {
readSector(bx, cx); 
} 
else if (ax == 3) {
readFile(bx , cx);
} else if (ax == 4) {
executeProgram(bx,cx);
} else if (ax == 5) {
    terminate();
} else if (ax == 6) {
    writeSector(bx, cx);
} else if (ax==7){
	deleteFile(bx);
} else if (ax == 8){
	writeFile(bx,cx,dx);
} else {
printString("There is an error!\0");
}
}


