#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int load=0,flag,flag1=0,flag2=0;// Flags for various operations // Global variables
struct stack{//define a stack for undo and redo operations
int index;//index of strings
int operation;//let 0 insert 1 delete
char data[1000];// Data associated with operation
struct stack * top;// Pointer to the top of the stack
struct stack * Next;// Pointer to the next element in the stack
};
struct QueueRecord{// Define a queue structure for string insertion
struct QueueRecord * Front;// Pointer to the front of the queue
struct QueueRecord * Rear;// Pointer to the rear of the queue
struct QueueRecord * Next;// Pointer to the next element in the queue
char data[1000];// Data associated with the queue element
};

//function prototypes
void printMenu();
struct stack * CreateStack();
void MakeEmptyStack(struct stack * s);
int IsEmptyStack(struct stack * s);
void Pop(struct stack *s);
void DisposeStack(struct stack* s);
void push(int index,int operation,char x[],struct stack * s);
struct QueueRecord* CreateQueue();
void MakeEmptyQueue(struct QueueRecord* Q);
void Enqueue(char x[],struct QueueRecord* Q);
void Dequeue(struct QueueRecord* Q);
int IsEmpty(struct QueueRecord* Q);
void LoadFile(char x[]);
void saveToOutputFile(char x[]);
void PrintTheLoadedText(char x[]);
void InsertStrings(int index,char x[],char word[]);
void InsertStringsByUser(char x[],struct stack* UndoStack);
void RemoveStringsByUser(char x[],struct stack * UndoStack);
void performUndoOperation(char x[],struct stack* UndoStack,struct stack* RedoStack);
void performRedoOperation(char x[],struct stack* RedoStack);
void printStackContent(struct stack* Stack);
int findIndexForInsert(char array[],char word[]);
int findIndex(char str[], char word[]);
int isJustSpace(char array[]) ;

//Main function
int main()
{
    int option;//the option that will be entered by the user
    char Array[1000];// Array to store input string from file

    // Create stacks for undo and redo operations
    struct stack * UndoStack=CreateStack();
    struct stack * RedoStack=CreateStack();
    do{// Menu loop
        printMenu();// Display menu to the user
        scanf("%d",&option);// Get user's choice
        getchar();//let gets in other function read string within incorporated spaces correctly // Remove the newline character from the input buffer
        switch(option){// Process user's choice
        case 1:LoadFile(Array);break;
        case 2:PrintTheLoadedText();break;
        case 3:InsertStringsByUser(Array,UndoStack);break;
        case 4:RemoveStringsByUser(Array,UndoStack);break;
        case 5:performUndoOperation(Array,UndoStack,RedoStack);break;
        case 6:performRedoOperation(Array,RedoStack);break;
        case 7:printf("Undo Stack:\n");printStackContent(UndoStack);printf("Redo Stack:\n");printStackContent(RedoStack);break;
        case 8:saveToOutputFile(Array);DisposeStack(UndoStack);DisposeStack(RedoStack);break;
        case 9:printf("exit application\n");DisposeStack(UndoStack);DisposeStack(RedoStack);exit(0);
        default:printf("There is no such operation ,try again!\n");break;
        }

    }while(option!=9);// Continue until user chooses to exit

    return 0;
}

void printMenu(){//display menu to user
printf("Welcome to my text editor application\n");
printf("Menu of my application\n");
printf("Please select an operation \n");
printf("1-Load the input file which contains the initial text\n");
printf("2-Print the loaded text\n");
printf("3-Insert strings to the text\n");
printf("4-Remove strings from the text\n");
printf("5-Perform Undo operation\n");
printf("6-Perform Redo operation\n");
printf("7-Print the Undo Stack and the Redo stack\n");
printf("8-Save the updated text to the output file\n");
printf("9-Exit\n");
}

struct stack * CreateStack(){// Function to create a stack
struct stack * s;
s=(struct stack*)malloc(sizeof(struct stack));
if(s==NULL){
    printf("out of space\n");
}
s->Next=NULL;
MakeEmptyStack(s);
return s;
}

void MakeEmptyStack(struct stack * s){// Function to make the stack empty
if(s==NULL){
    printf("out of space\n");
    exit(0);
}
else while (!IsEmptyStack(s))
    Pop(s);
}

int IsEmptyStack(struct stack * s){// Function to check if the stack is empty
return s->Next==NULL;
}

