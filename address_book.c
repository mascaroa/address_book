#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define N 10



struct contact{
	char first[40];
	char last[40];
	char add[100];
	char post[8];
	char ph[12];
	struct contact *pNext;
};



struct contact *recursiveReverse(struct contact *head, struct contact *prev);
void addCont();
void displayCont(int flag,int f);
void searchCont();
int saveCont();
int loadCont();
int deleteCont();
void sort();
int validPhone(char *ph);
void printPost(char *post);
void printPhone(char *ph);
char* endecode(char *add, int ende);
void wordCap(char *string);
int postCap(char *post);
int eraseList();

struct contact *pFirst = NULL;
struct contact *pLast = NULL;


//---------START OF MAIN---------------------------------------------------------------------------
int main(){
	int i=0, yn;


	//---------DO-WHILE LOOP, interactively inputs user selection until exit is selected-------
	do{
		printf("\n\n **** AWESOME CONTACT LIST PROGRAM V 42.0 ****\n 1. Add Contact.\n 2. Display all contacts.\n 3. Search for a contact.\n 4. Save contacts to file.\n 5. Load contacts from file.\n 6. Delete a contact.\n 7. Sort all contacts by last name. \n 8. Exit\n");
		scanf("%d",&yn);
		fflush(stdin);
		switch(yn){
			case 1:
				addCont();
				i++;
				break;
			case 2:
				displayCont(i+1,i);
				break;
			case 3:
				searchCont(i);
				break;
			case 4:
				saveCont();
				break;
			case 5:
				i = loadCont() + 1;
				break;
			case 6:
				printf("\nLocated and deleted %d contact(s).\n",deleteCont());
				break;
			case 7:
				sort();
				break;
			case 8:
				saveCont();
				printf("\n Goodbye!\n\n");
				return 0;
			}
	}while(yn !=9);

eraseList();
return 0;
}


/*---------ADD CONTACT FUNCTION--------------------------------------------------------------------
INPUT: struct array of type contact containing contact data, integer i specifying which # the added
contact will be.
FUNCTION: input contact (firstname, lastname, address, postal code, phone number) from keyboard, save
to the struct array.
OUTPUT: VOID
*/

struct contact *recursiveReverse(struct contact *head, struct contact *prev){
	struct contact* temp=NULL;
	if(head->pNext == NULL){
		head->pNext = prev;
		pFirst = head;
		return prev;
		}	
	
	else{
		temp = head->pNext;
		head->pNext = prev;
		return prev;
		}

}

void addCont(){
	char yn, temp;
	struct contact *pNew=NULL;	//temporary struct pointer to create new node
	pNew = (struct contact *) malloc(sizeof(struct contact));

	//---------User input: used fgets to eliminate buffer overflow errors.
	//---------NOTE: wordCap function also removes '\n' char from strings (from fgets), eliminating possible errors.
	printf("\nFirst Name\t:");
	scanf("%c",&temp);
	fgets(pNew->first,sizeof(pNew->first),stdin);
	wordCap(pNew->first);
	fflush(stdin);
	printf("\nLast Name\t:");
	fgets(pNew->last,sizeof(pNew->last),stdin);
	wordCap(pNew->last);
	fflush(stdin);
	printf("\nAddress\t\t:");
	fgets(pNew->add,sizeof(pNew->add),stdin);
	wordCap(pNew->add);
	fflush(stdin);
	printf("\nPostalCode\t:");
	fgets(pNew->post,sizeof(pNew->post),stdin);

	//---------postCap function returns a non-zero value if postal code is not of the format 'a#a#a#'
	while(postCap(pNew->post) != 0){
		printf("\nPlease enter a valid postal code:\n");
		fgets(pNew->post,sizeof(pNew->post),stdin);
		}
	fflush(stdin);
	printf("\nPhone\t\t:");
	fgets(pNew->ph,sizeof(pNew->ph),stdin);
	fflush(stdin);

	//---------validPhone function is used to validate the phone number (10 values, all numerical digits)
	while(validPhone(pNew->ph) != 1){
		printf("\nPlease enter a valid 10-digit phone number:\n");
		fgets(pNew->ph,sizeof(pNew->ph),stdin);
		}


	if(pFirst == NULL){	//if no data, set this as first/last node
		pFirst = pNew;
		pLast = pNew;
		}
	else{				//otherwise set the self-ref pointer of pNew to pFirst
		pNew->pNext = pFirst;	//then set pFirst = pNew
		pFirst = pNew;
		}

}

/*---------DISPLAY CONTACT FUNCTION----------------------------------------------------------------
INPUT: struct array of type contact, integer f used as a flag.
FUNCTION: will print to screen all contacts if f > N using printf and the functions printPost()
and printfPhone(), OR will print the contact stored at main[f] if f<N.
OUTPUT: VOID
*/

