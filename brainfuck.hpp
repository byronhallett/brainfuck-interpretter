#ifndef BRAINFUCK_H
#define BRAINFUCK_H
#include <vector>
#include <unordered_set>
#include <fstream>

namespace brainfuck
{
  static const size_t CELL_SIZE = 30000;
  // all command symbols
  enum Command : char
  {
    StartLoop = '[',
    EndLoop = ']',
    PrevCell = '<',
    NextCell = '>',
    DecCell = '-',
    IncCell = '+',
    Input = ',',
    Output = '.'
  };

  // Allow iterating over commands
  static const std::unordered_set<char> allCommands = {StartLoop, EndLoop, PrevCell, NextCell, DecCell, IncCell, Input, Output};
  // allow checking for command membership
  static bool isCommand(char);

  // cells are type byte
  typedef __UINT8_TYPE__ Cell;


  class Brainfuck
  {

  public:
    Brainfuck(size_t);
    void CommandsList();
    int Run();
    void AddCommand(char);

  private:
    // core brainfuck members
    std::vector<Command> commands;
    Command *commandPtr;
    std::vector<Cell> cells;
    Cell *cellPtr;

    // implementation specific members
    std::vector<Command*> startJmps;


    // load from filestream
    friend std::istream &operator>>(std::istream &is, Brainfuck &bf);
  };

} // namespace brainfuck

#endif
