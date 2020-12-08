/**
 * Kostra programu pro 3. projekt IZP 2018/19
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <ctype.h> //isdigit & isalpha
#include <string.h> //strcmp

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;			//pocet objektov v zhluku /current pocet
    int capacity;		//pocet objektov pre ktorĂŠ je rezervovanĂŠ miestov poli /max pocet
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
	assert(c != NULL);
	assert(cap >= 0);

	/*Moja implementácia*/

	c->size = 0;			/*pri inicializácií­ nieje v clusteri žiaden objekt*/
	c->capacity = cap;
	
	if(cap == 0)
		c->obj = NULL;

	c->obj = malloc(sizeof(struct obj_t) * cap); 
	if(c->obj == NULL)
		c->capacity = 0;
}
/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
	assert(c != NULL);
	assert(c);
	/*Uvoľnenie pamäte*/
	free(c->obj);		
	c->obj = NULL;
	c->size = 0;
	c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	/*Moja Implementácia
	 * ak je momentálny počet objektov menší­ ako kapacita, prirad
	 * inak alokuj novú veľkosť
	 */
   	 if(c->size >= c->capacity)
	 {
        	resize_cluster(c,c->capacity+CLUSTER_CHUNK);	/*CLUSTER_CHUNK je odporučená veľkosť pre append*/
       		c->obj[c->size] = obj;
        	c->size++;
    	}
   	else{
        	c->obj[c->size] = obj;
        	c->size++;
    	}
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    	//moj kod
	for(int i = 0; i < c2->size; i++)
		append_cluster(c1, c2->obj[i]);
	
	sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

	/*moja implementácia*/
    	clear_cluster(&carr[idx]);
	/*Posúvanie objektov v poli po vymazaní*/	
	for (int i = idx; i < narr - 1; i++) {
        	carr[i] = carr[i + 1];
    	}

	/*počet objektov*/
	narr = narr - 1;
	return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float x,y,result;
    x = y = result = 0;

    /*Hodnoty jednotlivých súradníc*/
    x = fabs(o1->x - o2->x);
    y = fabs(o1->y - o2->y);
    
    /*výpočet podľa vzorca pre euklidovsku vzdialenosť dvoch bodov*/
    result = (x*x)+(y*y);
    result = sqrt(result);
    
    return result;
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    /*prvá vzdialenosť*/
    float distance;
    distance = obj_distance(&c1->obj[0], &c2->obj[0]);
    
    /*vzdialenosť každého s každým*/
    for(int x = 0; x < c1->size; x++)
    {
        for(int y = 0; y < c2->size; y++)
        {
            float new_distance;
            new_distance = obj_distance(&c1->obj[x], &c2->obj[y]);
            if(distance > new_distance)
                distance = new_distance;
        }
    }

    return distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    
    /*prvá vzdialenosť*/
    float distance = cluster_distance(&carr[0], &carr[1]);
    
    int id1 = 0;
    int id2 = 0;

    for(int i = 0; i < narr; i++)
    {
    	/*j = i+1 aby boli rozdielne*/
        for(int j = i+1; j < narr; j++)
        {
        	/*porovnávanie jednotlivých výpočtov medzi sebou*/
            float new_distance = cluster_distance(&carr[i], &carr[j]);
            if(new_distance <= distance)
            {
                distance = new_distance;
          		id1 = i;
          		id2 = j;  	
            }
        }
    }
    *c1 = id1;
    *c2 = id2;
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
	assert(arr != NULL);
	
	//moj kod 
	FILE *file;
	if((file = fopen(filename, "r")) == NULL)
		return -1;
	int ch, pos, num, row = 1, count, cnt_r1;;
	ch = pos = num = count = cnt_r1 = 0;
	char checker[6];
	/*kontrola súborového vstupu*/
	while((ch = fgetc(file)) != '\n')		
	{
		/*kontrola prvého riadku*/
		if(row == 1 && pos <= 5 )
		{
			checker[pos] = ch;
			if(pos == 5)
			{
				checker[pos + 1] = '\0';
				if(strcmp("count=", checker)!=0)
					return -5;
			}
		}
		/*čítanie súboru*/
		if(ch == '\n')
			row++;
		
		if(pos >= 6 && row == 1)
		{
			num = ch - 48;			/*ASCII -> integer*/
				/*Kontrola zadaných súradníc*/
				if(isalpha((char) num))
					return -2;
				else{	/*Výsledok*/
					count = count * 10;		/*Posúvanie desatinného miesta každým cyklom*/
					count = count + num;
				}
		}
		pos++;
	}
	/*pole pre objekty, dynamicky alokované veľkosť clusteru * počet objektov*/
    	*arr = malloc(sizeof(struct cluster_t) * count);
	if(*arr == NULL)
	{
		*arr = NULL;
		return count;
	}

	
	/*Premenná pre priradenie hodnôt k jednotlivým objektom*/
	int id, objects;
	id = objects = 0;
	
	float x, y;
	x = y = 0;

	/*Načítanie súbora /OBJ ID/ X / Y
	 *Počet riadkov -1 = počet objektov
	**/
	int  i = 0;
	row = 0;
	for(i = 0; i < count; i++)
	{
		/*fscanf vracia počet úspešne zapísaných hodnôt*/
		if(fscanf(file, "%d%f%f", &id, &x, &y) != 3)
			return -4;
	
		row++;
		/*Kontrola súradníc­c*/
		if(y < 0 || y > 1000 || x < 0 || x > 1000)
		{
			*arr = NULL;
			return -3;
		}

		/*Vytvorenie dočasného objektu, a priradenie hodnôt zobratých zo súboru
		 * inicializácia clustera z momentálnych hodnôt a následné rozšírenie
		 * */	
		struct obj_t tmp_obj;
       	   	tmp_obj.id = id;
        	tmp_obj.x = x;
        	tmp_obj.y = y;
        	init_cluster(*arr + i, 1);
        	append_cluster(*arr + i, tmp_obj);

	}
	
	fclose(file);
	return i;
		
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

