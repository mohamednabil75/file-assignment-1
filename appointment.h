#ifndef appointment_h
#define appointment_h
#include <bits/stdc++.h>
using namespace std;
#include "ReadandWrite.h"
#include "search.h"

// u can find class RAW in " ReadandWrite.h" file

class appointment : public virtual RAW, public virtual searchapp
{
public:
    char id[15];
    char date[30];
    char docId[15];
    const static int size = 1000;

    // Add appointment record + update primary index
    void addAppointmentPI(fstream &file, appointment &a)
    {
        file.seekp(0, ios::end);
        if (file.tellp() == 0)
        {
            int header = -1;
            file.write((char *)&header, sizeof(int));
            char ch = '\n';
            file.write(&ch, sizeof(char));
        }
        int RRN = file.tellp();

        char record[size];
        strcpy(record, a.id);
        strcat(record, "|");
        strcat(record, a.date);
        strcat(record, "|");
        strcat(record, a.docId);
        strcat(record, "\n");

        short length = strlen(record);
        file.write((char *)&length, sizeof(short)); // binary length
        file.write(record, length);

        // Update Primary Index
        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "PrimaryAppointment.txt");

        for (auto &p : primIndexArray)
        {
            if (strcmp(p.id, a.id) == 0)
            {
                cout << "Appointment with this ID already exists\n";
                return;
            }
        }

        PIndex newEntry;
        strcpy(newEntry.id, a.id);
        newEntry.RRN = RRN;
        primIndexArray.push_back(newEntry);
        sort(primIndexArray.begin(), primIndexArray.end());
        writePrimIndex(primIndexArray, "PrimaryAppointment.txt");

        cout << "Appointment added and Primary Index updated.\n";
    }

    void deleteAppointmentPI(fstream &file, appointment &a)
    {
        file.clear();
        file.seekg(0, ios::beg);
        int firstdel;
        file.read((char *)&firstdel, sizeof(int)); // Get the Previous deleted record offset

        char checkdel;
        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "PrimaryAppointment.txt");
        int offset = getRecordRRN(primIndexArray, a.id);
        short len;
        file.seekg(offset, ios::beg);
        file.read((char *)&len, sizeof(short));
        len -= 6; // To skip the * and firstdel
        file.seekp(0, ios::beg);
        file.write((char *)&offset, sizeof(int));     // update header to new first deleted record
        file.seekp(offset + sizeof(short), ios::beg); // move to the deletion marker
        file.put('*');                                // mark as deleted
        file.write((char *)&firstdel, sizeof(int));   // point to previous deleted record
        while (len)
        {
            file.put(' ');
            len--;
        }

        file.close();

        /*         // Optional: Update and print available list
                vector<AppAvail> availarray;
                readAppAvail(availarray, "AppointmentAvail.txt");//Read available list
                printAppAvail(availarray);//Print available list */

        // Update Primary Index
        int size_of_vec = primIndexArray.size();
        for (auto &p : primIndexArray)
        {
            if (strcmp(p.id, a.id) == 0)
            {
                // Replace the found entry with the last entry and remove the last element
                strcpy(p.id, primIndexArray[size_of_vec - 1].id);
                p.RRN = primIndexArray[size_of_vec - 1].RRN;
                primIndexArray.pop_back(); // remove last element
                sort(primIndexArray.begin(), primIndexArray.end());
                writePrimIndex(primIndexArray, "PrimaryAppointment.txt");
                cout << "Appoinment deleted and Primary Index updated.\n";
                break;
            }
        }
    }
    void updateAppointmentDate(const char *appointmentID)
    {

        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "PrimaryAppointment.txt");

        int RRN = getRecordRRN(primIndexArray, appointmentID);
        if (RRN == -1)
        {
            cout << "Appointment with ID '" << appointmentID << "' not found!\n";
            return;
        }

        fstream file("appointment.txt", ios::in | ios::out | ios::binary);
        if (!file)
        {
            cout << "Error opening appointment file for update!\n";
            return;
        }

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char *)&length, sizeof(short));

        char *buffer = new char[length + 1];
        file.read(buffer, length);
        buffer[length] = '\0';

        stringstream ss(buffer);
        string currentId, currentDate, currentDocId;
        getline(ss, currentId, '|');
        getline(ss, currentDate, '|');
        getline(ss, currentDocId, '|');

        cout << "Current Appointment Information:\n";
        cout << "ID: " << currentId << "\n";
        cout << "Date: " << currentDate << "\n";
        cout << "Doctor ID: " << currentDocId << "\n\n";

        char newDate[30];
        cout << "Enter new date (max 29 characters): ";
        cin.getline(newDate, 30);

        if (strlen(newDate) == 0)
        {
            cout << "Date cannot be empty!\n";
            delete[] buffer;
            file.close();
            return;
        }

        char updatedRecord[size];
        strcpy(updatedRecord, appointmentID);
        strcat(updatedRecord, "|");
        strcat(updatedRecord, newDate);
        strcat(updatedRecord, "|");
        strcat(updatedRecord, currentDocId.c_str());
        strcat(updatedRecord, "\n");

        short newLength = strlen(updatedRecord);

        if (newLength <= length)
        {
            file.seekp(RRN + sizeof(short), ios::beg);
            file.write(updatedRecord, newLength);

            if (newLength < length)
            {
                for (int i = newLength; i < length; i++)
                {
                    file.put(' ');
                }
            }
            cout << "Appointment date updated successfully!\n";
        }
        else
        {
            cout << "Error: New date is too long for the allocated space!\n";
            cout << "Available space: " << length << " characters, Required: " << newLength << " characters\n";
        }

        delete[] buffer;
        file.close();
    }
};
#endif