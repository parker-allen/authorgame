//
//  main.cpp
//  Author-ID XTRA_CRED
//
//
//  Created by Parker Allen on 5/30/17.
//

#include <iostream>
#include <string>
#include <functional>
#include <unistd.h>
#include "DataBase.h"
using namespace std;

int main(int argc, const char * argv[]) {
   int i, randLineNum;
   string input, testLine, multiLines, author, strBreak = "terminateprogram",
   clear = "makespace";

   DataBaseEntry Dumas("alexandre_dumas.txt", "test_dumas.txt");

   DataBaseEntry Dickens("charles_dickens.txt", "test_dickens.txt");

   DataBaseEntry Austen("jane_austen.txt", "test_austen.txt");

   DataBaseEntry Verne("jules_verne.txt", "test_verne.txt");

   DataBaseEntry Twain("mark_twain.txt", "test_twain.txt");

   DataBaseEntry Stevenson("robert_louis_stevenson.txt", "test_stevenson.txt");

   cout << "To test your own writing, begin typing your own work." << endl;
   cout << "To test some writing from the authors we have set aside to " << \
   "test the program, type the authors name you want to test: dumas" << \
   ", dickens, austen, verne, twain, or stevenson" << endl << endl;

   while(input != strBreak){

      getline(cin, input);
      if(input == strBreak)
         break;
      else if(input == clear)
         cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
      while(cin.get() != '\n'){
         getline(cin, input);
      }

      // if they want to test one of the authors we already have
      if(input == "dumas" || input == "dickens" ||
         input == "austen" || input == "verne" || input == "twain" ||
         input == "stevenson") {

         multiLines.clear();
         testLine.clear();
         randLineNum = rand() % 800;
         input.insert(0, "test_");
         input.append(".txt");
         ifstream infile(input);

         if(!infile)
            cout << "Test file Not Found" << endl;

         for(i = 0; !infile.eof(); i++){
            getline(infile, testLine);
            if(i >= randLineNum && i <= randLineNum + 20){
               multiLines.append(testLine + " ");
            }
         }
         cout << multiLines << endl << endl;
         input = multiLines;
         infile.close();
      }

      // testing verse the different authors
      FindMax DumasProb("Alexandre Dumas", Dumas.createProb(input),
                        Dumas.getPercent());
      FindMax DickensProb("Charles Dickens", Dickens.createProb(input),
                          Dickens.getPercent());
      FindMax AustenProb("Jane Austen", Austen.createProb(input),
                         Austen.getPercent());
      FindMax VerneProb("Jules Verne", Verne.createProb(input),
                        Verne.getPercent());
      FindMax TwainProb("Mark Twain", Twain.createProb(input),
                        Twain.getPercent());
      FindMax StevensonProb("Robert Louis Stevenson",
                            Stevenson.createProb(input),
                            Stevenson.getPercent());

      author = findMaxProb(DumasProb, DickensProb, AustenProb, VerneProb,
                           TwainProb, StevensonProb);

      string comments[] = {"Hmmm, that's a toughie", "This writing stinks",
         "Wow this is interesting...", "Why do you write like this?",
         "Wow you're a great writer", "Yikes", "Oh geez", "This is awesome"};

      string answers[] = {"I'm guessing it's ", "Your writing style resembles ",
         "This text is really close to writing by ", "That sounds a lot like ",
         "This is probably written by "};

      int arrSize = (sizeof(comments) / sizeof(comments[0]));
      int randArrNum = rand() % arrSize;
      string comment = comments[randArrNum];

      arrSize = (sizeof(answers) / sizeof(answers[0]));
      randArrNum = rand() % arrSize;
      string answer = answers[randArrNum];

      cout << endl << comment;
      sleep(1);

      cout << ", let me think" << endl;
      sleep(2);

      if(author == "none"){
         cout << "\nThis doesn't match anybody that I know..." << endl
         << endl << endl;
      }
      else {
         cout << answer << author << endl << endl << endl;
         //            sortResults(DumasProb, DickensProb, AustenProb,
         //                        VerneProb, TwainProb, StevensonProb);
      }


   }

   return 0;
}
