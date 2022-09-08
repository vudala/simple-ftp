#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED

#define MARKER 0b01111110

#define byte unsigned char


enum Commands {
    LS = 0b000111,
    ls_mostra_na_tela = 0b111111,
    CD = 0b000110,
    GET = 0b001001,
    PUT = 0b001010,
    MKDIR = 0b001000,
};


enum Control {
    OK = 0b000001,
    ACK = 0b000011,
    NACK = 0b000010,
    ERROR = 0b010001,
    FIM = 0b101110,
    DESCRITOR = 0b011000,
    DADOS = 0b100000,
};


typedef struct __attribute__ ((__packed__)) Message {
    unsigned    mark:8;        // marcador de inicio
    unsigned    size:6;        // tamanho: 6 bits
    unsigned    seq:4;         // sequencia: 4 bits
    unsigned    type:6;        // tipo: 6 bits
    char        data[64];      // dados: size bytes
    unsigned    crc:8;         // crc: 8 bits
    Message();
    Message(unsigned size, unsigned seq, unsigned type, char * data);
} Message;

Message * new_msg(unsigned size, unsigned seq, unsigned type, char * data);

bool valid_msg(Message * m);

#endif