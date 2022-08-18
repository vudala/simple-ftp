#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED

#define MARKER 0b01111110

#define byte unsigned char

typedef struct __attribute__ ((__packed__)) message_t {
    byte mark;      // marcador de inicio
    byte size:6;    // tamanho 6 bits
    byte seq:4;     // sequencia 4 bits
    byte type:6;    // tipo 6 bits
    char *data;     // dados: size bytes
    byte count;     // paridade: 8 bits
} Message;


Message * new_msg(unsigned size, unsigned seq, unsigned type, const char * data);

#endif