/* ****************************************************
 * Vlastné funkcie *
 * process_args -> funckia na kontrolu argumentov
 *
 *
 * ***************************************************/

int main(int argc, char *argv[])
{
	int n = 0;
	struct cluster_t *clusters;
	/*Ošetrenie vstupov*/
	
	if(argc > 3)
	{
		fprintf(stderr, "ERROR: Vložené argumenty niesu v požadovanom formáte.");
		return -1;
	}else if(argc == 2){
		n = 1;
	}else if (argc == 3){
		char *endptr = NULL;
		n = strtol(argv[2], &endptr, 10);
		if(endptr[0] != '\0')
		{
			fprintf(stderr, "Vstup musí­ byť číslo!");
			return -1;
		}
	
	}else{
		fprintf(stderr, "ERROR: Neplatne zadané argumenty.");
		return 1;
	}

	int count = load_clusters(argv[1], &clusters);
	/*ošetrenie neočakávaných stavov*/
	if(count == -1)
	{
		fprintf(stderr, "ERROR: Chyba otvorenia súboru.");
			return 1;
		}else if(count == -2){
			fprintf(stderr, "ERROR: Chyba, zle zadané súradnice.");
			return 1;
		}else if(count == -3){
			fprintf(stderr,"ERROR: Zadané súradnice niesu v rozmedzi medzi 0 a 1000!");
			return 1;
		}else if(count == -4){
			fprintf(stderr, "ERROR: Chýbajú súradnice, alebo sú zle zadané.");
			return 1;
		}else if(count == -5){
			fprintf(stderr, "ERROR: Chyba v prvom riadku súboru. Formát: count='N'");
		}else{
			/*Cyklus pre zradovanie zhlukov podľa zadaných parametrov*/
			int c1 = 0, c2 = 0;	
			while(n < count)
			{
				find_neighbours(clusters, count, &c1, &c2);
         			merge_clusters(&clusters[c1], &clusters[c2]);
          			count = remove_cluster(clusters, count, c2);
			}
			print_clusters(clusters, count);
			
			/*Uvoľnenie použitej alokovanej pamäte pre každý cluster*/
			for(int i = 0; i < count; i++)	
				clear_cluster(&clusters[i]);
	
			free(clusters);
		}
	return 0;
}
