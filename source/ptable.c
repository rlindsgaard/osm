#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

/*
Tom datastruktur som I skal definere.
*/
typedef struct {
  void * inner[1024];
} ptabel_t;


/*
Sætter 'page' ind i sidetabellen 'pt'. Den logiske adresse
afgør hvor i sidetabellen 'page' skal placeres. Hvis
'page==NULL' slettes siden fra sidetabellen.
*/
void ptable_set(ptabel_t *pt, void *laddr, void *page) {
    /*
    Eksempel på udtagelse af p1 fra den logiske adresse.
    'uintptr_t' er en heltalstype som har ligeså mange
    bits som en peger (adresse).
    */
    uintptr_t la = (uintptr_t)laddr;
    uintptr_t p1;
    uintptr_t p2;
    ptabel_t * outer;
    
    p1 = (la >> 12) & 0x3ff;
    p2 = (la >> 22) & 0x3ff;
    
    if(page == NULL) 
    {
      outer = pt->inner[p2];
      outer->inner[p1] = NULL;
      free(outer->inner[p1]);
    } else {
      if(pt->inner[p2] == NULL)
      {
        pt->inner[p2] = calloc(1,sizeof(ptabel_t));
      }     
      
      outer = pt->inner[p2];
      outer->inner[p1] = page;
    }
}

/*
Returnerer pegeren ('page') som er blevet indsat med 'ptable_set'
for den pågældende logiske adresse 'laddr'. Hvis den logiske
adresse ikke eksisterer, returneres 'NULL'.
*/
void *ptable_get(ptabel_t *pt, void *laddr) {
    uintptr_t la = (uintptr_t)laddr;
    uintptr_t p1;
    uintptr_t p2;
    ptabel_t * outer;
    
    p1 = (la >> 12) & 0x3ff;
    p2 = (la >> 22) & 0x3ff;
    outer = pt->inner[p2];

    if(outer != NULL)
    {
      return outer->inner[p1];
    }    
    return NULL;
}


int main(int argc, char *argv[]) {
    /* Test program */
    ptabel_t *pt = calloc(1,sizeof(ptabel_t));
    void *page = calloc(1,4096);
    void *page2 = calloc(1,4096);
    int * someval = page+5;
    *someval = 123535;
    int * someotherval = page2+5;
    *someotherval = 7547547;
    void * laddr = (void *)0x2355ff;
    void * laddr2 = (void *) 0x123456;
    
    printf("Data initialised at %p\n",page);
    printf("Getting uninitalised data %p\n",ptable_get(pt,laddr));
    printf("Setting %p to %p\n",laddr,page);
    ptable_set(pt,laddr,page);
    printf("Setting %p to %p\n",laddr2,page2);
    ptable_set(pt,laddr2,page2);
    someval = (int*) (ptable_get(pt,laddr)+5);
    someotherval = (int*) (ptable_get(pt,laddr)+5);
    printf("Values are readable %d %d\n",*someval,*someotherval);
    printf("Now it exists at %p %p\n",ptable_get(pt,laddr),ptable_get(pt,laddr2));
    ptable_set(pt,laddr,NULL);
    printf("Now it doesn't %p but the other does %p\n",ptable_get(pt,laddr),ptable_get(pt,laddr2));
    ptable_set(pt,laddr2,NULL);
    free(page);
    free(page2);
    free(pt);
    return 0;
}
