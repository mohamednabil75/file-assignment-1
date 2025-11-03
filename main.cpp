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
       <<"10) Search for specific doctor\n"
       <<"11) Search for specific appointment\n"
       << "0) Exit\n";

    cin>>choice;
    cin.ignore();
    switch (choice)
    {
        case(1): {
            fstream file("doctor.txt", ios::out | ios::app);
            if (!file) {
                cout << "Error opening file!\n";
                return 1;
            }
            doctor doc;
            bool validID = false;
            while (!validID) {
                cout << "Enter doctor ID: ";
                cin.getline(doc.id, 15);

                vector<doctor::PIndex> tempIndex;
                doc.readPrimIndex(tempIndex);

                bool exists = false;
                for (auto &p: tempIndex)
                    if (strcmp(p.id, doc.id) == 0) exists = true;

                if (exists)
                    cout << "ID already exists. Enter a different ID.\n";
                else
                    validID = true;
            }

            cout << "Name: ";
            cin.getline(doc.name, 30);
            cout << "Address: ";
            cin.getline(doc.address, 30);

            doc.addDoctorPI(file, doc);
            file.close();
            break;
        }

        case 2: {
            fstream file("appointment.txt", ios::out | ios::app | ios::binary);
            if (!file) {
                cout << "Error opening file!\n";
                return 1;
            }

            appointment appoint;
            bool validID = false;

            while (!validID) {
                cout << "Enter appointment ID: ";
                cin.getline(appoint.id, 15);

                vector<appointment::PIndex> tempIndex;
                appoint.readPrimIndex(tempIndex);

                bool exists = false;
                for (auto &p : tempIndex)
                    if (strcmp(p.id, appoint.id) == 0) exists = true;

                if (exists)
                    cout << "ID already exists. Enter a different ID.\n";
                else
                    validID = true;
            }

            cout << "Date: ";
            cin.getline(appoint.date, 30);

            cout << "Doctor ID: ";
            cin.getline(appoint.docId, 15);

            appoint.addAppointmentPI(file, appoint);
            file.close();
            break;
        }
        case 10: {
            doctor d;
            d.searchDoctorById();
            break;

        }
        case 11: {
           appointment a;
            a.searchAppointmentById();
            break;

        }
        default:
            break;
    }
  return 0;
}