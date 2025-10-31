#include <cstring>
class appointment{
    char id[15];
    char date[30];
    char docId[10];
   public:
    void add(char id[15],char date[30],char docId[10]){
        strcpy(this->id,id);
        strcpy(this->date,date);
        strcpy(this->date,date);

    }
    void del(){};
    void update(){};
    
};