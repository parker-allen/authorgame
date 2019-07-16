//
//  DataBase.h
//  Process File and Hash it
//
//  Created by Parker Allen on 5/31/17.
//

#ifndef DataBase_h
#define DataBase_h

#include "HashTable.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include <unordered_map>
#include <algorithm>

//DataBaseEntry class declaration ---------------------------
class DataBaseEntry{
public:
    DataBaseEntry(const string &inputFile, const string &outStr);
    
    void clearProb() {totalProb = 0.0;}
    void calcProb();
    double createProb(string line);
    void addProb(const string & word);
    bool insert(const string & word, double num = 1);
    
    //accessors
    int getTotalLine() {return numOfLines;}
    int getTotalWords() {return totalWords;}
    double getProb() {return totalProb;}
    double getPercent() {return percentage;}
    
private:
    unordered_map<string, double> hashEntry;
    void editDBEntry(string line);
    int numOfLines = 0;
    int totalWords;
    double totalProb;
    unsigned long int percentage;
};


//DataBaseEntry method implementation -----------------------
DataBaseEntry::DataBaseEntry(const string &inputFile, const string &outStr){
    string line;
    
    ifstream infile(inputFile);
    ofstream outFile(outStr);
    if(!infile){
       cout << "infile Not Found" << endl;
       return;
    }
   if (!outFile) {
      cout << "outfile not found" << endl;
      return;
   }

    for(int lineNum = 0; !infile.eof(); lineNum++){
        getline(infile, line);

        line.erase(remove(line.begin(), line.end(), '\''), line.end());
        
        if(lineNum % 15 == 0){
            outFile << line << "\n";
        }
        else{
            editDBEntry(line);
        }
    }
    infile.close();
    outFile.close();
    
    calcProb();
}



//a method that goes through each word and cleans it of all special chars
void DataBaseEntry::editDBEntry(string line){
    string proLine;
    int wordStart = 0, wordEnd;
    bool inWord = false;
    
    //check each char keeping it if it's a char a-z
    for(int i = 0; i < line.length(); i++){
        proLine.clear();
        line[i] = tolower(line[i]);
        
        if(isalpha(line[i])){
            if(!inWord){
                wordStart = i;
                inWord = true;
            }
            
        }
        else{
            if(inWord){
                inWord = false;
                wordEnd = i;
                for(; wordStart < wordEnd; wordStart++)
                    proLine += line[wordStart];
                insert(proLine);
            }
        }
    }
    if(inWord){
        for(; wordStart < line.length(); wordStart++)
            proLine += line[wordStart];
        insert(proLine);
    }
    if(numOfLines == 0)
        insert("<UNK>", 0.2);

    numOfLines++;
}

bool DataBaseEntry::insert(const string & word, double num){
    totalWords++;
    unordered_map<string, double>::iterator iter = hashEntry.find(word);
    if(iter != hashEntry.end()){
        hashEntry.at(word) += 1;
        return true;
    }
    else{
        hashEntry.insert(iter, std::pair<string, double>(word, num));
        return true;
    }
}


void DataBaseEntry::calcProb(){
    unordered_map<string, double>::iterator iterBegin = hashEntry.begin();
    unordered_map<string, double>::iterator iterEnd = hashEntry.end();
    for(; iterBegin != iterEnd; iterBegin++)
        hashEntry.at(iterBegin->first) = log(iterBegin->second / totalWords);
    
}

void DataBaseEntry::addProb(const string & word){
    unordered_map<string, double>::iterator iter = hashEntry.find(word);
    if(iter != hashEntry.end()){
        totalProb += hashEntry.at(word);
        percentage += (exp(hashEntry.at(word)) * totalWords);
    }
    
    else{
        totalProb += hashEntry.at("<UNK>");
        percentage += (exp(hashEntry.at("<UNK>")) * totalWords);
    }
}

double DataBaseEntry::createProb(string line){
    clearProb();
    unsigned long int perc;
    percentage = perc;
    string proline;
    int i, wordStart = 0, wordEnd;
    bool inWord = false;
    
    for(i = 0; i < line.length(); i++){
        proline.clear();
        line[i] = tolower(line[i]);
        
        if(isalpha(line[i])){
            if(!inWord){
                wordStart = i;
                inWord = true;
            }
            
        }
        else{
            if(inWord){
                inWord = false;
                wordEnd = i;
                for(; wordStart < wordEnd; wordStart++)
                    proline += line[wordStart];
                addProb(proline);
            }
        }
    }
    if(inWord){
        for(; wordStart < line.length(); wordStart++)
            proline += line[wordStart];
        addProb(proline);
    }
    return getProb();
}



class FindMax{
public:
    FindMax(string auth, double prob, float percent) : author(auth),
        probability(prob), percentage(percent) {};
    string getAuthor() {return author;}
    double getProbability() {return probability;}
    float getPercent() {return percentage;}

protected:
    string author;
    double probability;
    double percentage;
};

string findMaxProb(FindMax compare1, FindMax compare2, FindMax compare3,
                            FindMax compare4, FindMax compare5, FindMax compare6){
    vector<FindMax> vecPtr;
    vecPtr.push_back(compare1);
    vecPtr.push_back(compare2);
    vecPtr.push_back(compare3);
    vecPtr.push_back(compare4);
    vecPtr.push_back(compare5);
    vecPtr.push_back(compare6);

    double maxProbability = vecPtr[0].getProbability();
    
    for(int i = 0; i < vecPtr.size(); i++){
        if(maxProbability == 0)
            maxProbability = vecPtr[i].getProbability();
        if(vecPtr[i].getProbability() > maxProbability &&
           vecPtr[i].getProbability() != 0)
            maxProbability = vecPtr[i].getProbability();
    }
    
    for(int i = 0; i < vecPtr.size(); i++){
        if(maxProbability == 0)
            return "none";
        if(maxProbability == vecPtr[i].getProbability())
            return vecPtr[i].getAuthor();
    }
    return "error occurred";
}


bool compareProbs(FindMax one, FindMax two){
    return (one.getPercent() > two.getPercent());
}

void sortResults(FindMax compare1, FindMax compare2,
                    FindMax compare3, FindMax compare4, FindMax compare5,
                    FindMax compare6){
    vector<FindMax> vecPtr;
    vecPtr.push_back(compare1);
    vecPtr.push_back(compare2);
    vecPtr.push_back(compare3);
    vecPtr.push_back(compare4);
    vecPtr.push_back(compare5);
    vecPtr.push_back(compare6);

    unsigned long int totProb = 0.0;
    long double prob;
    
    sort(vecPtr.begin(), vecPtr.end(), compareProbs);
    
    for(int i = 0; i < vecPtr.size(); i++)
        totProb += vecPtr[i].getPercent();
    
    for(int i = 0; i < vecPtr.size(); i++){
        prob = (vecPtr[i].getPercent() / totProb) * 100;
        cout << vecPtr[i].getAuthor() << " " << prob << "%  Natural Log Probability " << vecPtr[i].getProbability() << endl << endl;
    }
    cout << endl;
}
#endif /* DataBase_h */
