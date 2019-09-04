#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
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

char* getToken(string input,int i,int* ptr)
{
    int count=0;
    int k=0;
    int j=i;
    while(input[i]!=' ' && input[i]!= '\0')
    {
        count++;
        i++;
    }
    char* a = new char[count];
    while(count-- && input[j]!='\0')    //creating token
    {
        a[k] = input[j];
        k++;
        j++;
    }
    while(input[i]==' ' && input[i]!='\0')
    {
        i++;
    }
    *ptr = i;
    return a;
}

char* getCommand(string input, int i,int* ptr)
{
    int count=0;
    while(input[i]!='\0' && input[i]!=' ')
    {
        count++;
        i++;
    }
    i=0;
    char *a = new char[count+5];
    int k=5;
    a[0] = '/';
    a[1] = 'b';
    a[2] = 'i';
    a[3] = 'n';
    a[4] = '/';
    while(input[i]!='\0' && input[i]!=' ')
    {
        a[k] = input[i];
        i++;
        k++;
    }
    while((input[i]==' ') && input[i]!='\0')
    {
        i++;
    }
    *ptr = i;
    return a;
}

char* getStringToken(string input,int i,int* ptr)
{
    i++;
    int j=i;
    int count=0;
    while(input[i]!='"')    //scan for the next " and blindly create a token
    {
        count++;
        i++;
    }
    char *a = new char[count];
    int k=0;
    while(j!=i)
    {
        a[k]=input[j];
        k++;
        j++;
    }
    i++;
    while((input[i]==' ') && input[i]!='\0')    //ignore white spaces
    {
        i++;
    }
    *ptr = i;
    return a;
}

char** parseString(string input)
{
    int i=0,j,count,k,argcount=0;
    char **args = new char*[ARGLIMIT]();
    char* command = getCommand(input,i,&i);
    char* token;
    args[argcount] = command;
    argcount++;
    j=i;
    while(input[i]!='\0')           //main loop to scan the string
    {
        count=0;
        if(input[i]=='"')
        {
            token = getStringToken(input,i,&i);
        }
        else{
            token = getToken(input,i,&i);
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
                execv(args[0],args);
            }
            if(childid > 0)
            {
                //I'm in parent process
                // cout<<"In parent"<<endl;
                int status;
                wait(&status);
            }
            return 1;
        }

        case 1 :    //cd
        {
            if(!strcmp(args[1],"~"))
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
    /*pid_t childid = fork();
    if(childid < 0)
    {
        //error occured. Print msg and exit
        cout<<"Error forking"<<endl;
        return -1;
    }
    if(childid == 0)
    {
        //I'm in child process
        char **args;
        string command;
        do
        {
            cout<<"Enter command"<<endl;
            getline(cin,command);
            args = parseString(command);
            execv(args[0],args);
        }while(1);
    }
    if(childid > 0)
    {
        //I'm in parent process
        cout<<"In parent"<<endl;
        int status;
        wait(&status);
        cout<<status;
    }*/
    int status;
    string command;
    char** args;
    string ps1string = "> ";
    do{
        cout<<ps1string;
        getline(cin,command);
        args=parseString(command);
        //args = parseInput();
        status=executecommand(args);
    }while(status);
}

int main()
{
    loop();
    /*string command;
    getline(cin,command);
    char** args;
    args = parseString(command);
    cout<<args[0]<<" "<<args[1];*/
    return 0;
}