void Pop(struct stack *s){// Function to pop an element from the stack
struct stack * firstCell;
if(IsEmptyStack(s)){
    printf("Empty Stack\n");
}
else{
    firstCell=s->Next;
    s->Next=firstCell->Next;
    s->top=s->Next;
    free(firstCell);
}
}

void push(int index,int operation,char x[],struct stack * s){// Function to push an element onto the stack
struct stack * temp;
temp=(struct stack*)malloc(sizeof(struct stack));
if(temp==NULL){
    printf("out of space\n");
    exit(0);
}
 else{
    strcpy(temp->data,x);
    temp->operation=operation;
    temp->index=index;
    temp->Next=s->Next;
    s->Next=temp;
    s->top=temp;
}
}

void DisposeStack(struct stack* s){// Function to dispose of the stack
MakeEmptyStack(s);
free(s);
}

struct QueueRecord* CreateQueue(){// Function to create a queue
struct QueueRecord* Q;
Q=(struct QueueRecord*)malloc(sizeof(struct QueueRecord));
if(Q==NULL){
    printf("Out of Space");
    exit(0);
}
Q->Next=NULL;
MakeEmptyQueue(Q);
return Q;
}

void MakeEmptyQueue(struct QueueRecord* Q){// Function to make the queue empty
if(Q==NULL){
    printf("out of space\n");
    exit(0);
}
else while (!IsEmpty(Q))
    Dequeue(Q);
}

void Enqueue(char x[],struct QueueRecord* Q){// Function to enqueue an element into the queue
struct QueueRecord* temp;
temp=(struct QueueRecord*)malloc(sizeof(struct QueueRecord));
if(temp==NULL){
    printf("out of space\n");
    exit(0);
}
else{
    strcpy(temp->data,x);
    if(IsEmpty(Q)){
    Q->Next=temp;
    Q->Front=Q->Next;
    temp->Next=NULL;
    }
    else{
     Q->Rear->Next=temp;
     temp->Next=NULL;
    }
    Q->Rear=temp;
}
}

void Dequeue(struct QueueRecord* Q){// Function to dequeue an element from the queue
struct QueueRecord* temp;
if(IsEmpty(Q)){
    printf("Empty Queue!\n");
}
else {
    temp=Q->Next;
    Q->Next=temp->Next;
    Q->Front=Q->Next;//set front to the next element that stayed in the queue the longest
    free(temp);
}
}

int IsEmpty(struct QueueRecord* Q){// Function to check if the queue is empty
return Q->Next==NULL;
}

void LoadFile(char x[]){
FILE *input;// Declare a file pointer
input=fopen("originalText.txt","r");// Open the file  in read mode
if(input==NULL){// Check if the file was opened successfully
    printf("Error in opening the file!\n");// Print an error message if opening fails
    exit(0); // Exit the program if opening fails
}
char inputString[1000];// Define a string to store the input string from the file
fgets(inputString,sizeof(inputString),input);// Read a line from the file and store it in inputString
strcpy(x,inputString);// Copy the contents of inputString to the provided character array x
printf("Loaded successfully!\n");// Print a success message
fclose(input); // Close the file
load=1;// Set the load flag to indicate that a file has been loaded
flag=0;
}

void PrintTheLoadedText(){//print the loaded text
FILE *input;// Declare a file pointer
input=fopen("originalText.txt","r");// Open the file  in read mode
if(input==NULL){// Check if the file was opened successfully
    printf("Error in opening the file!\n");// Print an error message if opening fails
    exit(0); // Exit the program if opening fails
}
char inputString[1000];// Define a string to store the input string from the file
fgets(inputString,sizeof(inputString),input);// Read a line from the file and store it in inputString
printf("%s\n",inputString);// Print the contents of inputString to the console
fclose(input);// Close the file
}

void InsertStrings(int index,char x[],char s[]){
int StringLength;
char StringCopy[1000];
if(index==-1){//insert last
if(flag==0){//inserting last for the first time
StringLength=strlen(x)-1;
x[StringLength]=' ';// Add a space before appending
}
else {
strcat(x," ");// Add a space before appending
}
strcat(x,s);// Append the string to x
StringLength=strlen(x);
x[StringLength]='\0';// Null terminate the string
flag=1;
}
else {
     strcpy(StringCopy,x+index);// Copy the substring starting from index
     strcpy(x+index,s);// Insert the string at index
     if(flag1!=1){
        strcat(x," ");
     }
     strcat(x,StringCopy);// Append the copied substring after inserted string
}
}

