#include "commands.h"

#include "message.h"
#include "api.h"

#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

void send_stream(FILE * stream)
{
    char buffer[64];
    Message * msg = NULL;
    unsigned seq = 0;
    int type = DADOS;
    int bytes_read = fread(buffer, sizeof(char), 64, stream);
    do {
        if (feof(stream))
            type = FIM;

        assert_send(new Message(bytes_read, seq, type, buffer));
        free(msg);
        
        bytes_read = fread(buffer, sizeof(char), 64, stream);
        seq = (seq + 1) % 16;
    } while(bytes_read > 0);
}


string * recv_stream()
{
    Message * msg = NULL;
    unsigned seq = 0;
    int type = DADOS;

    string * result = new string;
    do {
        msg = assert_recv(seq);
    	for(int i = 0; i < msg->size; i++)
            (*result).push_back(msg->data[i]);
        free(msg);
        seq = (seq + 1) % 16;
    } while(msg->type != FIM);

    return result;
}

void execute_ls()
{
    FILE * f = popen("ls", "r");
    send_stream(f);
    fclose(f);
}