#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An error checked malloc() wrapper function
void *ec_malloc(unsigned int size) {
  void *ptr;
  ptr = malloc(size);
  if (ptr == NULL)
    __android_log_write(ANDROID_LOG_ERROR, "ec_malloc",
                        "in ec_malloc() on memory allocation");
  return ptr;
}
