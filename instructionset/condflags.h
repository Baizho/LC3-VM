#ifndef CONDFLAGS_H
#define CONDFLAGS_H

typedef enum condflags {
    FL_POS = 1 << 0, /* P */
    FL_ZRO = 1 << 1, /* Z */
    FL_NEG = 1 << 2, /* N */
}

#endif //CONDFLAGS_H