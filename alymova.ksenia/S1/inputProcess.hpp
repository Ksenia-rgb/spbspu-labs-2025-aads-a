#ifndef INPUT_PROCESS_HPP
#define INPUT_PROCESS_HPP
#include <list>
#include <sstream>
#include <cctype>
#include "list.hpp"
#include "iterators.hpp"

namespace alymova
{
  using pair_t = std::pair< std::string, alymova::List< size_t > >;
  using list_pair_t = alymova::List< pair_t >;
  using list_int_t = alymova::List< size_t >;

  void inputProcess(std::istream& in, list_pair_t& list);
  void outputProcess(std::ostream& out, const list_pair_t& list);
  void outputListInt(std::ostream& out, const list_int_t& list);
  void outputListString(std::ostream& out, const list_pair_t& list);
  list_int_t countSums(const list_pair_t& list);
  size_t findMaxListSize(const list_pair_t& list);
  bool isOverflowSumInt(size_t a, size_t b);
}
#endif
