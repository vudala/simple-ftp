#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED

#include <bits/stdc++.h>

#define MARKER 0b01111110

enum Commands {
    LS = 0b000111,
    CD = 0b000110,
    GET = 0b001001,
    PUT = 0b001010,
    MKDIR = 0b001000,
    CLOSE = 0b100111,
    PWD = 0b111001
};


enum Control {
    OK = 0b000001,
    ERROR = 0b010001,
    ACK = 0b000011,
    NACK = 0b000010,
    DADOS = 0b100000,
    FIM = 0b101110,
    DESCRITOR = 0b011000,
};


typedef struct __attribute__ ((__packed__)) Message {
    unsigned        mark:8;        // marcador de inicio
    unsigned        size:6;        // tamanho: 6 bits
    unsigned        seq:4;         // sequencia: 4 bits
    unsigned        type:6;        // tipo: 6 bits
    unsigned char   data[63];      // dados: size bytes
    unsigned char   crc:8;         // crc: 8 bits
    Message();
    Message(unsigned size, unsigned seq, unsigned type, char * data);
    Message(struct Mask * mask);
} Message;


typedef struct __attribute__ ((__packed__)) Mask {
    unsigned            mark:8;        // marcador de inicio
    unsigned            size:6;        // tamanho: 6 bits
    unsigned            seq:4;         // sequencia: 4 bits
    unsigned            type:6;        // tipo: 6 bits
    unsigned long long  data[63];      // mascara de 8 * size bytes
    unsigned char       crc:8;         // crc: 8 bits
    Mask();
    Mask(struct Message * msg);
} Mask;


Message * new_msg(unsigned size, unsigned seq, unsigned type, char * data);

bool valid_msg(Message * m);

std::string data_to_str(Message * m);

Message * build_descriptor(unsigned long long filesize);

#endif