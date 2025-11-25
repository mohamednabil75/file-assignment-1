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
        readFile.read((char*)&offset, sizeof(int));
        while (offset != -1)
        {
            readFile.seekg(offset, ios::beg);
            short length;
            readFile.read((char *)&length, sizeof(short));
            AppAvail a;
            a.offset = offset;
            a.length = length;
            availarray.push_back(a);
            readFile.seekg(offset + sizeof(short), ios::beg);
            readFile.get(); // to skip '*'
            readFile >> offset;
            readFile.close();
        }
    }

    // Read available records for insertion in Doctor file
    void readDocAvail(vector<DocAvail> &availarray, string filename)
    {
        ifstream readFile(filename);
        if (!readFile)
            return;
        int offset;
        readFile.read((char*)&offset, sizeof(int));
        while (offset != -1)
        {

            readFile.seekg(offset, ios::beg);
            short length;
            readFile.read((char *)&length, sizeof(short));
            DocAvail d;
            d.offset = offset;
            d.length = length;
            availarray.push_back(d);
            readFile.seekg(offset + 3 , ios::beg);
            readFile.read((char*)&offset, sizeof(int));

        }
        readFile.close();
    }
};
#endif