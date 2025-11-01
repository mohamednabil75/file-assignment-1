#include<cstring>
#include<fstream>
using namespace std;
class doctor{
public:
    char id[15];
    char name[30];
    char address[30];
    const static int size = 1000;

    void addDoctor(fstream &file , doctor &d){
        char record[size];
        strcpy(record,d.id);
        strcat(record,"|");
        strcat(record,d.name);
        strcat(record,"|");
        strcat(record,d.address);
        strcat(record,"\n");
        short length = strlen(record);
        file << length;
        file.write(record,length);
    }
    void del(){};
    void update(){};
};