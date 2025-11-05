#ifndef READANDWRITE_H
#define READANDWRITE_H
#include <bits/stdc++.h>
using namespace std;
class RAW{
    public:
    struct PIndex {
            int RRN;
            char id[15];

            bool operator<(const PIndex &p) const {
                return strcmp(id, p.id) < 0;
            }
        };
    /**
     *@param primIndexArray have sorted data of file"primary.txt"
     * merge data from parameter vector to file"primary.txt"
     */
    // Write primary index to file
    // Read primary index from file
    void writePrimIndex(vector<PIndex> &primIndexArray,string filename) {
        ofstream outFile(filename);
        for (auto &p : primIndexArray) {
            outFile << p.id << "|" << p.RRN << "\n";
        }
        outFile.close();
    }


    // Read primary index from file
    /**
    *@param primIndexArray empty vector
    * function merge data from "primary.txt" to vector u passed in parameter 
    */
    
    void readPrimIndex(vector<PIndex> &primIndexArray,string filename) {
            ifstream inFile(filename);
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
};
#endif