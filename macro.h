#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>
#include <stdlib.h>

#define TODO do{printf("%s:%d Unimplemented in function '%s'\n", __FILE__, __LINE__, __func__);fflush(stdout);exit(1);}while(0);


#endif /* MACRO_H */