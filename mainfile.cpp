#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<errno.h>
#include "parser.h"
#define ARGLIMIT 10
using namespace std;

int checkinbuilt(string command)
{
    if(command == "cd")
        return 1;
    if(command == "exit")
        return 2;
    return 0;
}
int checkToken(string command)
{
    if(command == "|")
        return 1;
    if(command == ">")
        return 2;
    if(command == ">>")
        return 3;
    return 0;
}

char** parseString(string input)
{
    int i=0,argcount=0;
    char **args = new char*[ARGLIMIT]();
    char* token;
    char* command = getCommand(input,&i);
    args[argcount] = command;
    argcount++;
    while(input[i]!='\0')
    {
        switch(input[i])
        {
            case '"' :
            {
                token = getStringToken(input,&i);
            }
            break;
            case '|' :
            {
                //handle pipe
                cout<<"pipe";
                exit(0);
            }
            break;
            case '<' :
            {
                if(input[i+1] == '<')
                {
                    //handle <<
                    cout<<"<<";
                }else{
                    //handle <
                    cout<<"<";
                }
                exit(0);
            }
            break;
            default :
            {
                token = getToken(input,&i);
            }
            break;
        }

        args[argcount] = token;
        argcount++;
    }
    args[argcount]=NULL;
    return args;
}

int executecommand(char** args)
{
    int isinbuilt = checkinbuilt(args[0]+5);
    switch(isinbuilt)
    {
        case 0:     //linux command
        {
            pid_t childid = fork();
            if(childid < 0)
            {
                //error occured. Print msg and exit
                cout<<"Error forking"<<endl;
                return -1;
            }
            if(childid == 0)
            {
                //I'm in child process
                int status = execv(args[0],args);
                if(status)
                {
                    cout<<strerror(errno)<<endl;
                    exit(-1);
                }
            }
            if(childid > 0)
            {
                //I'm in parent process
                // cout<<"In parent"<<endl;
                int status=0;
                wait(&status);
            }
            return 1;
        }

        case 1 :    //cd
        {
            if((args[1]!=NULL) && (!strcmp(args[1],"~")))
            {
                args[1] = getenv("HOME");
            }
            chdir(args[1]);
            return 1;
        }
        case 2 :    //exit
        {
            cout<<"Bye"<<endl;
            exit(0);
        }
    }
}

int loop()
{
    int status;
    string command;
    char** args;
    string ps1string = "> ";
    do{
        cout<<ps1string;
        getline(cin,command);
        args=parseString(command);
        status=executecommand(args);
    }while(status);
}

int main()
{
    loop();
    return 0;
}