void displayCont(int flag, int f){
	struct contact *pF = pFirst;		//temporary struct pointer to traverse list
	int i=1;
	if(flag > f){
		while(pF != NULL){
			printf("\nContact #%d:",i);
			printf("\n First Name\t:%s\n Last Name\t:%s\n",pF->first,pF->last);
			printf(" Address\t:%s",endecode(pF->add,0));
			printPost(pF->post);
			printPhone(pF->ph);
			printf("\n");
			i++;
			pF = pF->pNext;
			}
		}
	else{
		for(i=0;i<f;i++){
			pF = pF->pNext;
		}
		printf("\nContact %d:",f+1);
		printf("\n First Name\t:%s\n Last Name\t:%s\n ",pF->first,pF->last);
		printf("Address\t:%s ",endecode(pF->add,0));
		printPost(pF->post);
		printPhone(pF->ph);
		printf("\n");
		}

}

/*---------SEARCH CONTACT FUNCTION-----------------------------------------------------------------
INPUT: struct array of type contact containing all contact data.
FUNCTION: utilizes linear search to locate and print all contacts based on user-inputted lastName.
OUTPUT: VOID
*/


void searchCont(int f){
	//--------- disp array will be used as a flag to display specific contacts only
	//--------- if disp[i] = 1, contact stored at main[i] matches entered last name

	int i=0, disp[f], count=0;
	char last[40], temp;
	struct contact *pF = pFirst;
	printf("\nWhat is the last name of the contact?\n");
	scanf("%c",&temp);
	//--------- User inputted last name to search for
	//--------- Using wordCap function on inputted lastname makes this CASE INSENSITIVE 
	fgets(last,sizeof(last),stdin);
	wordCap(last);
	while(pF != NULL){
		if(strcmp(last,pF->last) == 0){
			disp[i]=1;
			count++;
		}
		else
			disp[i]=0;
		pF = pF->pNext;
		i++;
		}

	printf("\nFound %d result(s):\n",count);
	for(i=0; i<f; i++){
		if(disp[i] == 1)
			displayCont(i,i);
		}


}

/*---------SAVE CONTACT FUNCTION-------------------------------------------------------------------
INPUT: struct array of type contact containing all contact data.
FUNCTION: saves all contact data to a file 'contactlist.dat'.  Saved data adheres to the following
grammar:
	#(contact number)
	first name:.....
	last name:.....
	address:....
	postal code:.....
	phone:.....
the '#' symbol will be used as a delimiter for differentiating between stored contacts, and the ':'
symbol will be used for differentiating between data fields when inputting from file and deleting 
contacts.

OUTPUT: VOID.
*/

int saveCont(){
	FILE *add;
	char yn;
	int i = 0;
	struct contact *pF = pFirst;			//temporary struct pointer to traverse list
	if(pF == NULL)
		return 0;

	printf("\nOverwrite file 'contactlist.dat'?? (y/n):\n");
	scanf(" %c",&yn);
	fflush(stdin);
	if(yn == 'y')
		add = fopen("contactlist.dat","wt");
	else
		return 0;
	if(add == NULL)
		return 0;

	while(pF != NULL){
		fprintf(add,"\n#%d\n",i);
		fprintf(add,"First Name:%s\n",pF->first);
		fprintf(add,"Last Name:%s\n",pF->last);
		fprintf(add,"Address:%s\n",endecode(pF->add, 1));
		fprintf(add,"Postal Code:%s\n",pF->post);
		fprintf(add,"Phone:%s\n",pF->ph);
		i++;
		pF = pF->pNext;
		}
	return 1;
	printf("\nContact list saved successfully!\n");
	fclose(add);
}

/*---------LOAD CONTACT FUNCTION-------------------------------------------------------------------
INPUT: None, uses global variables.
FUNCTION: will read all contact data (up to N contacts) stored in file 'contactlist.dat' and save
each contact to the contact struct array.  See comment above (SAVE CONTACT FUNCTION) for saved file
grammar.
OUTPUT: integer describing the # of contacts entered. Error or user abort condition: return -1.
This will set the integer i in main function correctly to input the NEXT contact if addCont() is 
called afterwards.
*/

