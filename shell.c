void Match (char*);

main() {


while(1) {
char line[80];
int i=0;
interrupt(0x21,0,"Shell> \0",0,0);
interrupt(0x21,1,line,0,0);
Match(line);
}
while(1);
}

void Match (char* line) {

char* view = "view\0";
char* execute = "execute\0";
char* delete = "delete\0";
char* copy = "copy\0"; 
char* dir = "dir\0";
char* create = "create\0";
int i = 0;
int j = 0;
char directory[512];
int y = 0;
int flag = 0;

int sameV = 0;
int sameE = 0;
int sameD = 0;

int sameCopy = 0;
int samedir = 0;
int sameCreate = 0;

int counter = 0;
char compare[7];
char compare2[7];
int k = 0;
char buffer[13312];
int z = 0;
int count = 0;
char* array;
int cc = 12;

// we dont know from which index the word starts, therefore, we did this loop to figure out the index of the word starts to be able to read it
while(line[j]==0x0 || line[j]=='\n' || line[j]=='\r') 
{
     j++;	
}

counter = j;

//loop to check the word view
for (i=0; i < 4; i++) { // 4 is number of letters in view
 	
 if (line[j] == view[i]) {
    sameV = 1;
}
  else {
    sameV = 0;
    break;
}
j++;
}

//if not view, loop to check the word execute
if(sameV == 0) {
j = counter;
for (i = 0; i < 7; i++) { // 7 is the number of letters in execute
 if (line[j] == execute[i]) {
    sameE = 1;
}
  else {
    sameE = 0;
    break;
}
j++;
}
}

//if not view and not execute, loop to check the word delete
if (sameV == 0 && sameE == 0) {
j = counter;
for (i = 0; i < 6; i++) { // 6 is the number of letters in delete
 if (line[j] == delete[i]) {
    sameD = 1;
}
  else {
    sameD = 0;
    break;
}
j++;
}
}

if (sameV == 0 && sameE == 0 && sameD == 0) {
j = counter;
for (i = 0; i < 4; i++) { // 4 is the number of letters in copy
if (line[j] == copy[i]) {
    sameCopy = 1;
}
  else {
    sameCopy = 0;
    break;
}
j++;
}
}

if (sameV == 0 && sameE == 0 && sameD == 0 && sameCopy == 0) {
j = counter;
for (i = 0; i < 3; i++) { // 3 is the number of letters in dir
if (line[j] == dir[i]) {
    samedir = 1;
}
  else {
    samedir = 0;
    break;
}
j++;
}
}

if (sameV == 0 && sameE == 0 && sameD == 0 && sameCopy == 0 && samedir == 0) {
j = counter;
for (i = 0; i < 6; i++) { //6 is the number of letters in create
if (line[j] == create[i]) {
    sameCreate = 1;
}
  else {
    sameCreate = 0;
    break;
}
j++;
}
}

for (i = j+1; k < 6; i++) {
compare[k] = line[i];
k++;
}
compare[6] = '\0';

k =0;

if (sameCopy == 1) {
for (i = j+1+6+1; k < 6; i++) {
compare2[k] = line[i];
k++;
}
compare2[6] = '\0';
}

if (sameV == 1) {
interrupt(0x21, 3, compare, buffer, 0); /*read the file into buffer*/
interrupt(0x21, 0, buffer, 0, 0); 
if (buffer[0] == 0x00) {
interrupt(0x21,0,"File not found!\0",0,0);
}
}

else if (sameE == 1) {
interrupt(0x21, 4, compare, 0x2000, 0);
}
else if (sameD == 1) {
interrupt(0x21, 7, compare, 0, 0);
interrupt(0x21, 3, compare, buffer, 0); // try to read messag
interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer
} 

else if (sameCopy == 1) {
interrupt(0x21, 3, compare, buffer, 0); // try to read messag
if (buffer[0] == 0x00) {
interrupt(0x21,0,"File not found!\0",0,0);
}
interrupt(0x21, 8, compare2 , buffer, 1); //write file compare2
}

else if (samedir == 1) {
interrupt(0x21, 2, directory , 2, 0);
interrupt(0x21,0,"\r\n\0",0,0);
while(counter<512) {
for(y=-2; y<4;y++){
if(directory[counter+y] !=0){
interrupt(0x10, 0xe*256+directory[counter + y], 0, 0,0,0);
}
else{
break;
}
}
interrupt(0x21,0,"\r\n\0",0,0);
counter = counter + 32;
}
interrupt(0x10, 0xe*256+0x00, 0, 0 ,0);
//kameli henna ya monica
}

else if (sameCreate == 1) {
interrupt(0x21, 8, compare , "", 1); //write file compare
while (1) {
interrupt(0x21,1,buffer,0,0);
interrupt(0x21, 8, compare , buffer, 1);
interrupt(0x21, 3, compare, array, 0);
while(buffer[cc] != '\0'){
	cc++;
			}
if (buffer[cc] == 0) {
break;
}
else {
//interrupt(0x21, 8, compare , buffer, 1);
}
}

/*flag = 1;
while (flag = 1) {
interrupt(0x21,1,buffer,0,0);
interrupt(0x21, 8, compare , buffer, 1); //write file compare
if (buffer[0] == '\0') {
interrupt(0x21, 0, "ana henna\0", 0, 0);
flag =0;
break;
}
while (z<13312) {
buffer[z] = 0x00;
z++;
}
}*/
}

/*while (1) {
//interrupt(0x21,0,"This> \0",0,0);
interrupt(0x21,1,buffer,0,0);
interrupt(0x21, 8, compare , buffer, 1); //write file compare
while(buffer[cc] != '\0'){
	cc++;
			}
//interrupt(0x21, 3, compare, array, 0); // try to read messag
//if (buffer[cc] == 0x00) {
if (buffer == 0x00) {
interrupt(0x21, 0, "ana henna\0", 0, 0); //print out the contents of buffer
//interrupt(0x21, 8, compare , buffer, 1); //write file compare
interrupt(0x21, 0, buffer[cc], 0, 0); //print out the contents of buffer
break;
 }
//cc++;
}
*/
//}

else 
{
interrupt(0x21,0,"Bad Command!\0",0,0);
}

while (z<13312) {
buffer[z] = 0x00;
z++;
}
}


