/*
 *	Zadanie z IZP projekt č.1: Práca s textom
 *	FIT VUT
 *	Samuel Líška
*/

/*---------------------------------------*/

/**
 *** Includované knižnice, použité pri tvorbe programu ***
 *
 * "stdio.h"  - Klasická C knižnica so zákl. funkciami
 * "string.h" - String knižnica je využitá pre rozpoznanie argumentov programu
 * "math.h"   - Matematická knižnica. Použitá z dôvodu porovnanie výsledkov
 * "stdlib.h" - Knižnica využitá kôli funkciam: atoi(char -> integer), atof(char -> float/double)
 *
 **/

#include <stdio.h>
#include <string.h>	
#include <math.h>	
#include <stdlib.h>

/**
 *** Prototypy funkcií na výpočet logaritmov a exponenciálnych funkcií ***
 * 
 * "x" - spracovávané číslo zadávané užívateľom
 * "y" - veľkosť mocniny 
 * "n" - počet iterácií pri výpočte (čím väčšie N, tým presnejší výpočet)
 *
 **/
double taylor_log(double x, unsigned int n);

double cfrac_log(double x , unsigned int n);

double taylor_pow(double x, double y, unsigned int n);

double taylorcf_pow(double x, double y, unsigned int n);

int checkArg(double x, double y);

int specialChars(int argc, char *argv[]);

double specialLogValues(double x);

void errorMessage();

void hintMessage();

/**
 *** Main funkcia –> Rozpoznanie argumentov, kontrola a  spustenie danej funkcie na výpočet, výpis výsledkov. ***
 **/ 

int main(int argc, char *argv[])
{
	if(argc > 1 && argc < 6)				//kontrola počtu argumentov
	{
		if(strcmp(argv[1], "--log") == 0 && specialChars(argc, argv) == 0 && argc == 4)		//kontrola či boli zadané validný vstup	
		{
			double x = atof(argv[2]);		//použitie atof nebezpečnej funkcie atof až po specialChars funkcií
			int n = atoi(argv[3]);			//použitie atoi až po specialChars
			if(checkArg(x,1) == 1)			//dalšia verifikácia vstupu
			{
				errorMessage();
				return -1;
			}
			printf("	 log(%g) = %.12g\n", x, log(x));
			printf("   cfrac_log(%g) = %.12g\n", x, cfrac_log(x, n));
			printf("  taylor_log(%g) = %.12g\n", x,taylor_log(x, n));
		}else if(strcmp(argv[1], "--pow") == 0 && specialChars(argc, argv) == 0 && argc == 5)
		{
			double x = atof(argv[2]);
			double y = atof(argv[3]);
			int n = atoi(argv[4]);
			if(checkArg(x,y) == 1)
			{
				errorMessage();
				return -1;
			}
			printf(" 	 log(%g, %g) = %.12g\n", x, y, pow(x, y));
			printf("  taylor_pow(%g, %g) = %.12g\n", x, y, taylor_pow(x, y, n));
			printf("taylorcf_pow(%g, %g) = %.12g\n", x, y, taylorcf_pow(x, y, n));
		}else{
			errorMessage();
			return -1;
		}
	}else{
		hintMessage();
		return -1;
	}
	return 0;
}

/**
 ***  Výpočet logaritmu zo základu X v N iteráciach ***
 * 
 * Funkcia "taylor_log" je rozdelená do dvoch vetiev, s dvomi rozdielnimi vzorcami.
 * Prvý počíta pre "x" menšie ako 1, a druhá pre "x" váčšie ako 1
 *
 **/
double taylor_log(double x, unsigned int n) //n - rozvoj polynomu x - cislo
{
	double verify = specialLogValues(x);  			//kontrola špeciálnych hodnôt (t.j. inf, nan, 1 ,0) 
	if(verify != x || verify == INFINITY)
		return verify;
	if(1.0 > x)
	{
		double sum = 0, calc1 = 1.0, calc2 = 1.0;	//sum - súčet, calc1 - prvá časť výpočtu, calc2 - druhá časť výpočtu
		x = 1.0 - x;					//úprava x-ka kôli tomu že vzorec počíta logaritmus pre 1-x
		for (unsigned int i = 1; i <= n; i++)
	       	{
			calc1 = calc1 * x;			//výpočet mocniny vo vzorci
			calc2 = calc1/i;			//zlomok vo vzorci
			sum = sum - calc2;			//pričítanie k súčtu
		}
		return sum;
	}else if(1.0 < x){
		double sum = 0, calc1 = 1.0, calc2 = 1.0;
		for(unsigned int i = 1; i <= n; i++)
		{	
			calc1 = calc1 * ((x-1)/x);		//vrchná časť vzorca (čitateľ)
			calc2 = calc1 / i;			//zlomok
			sum = sum - calc2;
		}
		if(sum < 0)					//ked výjde záporn číslo prehodí znamienko
			return sum*(-1);
		else
			return sum;
	}
	return 0;
}

/**
 ***Funkcia cfrac_log -> vypočíta prirodzený logaritmus čísla "X" v "N" iteráciach*** 
 *
 * Funkcia počíta pomocou metódy zreťazených zlomkov. Tým že počítame rekurzívne potrebujeme najskôr
 * vypočítať posledný člen, od ktorého budeme následne odpočítavať predošlé členy
 *
 **/