int loadCont(){
	FILE* add;
	int i=0;
	char temp, yn = 'y';
	struct contact* pF=NULL;
	printf("\nOverwrite runtime data?? (y/n):\n");
	scanf(" %c",&yn);
	fflush(stdin);
	if(yn == 'y' || yn == 'Y'){
		add = fopen("contactlist.dat","rt");
		eraseList();
		}
	else
		return -1;
	if(add == NULL){
		printf("\nFile not found!!!\n");
		return -1;
		}
	temp = fgetc(add);
	eraseList();
	do{
		pF = (struct contact *) malloc(sizeof(struct contact));	
		while( temp != '#' && temp != EOF){ temp = fgetc(add);}
		if(temp == EOF)
			break;
		while((temp!= '#') && (temp != EOF)){temp=fgetc(add);}
		fscanf(add,"%d",&i);

		while((temp!= ':') && (temp != EOF)){temp=fgetc(add);}
		fgets(pF->first,sizeof(pF->first),add);
		wordCap(pF->first);
			temp=fgetc(add);
		while((temp!= ':') && (temp != EOF)){temp=fgetc(add);}
		fgets(pF->last,sizeof(pF->last),add);
		wordCap(pF->last);
			temp=fgetc(add);
		while((temp!= ':') && (temp != EOF)){temp=fgetc(add);}
		fgets(pF->add,sizeof(pF->add),add);
		wordCap(pF->add);
			temp=fgetc(add);
		while((temp!= ':') && (temp != EOF)){temp=fgetc(add);}
		fgets(pF->post,sizeof(pF->post),add);
		postCap(pF->post);
			temp = fgetc(add);
		while((temp!= ':') && (temp != EOF)){temp=fgetc(add);}
		fgets(pF->ph,sizeof(pF->ph),add);
		if(pFirst == NULL){		//if no current nodes in list, set this as first
			pFirst = pF;
			pLast = pF;
			}
		else{
			pLast->pNext = pF;	//otherwise set this as last node
			pLast = pF;
			pLast->pNext = NULL;
			}
		temp = fgetc(add);
		pF = NULL;
		}while(temp != EOF);
	fclose(add);
	return i;
}

/*---------DELETE CONTACT FUNCTION-----------------------------------------------------------------
INPUT: None, uses global variables.
FUNCTION: this function searches for a specific contact in a file contactlist.dat by first AND 
last name and deletes this contact data by copying all of the other data (before and after) to a 
file temp.dat, then deleting contactlist.dat file and renaming temp.dat to contactlist.dat.
The '#' is used as a delimiter to differentiate between contacts.
OUTPUT: integer describing the number of contacts deleted.
*/

int deleteCont(){
	char delFirst[40],delLast[40];
	struct contact* pF = pFirst;
	struct contact* thisOne = NULL;
	int count = 0;
	char temp;
	scanf("%c",&temp);
	printf("\nPlease enter the FIRST name of the contact you wish to delete:\n");
	fgets(delFirst,sizeof(delFirst),stdin);
	wordCap(delFirst);
	fflush(stdin);
	printf("\nPlease enter the LAST name of the contact you wish to delete:\n");
	fgets(delLast,sizeof(delLast),stdin);
	wordCap(delLast);
	while(pF != NULL){
		if(strcmp(delLast,pF->last) == 0){
			if(strcmp(delFirst,pF->first) == 0){
				if(pF->pNext == NULL && pF != pFirst){
					thisOne->pNext = NULL;
					free(pF);
					pF = NULL;
					}
				else if(pF == pFirst && pF->pNext == NULL){
					free(pF);
					pF = NULL;
					pFirst = NULL;
					}
				else if(pF == pFirst && pF->pNext != NULL){
					pFirst = pF->pNext;
					free(pF);
					pF = pFirst;
					if(pF != NULL){
						thisOne = pF;
						pF = pF->pNext;
						}
					}
				else{
					//pF = pF->pNext;
					thisOne->pNext = pF->pNext;
					thisOne = pF;
					pF = pF->pNext;
					free(thisOne);
					}
				count++;
				}
			else if(pF != NULL){
				thisOne = pF;
				pF = pF->pNext;
				}
			}
		else if(pF != NULL){
			thisOne = pF;
			pF = pF->pNext;
			}
	}


	return count;
}

/*---------PHONE NUMBER VALIDATE FUNCTION----------------------------------------------------------
INPUT: None, uses global variables.
FUNCTION: determines if the input string consists solely of numbers (isdigit()) and is of length 10.
OUTPUT: returns 1 if input IS a valid phone number, 0 otherwise.
*/

int validPhone(char *ph){
	int i=0;
	char *num;
	num = ph;
	while(*num != '\0' && *num != '\n'){
		if(!isdigit(*num))
			return 0;
		*num++;
		i++;
		}
	if(i == 10)
		return 1;
	else
		return 0;
}

/*---------SORT LIST FUNCTION-----------------------------------------------------------------
INPUT: None, uses global variables.
FUNCTION: Sort the linked list by last name.
OUTPUT: NULL
*/

void sort(){
	struct contact* pF;
	struct contact* comp;
	struct contact* temp = NULL;
	int flag = 0;
	int i = 1;
	while(flag < i-1){
		flag = 0;
		i = 0;
		pF = pFirst;
		comp = pFirst->pNext;
		while(comp != NULL && pF != NULL){
			if(strcmp(pF->last,comp->last) > 0){
				if(temp == NULL){
					pFirst = comp;
					pFirst->pNext = pF;
					pF->pNext = comp->pNext;
					comp = pFirst->pNext;
					pF = comp->pNext;
					}
				else{
					temp->pNext = comp;
					pF->pNext = comp->pNext;
					comp->pNext = pF;
					}
				}
			else
				flag++;
			temp = pF;
			pF = pF->pNext;
			comp = comp->pNext;
			i++;
			}
		}
}


