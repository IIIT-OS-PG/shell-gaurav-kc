#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<errno.h>
#include<vector>
#include<fcntl.h>
#include<fstream>
#include<map>
#define ARGLIMIT 10
#define HISTSIZE 10
#include "prototypes.h"
#include "historyhandler.h"
#include "handlepipe.h"
#include "handlepiperedirected.h"
#include "parser.h"
#include "commandexecution.h"
#include "ioredirection.h"

map<string,string> al;

int checkinbuilt(string command)
{
    if(command == "cd")
        return 1;
    if(command == "exit")
        return 2;
    if(command == "alias")
        return 3;
    if(command == "$$")
        return 4;
    if(command == "$?")
        return 5;
    //handle custom command in executecommand and executecommandwithoutfork functions
    return 0;
}

using namespace std;

int loop()
{
    int status;
    string command;
    char** args;
	sethistory(HISTSIZE);
    load_history();
    string ps1string = "> ";
    do{
        cout<<ps1string;
        getline(cin,command);
        insertinhistory(command);
        args=parseString(command,0,command.size());
        /*int p=0;
        while(args[p]!=NULL)
        {
            cout<<args[p]<<" "<<endl;
            p++;
        }*/
        if(args!=NULL)
            status=executecommand(args);
        //cout<<getRecent();
    }while(1);
}

int main()
{
    loop();
    return 0;
}
