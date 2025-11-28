#ifndef search_H
#define search_H
#include <bits/stdc++.h>
#include "ReadandWrite.h"
using namespace std;
// u can find class RAW in " ReadandWrite.h" file

class search : public virtual RAW
{
public:
    int getRecordRRN(vector<PIndex> &primIndexArray, const char *id)
    {
        int low = 0, high = primIndexArray.size() - 1;
        while (low <= high)
        {
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
class searchdoc : public virtual search
{
public:
    // Binary search in primary index
    /**
     * @param ID for id u want to serch for
     * @param SearchAndOutput if true it output data of doctor if false it returns only RRN of doctor without output it
     */
    int searchDoctorById(char ID[15], bool SearchAndOutput)
    {
        fstream primIndex("Primary.txt", ios::in | ios::binary);
        ifstream file("doctor.txt");

        if (!file || !primIndex)
        {
            cout << "Error: could not open files.\n";
            return -1;
        }

        vector<PIndex> PrimIndexArray;
        readPrimIndex(PrimIndexArray, "primary.txt");
        int RRN = getRecordRRN(PrimIndexArray, ID);
        if (RRN == -1)
        {
            cout << "Doctor not found!\n";
            return -1;
        }
        else if (SearchAndOutput == 0)
        {
            return RRN;
        }

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char *)&length, sizeof(short));

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
        return RRN;
    }
    void searchDoctorById(const char ID[15], bool id,bool name,bool address)
    {
        fstream primIndex("Primary.txt", ios::in | ios::binary);
        ifstream file("doctor.txt");

        if (!file || !primIndex)
        {
            cout << "Error: could not open files.\n";
            return ;
        }

        vector<PIndex> PrimIndexArray;
        readPrimIndex(PrimIndexArray, "primary.txt");
        int RRN = getRecordRRN(PrimIndexArray, ID);
        if (RRN == -1)
        {
            cout << "Doctor not found!\n";
            return ;
        }

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char *)&length, sizeof(short));

        char *buffer = new char[length + 1];
        file.read(buffer, length);
        buffer[length] = '\0';

        stringstream ss(buffer);
        string id1, name1, address1;
        getline(ss, id1, '|');
        getline(ss, name1, '|');
        getline(ss, address1, '|');

        if (id==1)
        cout << "ID: " << id1 << "\n";
        if (name==1)
        cout << "Name: " << name1 << "\n";
        if (address==1)
        cout << "Address: " << address1 << "\n";

        delete[] buffer;

