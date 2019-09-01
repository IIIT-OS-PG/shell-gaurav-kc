#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;

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
        char a[] = "-l";
        char e[] = "/bin/ls";
        char *args[] = {e,a,NULL};
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
