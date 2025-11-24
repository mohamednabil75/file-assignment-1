#ifndef doctor_h
#define doctor_h
#include <bits/stdc++.h>
using namespace std;
#include "ReadandWrite.h"
#include "search.h"
// u can find class RAW in " ReadandWrite.h" file

class doctor : public virtual RAW, public virtual searchdoc
{
public:
    char id[15];
    char name[30];
    char address[30];
    const static int size = 1000;

    /**
     *@param file the file u want to store data in
     *@param d take data of doctor u want to add in primary index
     *add doctor data in file ("primary.txt")
     */
    // Add appointment record + update primary index
    void addDoctorPI(fstream &file, doctor &d)
    {
        file.seekp(0, ios::end);
        if (file.tellp() == 0) // To check if the file is empty
        {
            // Write the header
            int header=-1;
            file.write((char*)&header, sizeof(int));
            char ch='\n';
            file.write(&ch,sizeof(char));
        }
        file.seekp(0, ios::end);
        int RRN = file.tellp();
        char record[size];
        strcpy(record, d.id);
        strcat(record, "|");
        strcat(record, d.name);
        strcat(record, "|");
        strcat(record, d.address);
        strcat(record, "\n");

        short length = strlen(record);
        file.write((char *)&length, sizeof(short));
        file.write(record, length);
        // Update Primary Index
        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "Primary.txt");

        for (auto &p : primIndexArray)
        {
            if (strcmp(p.id, d.id) == 0)
            {
                cout << "Doctor with this ID already exists\n";
                return;
            }
        }

        PIndex newEntry;
        strcpy(newEntry.id, d.id);
        newEntry.RRN = RRN;
        primIndexArray.push_back(newEntry);
        sort(primIndexArray.begin(), primIndexArray.end());
        writePrimIndex(primIndexArray, "Primary.txt");

        cout << "Doctor added and Primary Index updated.\n";
    }

    void deleteDoctor(fstream &file, doctor &d)
    {
        file.clear();
        file.seekg(0, ios::beg);
        int firstdel;
        file.read((char *)&firstdel, sizeof(int));// Get the Previous deleted record offset

        char checkdel;
        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray,"Primary.txt");
        int offset = getRecordRRN(primIndexArray, d.id);
        short len;
        file.seekg(offset, ios::beg);
        file.read((char *)&len, sizeof(short));
        len -= 6; // To skip the * and firstdel
        file.seekp(0, ios::beg);
        file.write((char *)&offset, sizeof(int));    // update header to new first deleted record
        file.seekp(offset + sizeof(short), ios::beg); // move to the deletion marker
        file.put('*');                                // mark as deleted
        file.write((char *)&firstdel, sizeof(int));// point to previous deleted record
        while (len)
        {
            file.put(' ');
            len--;
        }

        file.close();

        /*
            // Optional: Update and print available list
                vector<DocAvail> docAva;
                readDocAvail(docAva,"doctor.txt");//Read available list
                printDocAvail(docAva);//Print available list
                */

        // Update Primary Index
        int size_of_vec = primIndexArray.size();
        for (auto &p : primIndexArray)
        {
            if (strcmp(p.id, d.id) == 0)
            {
                // Replace the found entry with the last entry and remove the last element
                strcpy(p.id, primIndexArray[size_of_vec - 1].id);
                p.RRN = primIndexArray[size_of_vec - 1].RRN; // Assign the last RRN to current position
                primIndexArray.pop_back();                   // remove last element
                sort(primIndexArray.begin(), primIndexArray.end());
                writePrimIndex(primIndexArray, "Primary.txt");
                cout << "Doctor deleted and Primary Index updated.\n";
                break;
            }
        }
    }

    void updateDoctorName(const char *doctorID)
    {

        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "Primary.txt");

        int RRN = getRecordRRN(primIndexArray, doctorID);
        if (RRN == -1)
        {
            cout << "Doctor with ID '" << doctorID << "' not found!\n";
            return;
        }

        fstream file("doctor.txt", ios::in | ios::out | ios::binary);
        if (!file)
        {
            cout << "Error opening doctor file for update!\n";
            return;
        }

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char *)&length, sizeof(short));

        char *buffer = new char[length + 1];
        file.read(buffer, length);
        buffer[length] = '\0';

        stringstream ss(buffer);
        string currentId, currentName, currentAddress;
        getline(ss, currentId, '|');
        getline(ss, currentName, '|');
        getline(ss, currentAddress, '|');

        cout << "Current Doctor Information:\n";
        cout << "ID: " << currentId << "\n";
        cout << "Name: " << currentName << "\n";
        cout << "Address: " << currentAddress << "\n\n";

        char newName[30];
        cout << "Enter new name (max 29 characters): ";
        cin.getline(newName, 30);

        if (strlen(newName) == 0)
        {
            cout << "Name cannot be empty!\n";
            delete[] buffer;
            file.close();
            return;
        }
        char updatedRecord[size];
        strcpy(updatedRecord, doctorID);
        strcat(updatedRecord, "|");
        strcat(updatedRecord, newName);
        strcat(updatedRecord, "|");
        strcat(updatedRecord, currentAddress.c_str());
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
            cout << "Doctor name updated successfully!\n";
        }
        else
        {
            cout << "Error: New name is too long for the allocated space!\n";
            cout << "Available space: " << length << " characters, Required: " << newLength << " characters\n";
        }

        delete[] buffer;
        file.close();
    }

    void updateDoctorAddress(const char *doctorID)
    {

        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "Primary.txt");

        int RRN = getRecordRRN(primIndexArray, doctorID);
        if (RRN == -1)
        {
            cout << "Doctor with ID '" << doctorID << "' not found!\n";
            return;
        }

        fstream file("doctor.txt", ios::in | ios::out | ios::binary);
        if (!file)
        {
            cout << "Error opening doctor file for update!\n";
            return;
        }

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char *)&length, sizeof(short));

        char *buffer = new char[length + 1];
        file.read(buffer, length);
        buffer[length] = '\0';

        stringstream ss(buffer);
        string currentId, currentName, currentAddress;
        getline(ss, currentId, '|');
        getline(ss, currentName, '|');
        getline(ss, currentAddress, '|');

        cout << "Current Doctor Information:\n";
        cout << "ID: " << currentId << "\n";
        cout << "Name: " << currentName << "\n";
        cout << "Address: " << currentAddress << "\n\n";

        char newAddress[30];
        cout << "Enter new address (max 29 characters): ";
        cin.getline(newAddress, 30);

        if (strlen(newAddress) == 0)
        {
            cout << "Address cannot be empty!\n";
            delete[] buffer;
            file.close();
            return;
        }

        char updatedRecord[size];
        strcpy(updatedRecord, doctorID);
        strcat(updatedRecord, "|");
        strcat(updatedRecord, currentName.c_str());
        strcat(updatedRecord, "|");
        strcat(updatedRecord, newAddress);
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
            cout << "Doctor address updated successfully!\n";
        }
        else
        {
            cout << "Error: New address is too long for the allocated space!\n";
            cout << "Available space: " << length << " characters, Required: " << newLength << " characters\n";
        }

        delete[] buffer;
        file.close();
    }
};

#endif