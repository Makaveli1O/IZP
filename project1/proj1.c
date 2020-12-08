/*
Zadanie z IZP projekt č.1: Práca s textom
FIT VUT
Samuel Líška
*/
#include <stdio.h> 		//includovanie stdio.h pre štandardné príkazy jazyka C (printf,scanf .a pod.)
#include <string.h> 		//includovanie z dôvodu práce s reťazcom
#include <stdbool.h>		//kôli využivania booleanu
#define LINE_LENGTH  1001	//definícia konštanty

void hint()			
{
	printf("Nápoveda k programu: \nSpustite program pomocou príkazu: ./proj1 príkazový_súbor vstup\n");
}

void isEmpty(char input[LINE_LENGTH])				//ak je input prázdny načítaj zo subora
{
	if(input[0] == '\0')
		fgets(input, LINE_LENGTH, stdin);
}

int removeCommandShortcut(char command[LINE_LENGTH])
{
	if(command[0]) 						//odsranenie znaku na nultom indexe
		command++;
	return 0;
}

char *removeBlankSpace(char input[LINE_LENGTH]) 		//vymazanie bieloznaku na konci riadku
{
	isEmpty(input);
	int i = 0;
	while(input[i])						//cyklus cez input string 
	{	
		if(input[i]=='\n') 				//ak nájde znak nového riadku, tak ho nahradí koncovým znakom
		{
			input[i]='\0';
			return input;
		}
	i++;
	}
return input;
}

char *addWhiteSpace(char command[LINE_LENGTH])		 	//pridá na koniec stringu znak nového riadku
{
	unsigned int i;						//i je deklarované pred cyklom kôli neskorším úpravam mimo FOR cyklu
	for(i = 0;i<=strlen(command);i++) 			//cyklus cez vložený string
	{
		if(command[i]=='\0')				//ak nájde konečný znak vyskočí z cyklu
		break;
	}	
	command[i]   = '\n';				//na pozíciu kurzoru v stringu vloží znak konca riadku
	command[i+1] = '\0';					//na pozíciu za kurzor vloží koniec stringu
	return command;
}

int insertContent(char command[LINE_LENGTH])
{
	removeCommandShortcut(command);				//command++ odstráni command[0] (posunie)
	for(unsigned int i = 1; i < strlen(command); i++)
		printf("%c",command[i]);
	return 0;
}

int beforeContent(char input[LINE_LENGTH],char command[LINE_LENGTH])
{
	isEmpty(input); 					//ak je prázdne input(1 miesto v commands file)
	removeBlankSpace(command);
	command++;						//vymazanie prveho znake(meno prikazu)
	int inputLen =  strlen(input), commandLen = strlen(command), i = 0,j = 0;
	int overall  = inputLen + commandLen;
	char inputCPY[LINE_LENGTH];				//dočasný string kôli zachovanie pôvodnej hodnoty ktorú neskôr potrebujem
       	strcpy(inputCPY, input);	
	for(i = 0; i<commandLen; i++)
		input[i] = command[i];
	
	int pos = i;
	for(i = pos; i<overall; i++)
		input[i] = inputCPY[j++];

	input[i+1] = '\0';
	return 0;
}


int appendContent(char input[LINE_LENGTH], char command[LINE_LENGTH])
{
	isEmpty(input);						//ak je prázdny
	removeBlankSpace(input);				//odstráni koniec riadku
	removeCommandShortcut(command);				//odstráni začiatok príkazu (skratku)
	unsigned int inputLen =  strlen(input), commandLen = strlen(command);
	int overallLen = inputLen + commandLen, i, j =0;
	for(i = inputLen; i < overallLen; i++)
		input[i] = command[++j];

	input[i+1] ='\0';					//ukončenie stringu na pozícií i+1
	return 0;
}