void InsertStringsByUser(char x[],struct stack* UndoStack){// Function to insert strings by user input
flag1=0,flag2=0;
if(load==0){// Check if a file has been loaded
    printf("Please Load First\n!");
}
else{
int choice,index,Add;
char InsertedString[1000],word[1000];
printf("Choose where to insert(1-3):\n");
printf("1-Last\n2-Middle(includes adding a string to a substring)\n3-First\n");
scanf("%d",&choice);
getchar();
printf("Enter the string to be inserted\n");
gets(InsertedString);
switch (choice){
    case 1:index=-1;break;
    case 2:printf("%s\n",x);
    printf("Enter a word or a sub word to insert after!\nEnter -1 if you want to add the string to the left of the first word\n");
    gets(word);
    index=findIndexForInsert(x,word);// Find index to insert after
    if(index==-2){
        index=findIndex(x,word);// Find index of the word
        index+=strlen(word);// Move index to the end of the word
        Add=atoi(word);// Convert word to integer
        if(index==-2&&Add==-1){
            index=0;
            flag1=1;
        }
    }
    else{
      index+=strlen(word)+1;// Move index to the end of the word plus one space
    }
    break;
    case 3:index=0;break;
    default:printf("Non valid input try again\n");flag2=1;break;
}
if(index!=-2&&flag2!=1){// Insert substring
struct QueueRecord* Q=CreateQueue();// Create a queue to store words
char * tok=strtok(InsertedString," ");// split the inserted string
while(tok!=NULL){
strcpy(word,tok);
Enqueue(word,Q); // Enqueue words into the queue
tok=strtok(NULL," ");//move to the next token
}
while(!IsEmpty(Q)){// Dequeue and insert words into x
strcpy(word,Q->Front->data);
Dequeue(Q);
push(index,0,word,UndoStack);// Push  to the undo stack
InsertStrings(index,x,word);// Insert the word into x
if(index!=-1){// Update index if not inserting at the end
    index=findIndexForInsert(x,word);
    index+=strlen(word)+1;// Move index to the end of the inserted word plus one space
}
}
free(Q);// Free memory allocated for the queue
}
else printf("Word to insert after is not found try again!\n");
}
}

void RemoveStrings(int index,char x[],char DeletedString[]){
int i,StringLength,OriginalStringLength=strlen(x);
// Calculate the length of the string to be removed
if(isJustSpace(DeletedString)){
StringLength=strlen(DeletedString);
}
else{
StringLength=strlen(DeletedString)+1;// Include the space after the string
}
index+=StringLength;// Adjust the index to point to the end of the string to be removed
for(i=index;i<OriginalStringLength;i++){// Shift the characters in the string to remove the specified substring
x[i-StringLength]=x[i];// Move characters backward
}
x[strlen(x)-StringLength]='\0';// Null-terminate the string to ensure it ends at the correct position
}

void RemoveStringsByUser(char x[],struct stack * UndoStack){
if(load==0){
    printf("Please Load File!\n");// Prompt user to load the file first
}
else{
int index;
char DeletedString[101];
printf("%s\n",x);// Print the current text
printf("Insert the word or string to be removed!\n");
gets(DeletedString);// Get the word or string to be removed from the user
index=findIndex(x,DeletedString);// Find the index of the word or string in the text
if(index!=-2){// If the word or string is found in the text
RemoveStrings(index,x,DeletedString);// Remove the word or string from the text
push(index,1,DeletedString,UndoStack);// Push  to the undo stack
}
}
}

void printStackContent(struct stack* Stack){// Function to print the content of the stack
if(!IsEmptyStack(Stack)){
char data[1000];//string to store data from the stack
int index,operation;// Variables to store index and operation type
// Create temporary stacks to manipulate the stack content
struct stack* temp1=CreateStack();
struct stack* temp2=CreateStack();
struct stack* temp=Stack->Next;// Start from the first node of the stack
while(temp!=NULL){//copy stack content to another stack (order is inversed)
    push(temp->index,temp->operation,temp->data,temp1);// Push elements to temp1
    temp=temp->Next;// Move to the next node
}
while(!IsEmptyStack(temp1)){//copy stack content to another stack again to maintain order of the original stack
    push(temp1->top->index,temp1->top->operation,temp1->top->data,temp2);// Push elements to temp2
    Pop(temp1);// Pop the top element from temp1
}
if(IsEmptyStack(temp2)){
printf("list is empty\n");// Print a message if the stack is empty
}
else {// Print the contents of the stack
while(!IsEmptyStack(temp2)){//printing loop pops the element from the stack then prints it
// Retrieve index, operation, and data from the stack
index=temp2->top->index;
operation=temp2->top->operation;
strcpy(data,temp2->top->data);
Pop(temp2);// Pop the top element from the stack
// Print the data, operation type, and index
printf("%s\t",data);
if(operation==0){
    printf("Insert\t");
}
else printf("remove\t");
printf("%d\n",index);
}
}
free(temp1);// Free the memory allocated for temp1
free(temp2);// Free the memory allocated for temp2
}
else printf("list is empty\n");// Print a message if the stack is empty
}

