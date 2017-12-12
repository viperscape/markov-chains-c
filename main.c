#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chain.h"

int main (int argc, char *argv[]) {
    srand ((unsigned)time(NULL));
    names_test("training_data.txt", 15);
  
    return 0;   
}