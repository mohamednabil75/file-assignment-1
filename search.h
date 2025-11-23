#ifndef search_H
#define search_H
#include <bits/stdc++.h>
#include "ReadandWrite.h"
using namespace std;
 // u can find class RAW in " ReadandWrite.h" file

class search:public virtual RAW{
    public:    
        int getRecordRRN(vector<PIndex> &primIndexArray, const char *id) {
        int low = 0, high = primIndexArray.size() - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            int cmp = strcmp(id, primIndexArray[mid].id);
            if (cmp < 0)
                high = mid - 1;
            else if (cmp > 0)
                low = mid + 1;
            else
                return primIndexArray[mid].RRN;
        }
        return -1;
    }


};
class searchdoc:public virtual search{
    public:
    
    // Binary search in primary index
    /**
     * @param ID for id u want to serch for
     * @param SearchAndOutput if true it output data of doctor if false it returns only RRN of doctor without output it
     */
/*     int searchDoctorById(char ID[15],bool SearchAndOutput) {
        fstream primIndex("Primary.txt", ios::in | ios::binary);
        ifstream file("doctor.txt");

        if (!file || !primIndex) {
            cout << "Error: could not open files.\n";
            return -1;
        }

        vector<PIndex> PrimIndexArray;
        readPrimIndex(PrimIndexArray,"primary.txt");
        int RRN = getRecordRRN(PrimIndexArray, ID);
        if (RRN == -1) {
            cout << "Doctor not found!\n";
            return -1;
        }
        else if (SearchAndOutput==0){
            return RRN;
        }

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char*)&length, sizeof(short));

        char *buffer = new char[length + 1];
        file.read(buffer, length);
        buffer[length] = '\0';

        stringstream ss(buffer);
        string id, name, address;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, address, '|');

        cout << "Doctor found:\n";
        cout << "ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Address: " << address << "\n";

        delete[] buffer;


        file.close();
        primIndex.close();
    } */
};
class searchapp:public search{
    public:
    // Search appointment by ID
    void searchAppointmentById(char ID[15]) {
        fstream primIndex("PrimaryAppointment.txt", ios::in | ios::binary);
        ifstream file("appointment.txt");

        if (!file || !primIndex) {
            cout << "Error: could not open files.\n";
            return;
        }

        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray,"PrimaryAppointment.txt");

       

        int RRN = getRecordRRN(primIndexArray, ID);
        if (RRN == -1) {
            cout << "Appointment not found!\n";
            return;
        }

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char*)&length, sizeof(short));

        char *buffer = new char[length + 1];
        file.read(buffer, length);
        buffer[length] = '\0';

        stringstream ss(buffer);
        string id, date, docId;
        getline(ss, id, '|');
        getline(ss, date, '|');
        getline(ss, docId, '|');

        cout << "Appointment found:\n";
        cout << "ID: " << id << "\n";
        cout << "Date: " << date << "\n";
        cout << "Doctor ID: " << docId << "\n";

        delete[] buffer;
        file.close();
        primIndex.close();
    }
};
#endif