void performUndoOperation(char x[],struct stack* UndoStack,struct stack* RedoStack){
if(!IsEmptyStack(UndoStack)){//perform the undo operation while the stack is not empty
int index=UndoStack->top->index;// Get the index from the top of the stack
int operation=UndoStack->top->operation;// Get the operation type from the top of the stack
if(operation==0){//if operation was insert remove string and push to the redo stack
operation=1;// Change the operation type to remove
if(index==-1){// If the index is -1, find the index of the string in the text
index=findIndex(x,UndoStack->top->data);
push(-1,operation,UndoStack->top->data,RedoStack);// Push to the redo stack
}
else{
    push(index,operation,UndoStack->top->data,RedoStack);
}
RemoveStrings(index,x,UndoStack->top->data);// Remove the string from the text
}
else{//if operation was  remove ,insert string and push to the redo stack
InsertStrings(index,x,UndoStack->top->data);// Insert the string into the text
operation=0;// Change the operation type to insert
push(index,operation,UndoStack->top->data,RedoStack);// Push to the redo stack
}
Pop(UndoStack);// Remove the node from the undo stack
} else printf("Undo operation can't be performed!\n");// Print a message if the undo stack is empty
}

void performRedoOperation(char x[],struct stack* RedoStack){
if(!IsEmptyStack(RedoStack)){//perform the redo operation while the stack is not empty
int index=RedoStack->top->index;// Get the index from the top of the stack
int operation=RedoStack->top->operation;// Get the operation type from the top of the stack
if(operation==0){//if operation was insert ,just remove string from the text
RemoveStrings(index,x,RedoStack->top->data);// Remove the string from the text
}
else{//if operation was remove ,just insert string from the text
InsertStrings(index,x,RedoStack->top->data);// Insert the string into the text
}
Pop(RedoStack);// Remove the node from the redo stack
}
else printf("Redo operation can't be performed!\n");// Print a message if the redo stack is empty
}

void saveToOutputFile(char x[]){
FILE *out;//define a pointer that will be used to open the file
out=fopen("output.txt","w");// Open the file  in write mode
if (out == NULL){// Check if the file was opened successfully
printf("Error in opening the file for writing!\n");// Print an error message if opening fails
exit(0);// Exit the program if opening fails
}
if(load==1){//check if the string was already loaded
    fprintf(out,"%s\n",x);// Write the contents of string 'x' to the file
    printf("Successfully saved!\n");// Print a success message
}
else printf("File has not loaded!\n");// Print a message indicating that the file has not been loaded
fclose(out);// Close the file
}

int findIndexForInsert(char array[],char word[]){// Function to find the index where a word to insert after is in array
char StringCopy1[101]; // Temporary copy of the array
int i = 0; // Index counter
strcpy(StringCopy1,array);// Copy the input array to a temporary array
char * tok=strtok(StringCopy1," ");// split the temporary array by space
while(tok!=NULL){// Loop until there are no more tokens
    if(strcasecmp(tok,word)==0){// If the token matches the word
        return i;// Return the index
    }
    i+= strlen(tok)+1;// Update index for the next token
    tok=strtok(NULL," ");// Move to the next token
}
return -2;// Return -2 if the word is not found
}

int findIndex(char str[], char word[]) {// Function to find the index of the first occurrence of a word or sub string in a string
    char *ptr = strstr(str, word);// Find the first occurrence of the word in the string
    if (ptr != NULL) {// If the word is found
        return strlen(str)-strlen(ptr);// Return the index of the word in the string
    }
    return -2; // Return -2 if the word is not found
}

int isJustSpace(char array[]) {//function to know if the string to be inserted or deleted is just a space
int i=0;// Index counter
for(i=0;array[i]!='\0';i++){// Loop until the end of the string
if (array[i]==' ') {//if space return true
return 1;
}
}
return 0;
}
