/*
  ==============================================================================

    Helpers.cpp
    Created: 4 Mar 2023 1:02:18am
    Author:  krzys

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