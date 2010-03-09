#ifndef _ERRSTOP_H
#define _ERRSTOP_H
#include <stdio.h>
#include <stdlib.h>

#define err_stop(STR) { printf(__FILE__ " %5d:" STR "\n", __LINE__); exit(-1); }

#endif
