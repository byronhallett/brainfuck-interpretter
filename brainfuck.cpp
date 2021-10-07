#include <iostream>
#include <fstream>
#include <vector>
#include "brainfuck.hpp"

static bool brainfuck::isCommand(char c)
{
  return allCommands.count(c) == 1;
};

brainfuck::Brainfuck::Brainfuck(size_t cellCount)
{
  std::cout << "Brain:" << std::endl;
  commands = std::vector<Command>{};
  commandPtr = nullptr;
  startJmps = std::vector<Command *>{};
  auto count = cellCount == 0 ? CELL_SIZE : cellCount;
  cells = std::vector<Cell>(count);
  cellPtr = nullptr;
}

void brainfuck::Brainfuck::CommandsList()
{
  std::for_each(commands.begin(), commands.end(), [](auto comm)
                { std::cout << char(comm); });
  std::cout << std::endl;
}

int brainfuck::Brainfuck::Run()
{
  std::cout << "Fuck:" << std::endl;
  // initialise pointers
  commandPtr = &commands.front();
  cellPtr = &cells.front();
  int cellID = 0;

  int skipDepth = 0;

  // run until end of program
  while (*commandPtr != 0)
  {

    auto currentCommandPtr = commandPtr++;
    // used to skip until end of loop
    if (skipDepth > 0 && *currentCommandPtr != EndLoop && *currentCommandPtr != StartLoop)
      continue;

    // perform the corresponding operation
    switch (*currentCommandPtr)
    {
    case StartLoop:
      if (skipDepth > 0)
      {
        skipDepth += 1;
        break;
      }
      if (*cellPtr == 0)
      {
        skipDepth += 1; // end of loop count, ignore loop body
      }
      else
      { // loop begin, store jmp point
        startJmps.push_back(currentCommandPtr);
      }
      break;
    case EndLoop:
      if (skipDepth > 0)
      {
        --skipDepth;
        break;
      }
      // deliberately not using currentCommandPtr
      commandPtr = startJmps.back();
      startJmps.pop_back();
      break;
    case PrevCell:
      --cellPtr;
      cellID -= 1;
      break;
    case NextCell:
      ++cellPtr;
      cellID += 1;
      break;
    case DecCell:
      --*cellPtr;
      break;
    case IncCell:
      ++*cellPtr;
      break;
    case Input:
      std::cin >> *cellPtr;
      break;
    case Output:
      std::cout << char(*cellPtr);
      break;
    default:
      break;
    }
  }
  std::cout << std::endl;
  return 0;
}

std::istream &brainfuck::operator>>(std::istream &is, Brainfuck &bf)
{
  while (!is.eof()) //stream whole file
  {
    char c;
    is.get(c);
    if (brainfuck::isCommand(c))
    {
      bf.commands.push_back(Command(c));
    }
  }
  return is;
}

int main(int argc, const char **argv)
{
  const char *filename = nullptr;
  size_t cellCount = 0;
  for (size_t i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-c") == 0) // strings same
    {
      // skip to option value
      cellCount = atol(argv[++i]);
      continue;
    }
    // not an option, use as filename
    filename = argv[i];
  }
  if (filename == nullptr)
  {
    std::cerr << "Usage: " << argv[0] << " [-c number_of_cells] filename.bf" << std::endl;
    return 1;
  }

  // input guard

  // open stream to supplied file
  auto stream = std::ifstream();
  stream.open(filename);

  // parse stream with Brainfuck object
  brainfuck::Brainfuck bf{cellCount};
  stream >> bf;
  bf.CommandsList();

  // run commands
  auto result = bf.Run();

  // tidy up
  stream.close();
  return result;
}
