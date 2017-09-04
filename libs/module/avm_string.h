#ifndef _AVM_STRING_H_
#define _AVM_STRING_H_

struct avm_string_s {
    unsigned char * string;
    unsigned int    len;
};

#define avm_string(str)     { (unsigned char *)(str), sizeof((str) - 1) }
#define avm_null_string     { 0, NULL }

#endif