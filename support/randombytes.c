#include "randombytes.h"
#include <stdlib.h>  // arc4random_buf

void randombytes(uint8_t *out, size_t outlen) {
    arc4random_buf(out, outlen);
}
