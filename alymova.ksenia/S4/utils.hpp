#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <functional>
#include <map>

namespace alymova
{
  using Dataset_t = std::map< size_t, std::string, std::less< size_t > >;
  using CompositeDataset_t = std::map< std::string, Dataset_t, std::less< std::string > >;
  using CommandDataset_t = std::map<
    std::string,
    std::function< void(CompositeDataset_t&) >,
    std::less< std::string >
  >;

  struct PrintCommand
  {
    std::istream& in;
    std::ostream& out;
    void operator()(const CompositeDataset_t& dicts);
  };
  struct ComplementCommand
  {
    std::istream& in;
    void operator()(CompositeDataset_t& dicts);
  };
  struct IntersectCommand
  {
    std::istream& in;
    void operator()(CompositeDataset_t& dicts);
  };
  struct UnionCommand
  {
    std::istream& in;
    void operator()(CompositeDataset_t& dicts);
  };

  CompositeDataset_t readDictionaryFile(std::istream& in);
  CommandDataset_t complectCommands();
}

#endif
