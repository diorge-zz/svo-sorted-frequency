/*!
 * \file        naivefrequency.cpp
 * \brief       Program for frequency count of SVO{N} files.
 * \author      Diorge Brognara
 * \date        2016
 * \copyright   Copyright (C) Diorge Brognara 2016. All rights MIT Licensed.
*/

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <set>


//! Data in each row in the SVO{N} file
struct SvoRow {
   std::string s;
   std::string v;
   std::string o;
   int n;
};


typedef std::pair<std::string, size_t> TPair;

using PriorityType = std::tuple<std::string, std::string, size_t>;


struct sndgt {
    template <class T1, class T2, class T3>
    bool operator () (const std::tuple<T1, T2, T3> &p1,
                             const std::tuple<T1, T2, T3> &p2) const {
        return std::get<2>(p1) > std::get<2>(p2);
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

   int pairCount;
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

   auto sndgtr =
       [] (const PriorityType& t1, const PriorityType& t2)
       { return std::get<2>(t1) > std::get<2>(t2); };

   std::ios_base::sync_with_stdio(false);
   std::string currentSubject = "";
   std::unordered_map<std::string, size_t> current;
   std::set<PriorityType, decltype(sndgtr)> total(sndgtr);


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
        size_t n = std::min(total.size(), (size_t)pairCount);
        std::set<PriorityType, decltype(sndgtr)>::iterator nth = total.cbegin();
        for (size_t i = 0; i < n; i++)
            ++nth;
        std::set<PriorityType, decltype(sndgtr)>::iterator last = total.cend();
        total.erase(nth, last);
      }
      current[row.o] += row.n;
      currentSubject = row.s;
   }

   for (auto &v : total) {
       std::cout << "(" << std::get<0>(v) << ", " << std::get<1>(v) << ") = " << std::get<2>(v) << "\n";
   }
}
