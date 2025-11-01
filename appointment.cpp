#include <cstring>
#include<fstream>
using namespace std;
class appointment{
public:
    char id[15];
    char date[30];
    char docId[15];
    const static int size = 1000;

    void addAppointment(fstream &file , appointment &a){
        char record[size];
        strcpy(record,a.id);
        strcat(record,"|");
        strcat(record,a.date);
        strcat(record,"|");
        strcat(record,a.docId);
        strcat(record,"\n");
        short length = strlen(record);
        file << length;
        file.write(record,length);
    }

    void del(){};
    void update(){};

};