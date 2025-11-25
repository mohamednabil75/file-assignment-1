#include <iostream>
#include "doctor.h"
#include "appointment.h"
#include "query.h"
#include <fstream>
using namespace std;
int main()
{
    // query q;
    // cout << endl;
    // q.parse("select id,name,docid from doctor,appointment");
    // vector<string> name{"id","name","addres"};
    // cout<<q.checkCoulmn("doctor",name)<<endl;
    int choice;
    while (true)
    {
        cout << "1) Add new doctor\n"
             << "2) Add new appointment\n"
             << "3) Update doctor name\n"
             << "4) Update doctor address\n"
             << "5) Update appointment date\n"
             << "6) Delete appointment\n"
             << "7) Delete doctor\n"
             << "8) Print doctor info\n"
             << "9) Print appointment info\n"
             << "10) Write query\n"
             << "11) Search for specific doctor\n"
             << "12) Search for specific appointment\n"
             << "0) Exit\n";
        cin >> choice;
        cin.ignore();
        if (choice == 0)
            break;
        switch (choice)
        {
        case (1):
        {
            fstream file("doctor.txt", ios::out|ios::in );
            if (!file)
            {
                cout << "Error opening file!\n";
                return 1;
            }
            doctor doc;
            bool validID = false;
            while (!validID)
            {
                cout << "Enter doctor ID: ";
                cin.getline(doc.id, 15);

                vector<doctor::PIndex> tempIndex;
                doc.readPrimIndex(tempIndex, "primary.txt");

                bool exists = false;
                for (auto &p : tempIndex)
                    if (strcmp(p.id, doc.id) == 0)
                        exists = true;

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

        case 2:
        {
            fstream file("appointment.txt", ios::out | ios::app | ios::binary);
            if (!file)
            {
                cout << "Error opening file!\n";
                return 1;
            }

            appointment appoint;
            bool validID = false;

            while (!validID)
            {
                cout << "Enter appointment ID: ";
                cin.getline(appoint.id, 15);

                vector<appointment::PIndex> tempIndex;
                appoint.readPrimIndex(tempIndex, "PrimaryAppointment.txt");

                bool exists = false;
                for (auto &p : tempIndex)
                    if (strcmp(p.id, appoint.id) == 0)
                        exists = true;

                if (exists)
                    cout << "ID already exists. Enter a different ID.\n";
                else
                    validID = true;
            }

            cout << "Date: ";
            cin.getline(appoint.date, 30);

            cout << "Doctor ID: ";
            cin.getline(appoint.docId, 15);
            doctor d;
            // int docfound = d.searchDoctorById(appoint.docId, 0);
            // if (docfound == -1)
            // continue;
            appoint.addAppointmentPI(file, appoint);
            file.close();
            break;
        }
        case 3: // Update doctor name
        {
            char doctorID[15];
            cout << "Enter Doctor ID to update name: ";
            cin.getline(doctorID, 15);

            doctor doc;
            doc.updateDoctorName(doctorID);
            break;
        }
        case 4: // Update doctor address
        {
            char doctorID[15];
            cout << "Enter Doctor ID to update address: ";
            cin.getline(doctorID, 15);

            doctor doc;
            doc.updateDoctorAddress(doctorID);
            break;
        }
        case 5: // Update appointment date
        {
            char appointmentID[15];
            cout << "Enter Appointment ID to update date: ";
            cin.getline(appointmentID, 15);

            appointment app;
            app.updateAppointmentDate(appointmentID);
            break;
        }

        case 6:
        {
            fstream file("appointment.txt", ios::out | ios::in | ios::binary);
            if (!file)
            {
                cout << "Error opening file!\n";
                return 1;
            }
            appointment app;
            bool validID = false;
            while (!validID)
            {
                cout << "Enter appiontment ID: ";
                cin.getline(app.id, 15);

                vector<appointment::PIndex> tempIndex;
                app.readPrimIndex(tempIndex, "PrimaryAppointment.txt"); // Need to consider changing filename

                bool found = false;
                for (auto &p : tempIndex)
                    if (strcmp(p.id, app.id) == 0)
                    {
                        found = true;
                        break;
                    }

                if (!found)
                    cout << "This ID is not found to be deleted\n";
                else
                    validID = true;
            }
            app.deleteAppointmentPI(file, app);
            file.close();
            break;
        }
        case 7:
        {
            fstream file("doctor.txt", ios::out | ios::in | ios::binary);
            if (!file)
            {
                cout << "Error opening file!\n";
                return 1;
            }
            doctor doc;
            bool validID = false;
            while (!validID)
            {
                cout << "Enter doctor ID: ";
                cin.getline(doc.id, 15);

                vector<doctor::PIndex> tempIndex;
                doc.readPrimIndex(tempIndex, "primary.txt");

                bool found = false;

                for (auto &p : tempIndex)
                {
                    if (strcmp(p.id, doc.id) == 0)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    cout << "This ID is not found to be deleted\n";
                else
                    validID = true;
            }
            doc.deleteDoctor(file, doc);
            file.close();
            break;
        }

        case 11:
        {
            doctor d;
            char ID[15];
            cout << "Enter Target Doctor ID: ";
            cin >> ID;
            d.searchDoctorById(ID,1);
            break;
        }
        case 12:
        {
            appointment a;
            char ID[15];
            cout << "Enter Target Appointment ID: ";
            cin >> ID;
            a.searchAppointmentById(ID);
            break;
        }

        default:
            break;
        }
    }
    return 0;
}