int nNumber(char command[LINE_LENGTH])				//nN príkaz na vypísanie riadkov konvertuje cislo na int z ASCII
{
	int number = 0;
	for(unsigned int i = 0; i<strlen(command); i++)		//cyklus do dĺžky commandu
	{
		if(command[i] >= '0' && command[i] <= '9')	//ak je char v rozmedzí 0 - 9
		{
			number = command[i];
			number = number - 48;			//ASCII hodnota - 48 aby som dostal reálu integer hodnotu
			if(i>1&&i<3)				//ak je viac ako 9 a menej ako 100 
			{
				number = ((command[i-1]-48)*10)+number;	//1miesto *10 + druhe miesto 
			}
		}
	}
	return number;
}

int linePrint(char input[LINE_LENGTH], char command[LINE_LENGTH], char previousCMD)
{
	int count = 1, numRows = nNumber(command);		//počítadlo, numRows - číslo za príkazom
	if(input[0] == '\0')					//ak je input prázdny
		fgets(input, LINE_LENGTH, stdin);
	if(numRows<=1)						//pre n
	{
		
		if(previousCMD == 'g')				//previousCMD = predošlý vykonaný príkaz
		{
			previousCMD = '\0';
			fgets(input, LINE_LENGTH, stdin);
		}else if(previousCMD == 'n'){
			previousCMD = '\0';
			fgets(input, LINE_LENGTH, stdin);
			printf("%s",input);
		}else{
			printf("%s", input);
			fgets(input, LINE_LENGTH, stdin);
		}	
	}else if(numRows>1)					//pre nN
       	{
    		if(numRows==count)
		{
       			numRows = 0;
       			return 0;	
		}else{
			printf("%s", input);
       			for(int i = 1; i<numRows; i++)		//výpis počtu riadkov nN
       			{
				fgets(input, LINE_LENGTH, stdin);
       				printf("%s",input);
       				count++;
       			}
		}
	}	
	return 0;
}

int patternReplacement(char input[LINE_LENGTH], char command[LINE_LENGTH])
{	
	isEmpty(input);
	char oddelovac, c, pattern[LINE_LENGTH], replacement[LINE_LENGTH];
	oddelovac = command[1];
	int position   = 2;					// pozicia od 2jky pretože index 1 - meno commandu, index 2 - oddelovací znak
	unsigned int i = 2;
	for(i = 2; i < strlen(command); i++)			//for loop pre ulozenie PATTERN medzi oddelovacmi  do stringu
	{
		c = command[i];
		if(c == oddelovac)
			break;
		pattern[i-2] = c;
		position++;
	}
	pattern[i-2] = '\0';					//na konci patternu treba vložiť znak konca reťazca
	position     = position +1;				//pričítanie 1tky kôli preskočeniu deliaceho znaku medzi patternom a replacement
	for(i=position;i < strlen(command); i++)
	{
		c = command[i];
		if(c=='\0')
			break;
		replacement[i-position] = c;
	}
	replacement[i]   = '\n';				
	replacement[i+1] = '\0';	
	//skopirovanie konca riadku za REPLACEMENTOM
	char *temporaryReplacement = strstr(input, pattern);
	char afterReplacement[LINE_LENGTH];
	int j = 0;
	for(unsigned int i = strlen(pattern); i<strlen(input); i++)
		afterReplacement[++j] = temporaryReplacement[i];
	//koniec kopirovania konca riadku
	char *word  = strstr(input,pattern);
	if(word==NULL)
		printf("Prazdny");
	else
	{
		unsigned int pos = word - input;		//odčitanie pointerov pre ziskanie pozície "pattern" reťazca v inpute
		int j = 0, currentPos = 0;			//j = druhé počítadlo,k = tretie počítadlo  dĺžka replacement reťazca
		
		for(unsigned int i = pos; i<strlen(input); i++)	//tento cyklus replacne iba do dĺžky pôvodného slova, vloží REPLACEMENT do stringu	
		{
			input[i]   = replacement[j];			 
			j++;
			currentPos = i;
		}
		for(unsigned int i = j; i<strlen(replacement);i++)	//tento cyklus doplní 
			input[++currentPos] = replacement[i];
		removeBlankSpace(input);					//vymazanie konca riadku
		char inputTemp[LINE_LENGTH];					//dočasný string
		strcpy(inputTemp, input);					//skopírovanie input do dočasného stringu
		unsigned int inputLen = strlen(input),afterLen = strlen(afterReplacement);	
		int overallLen = inputLen + afterLen;
		j = 0;
		for(int i = inputLen; i < overallLen; i++)
			input[i] = afterReplacement[j++];
	}
return 0;
}

