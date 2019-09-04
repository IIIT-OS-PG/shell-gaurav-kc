using namespace std;

char* getToken(string input,int* ptr)
{
    int count=0;
    int k=0;
    int i=*ptr;
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
    //cout<<"get token "<<a<<endl;
    return a;
}

char* getCommand(string input,int* ptr)
{
    int i=*ptr;
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
    //cout<<"get command "<<a<<endl;
    return a;
}

char* getStringToken(string input,int* ptr)
{
    int i=*ptr;
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
    //cout<<"get string token "<<a<<endl;
    return a;
}
