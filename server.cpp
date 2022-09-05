
#include "api.h"

#include <bits/stdc++.h>

using namespace std;

char * Interface;

int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    while (1) {
        Message * m = fetch_msg();
        while (!valid_msg(m)) {
            send_nack(m->seq);
            m = fetch_msg();
        }
        cout << m->data << '\n';
    }
}