        file.close();
        primIndex.close();
        return ;
    }
    void searchByDoctorName(string doctorName,
                            string secIndexFile = "Doctors_Secondary.txt",
                            string secListFile = "Doctors_Secondary_List.txt",
                            string primaryFile = "Primary.txt",
                            string dataFile = "doctor.txt")
    {
        vector<DocSecIndex> secIndex;
        vector<DocSecNode> nodes;
        readDocSecondaryIndex(secIndex, secIndexFile);
        readDocSecondaryList(nodes, secListFile);

        int pos = binarySearchDocName(secIndex, doctorName);
        if (pos == -1)
        {
            cout << "No doctors found with name: " << doctorName << "\n";
            return;
        }

        int head = secIndex[pos].head;
        if (head == -1)
        {
            cout << "No doctors linked to this name.\n";
            return;
        }

        vector<PIndex> primary;
        readPrimIndex(primary, primaryFile);

        int cur = head;

        while (cur != -1)
        {
            string docID = nodes[cur].doctorID;

            int rrn = -1;
            for (auto &p : primary)
            {
                if (docID == p.id)
                {
                    rrn = p.RRN;
                    break;
                }
            }

            if (rrn == -1)
            {
                cout << "Error: DoctorID found in secondary but not in primary.\n";
                cur = nodes[cur].next;
                continue;
            }

            ifstream in(dataFile);
            if (!in)
            {
                cout << "Cannot open Doctors file.\n";
                return;
            }

            in.seekg(rrn, ios::beg);

            short len = 0;
            in.read((char *)&len, sizeof(short));

            if (len <= 0)
            {
                cout << "Deleted or empty record.\n";
                cur = nodes[cur].next;
                continue;
            }

            string record(len, ' ');
            in.read(&record[0], len);

            cout << "Doctor Record: " << record << "\n";

            in.close();

            cur = nodes[cur].next;
        }
    }
    void searchByDoctorName(string doctorName,
                            bool id,
                            bool name,
                            bool address,
                            string secIndexFile = "Doctors_Secondary.txt",
                            string secListFile = "Doctors_Secondary_List.txt",
                            string primaryFile = "Primary.txt",
                            string dataFile = "doctor.txt")
    {
        vector<DocSecIndex> secIndex;
        vector<DocSecNode> nodes;
        readDocSecondaryIndex(secIndex, secIndexFile);
        readDocSecondaryList(nodes, secListFile);

        int pos = binarySearchDocName(secIndex, doctorName);
        if (pos == -1)
        {
            cout << "No doctors found with name: " << doctorName << "\n";
            return;
        }

        int head = secIndex[pos].head;
        if (head == -1)
        {
            cout << "No doctors linked to this name.\n";
            return;
        }

        vector<PIndex> primary;
        readPrimIndex(primary, primaryFile);

        int cur = head;

        while (cur != -1)
        {
            string docID = nodes[cur].doctorID;

            int rrn = -1;
            for (auto &p : primary)
            {
                if (docID == p.id)
                {
                    rrn = p.RRN;
                    break;
                }
            }

            if (rrn == -1)
            {
                cout << "Error: DoctorID found in secondary but not in primary.\n";
                cur = nodes[cur].next;
                continue;
            }

            ifstream in(dataFile);
            if (!in)
            {
                cout << "Cannot open Doctors file.\n";
                return;
            }

            in.seekg(rrn, ios::beg);

            short len = 0;
            in.read((char *)&len, sizeof(short));

            if (len <= 0)
            {
                cout << "Deleted or empty record.\n";
                cur = nodes[cur].next;
                continue;
            }

            string record(len, ' ');
            in.read(&record[0], len);
            stringstream ss(record);
            string id1,name1,address1;
            getline(ss, id1, '|');
            getline(ss, name1, '|');
            getline(ss, address1, '|');
            string finalrecord;
            finalrecord =id==true?id1+"|":"";
            finalrecord +=name==true?name1+"|":"";
            finalrecord +=address==true?address1+"|":"";



            cout << "Doctor Record: " << finalrecord << "\n";

            in.close();

            cur = nodes[cur].next;
        }
    }
};
class searchapp : public search
{
public:
    // Search appointment by ID
    int searchAppointmentById(char ID[15],bool SearchAndOutput)
    {
        // fstream primIndex("PrimaryAppointment.txt", ios::in | ios::binary);
        ifstream file("appointment.txt");

        if (!file)
        {
            cout << "Error: could not open files.\n";
            return -1;
        }

        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "PrimaryAppointment.txt");

