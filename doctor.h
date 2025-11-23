#ifndef doctor_h
#define doctor_h
#include <bits/stdc++.h>
using namespace std;
#include "ReadandWrite.h"
#include "search.h"
// u can find class RAW in " ReadandWrite.h" file

class doctor : public virtual RAW, public searchdoc
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
            file << "Header: " << -1 << '\n';
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
        file.seekg(8, ios::beg);
        int firstdel;
        file >> firstdel; // Get the Previous deleted record offset

        char checkdel;
        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "Primary.txt");
        int rnn = getRecordRRN(primIndexArray, d.id);

        file.seekp(8, ios::beg);
        file << rnn;                               // update header to new first deleted record
        file.seekp(rnn + sizeof(short), ios::beg); // move to the deletion marker
        file.put('*');                             // mark as deleted
        file << firstdel;                          // point to previous deleted record
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
};

#endif