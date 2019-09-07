using namespace std;


int executecommand(char** args)
{
    int isinbuilt = checkinbuilt(args[0]+5);
    /* use this code to see what arguments are received
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
                ofstream ofs;
                ofs.open("status.txt");
                ofs<<0;
                ofs.close();
                int status = execv(args[0],args);
                if(status)
                {
                    cout<<strerror(errno)<<errno<<endl;
                    ofstream ofs;
                    ofs.open("status.txt");
                    ofs<<errno;
                    ofs.close();
                    exit(-1);
                }
            }
            if(childid > 0)
            {
                //I'm in parent process
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
        	savetohistory();
            cout<<"Bye"<<endl;
            exit(0);
        }
        break;
        case 3 :    //alias
        {
            int h=2;
            string p;
            p=args[1];
            while(args[h]!=NULL)
            {
                p=p+" "+args[h];
                h++;
            }
            h=0;
            while(p[h]!=' ' && p[h]!='=')
                h++;
            int i=0;
            char* aliasname = getToken(p,&i,h);
            while(p[h]==' ' || p[h]=='=')
                h++;
            i=h;
            int count=0;
            while(p[h]!='\0')
            {
                count++;
                h++;
            }
            char* aliascomm = new char[count+1];
            int j=0;
            while(count--)
            {
                aliascomm[j]=p[i];
                j++;
                i++;
            }
            aliascomm[j]='\0';
            //cout<<"key "<<aliasname<<" val "<<aliascomm<<endl;
            al[aliasname]=aliascomm;
        }
        break;
        case 4 :    //$$
        {
            cout<<getpid()<<endl;
            return 1;
        }
        break;
        case 5 :    //$?
        {
            ifstream ifs;
            ifs.open("status.txt");
            int prev_status;
            ifs>>prev_status;
            cout<<prev_status<<endl;
            return 1;
        }
    }
}

int executecommandwithoutfork(char** args)
{
    //this function executes a command without forking.
    // I use this function instead of execv directly to handle errors or
    //call builtin functions
    int isinbuilt = checkinbuilt(args[0]+5);
    /* use this code to see what arguments are received
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
            ofstream ofs;
            ofs.open("status.txt");
            ofs<<0;
            ofs.close();
            int status = execv(args[0],args);
            if(status)
            {
                cout<<strerror(errno)<<errno<<endl;
                ofstream ofs;
                ofs.open("status.txt");
                ofs<<errno;
                ofs.close();
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
        	savetohistory();
            cout<<"Bye"<<endl;
            exit(0);
        }
        break;
        case 3 :    //alias
        {

        }
        break;
        case 4 :    //$$
        {
            cout<<getpid()<<endl;
            return 1;
        }
        break;
        case 5 :    //$?
        {
            ifstream ifs;
            ifs.open("status.txt");
            int prev_status;
            ifs>>prev_status;
            cout<<prev_status<<endl;
            return 1;
        }
    }
}
