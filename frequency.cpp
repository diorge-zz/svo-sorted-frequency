/*!
 * \file        frequency.cpp
 * \brief       Program for frequency count of sorted SVO{N} files.
 * \author      Diorge Brognara
 * \date        2016
 * \copyright   Copyright (C) Diorge Brognara 2016. All rights MIT Licensed.
*/

#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>


//! Data in each row in the SVO{N} file
struct SvoRow {
   std::string s;
   std::string v;
   std::string o;
   int n;
};


//! Triple (Subject, Object, Occurrences) for sorted counting
using sonTriple = std::tuple<std::string, std::string, long int>;


//! Compares two sonTriples, used for sorting by highest N
struct higherNfirst {
    bool operator () (const sonTriple &t1, const sonTriple &t2) const {
        return std::get<2>(t1) > std::get<2>(t2);
    }
};


//! Prints usage message
void usage(const char* progName) {
      std::cerr << "usage: " << progName << " pairCount\n"
                << "\tpairCount: amount of top frequent pairs to search (positive integer)\n";
}


//! SVO{N} frequency counter program
/*! 
 * \param argc Must be at least 2
 * \param argv Second element (first argument) must be a positive integer, other elements are discarded
 SVO input is expected on stdin. Output is on stdout.
*/
int main(int argc, char** argv) {
   if (argc < 2) {
      usage(argv[0]);
      return -1;
   }

   long int pairCount;
   try {
      pairCount = std::stoi(argv[1]);
      if (pairCount <= 0) {
         usage(argv[0]);
         return -1;
      }
   } catch (const std::invalid_argument& ia) {
      usage(argv[0]);
      return -1;
   } catch (const std::out_of_range& oor) {
      usage(argv[0]);
      return -1;
   }


   std::ios_base::sync_with_stdio(false);
   std::string currentSubject = "";
   std::unordered_map<std::string, size_t> current;
   std::set<sonTriple, higherNfirst> total;


   while (std::cin.peek() != std::char_traits<char>::eof()) {
      SvoRow row;
      std::getline(std::cin, row.s, '\t');
      std::getline(std::cin, row.v, '\t');
      std::getline(std::cin, row.o, '\t');
      std::string tempString;
      std::getline(std::cin, tempString);
      row.n = std::stoi(tempString);

      if (row.s != currentSubject) {
        for (auto &elem : current) {
            total.insert(std::make_tuple(currentSubject, elem.first, elem.second));
        }
        current.clear();
        long int n = std::min((long int)total.size(), pairCount);
        std::set<sonTriple, higherNfirst>::iterator nth = total.cbegin();
        for (long int i = 0; i < n; i++)
            ++nth;
        std::set<sonTriple, higherNfirst>::iterator last = total.cend();
        total.erase(nth, last);
      }
      current[row.o] += row.n;
      currentSubject = row.s;
   }

   for (auto &v : total) {
       std::cout << "(" << std::get<0>(v) << ", " << std::get<1>(v) << ") = " << std::get<2>(v) << "\n";
   }
}
