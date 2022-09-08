
#include "api.h"

#include <bits/stdc++.h>

using namespace std;

char * Interface;

int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    if (fetch_msg(true)) cout << "success\n";
    else cout << "timeout\n";

    // recebe uma mensagem
    Message * m = fetch_msg(false);
    while(!valid_msg(m)) {
        free(m);
        send_nack(m->seq);
        m = fetch_msg(false);
    }
    send_ok();
    
    // faz a operacao
    switch (m->type)
    {
    case LS:
        /* code */
        break;
    case CD:
        /* code */
        break;
    case GET:
        /* code */
        break;
    case PUT:
        /* code */
        break;
    case MKDIR:
        /* code */
        break;
    default:
        // desconhecido
        break;
    }
    
}