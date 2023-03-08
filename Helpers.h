/*
  ==============================================================================

    Helpers.h
    Created: 4 Mar 2023 1:02:18am
    Author:  krzys

  ==============================================================================
*/

#pragma 
#include <vector>
#include <string>
namespace Helpers 
{
   /** INPUT string separated by commas and returns vector of strings -- this is the code used in the MerkelRex project in earilier in this OOP module*/
   std::vector<std::string> tokenise(std::string csvLine, char separator);
}