#include <bits/stdc++.h>
using namespace std;
#include "ReadandWrite.h"
#include "search.h"


 // u can find class RAW in " ReadandWrite.h" file

class appointment:public virtual RAW,public virtual searchapp {
public:
    char id[15];
    char date[30];
    char docId[15];
    const static int size = 1000;

  

    // Add appointment record + update primary index
    void addAppointmentPI(fstream &file, appointment &a) {
        file.seekp(0, ios::end);
        int RRN = file.tellp();

        char record[size];
        strcpy(record, a.id);
        strcat(record, "|");
        strcat(record, a.date);
        strcat(record, "|");
        strcat(record, a.docId);
        strcat(record, "\n");

        short length = strlen(record);
        file.write((char*)&length, sizeof(short)); // binary length
        file.write(record, length);

        // Update Primary Index
        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray,"PrimaryAppointment.txt");

        for (auto &p : primIndexArray) {
            if (strcmp(p.id, a.id) == 0) {
                cout << "Appointment with this ID already exists\n";
                return;
            }
        }

        PIndex newEntry;
        strcpy(newEntry.id, a.id);
        newEntry.RRN = RRN;
        primIndexArray.push_back(newEntry);
        sort(primIndexArray.begin(), primIndexArray.end());
        writePrimIndex(primIndexArray,"PrimaryAppointment.txt");

        cout << "Appointment added and Primary Index updated.\n";
    }
    // Add appointment record without primary index (just for initial data)
    void addAppointment(fstream &file, appointment &a) {
        char record[size];
        strcpy(record, a.id);
        strcat(record, "|");
        strcat(record, a.date);
        strcat(record, "|");
        strcat(record, a.docId);
        strcat(record, "\n");
        short length = strlen(record);
        file.write((char*)&length, sizeof(short));
        file.write(record, length);
    }
};
