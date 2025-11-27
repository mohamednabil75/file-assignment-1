#ifndef READANDWRITE_H
#define READANDWRITE_H
#include <bits/stdc++.h>
using namespace std;
class RAW
{
public:
    struct PIndex
    {
        int RRN;
        char id[15];

        bool operator<(const PIndex &p) const
        {
            int p1 = stoi(id);
            int p2 = stoi(p.id);
            return p2 > p1;
        }
    };
    struct DocAvail
    {
        int offset;
        int length;
        bool operator<(const DocAvail &d) const
        {
            return length < d.length;
        }
    };
    struct AppAvail
    {
        int offset;
        int length;
        bool operator<(const AppAvail &a) const
        {
            return length < a.length;
        }
    };
    struct DocSecIndex
    {
        char doctorName[30];
        int head;
    };

    struct DocSecNode
    {
        char doctorID[15];
        int next;
    };
    struct AppSecIndex
    {
        char doctorID[15]; // Secondary key
        int head;          // pointer to linked list head
    };

    struct AppSecNode
    {
        char appointmentID[15];
        int next;
    };

    /**
     *@param primIndexArray have sorted data of file"primary.txt"
     * merge data from parameter vector to file"primary.txt"
     */
    // Write primary index to file
    // Read primary index from file
    void writePrimIndex(vector<PIndex> &primIndexArray, string filename)
    {
        ofstream outFile(filename);
        for (auto &p : primIndexArray)
        {
            outFile << p.id << "|" << p.RRN << "\n";
        }
        outFile.close();
    }

    // Read primary index from file
    /**
     *@param primIndexArray empty vector
     * function merge data from "primary.txt" to vector u passed in parameter
     */

