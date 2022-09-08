#include "commands.h"

#include "message.h"
#include "api.h"

#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

void execute_ls()
{
    FILE * f = popen("ls", "r");
    send_stream(f);
    fclose(f);
}