/*
  ==============================================================================

    Helpers.cpp
    Created: 4 Mar 2023 1:02:18am
    Author:  Candidate No. EX2765

  ==============================================================================
*/

#include "Helpers.h"

std::vector<std::string> Helpers::tokenise(std::string csvLine, char separator)
{
   std::vector<std::string> tokens;

   signed int start, end;
   std::string token;

   start = csvLine.find_first_not_of(separator, 0);

   do {
      end = csvLine.find_first_of(separator, start);
      if (start == csvLine.length() || start == end) break;
      if (end >= 0) token = csvLine.substr(start, end - start);
      else token = csvLine.substr(start, csvLine.length() - start);
      tokens.push_back(token);
      start = end + 1;
   } while (end != std::string::npos);

   return tokens;

};

std::string Helpers::convertTimeInSecondsToMMSS(int seconds)
{
   int min = seconds / 60;        // get the minutes
   int sec = seconds % 60; // get the remaining seconds
   std::string secs = "00";
   if (sec < 10)
   {
      secs = "0" + std::to_string(sec);
   }
   else
   {
      secs= std::to_string(sec);
   }
   std::string result = std::to_string(min) + ":" + secs; // concatenate the strings with a colon
   return result;
}