#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#define ARGLIMIT 10
using namespace std;

char** parseString(string input)
{
    int i=0,j,count,k,argcount=0;
    char **args = new char*[ARGLIMIT]();
    while(input[i]!='\0' && input[i]!=' ')
    {
        count++;
        i++;
    }
    i=0;
    char *a = new char[count+5];
    k=5;
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
    args[argcount] = a;
    argcount++;
    j=i;
    while(input[i]!='\0')
    {
        count=0;
        while(input[i]!=' ' && input[i]!='\0')
        {
            count++;
            i++;
        }
        while((input[i]==' ') && input[i]!='\0')
        {
            i++;
        }
        char *a = new char[count];
        k=0;
        while((input[j]!=' ') && input[j]!='\0')
        {
            a[k] = input[j];
            k++;
            j++;
        }
        j=i;
        args[argcount] = a;
        argcount++;
    }
    cout<<args[0]<<" .. "<<args[1]<<endl;
    args[argcount]=NULL;
    return args;
}

int main()
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
        char **args;
        string command;
        cout<<"Enter command"<<endl;
        getline(cin,command);
        args = parseString(command);
        execv(args[0],args);
    }
    if(childid > 0)
    {
        //I'm in parent process
        cout<<"In parent"<<endl;
        int status;
        wait(&status);
        if(WIFEXITED(status))
            cout<<"Child successfully terminated"<<endl;
    }
}
