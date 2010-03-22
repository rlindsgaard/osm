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
    uintptr_t offset;
    ptabel_t * outer;
    ptabel_t * inner;
    
    p1 = (la >> 12) & 0x3ff;
    p2 = (la >> 22) & 0x3ff;
    offset = la & 0x3ff;
    outer = pt->inner[p2];
    printf("p1: %d p2: %d offset %d\n",(int)p1,(int)p2,(int)offset);
    if(page == NULL) 
    {
      pt->inner[p2] = NULL;
      free(outer->inner);
    } else {
      if(outer == NULL)
      {
        pt->inner[p2] = calloc(1,sizeof(ptabel_t));
      }
      printf("lalla\n");
    outer = pt->inner[p2];
      if(outer->inner[p1] == NULL)
      {
        outer->inner[p1] = calloc(1,4096);
      }
      printf("inner address %p\n",outer->inner[p1]);
      inner = outer->inner[p1]+offset;
      inner = page;
      printf("inner is now %p\n",outer->inner[p1]+offset);
    }
}

/*
Returnerer pegeren ('page') som er blevet indsat med 'ptable_set'
for den pågældende logiske adresse 'laddr'. Hvis den logiske
adresse ikke eksisterer, returneres 'NULL'.
*/
void *ptable_get(ptabel_t *pt, void *laddr) {
    uintptr_t la = (uintptr_t)laddr;
  printf("getting %d\n",(int) la);
    uintptr_t p1;
    uintptr_t p2;
    ptabel_t * outer;
    uintptr_t offset;
    p1 = (la >> 12) & 0x3ff;
    p2 = (la >> 22) & 0x3ff;
    offset = la & 0x3ff;
    printf("p1: %d p2: %d offset %d\n",(int)p1,(int)p2,(int)offset);
    if(pt->inner[p2] != NULL)
    {
      outer = pt->inner[p2];
      return outer->inner[p1]+offset;
    }    
    return NULL;
}


int main(int argc, char *argv[]) {
    /* Test program */
    ptabel_t *pt = calloc(1,sizeof(ptabel_t));
    int page = 123535;
    void * laddr =0x2355ff; 
    void * faddr =0xdddf1252ff; 
    ptable_set(pt,laddr,(void *) page);
    page = (*(int) ptable_get(pt,laddr));

    printf("Returned %d\n", page);
/*    ptable_set(pt,laddr,NULL);
    printf("laddr is now %p\n",ptable_get(pt,laddr));
    ptable_set(pt,faddr,(void *) page);
    printf("Trying a very large address %d\n",(int) ptable_get(pt,faddr));*/
    return 0;
}
