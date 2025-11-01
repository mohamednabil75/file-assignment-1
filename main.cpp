#include <iostream>
#include "doctor.cpp"
#include "appointment.cpp"
#include<fstream>
using namespace std;
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
    cin.ignore();
    switch (choice)
    {
        case 1: {
            fstream file("doctor.txt",ios::out | ios::app);
            if (!file) {
                cout << "Error opening file!\n";
                return 1;
            }
            doctor doc;
            cout <<"Enter doctor data: " << endl;
            cout<<"id:";
            cin.getline(doc.id,15);
            cout<<"Name:";
            cin.getline(doc.name,30);
            cout<<"address:";
            cin.getline(doc.address,30);
            doc.addDoctor(file , doc);
            file.close();
            break;
        }
        case 2: {
            fstream file2("appointment.txt",ios::out|ios::app);
            appointment appoint;
            if (!file2) {
                cout << "Error opening file!\n";
                return 1;
            }
            cout<<"id:";
            cin.getline(appoint.id,15);
            cout<<"date:";
            cin.getline(appoint.date,30);
            cout<<"doctor id:";
            cin.getline(appoint.docId,15);
            appoint.addAppointment(file2 , appoint);
            file2.close();
            break;
        }
        default:
            break;
    }
  return 0;
}