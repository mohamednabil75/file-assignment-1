#include <iostream>
#include "doctor.cpp"
#include "appointment.cpp"
int main(){
    int choice;
  cout << "1) Add new doctor\n"
     << "2) Add new appointment\n"
     << "3) Update doctor name\n"
     << "4) Update appointment date\n"
     << "5) Delete appointment\n"
     << "6) Delete doctor\n"
     << "7) Print doctor info\n"
     << "8) Print appointment info\n"
     << "9) Write query\n"
     << "0) Exit\n";

    cin>>choice;
    switch (choice)
    {
    case 1:
        doctor doc;
        char id[15],name[30],address[30];
        cout<<"id:";cin>>id;
        cout<<"Name:";cin>>name;
        cout<<"address:";cin>>address;
        doc.add(id,name,address);
        break;
    case 2:
        appointment appoint;
        char id[15],date[30],docid[15];
        cout<<"id:";cin>>id;
        cout<<"date:";cin>>date;
        cout<<"doctor id:";cin>>docid;
        
    default:
        break;
    }

}