#ifndef QUERY_H
#define QUERY_H

#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
using namespace std;

#include "doctor.h"
#include "ReadandWrite.h"
#include "search.h"

RAW readAwrite;
searchdoc searchdoc;
searchapp searchapp;


static inline string ltrim(const string &s) {
    size_t i = s.find_first_not_of(" \t\n\r");
    return (i == string::npos) ? "" : s.substr(i);
}
static inline string rtrim(const string &s) {
    size_t i = s.find_last_not_of(" \t\n\r");
    return (i == string::npos) ? "" : s.substr(0, i + 1);
}
static inline string trim(const string &s) {
    return rtrim(ltrim(s));
}

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string temp;
    stringstream ss(s);
    while (getline(ss, temp, delimiter)) {
        string t = trim(temp);
        if (!t.empty()) tokens.push_back(t);
    }
    return tokens;
}

class query {
    vector<vector<string>> tables{
        {"doctor", "id", "name", "address"},
        {"appointment", "id", "date", "docid"},
    };

public:
    bool checkTables(const vector<string> &tableNames) {
        if (tableNames.empty()) return false;
        for (const string &name : tableNames) {
            auto it = find_if(tables.begin(), tables.end(),
                              [&](const vector<string> &t) { return t[0] == name; });
            if (it == tables.end()) {
                cout << "Invalid table name: " << name << endl;
                return false;
            }
        }
        return true;
    }

    bool checkColumns(const vector<string> &tableNames, const vector<string> &columnNames) {
        if (columnNames.empty()) return false;
        if (columnNames.size() == 1 && columnNames[0] == "all") return true;

        for (const string &col : columnNames) {
            bool found = false;
            for (const string &TN : tableNames) {
                auto it = find_if(tables.begin(), tables.end(),
                                  [&](const vector<string> &t) { return t[0] == TN; });
                if (it != tables.end()) {
                    for (size_t i = 1; i < it->size(); ++i) {
                        if ((*it)[i] == col) {
                            found = true;
                            break;
                        }
                    }
                }
                if (found) break;
            }
            if (!found) {
                cout << "Invalid column: " << col << endl;
                return false;
            }
        }
        return true;
    }

    bool getvarcondition(const string &query, string &tableOut, string &varname, string &operat, string &value) {
        int whereWord = query.find("where");
        if (whereWord == string::npos) return false;

        string condition = trim(query.substr(whereWord + 5));

        vector<string> ops = {"<=", ">=", "<>", "=", "<", ">"};
        size_t operatpos = string::npos;

        for (const string &op : ops) {
            size_t pos = condition.find(op);
            if (pos != string::npos) {
                operat = op;
                operatpos = pos;
                break;
            }
        }

        if (operatpos == string::npos) {
            cout << "Invalid WHERE condition." << endl;
            return false;
        }

        string left = trim(condition.substr(0, operatpos));
        value = trim(condition.substr(operatpos + operat.size()));

        stringstream ss(left);
        if (!(ss >> tableOut >> varname)) {
            ss.clear();
            ss.str(left);
            if (!(ss >> varname)) {
                cout << "Invalid condition format." << endl;
                return false;
            }
            tableOut = "";
        }

        if (value.size() >= 2) {
            if ((value.front() == '\'' && value.back() == '\'') ||
                (value.front() == '"' && value.back() == '"'))
                value = value.substr(1, value.size() - 2);
        }

        return true;
    }

    vector<string> getColumns(const string &query) {
        size_t fromWord = query.find("from");
        size_t selectWord = query.find("select");
        if (fromWord == string::npos || selectWord == string::npos) return {};
        size_t start = selectWord + 6;
        string columnsRange = query.substr(start, fromWord - start);
        return split(columnsRange, ',');
    }

    vector<string> getTables(const string &query) {
        size_t fromWord = query.find("from");
        if (fromWord == string::npos) return {};
        size_t whereWord = query.find("where", fromWord + 4);
        size_t start = fromWord + 4;
        string tablesRange = (whereWord == string::npos)
                                 ? query.substr(start)
                                 : query.substr(start, whereWord - start);
        return split(tablesRange, ',');
    }