/*---------PRINT PHONE FUNCTION--------------------------------------------------------------------
INPUT: pointer to char (string)
FUNCTION: print string (pointer to 10-digit phone number) in the format (###) ###-####
OUTPUT: VOID
*/

void printPhone(char *ph){
	int i=0;
	char *ptr = ph;
	printf("\n Phone\t\t:(");
	while(*ptr != '\0'){
		if(i==3){
			printf(") ");
			}
		else if(i==6){
			printf("-");
			}
		printf("%c",*ptr);
		*ptr++;
		i++;
		}

}

/*---------PRINT POSTAL CODE FUNCTION--------------------------------------------------------------
INPUT: pointer to char (string)
FUNCTION: print string (pointer to postal code) in the format A#A-#A#
OUTPUT: VOID
*/
void printPost(char *post){
int i=0;
char *postptr;
postptr = post;
printf("\n Postal Code\t:");
while(*postptr != '\0'){
	if(i==3)
		printf("-");
	printf("%c",*postptr);
	*postptr++;
	i++;
	}

}

/*---------ENCODE/DECODE FUNCTION------------------------------------------------------------------
INPUT: pointer to char (string), integer used as flag
FUNCTION: if the flag (ende) is 1, the inputted string (representing an address) is ENCODED by
replacing all whitespace with '-' to eliminate errors when reading back from file.
if the flag (ende) is 0, th inputted address is DECODED by replacing all '-' with whitespace, 
this formats the address for printing.
OUTPUT: pointer to char, same as the char pointer input.
*/

char* endecode(char *add, int ende){
	char *addptr;
	addptr = add;
	while(*addptr != '\0'){
		if(ende == 1){
			if(*addptr == ' ')
				*addptr = '-';
			}
		else{
			if(*addptr == '-')
				*addptr = ' ';
			}
		*addptr++;
		}
	return(add);

}


/*---------WORDCAP FUNCTION------------------------------------------------------------------------
INPUT: pointer to char (string).
FUNCTION:  will convert all first letters of words found in the string (the first letter and all 
subsequent letters directly following whitespace) to uppercase, and all other letters to lowercase.
This function also removes the '\n' character saved by fgets() into some strings read from stdin.
OUTPUT: VOID
*/
void wordCap(char *string){
	char *ptr;
	ptr = string;
	while(*ptr != '\0'){
		if(isalpha(*ptr)){
			*ptr = toupper(*ptr);
			*ptr++;
			while(*ptr != ' ' && *ptr != '\0' && *ptr != '\n'){
				*ptr = tolower(*ptr);
				*ptr++;
				}
			}
		else if(*ptr == '\n')
			*ptr = '\0';
		else
			*ptr++;
		}
}

/*---------POSTCAP FUNCTION------------------------------------------------------------------------
INPUT: pointer to char (string) describing a postal code.
FUNCTION: this will convert all letters of the postal code to upper case using toupper(). 
The postal code is then checked for validity by verifying that the first and every other char is a
letter, and that the second and every other char is a number(using isalpha() and isdigit()).
OUTPUT: returns 0 if valid postal code, 1 otherwise.
*/

int postCap(char *post){
	char *ptr;
	int i;
	ptr = post;
	while(*ptr != '\0'){
		if(isalpha(*ptr)){
			*ptr = toupper(*ptr);
			*ptr++;
			}
		else if(*ptr == '\n')
			*ptr = '\0';
		else
			*ptr++;
		}
	ptr = post;
	for(i=0;i<6;i++){
		if((i%2 == 0) && isdigit(*ptr)) //--------- values corresponding to i=0,2,4 should NOT be digits
			return 1;
		//--------- values corresponding to i=1,3,5 should NOT be letters
		else if((i%2 == 1) && isalpha(*ptr))
			return 1;
		else
			*ptr++;
		}
	return 0;
}

/*---------ERASE LIST FUNCTION---------------------------------------------------------------------
INPUT: none, uses global variables
FUNCTION: traverse list and clear all allocated memory (free each node)
OUTPUT: 1 if successful, 0 otherwise.
*/
int eraseList(){
	struct contact *next = pFirst;		//struct pointer pointing to next node
	struct contact *thisOne = pFirst;		//struct pointer pointing to node to be deleted
	while(next != NULL){
		next = thisOne->pNext;
		free(thisOne);
		thisOne = next;
		}
		pFirst = NULL;
		pLast = NULL;

	return 1;
}

