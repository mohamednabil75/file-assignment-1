#include <bits/stdc++.h>
using namespace std;

class doctor {
public:
    char id[15];
    char name[30];
    char address[30];
    const static int size = 1000;

    struct PIndex {
        int RRN;
        char id[15];

        bool operator<(const PIndex &p) const {
            return strcmp(id, p.id) < 0;
        }
    };

    void writePrimIndex(vector<PIndex> &primIndexArray) {
        ofstream outFile("Primary.txt");
        for (auto &p : primIndexArray) {
            outFile << p.id << "|" << p.RRN << "\n";
        }
        outFile.close();
    }

    void readPrimIndex(vector<PIndex> &primIndexArray) {
        ifstream inFile("Primary.txt");
        if (!inFile) return;
        string line;
        while (getline(inFile, line)) {
            PIndex p;
            stringstream ss(line);
            string idStr, rrnStr;
            getline(ss, idStr, '|');
            getline(ss, rrnStr, '|');
            strcpy(p.id, idStr.c_str());
            p.RRN = stoi(rrnStr);
            primIndexArray.push_back(p);
        }
        inFile.close();
    }
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
        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray);

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
        writePrimIndex(primIndexArray);

        cout << "Doctor added and Primary Index updated.\n";
    }



    void searchDoctorById() {
        fstream primIndex("Primary.txt", ios::in | ios::binary);
        ifstream file("doctor.txt");

        if (!file || !primIndex) {
            cout << "Error: could not open files.\n";
            return;
        }

        vector<PIndex> PrimIndexArray;
        readPrimIndex(PrimIndexArray);

        char ID[15];
        cout << "Enter Target Doctor ID: ";
        cin >> ID;

        int RRN = getRecordRRN(PrimIndexArray, ID);
        if (RRN == -1) {
            cout << "Doctor not found!\n";
            return;
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
    }


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


