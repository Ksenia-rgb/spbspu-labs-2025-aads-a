#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <map>
#include <functional>
#include "utils.hpp"
#include "tree-2-3.hpp"
#include "tree-node.hpp"
#include "tree-iterators.hpp"

int main(int argc, char** argv)
{
  using namespace alymova;
  using Dataset_t = std::map< size_t, std::string, std::less< size_t > >;
  using CompositeDataset_t = std::map< std::string, Dataset_t, std::less< std::string > >;
  using CommandDataset_t = std::map<
    std::string,
    std::function< void(CompositeDataset_t&) >,
    std::less< std::string >
  >;

  if (argc != 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "<INCORRECT FILE>\n";
    return 1;
  }

  try
  {
    CompositeDataset_t dataset_comp = readDictionaryFile(file);
    file.clear();

    CommandDataset_t dataset_commands = complectCommands();
    std::string command;
    while ((std::cin >> command))
    {
      dataset_commands.at(command)(dataset_comp);
      if (command == "print")
      {
        std::cout << '\n';
      }
    }
  }
  catch(const std::exception& e)
  {
    std::cout << "<INVALID COMMAND>\n";
  }
  /*TwoThreeTree< int, std::string, std::less< int > > tree;
  std::pair< int, std::string > p1(1, "cat");
  std::pair< int, std::string > p2(2, "dog");
  std::pair< int, std::string > p3(3, "fly");
  std::pair< int, std::string > p4(4, "boo");
  std::pair< int, std::string > p5(5, "grd");
  std::pair< int, std::string > p6(6, "bread");
  std::pair< int, std::string > p7(10, "kitten");
  tree.insert(p1);
  tree.insert(p2);
  tree.insert(p3);
  tree.insert(p4);
  tree.insert(p5);
  tree.insert(p6);
  tree.insert(p7);

  {
    TwoThreeTree< int, char, std::less< int > > map;
    auto it = map.cbegin();
    for (int i = 5; i > 0; --i)
    {
      map.emplace_hint(it, i, 'd');
      it = map.cend();
    }
    std::cout << map.size();
    for (auto it = map.begin(); it != map.end(); it++)
    {
      std::cout << it->first << it->second << '\n';
    }
  }*/

}