double cfrac_log(double x, unsigned int n)
{
	double verify = specialLogValues(x);		//kontrola špeciálnych hodnôt (t.j. inf, nan, 1 ,0) 
	if(verify != x || verify == INFINITY)
		return verify;	
	n = n - 1;					//Počítame s n-1 iteráciami pretože poslednú iteráciu pripočítame na konci pri returne
	double a = 0.0, b = 0.0, sum  = n + (1 + n);
	x = (x-1) / (x+1);				//úprava x-ka pretože počítame podľa vzorca pre logaritmus(x-1)/(x+1)
	while(n >= 1)
	{
		b = n + (n-1);				//čísla pred zlomkom
		a = (n * n) * (x * x);			//vrchná časť zlomku
		sum = b - (a/sum);  			//vžsledok cyklu
		n = n - 1;
	}
	double result = 2 * x / sum;			//nakoniec pridám prvú iteráciu
	return result;
}

/**
 *** Funkcia taylor_pow -> vypočíta exponenciálnu funkciu so vstupnými argumanetami "x" - číslo ktoré chceme umocniť, "y" veľkosť mocniny ***
 *
 * a "n" počet iterácií výpočtu. Po úprave pôvodneho vzorca som sa dostal ku vzorcu => minule_x * (y*ln) / i
 * výsledok som potom pripočítal k súčtu a pokračoval -> logaritmus vypočítaný pomocou cfrac_log
 *
 **/
double taylor_pow(double x, double y, unsigned int n)
{
	if(x == 1)
		return 1;
	double log = taylor_log(x, n);			//využitie vypočítania logaritmu pomocou tayloroveho polynomu z predošlej funkcie
	double sum = 1.0, calc = 1.0;
	for(unsigned int i = 1; i < n ; i++)
	{
		calc = calc * (y * log) / i;		//počítanie podľa upraveného vzorca
		sum = sum + calc;			//prirátanie k súčtu každú iteráciu
	}
	return sum;
}

/**
 *** Funkcia taylorcf_pow -> vypočíta exponenciálnu funkciu so vstupnými argumanetami "x" - číslo ktoré chceme umocniť, "y" veľkosť mocniny ***
 *
 * a "n" počet iterácií výpočtu. Po úprave pôvodneho vzorca som sa dostal ku vzorcu => minule_x * (y*ln) / i
 * výsledok som potom pripočítal k súčtu a pokračoval -> logaritmus vypočítaný pomocou cfrac_log
 *
 **/

double taylorcf_pow(double x, double y, unsigned int n)
{
	if(x == 1)
		return 1;
	double log = cfrac_log(x, n);			//využitie vypočítania logaritmu pomocou zreťazených zlomkov
	double sum = 1.0, calc = 1.0;

	for(unsigned int i = 1; i < n; i++)
	{
		calc = calc * (y * log) / i;
		sum = sum + calc;
	}
	return sum;
}

/**
 *** Funkcia checkArg -> kontroluje zadané argumenty, či niesu záporné ***
 **/

int checkArg(double x, double y)
{
	if(x < 0)
		return 1;
	if(y < 0)
		return 0;
	return 0;
}

/**
 ***Funkcia specialChars -> kontroluje zadané argumenty, či sú v požadovanom formáte
 **/

int specialChars(int argc, char *argv[])
{
	char *container;
	double num = 0.0;
	for(int i = 2; i < argc; i++)
	{
		num = strtod(argv[i], &container);
		if(container[0] != '\0')
			return 1;
	}
    num++;
	return 0;
}

/**
 ***Funkcia specialLogValues -> funkcia pre spracovanie vložených špeciálnych validných hodnôt do výpočtu(inf, nan, 1 ,0)
 * pre porovnanie som používal výskedky z matematickej knižnice 
 **/

double specialLogValues(double x)
{
	if(x == 0)
		return -INFINITY; 
	else if(isnan(x))
		return NAN;
	else if(isinf(x))
		return INFINITY;
	else if(x == 1)
		return 0;
	return x;


}

/**
 *** Funkcia errorMessage -> vypisuje na štandardný chybový výstup chybové hlásenie  ***
 **/

void errorMessage()
{
	fprintf(stderr,"ERROR: boli zadané nesprávne argumenty.(Čísla, špecialne znaky, záporne čísla a pod.), alebo bol zadaný neúplny vstup.");
}

/**
 *** Funkcia hintMessage -> vypisuje na štandardný chybový výstup (pri nesprávnom spustení programu) nápovedu k programu ***
 **/

void hintMessage()
{
	fprintf(stderr,"ERROR: nesprávne spustenie programu.\nNápoveda k programu:\nProgram vie počítať logaritmus a exponenciálnu funkciu pomocou základných matematických operácií.\n\t\t1.Pre logaritmus použite príkaz ./proj2 --log X(číslo) N(iterácie)\n\t\t2.Pre exponenciálnu funkciu príkaz ./proj2 --pow X(číslo) Y(mocninu) N(iterácie))");
}
