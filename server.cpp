
#include "api.h"

#include <bits/stdc++.h>

using namespace std;

char * Target_Addr;

int main(int argc, char * argv[]) {
    Target_Addr = argv[1];

    init_con();

    Message * m = fetch_msg();
    while (!valid_msg(m)) {
        send_nack(m->seq);
        m = fetch_msg();
    }
    
    cout << m->data << '\n';
}