    void readPrimIndex(vector<PIndex> &primIndexArray, string filename)
    {
        ifstream inFile(filename);
        if (!inFile)
            return;
        string line;
        while (getline(inFile, line))
        {
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

    // Print available records for insertion in Doctor file
    void printDocAvail(vector<DocAvail> &availarray)
    {
        sort(availarray.begin(), availarray.end());
        if (availarray.size() == 0)
        {
            cout << "No available records\n";
            return;
        }
        cout << "Available records offsets:\n";
        cout << "[";
        int size_counter = availarray.size() - 1;
        for (auto &d : availarray)
        {
            if (size_counter != 0)
            {
                cout << "(" << d.offset << ":" << d.length << ")" << ", ";
                size_counter--;
                continue;
            }
            if (d.offset < 0)
                continue; // to avoid printing -1 at the end
            cout << "(" << d.offset << ":" << d.length << ")" << "]\n";
        }
    }

    // Print available records for insertion in Appointment file
    void printAppAvail(vector<AppAvail> &availarray)
    {
        sort(availarray.begin(), availarray.end());
        if (availarray.size() == 0)
        {
            cout << "No available records\n";
            return;
        }
        cout << "Available records offsets:\n";
        cout << "[";
        int size_counter = availarray.size() - 1;
        for (auto &a : availarray)
        {
            if (size_counter != 0)
            {
                cout << "(" << a.offset << ":" << a.length << ")" << ", ";
                continue;
            }
            size_counter--;
            cout << "(" << a.offset << ":" << a.length << ")" << "]\n";
        }
    }

    // Read available records for insertion in Appointment file
    void readAppAvail(vector<AppAvail> &availarray, string filename)
    {
        ifstream readFile(filename);
        if (!readFile)
            return;
        int offset;
        readFile.read((char *)&offset, sizeof(int));
        while (offset != -1)
        {
            readFile.seekg(offset, ios::beg);
            short length;
            readFile.read((char *)&length, sizeof(short));
            AppAvail a;
            a.offset = offset;
            a.length = length;
            availarray.push_back(a);
            readFile.seekg(offset + 3, ios::beg);
            readFile.read((char *)&offset, sizeof(int));
        }
        readFile.close();
    }

    // Read available records for insertion in Doctor file
    void readDocAvail(vector<DocAvail> &availarray, string filename)
    {
        ifstream readFile(filename);
        if (!readFile)
            return;
        int offset;
        readFile.read((char *)&offset, sizeof(int));
        while (offset != -1)
        {

            readFile.seekg(offset, ios::beg);
            short length;
            readFile.read((char *)&length, sizeof(short));
            DocAvail d;
            d.offset = offset;
            d.length = length;
            availarray.push_back(d);
            readFile.seekg(offset + 3, ios::beg);
            readFile.read((char *)&offset, sizeof(int));
        }
        readFile.close();
    }
    void writeDocSecondaryIndex(vector<DocSecIndex> &secIndex, string filename)
    {
        ofstream out(filename);
        for (auto &entry : secIndex)
        {
            out << entry.doctorName << "|" << entry.head << "\n";
        }
        out.close();
    }
    void readDocSecondaryIndex(vector<DocSecIndex> &secIndex, string filename)
    {
        ifstream in(filename);
        if (!in)
            return;

        string line;
        while (getline(in, line))
        {
            stringstream ss(line);
            string name, headStr;

            getline(ss, name, '|');
            getline(ss, headStr, '|');

            DocSecIndex e;
            strcpy(e.doctorName, name.c_str());
            e.head = stoi(headStr);

            secIndex.push_back(e);
        }
        in.close();
    }
    void writeDocSecondaryList(vector<DocSecNode> &nodes, string filename)
    {
        ofstream out(filename);
        for (auto &n : nodes)
        {
            out << n.doctorID << "|" << n.next << "\n";
        }
        out.close();
    }
    void readDocSecondaryList(vector<DocSecNode> &nodes, string filename)
    {
        ifstream in(filename);
        if (!in)
            return;

        string line;
        while (getline(in, line))
        {
            stringstream ss(line);
            string idStr, nextStr;

            getline(ss, idStr, '|');
            getline(ss, nextStr, '|');

            DocSecNode node;
            strcpy(node.doctorID, idStr.c_str());
            node.next = stoi(nextStr);

            nodes.push_back(node);
        }
        in.close();
    }
    int binarySearchDocName(const vector<DocSecIndex> &secIndex, const string &name)
    {
        int l = 0, r = secIndex.size() - 1;
        while (l <= r)
        {
            int mid = (l + r) / 2;
            int cmp = strcmp(secIndex[mid].doctorName, name.c_str());

            if (cmp == 0)
                return mid;

            if (cmp < 0)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return -1;
    }
    void insertIntoDocSecondary(string doctorName, string doctorID,
                                string secIndexFile = "Doctors_Secondary.txt",
                                string secListFile = "Doctors_Secondary_List.txt")
    {
        vector<DocSecIndex> secIndex;
        vector<DocSecNode> nodes;

        readDocSecondaryIndex(secIndex, secIndexFile);
        readDocSecondaryList(nodes, secListFile);
        /* sort(secIndex.begin(), secIndex.end(),
             [](const DocSecIndex &a, const DocSecIndex &b)
             {
                 return strcmp(a.doctorName, b.doctorName) < 0;
             }); */

        int pos = binarySearchDocName(secIndex, doctorName);

        DocSecNode newNode;
        strcpy(newNode.doctorID, doctorID.c_str());
        newNode.next = -1;

        int newNodeIndex = nodes.size();

        nodes.push_back(newNode);

        if (pos == -1)
        {
            cout << "=====================================\n";
            DocSecIndex entry;
            strcpy(entry.doctorName, doctorName.c_str());
            entry.head = newNodeIndex;

            secIndex.push_back(entry);

            sort(secIndex.begin(), secIndex.end(),
                 [](const DocSecIndex &a, const DocSecIndex &b)
                 {
                     return strcmp(a.doctorName, b.doctorName) < 0;
                 });

            writeDocSecondaryIndex(secIndex, secIndexFile);
            writeDocSecondaryList(nodes, secListFile);
            return;
        }
        newNode.next = secIndex[pos].head;
        nodes[newNodeIndex] = newNode;
        secIndex[pos].head = newNodeIndex;
        writeDocSecondaryIndex(secIndex, secIndexFile);
        writeDocSecondaryList(nodes, secListFile);
    }
    void updateDocSecondary(string oldName, string newName, string doctorID,
                            string secIndexFile = "Doctors_Secondary.txt",
                            string secListFile = "Doctors_Secondary_List.txt")
    {
        vector<DocSecIndex> secIndex;
        vector<DocSecNode> nodes;
        readDocSecondaryIndex(secIndex, secIndexFile);
        readDocSecondaryList(nodes, secListFile);
        int pos = binarySearchDocName(secIndex, oldName);
        if (pos == -1)
        {
            cout << "Old Name not found in secondary index.\n";
            return;
        }

        int head = secIndex[pos].head;
        int prev = -1;
        int cur = head;

        while (cur != -1)
        {
            if (strcmp(nodes[cur].doctorID, doctorID.c_str()) == 0)
            {
                if (prev == -1)
                {
                    secIndex[pos].head = nodes[cur].next;
                }
                else
                {
                    nodes[prev].next = nodes[cur].next;
                }
                break;
            }

            prev = cur;
            cur = nodes[cur].next;
        }

        if (secIndex[pos].head == -1)
        {
            secIndex.erase(secIndex.begin() + pos);
        }

        writeDocSecondaryIndex(secIndex, secIndexFile);
        writeDocSecondaryList(nodes, secListFile);

        insertIntoDocSecondary(newName, doctorID, secIndexFile, secListFile);

        // cout << "Doctor name updated in secondary index.\n";
    }
    void readAppSecondaryIndex(vector<AppSecIndex> &secIndex,
                               string filename = "Appointments_Secondary.txt")
    {
        ifstream in(filename);
        if (!in)
            return;

        string line;
        while (getline(in, line))
        {
            stringstream ss(line);
            string docID, headStr;

            getline(ss, docID, '|');
            getline(ss, headStr, '|');

            AppSecIndex e;
            strcpy(e.doctorID, docID.c_str());
            e.head = stoi(headStr);

            secIndex.push_back(e);
        }
        in.close();
    }
    void writeAppSecondaryIndex(vector<AppSecIndex> &secIndex, string filename = "Appointments_Secondary.txt")
    {
        ofstream out(filename);

        for (auto &e : secIndex)
        {
            out << e.doctorID << "|" << e.head << "\n";
        }

        out.close();
    }
    void readAppSecondaryList(vector<AppSecNode> &nodes, string filename = "Appointments_Secondary_List.txt")
    {
        ifstream in(filename);
        if (!in)
            return;

        string line;
        while (getline(in, line))
        {
            stringstream ss(line);
            string appID, nextStr;

            getline(ss, appID, '|');
            getline(ss, nextStr, '|');

            AppSecNode node;
            strcpy(node.appointmentID, appID.c_str());
            node.next = stoi(nextStr);

            nodes.push_back(node);
        }
        in.close();
    }
    void writeAppSecondaryList(vector<AppSecNode> &nodes, string filename = "Appointments_Secondary_List.txt")
    {
        ofstream out(filename);

        for (auto &n : nodes)
        {
            out << n.appointmentID << "|" << n.next << "\n";
        }

        out.close();
    }
    int binarySearchAppDoctorID(const vector<AppSecIndex> &secIndex, const string &doctorID)
    {
        int l = 0, r = secIndex.size() - 1;

        while (l <= r)
        {
            int mid = (l + r) / 2;
            int cmp = strcmp(secIndex[mid].doctorID, doctorID.c_str());

            if (cmp == 0)
                return mid;
            if (cmp < 0)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return -1;
    }
    void insertIntoAppSecondary(string doctorID, string appointmentID,
                                string indexFile = "Appointments_Secondary.txt",
                                string listFile = "Appointments_Secondary_List.txt")
    {
        vector<AppSecIndex> secIndex;
        vector<AppSecNode> nodes;

        readAppSecondaryIndex(secIndex, indexFile);
        readAppSecondaryList(nodes, listFile);

        int pos = binarySearchAppDoctorID(secIndex, doctorID);

        AppSecNode newNode;
        strcpy(newNode.appointmentID, appointmentID.c_str());
        newNode.next = -1;

        int newNodeIndex = nodes.size();
        nodes.push_back(newNode);

        if (pos == -1)
        {
            AppSecIndex entry;
            strcpy(entry.doctorID, doctorID.c_str());
            entry.head = newNodeIndex;

            secIndex.push_back(entry);

            sort(secIndex.begin(), secIndex.end(),
                 [](const AppSecIndex &a, const AppSecIndex &b)
                 {
                     return strcmp(a.doctorID, b.doctorID) < 0;
                 });

            writeAppSecondaryIndex(secIndex, indexFile);
            writeAppSecondaryList(nodes, listFile);
            return;
        }

        newNode.next = secIndex[pos].head;
        nodes[newNodeIndex] = newNode;
        secIndex[pos].head = newNodeIndex;

        writeAppSecondaryIndex(secIndex, indexFile);
        writeAppSecondaryList(nodes, listFile);
    }
};
#endif