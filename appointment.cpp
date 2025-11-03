#include <bits/stdc++.h>
using namespace std;

class appointment {
public:
    char id[15];
    char date[30];
    char docId[15];
    const static int size = 1000;

    struct PIndex {
        int RRN;
        char id[15];

        bool operator<(const PIndex &p) const {
            return strcmp(id, p.id) < 0;
        }
    };

    // Write primary index to file
    void writePrimIndex(vector<PIndex> &primIndexArray) {
        ofstream outFile("PrimaryAppointment.txt");
        for (auto &p : primIndexArray) {
            outFile << p.id << "|" << p.RRN << "\n";
        }
        outFile.close();
    }

    // Read primary index from file
    void readPrimIndex(vector<PIndex> &primIndexArray) {
        ifstream inFile("PrimaryAppointment.txt");
        if (!inFile) return; // File may not exist yet
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

    // Binary search in primary index
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
        readPrimIndex(primIndexArray);

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
        writePrimIndex(primIndexArray);

        cout << "Appointment added and Primary Index updated.\n";
    }

    // Search appointment by ID
    void searchAppointmentById() {
        fstream primIndex("PrimaryAppointment.txt", ios::in | ios::binary);
        ifstream file("appointment.txt");

        if (!file || !primIndex) {
            cout << "Error: could not open files.\n";
            return;
        }

        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray);

        char ID[15];
        cout << "Enter Target Appointment ID: ";
        cin >> ID;

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
