using namespace std;

int executecommand(char** args);
int executecommandwithoutfork(char** args);
void handlepipe(string input);
void sethistory(int lim);
void load_history();
void insertinhistory(string command);
string getRecent();
void reset();
void savetohistory();
void handleredirection(string input,int index);
void handleappend(string input,int index);
char** parseString(string input,int lim1,int lim2);
char* getToken(string input,int* ptr,int lim2);
char* getCommand(string input,int* ptr, int lim2);
char* getStringToken(string input,int* ptr, int lim2);
int checkinbuilt(string command);
