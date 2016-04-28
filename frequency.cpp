/*!
 * \file        naivefrequency.cpp
 * \brief       Program for frequency count of SVO{N} files.
 * \author      Diorge Brognara
 * \date        2016
 * \copyright   Copyright (C) Diorge Brognara 2016. All rights MIT Licensed.
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <sstream>
#include <vector>
#include <stdexcept>


/*! \var    typedef std::pair<std::string, std::string> TKey
 *  \brief  (S, O) key used in the mapping
*/
typedef std::pair<std::string, std::string> TKey;

/*! \var    typedef std::pair<TKey, int> TPair
    \brief  type_value used in the mapping
*/
typedef std::pair<TKey, int> TPair;

//! Data in each row in the SVO{N} file
struct SvoRow {
   std::string s;
   std::string v;
   std::string o;
   int n;
};


//! Definition of custom hash for std::pair<T1, T2>
struct hashPair {
   template <class T1, class T2>
   std::size_t operator () (const std::pair<T1, T2> &p) const {
      auto value = 0x345678;
      auto h1 = std::hash<T1>{}(p.first);
      auto h2 = std::hash<T2>{}(p.second);
      value = (100003 * value) ^ h1;
      value = (100003 * value) ^ h2;
      return value;
   }
};


//! Compares two TPair by greater integer value
bool compareTuples(TPair a, TPair b) {
   return a.second > b.second;
}


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


   std::unordered_map<TKey, int, hashPair> pairs;
   std::ios_base::sync_with_stdio(false);

   while (std::cin.peek() != std::char_traits<char>::eof()) {
      SvoRow row;
      std::getline(std::cin, row.s, '\t');
      std::getline(std::cin, row.v, '\t');
      std::getline(std::cin, row.o, '\t');
      std::string tempString;
      std::getline(std::cin, tempString);
      row.n = std::stoi(tempString);
      pairs[std::make_pair(row.s, row.o)] += row.n;
   }

   pairCount = std::min(pairCount, (int)pairs.size());
   std::vector<TPair> ordered;
   ordered.resize(pairCount);

   std::partial_sort_copy(pairs.begin(), pairs.end(),
                     ordered.begin(), ordered.end(), compareTuples);

   for (auto &elem : ordered) {
      std::cout << "(" << elem.first.first << ", " << elem.first.second
                  << ") = " << elem.second << "\n";
   }
}
