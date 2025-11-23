#ifndef query_H
#define query_H
using namespace std;
#include "doctor.h"
#include <bits/stdc++.h>
#include <iostream>
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != string::npos) {
        string word=s.substr(start, end - start);
        word.erase(remove(word.begin(), word.end(), ' '), word.end());
        tokens.push_back(word);
        start = end + 1;
        end = s.find(delimiter, start);
    }
    string word=s.substr(start);
    word.erase(remove(word.begin(), word.end(), ' '), word.end());
    tokens.push_back(word); // last token
    return tokens;
}
class query{
    string condition;
    vector<vector<string>>tables { 
     {"doctor","id","name","address"} ,
     {"appointment","id","date","docid"},
    };

    public:
     bool checkTables(const vector<string> &tableNames) {
        if(tableNames.size()==0)return false;
        for (const string &name : tableNames) {
            auto it = find_if(tables.begin(), tables.end(),
                              [&](const vector<string> &t) {
                                  return  t[0] == name;
                              });

            if (it == tables.end()) {
                cout<<"wrong tables entered"<<endl;
                return false; 
            }
        }
        return true; 
    }

/*     bool checkCoulmns(const vector<string> tablename,const vector<string> &coulmnNames){
        if(coulmnNames.size()==0)return false;
        if(coulmnNames.size()==1&&coulmnNames[0]=="*")return true;

        for (const string &name : coulmnNames) {
            auto it = find_if(tables.begin(), tables.end(),
                              [&](const vector<string> &t) {
                                for(const string TN:tablename )
                                if(t[0]==TN){
                                    for(int i=1;i<t.size();i++){
                                        if(t[i]==name)return true;
                                    }
                              }});

            if (it == tables.end()) {
                cout<<"wrong coulmn entered"<<endl;
                return false; 
            }
        }
        return true; 

    }; */
    vector<string> getCoulmns(string query){
        int fromWord=query.find("from");
        int whereWord=query.find("where");
        string coulmnsRange=query.substr(6,fromWord-6);
        vector<string> coulmns= split(coulmnsRange,',');
        return coulmns;
    }
      vector<string> getTables(string query){
        int fromWord=query.find("from");
        int whereWord=query.find("where");
        string tablesRange=query.substr(fromWord+4,whereWord-fromWord);
        vector<string> coulmns= split(tablesRange,',');
        return coulmns;
    }
    void parse(string query){
        if(query.find("select")==0){
        vector<string> coulmns= getCoulmns(query);
        vector<string> tables= getTables(query);
        // if(checkCoulmns(tables,coulmns)&&checkTables(tables))cout<<"parsing"<<endl;
        // for(int i=0;i<tables.size();i++)cout<<tables[i]<<endl;
        // checkTables(tables);
        // checkCoulmns(tables,coulmns);
        // for(int i=0;i<coulmns.size();i++)cout<<coulmns[i]<<endl;
        // for(int i=0;i<tables.size();i++)cout<<tables[i]<<endl;





    }

    }
}
;
#endif