        int RRN = getRecordRRN(primIndexArray, ID);
        if (RRN == -1)
        {
            cout << "Appointment not found!\n";
            return -1;
        }
        if (SearchAndOutput==0)return RRN;
        cout << "RRN is: " << RRN << endl;

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char *)&length, sizeof(short));

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
        // primIndex.close();
        return RRN;
    }
    void searchAppointmentById(const char ID[15],bool id,bool name,bool docid)
    {
        // fstream primIndex("PrimaryAppointment.txt", ios::in | ios::binary);
        ifstream file("appointment.txt");


        vector<PIndex> primIndexArray;
        readPrimIndex(primIndexArray, "PrimaryAppointment.txt");

        int RRN = getRecordRRN(primIndexArray, ID);

        file.seekg(RRN, ios::beg);
        short length;
        file.read((char *)&length, sizeof(short));

        char *buffer = new char[length + 1];
        file.read(buffer, length);
        buffer[length] = '\0';

        stringstream ss(buffer);
        string id1, date1, docId1;
        getline(ss, id1, '|');
        getline(ss, date1, '|');
        getline(ss, docId1, '|');
        cout << "ID: " << id1 << "\n";
        cout << "Date: " << date1 << "\n";
        cout << "Doctor ID: " << docId1 << "\n";

        delete[] buffer;
        file.close();
        // primIndex.close();
        return ;
    }
    void searchByDoctorID(string doctorID,
                          string secIndexFile = "Appointments_Secondary.txt",
                          string secListFile = "Appointments_Secondary_List.txt",
                          string primaryFile = "PrimaryAppointment.txt",
                          string dataFile = "appointment.txt")
    {
        vector<AppSecIndex> secIndex;
        vector<AppSecNode> nodes;

        readAppSecondaryIndex(secIndex, secIndexFile);
        readAppSecondaryList(nodes, secListFile);

        int pos = binarySearchAppDoctorID(secIndex, doctorID);

        if (pos == -1)
        {
            cout << "No appointments found for DoctorID = " << doctorID << "\n";
            return;
        }

        int head = secIndex[pos].head;

        if (head == -1)
        {
            cout << "No appointments linked to this doctor.\n";
            return;
        }

        vector<PIndex> primary;
        readPrimIndex(primary, primaryFile);

        int cur = head;

        while (cur != -1)
        {
            string appID = nodes[cur].appointmentID;

            int rrn = -1;
            for (auto &p : primary)
            {
                if (appID == p.id)
                {
                    rrn = p.RRN;
                    break;
                }
            }

            if (rrn == -1)
            {
                cout << "Error: AppointmentID found in secondary but missing in primary.\n";
                cur = nodes[cur].next;
                continue;
            }

            // 5. Read the actual appointment record using RRN
            ifstream in(dataFile);
            if (!in)
            {
                cout << "Cannot open Appointments file.\n";
                return;
            }

            in.seekg(rrn, ios::beg);

            short len = 0;
            in.read((char *)&len, sizeof(short));

            if (len <= 0)
            {
                cout << "Deleted or empty record.\n";
                in.close();
                cur = nodes[cur].next;
                continue;
            }

            string record(len, ' ');
            in.read(&record[0], len);

            cout << "Appointment Record: " << record << "\n";

            in.close();

            cur = nodes[cur].next;
        }
    }
     void searchByDoctorID(string doctorID,
                          bool id ,
                          bool date,
                          bool docid,
                          string secIndexFile = "Appointments_Secondary.txt",
                          string secListFile = "Appointments_Secondary_List.txt",
                          string primaryFile = "PrimaryAppointment.txt",
                          string dataFile = "appointment.txt")
    {
        vector<AppSecIndex> secIndex;
        vector<AppSecNode> nodes;

        readAppSecondaryIndex(secIndex, secIndexFile);
        readAppSecondaryList(nodes, secListFile);

        int pos = binarySearchAppDoctorID(secIndex, doctorID);

        if (pos == -1)
        {
            cout << "No appointments found for DoctorID = " << doctorID << "\n";
            return;
        }

        int head = secIndex[pos].head;

        if (head == -1)
        {
            cout << "No appointments linked to this doctor.\n";
            return;
        }

        vector<PIndex> primary;
        readPrimIndex(primary, primaryFile);

        int cur = head;

        while (cur != -1)
        {
            string appID = nodes[cur].appointmentID;

            int rrn = -1;
            for (auto &p : primary)
            {
                if (appID == p.id)
                {
                    rrn = p.RRN;
                    break;
                }
            }

            if (rrn == -1)
            {
                cout << "Error: AppointmentID found in secondary but missing in primary.\n";
                cur = nodes[cur].next;
                continue;
            }

            // 5. Read the actual appointment record using RRN
            ifstream in(dataFile);
            if (!in)
            {
                cout << "Cannot open Appointments file.\n";
                return;
            }

            in.seekg(rrn, ios::beg);

            short len = 0;
            in.read((char *)&len, sizeof(short));

            if (len <= 0)
            {
                cout << "Deleted or empty record.\n";
                in.close();
                cur = nodes[cur].next;
                continue;
            }

            string record(len, ' ');
            in.read(&record[0], len);
            stringstream ss(record);
            string id1;
            string date1;
            string docid1;
            getline(ss, id1, '|');
            getline(ss, date1, '|');
            getline(ss, docid1, '|');
            string finalrecord=(id==true)?id1+"|":"";
            finalrecord+=(date==true)?date1+"|" : "";
            finalrecord+=(docid==true)?docid1:"";


            cout << "Appointment Record: " << finalrecord << "\n";

            in.close();

            cur = nodes[cur].next;
        }
    }
};
#endif