    bool getrecords(const string &query) {
        string condTable, varname, operat, value;
        bool hasWhere = (query.find("where") != string::npos);

        if (hasWhere) {
            if (!getvarcondition(query, condTable, varname, operat, value))
                return false;
        }

        vector<string> columns = getColumns(query);
        vector<string> tablesInQuery = getTables(query);

        if (!checkTables(tablesInQuery)) return false;
        if (!checkColumns(tablesInQuery, columns)) return false;

        string targetTable = condTable.empty()
                                 ? (tablesInQuery.empty() ? "" : tablesInQuery[0])
                                 : condTable;

        if (targetTable == "doctor" || targetTable == "appointment") {
            vector<RAW::PIndex> all;

            string indexFile = (targetTable == "doctor") ? "primary.txt" : "PrimaryAppointment.txt";
            readAwrite.readPrimIndex(all, indexFile);

            bool validColumn = false;
            for (auto &t : tables) {
                if (t[0] == targetTable) {
                    for (size_t i = 1; i < t.size(); i++) {
                        if (t[i] == varname) validColumn = true;
                    }
                }
            }
            if (!validColumn && find(columns.begin(),columns.end(),"all")==columns.end() ) {
                return false;
            }
            if (varname=="id") {
                if (targetTable == "doctor") {
                    bool namefound ;
                    bool idfound ;
                    bool addressfound ;

                    if (find(columns.begin(),columns.end(),"all")!=columns.end()) {
                        addressfound=true;
                        idfound=true;
                        namefound=true;
                    }
                    else {
                        namefound=find(columns.begin(),columns.end(),"name")!=columns.end();
                        idfound=find(columns.begin(),columns.end(),"id")!=columns.end();
                        addressfound=find(columns.begin(),columns.end(),"address")!=columns.end();
                    }

                    searchdoc.searchDoctorById(value.c_str(),idfound,namefound,addressfound);
                }
            }
                if (targetTable == "appointment") {
                    bool docidfound ;
                    bool idfound ;
                    bool datefound ;

                    if (find(columns.begin(),columns.end(),"all")!=columns.end()) {
                        docidfound=true;
                        idfound=true;
                        datefound=true;
                    }
                    else {
                        docidfound=find(columns.begin(),columns.end(),"docid")!=columns.end();
                        idfound=find(columns.begin(),columns.end(),"id")!=columns.end();
                        datefound=find(columns.begin(),columns.end(),"date")!=columns.end();
                    }

                    searchapp.searchAppointmentById(value.c_str(),idfound,docidfound,datefound);
                }






            }
                if (varname=="docid"&&targetTable == "appointment") {
                    bool docidfound ;
                    bool idfound ;
                    bool datefound ;

                    if (find(columns.begin(),columns.end(),"all")!=columns.end()) {
                        docidfound=true;
                        idfound=true;
                        datefound=true;
                    }
                    else {
                        docidfound=find(columns.begin(),columns.end(),"docid")!=columns.end();
                        idfound=find(columns.begin(),columns.end(),"id")!=columns.end();
                        datefound=find(columns.begin(),columns.end(),"date")!=columns.end();
                    }

                    searchapp.searchByDoctorID(value,idfound,docidfound,datefound);
                }
            if (varname=="name"&&targetTable == "doctor") {
                bool namefound ;
                bool idfound ;
                bool addressfound ;

                if (find(columns.begin(),columns.end(),"all")!=columns.end()) {
                    addressfound=true;
                    idfound=true;
                    namefound=true;
                }
                else {
                    namefound=find(columns.begin(),columns.end(),"name")!=columns.end();
                    idfound=find(columns.begin(),columns.end(),"id")!=columns.end();
                    addressfound=find(columns.begin(),columns.end(),"address")!=columns.end();
                }

                searchdoc.searchByDoctorName(value,idfound,namefound,addressfound);
            }

                    return true;
            }







    void parse(string queryStr) {
        string q = queryStr;
        for (char &c : q) c = tolower(c);

        if (q.find("select") == 0) {
            getrecords(q);
        } else {
            cout << "Only SELECT queries are supported." << endl;
        }
    }
};

#endif
