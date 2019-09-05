#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<errno.h>
#include<vector>
#include "parser.h"
#define ARGLIMIT 10
using namespace std;
char** parseString(string input,int lim1,int lim2);
int executecommand(char** args);
int executecommandwithoutfork(char** args);

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
void handleredirection(string input)
{
    
}

void handlepipe(string input)
{
    vector<int> indices;
    int count=0;
    int lim1,lim2;
    int j=0;
    int l;
    int i=0;
    int m=0;
    int tempcount=0;
    char** token = new char*[20];
    for(i=0;i<input.size();i++)
    {
        tempcount++;
        if(input[i]=='|')
        {
            count++;
            indices.push_back(i);
            l=0;
            char *temp = new char[tempcount-2];
            for(int k=j;k<(i-1);k++)
            {
                temp[l] = input[k];
                l++;
            }
            //cout<<temp<<endl;
            token[m]=temp;
            m++;
            tempcount=0;
            i++;
            j=i+1;
        }
    }
    indices.push_back(input.size());
    l=0;
    char *temp = new char[tempcount-2];
    for(int k=j;k<(i-1);k++)
    {
        temp[l] = input[k];
        l++;
    }
    token[m]=temp;
    //cout<<temp<<endl;
    m++;
    token[m]=NULL;
    //now count has the number of pipes present in the string.
    //we would require that many pipes
    int fdarray[count][2];
    for(int i=0;i<count;i++)
    {
        pipe(fdarray[i]);
    }
    //pipes have been initialized
    pid_t pidrecord[count+1];   //there will be pipe+1 processes
    //cout<<"count is "<<count<<endl;
    for(int i=0;i<=count;i++)
    {
        //the loop creates childs for each Command
        pid_t pid = fork();
        pidrecord[i]=pid;
        if(pid==0)
        {
            char **args;
            //we are inside the child process
            //we need to consider 3 cases here.
            //1. The child is executing the first Command
            //2. The child is executing an intermediate Command
            //3. The child is executing the last Command
            if(i==0)
            {
                //the first command
                dup2(fdarray[0][1],1);
                //cout<<token[i]<<"  llll  f  ";
                args = parseString(token[i],0,indices[i]-1);
                /*int p=0;
                while(args[p]!=NULL)
                {
                    cout<<args[p]<<" /\\";
                    p++;
                }*/
            }else{
                if(i==count)
                {
                    //the last command
                    dup2(fdarray[i-1][0],0);
                    int index = indices[i]-indices[i-1]-2;
                    //cout<<token[i]<<"  llll  l  ";
                    args = parseString(token[i],0,index);
                    /*int p=0;
                    while(args[p]!=NULL)
                    {
                        cout<<args[p]<<" /\\";
                        p++;
                    }*/
                }else{
                    dup2(fdarray[i-1][0],0);
                    dup2(fdarray[i][1],1);
                    int index = indices[i]-indices[i-1]-2;
                    //cout<<token[i]<<"  llll  i  ";
                    args = parseString(token[i],0,index);
                    /*int p=0;
                    while(args[p]!=NULL)
                    {
                        cout<<args[p]<<" /\\";
                        p++;
                    }*/
                    //an intermediate Command
                }
            }
            //exit(0);
            for(int j=0;j<count;j++)
            {
                close(fdarray[j][0]);
                close(fdarray[j][1]);
            }
            //execv(args[0],args);
            executecommandwithoutfork(args);
        }       //the child process ends here
        //cout<<"\n"<<token[i]<<"  "<<indices[i]<<endl;
    }
    //we are out of loop now. Lets make sure all processes are executed
    for(int j=0;j<count;j++)
    {
        close(fdarray[j][0]);
        close(fdarray[j][1]);
    }
    while(wait(NULL)>0);
}

char** parseString(string input,int lim1,int lim2)
{
    int i=lim1,argcount=0;
    char **args = new char*[ARGLIMIT]();
    char* token;
    for(int i=0;i<input.size();i++)
    {
        if(input[i]=='|')
        {
            handlepipe(input);
            return NULL;
        }
        if(input[i]=='<')
        {
            handleredirection(input);
            return NULL;
        }
    }
    char* command = getCommand(input,&i,lim2);
    args[argcount] = command;
    argcount++;
    while((i<lim2) && (input[i]!='\0'))
    {
        switch(input[i])
        {
            case '"' :
            {
                token = getStringToken(input,&i,lim2);
            }
            break;
            default :
            {
                token = getToken(input,&i,lim2);
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
    /*
    cout<<"Args .. ";
    int i=0;
    while(args[i]!=NULL)
    {
        cout<<".. "<<args[i]<<endl;
        i++;
    }*/
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
                return 1;
            }
            exit(0);
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

int executecommandwithoutfork(char** args)
{
    int isinbuilt = checkinbuilt(args[0]+5);
    /*
    cout<<"Args .. ";
    int i=0;
    while(args[i]!=NULL)
    {
        cout<<".. "<<args[i]<<endl;
        i++;
    }*/
    switch(isinbuilt)
    {
        case 0:     //linux command
        {
            int status = execv(args[0],args);
            if(status)
            {
                cout<<strerror(errno)<<endl;
                exit(-1);
            }
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
        args=parseString(command,0,command.size());
        if(args!=NULL)
            status=executecommand(args);
    }while(1);
}

int main()
{
    loop();
    return 0;
}