void goToCommand(char command[LINE_LENGTH], FILE *commands_file)
{
	int gN = nNumber(command);  				//GN cislo za gckom, commandsNum pocet vestkých commandov
	fseek(commands_file, 0, SEEK_SET);
	char mesh[LINE_LENGTH];
	for(int i = 1; i < gN ; i++)
		fgets(mesh, LINE_LENGTH, commands_file);
}

int dCommand(char command[LINE_LENGTH], char input[LINE_LENGTH])//dN vymazanie riadkov
{
	input[0] = '\0';					//vyprazdnenie stringu
	int dNum = nNumber(command);				//číslo za d-čkom
	for(int i = 1; i < dNum; i++)
	{
		fgets(input, LINE_LENGTH, stdin);
		input[0] = '\0';		
	}
	return 1;
}

void printOthers(FILE *commands_file, char input[LINE_LENGTH]) 	//vypísanie ostatných nespracovaných riadkov
{
	if(feof(commands_file))
	{
		while(!feof(stdin))		
		{
			printf("%s", input);
			fgets(input, LINE_LENGTH, stdin);
		}
	}
}

int commandsExecution(char* argv[])
{
	char input[LINE_LENGTH], command[LINE_LENGTH];		//buffer
	FILE *commands_file;					//súbor s príkazmi
	commands_file = fopen(argv[1],"r");
	if(commands_file==NULL) 				//ak jeden zo súborov nepodarí načítať
	{							//zastav program
		fprintf(stderr,"Chyba pri načítaní súboru.");
		return 1;
	}else{
		char previousCMD = '\0';			//predošlý príkaz
		bool eC = false, print = false, append = false, before = false, insert = false, replacement = false, gTo = false;
		while(fgets(command, LINE_LENGTH,commands_file))//cyklus ktorý prechádza príkazmi
		{
			//kotroly predošlých príkazov
			if(before)
			{
				previousCMD = 'b';
				before      = false;
			}
			else if(append)
			{
				previousCMD = 'a';
				append      = false;	
			}
			else if(insert)
			{
				previousCMD = 'i';
				insert      = false;
			}
			else if(replacement)
			{
				previousCMD = 's';
				replacement = false;
			}
			else if(print)
			{
				previousCMD = 'n';
				print       = false;	
			}
			else if (gTo)
			{
				previousCMD = 'g';
				gTo         = false;
			}
			else if (eC)
			{
				previousCMD = 'e';
				eC          = false;
			}
			
			char c = command[0]; //prvé pismeno riadku je vždy názov príkazu 
			switch(c)
			{
				case 'a':
					appendContent(input,command);
					append = true;
					continue;
				break;
				case 'n':
					 linePrint(input, command, previousCMD);
					 print = true;
				break;	
				case 'i':
					insertContent(command);
					insert = true;
					continue;
				break;
				case 'b':
					beforeContent(input,command);
					before = true;
					continue;
				break;
				case 'd':
					dCommand(command, input);
				break;
				case 'r': 
					removeBlankSpace(input);
				break;
				case 's':
					patternReplacement(input, command);
					replacement = true;
					continue;
				break;
				case 'q':
					fclose(commands_file);
					return 0;	
				break;
				case 'g':
					goToCommand(command, commands_file);
					gTo = true;
					continue;
				break;
				case 'e':
					addWhiteSpace(input);
					eC = true;
					continue;
				break;
			}
			continue;
		}
		printOthers(commands_file, input);	//vypísanie nespracovaných príkazov

	}
	fclose(commands_file);
return 0;
}

int main(int argc, char* argv[])
{
	if(argc>1&&argc<3)					//ak je argumentov menej ako 1 alebo viac ako 3 napoveda
		commandsExecution(argv); 			//vykonávanie príkazov
	else
		hint();
	return 0;
}
