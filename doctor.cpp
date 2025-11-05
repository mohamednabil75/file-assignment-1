#include <bits/stdc++.h>
using namespace std;
#include "ReadandWrite.h"
#include "search.h"
 // u can find class RAW in " ReadandWrite.h" file

class doctor:public virtual RAW,public  searchdoc {
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
    void addDoctorPI(fstream &file, doctor &d) {
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
        file.write((char*)&length, sizeof(short));
        file.write(record, length);
        // Update Primary Index
        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray,"Primary.txt");

        for (auto &p : primIndexArray) {
            if (strcmp(p.id, d.id) == 0) {
                cout << "Doctor with this ID already exists\n";
                return;
            }
        }

        PIndex newEntry;
        strcpy(newEntry.id, d.id);
        newEntry.RRN = RRN;
        primIndexArray.push_back(newEntry);
        sort(primIndexArray.begin(), primIndexArray.end());
        writePrimIndex(primIndexArray,"Primary.txt");

        cout << "Doctor added and Primary Index updated.\n";
    }


// Search doctor by ID



    // Add doctor record without primary index (just for initial data)
    /** 
    *@param file file that store data of doctors
    *@param d doctor that u want to add to file 
    *store data of doctor to file (doctor.txt)
    */
    void addDoctor(fstream &file, doctor &d) {
        char record[size];
        strcpy(record, d.id);
        strcat(record, "|");
        strcat(record, d.name);
        strcat(record, "|");
        strcat(record, d.address);
        strcat(record, "\n");
        short length = strlen(record);
        file << length;
        file.write(record, length);
    }

};


