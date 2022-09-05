
#include "api.h"

#include <bits/stdc++.h>

using namespace std;

char * Interface;

int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    // recebe uma mensagem
    Message * m = fetch_msg(false);

    while(!valid_msg(m)) {
        free(m);
        send_nack(0);
        m = fetch_msg(false);
    }
    send_ack(0);
}