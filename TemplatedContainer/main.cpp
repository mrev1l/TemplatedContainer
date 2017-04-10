#include "stdafx.h"
#include "CArray\CArray.h"

#include <functional> 
#include <algorithm>  
#include <random>
#include <iostream>
#include <string>

typedef std::vector<char> char_array;
static char_array charset()
{
  return char_array(
  {
    'a','b','c','d','e','f',
    'g','h','i','j','k',
    'l','m','n','o','p',
    'q','r','s','t','u',
    'v','w','x','y','z'
  });
};

static std::string random_string(
    const size_t                    _length, 
    const std::function<char(void)> _rand_char
  )
{
  std::string str(_length, 0);
  std::generate_n(str.begin(), _length, _rand_char);
  return str;
}

template <typename TData>
static void PrintArray(
    const structures::CArray<TData> & _array, 
    const char                              _delim
  )
{
  std::cout << "Array's size = " << _array.size() << std::endl;
  std::cout << "Array's capacity = " << _array.capacity() << std::endl;
  for (unsigned int i = 0; i < _array.size(); i++)
    std::cout << _array[i] << _delim;
  std::cout << std::endl;
}

int main()
{
  std::default_random_engine randEngine(std::random_device{}());

  const auto ch_set = charset();

  std::uniform_int_distribution<> dist (0, ch_set.size() - 1);

  auto randchar = [ch_set, &dist, &randEngine]()
  {
    return ch_set[dist(randEngine)];
  };

  structures::CArray<int> intArray;

  std::uniform_int_distribution<> valueRange(0, 100);

  // 1. Working with integer values
  std::cout << "Adding 20 random values into array \n";

  for (unsigned int i = 0; i < 20; i++)
    intArray.push_back(valueRange(randEngine));
  PrintArray(intArray, ' ');

  std::cout << "Sorting the array in ascending order \n";
  std::sort(
      intArray.begin(),
      intArray.end(),
      [&](const int & a, const int & b)
      {
        return a < b;
      });  
  PrintArray(intArray, ' ');

  std::cout << "Deleting even elments \n";
  for (int i = intArray.size() - 1; i >= 0; i -= 2)
    intArray.erase(i);
  PrintArray(intArray, ' ');

  std::cout << "Adding random value on random positions \n";
  for (int i = 0; i < 10; i++)
  {
    std::uniform_int_distribution<> posRange(0, intArray.size());
    intArray.insert(posRange(randEngine), valueRange(randEngine));
  }
  PrintArray(intArray, ' ');

  std::cout << "Clearing the array \n";
  intArray.clear();
  PrintArray(intArray, ' ');

  // 2. Working with strings
  std::cout << "Adding 15 random strings \n";
  structures::CArray<std::string>  stringArray;
  for (unsigned int i = 0; i < 15; i++)
    stringArray.push_back(random_string(rand() % 26 + 1, randchar));
  PrintArray(stringArray, '\n');
  
  std::cout << "Sorting the array \n";
  // Sorting in an alphabetical order
  std::sort(stringArray.begin(), stringArray.end(),
    [&](const std::string & a, const std::string & b)
    {
      for (unsigned int i = 0; i < a.size() &&
          i < b.size(); i++)
      {
        if (a[i] < b[i])
          return true;
        if (a[i] > b[i])
          return false;
      }
      return a.size() < b.size();
    });
  PrintArray(stringArray, '\n');
  
  std::cout << "Erasing strings with letters \'a\', \'b\', \'c\', \'d\', \'e\' \n";
  for (unsigned int i = 0; i < stringArray.size(); i++)
  {
    auto a = stringArray[i].find('a', 0);
    auto b = stringArray[i].find('b', 0);
    auto c = stringArray[i].find('c', 0);
    auto d = stringArray[i].find('d', 0);
    auto e = stringArray[i].find('e', 0);
  
    if (a != -1 ||
        b != -1 ||
        c != -1 ||
        d != -1 ||
        e != -1)
    {
      stringArray.erase(i);
      i--;
    }
  }
  PrintArray(stringArray, '\n');

  std::cout << "Adding random strings on random positions \n";
  for (unsigned int i = 0; i < 3; i++)
  {
    const int idx = rand() % stringArray.size();
    stringArray.insert(idx, random_string(rand() % 26 + 1, randchar));
  }
  PrintArray(stringArray, '\n');

  std::getchar();
  return 0;
}