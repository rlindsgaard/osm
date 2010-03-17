#include <stdlib.h>
#include <inttypes.h>

/*
Tom datastruktur som I skal definere.
*/
typedef struct {
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

    p1 = (la >> 12) & 0x3ff;
}

/*
Returnerer pegeren ('page') som er blevet indsat med 'ptable_set'
for den pågældende logiske adresse 'laddr'. Hvis den logiske
adresse ikke eksisterer, returneres 'NULL'.
*/
void *ptable_get(ptabel_t *pt, void *laddr) {

    return NULL;
}


int main(int argc, char *argv[]) {
    /* Test program */
    return 0;
}
