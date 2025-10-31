#include<cstring>
class doctor{
    char id[15];
    char name[30];
    char address[30];
  public:
    void add(char id[15],char name[30],char address[30]){
        strcpy(this->id,id);
        strcpy(this->name,name);
        strcpy(this->address,address);
        //continue to add data into file
    }
    void del(){};
    void update(){};
};