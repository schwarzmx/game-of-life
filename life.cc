#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using std::cout;
using std::endl;
using std::string;
using std::system;
using std::vector;
using std::out_of_range;

typedef vector<char> row;
typedef vector<row> matrix;
typedef vector<char>::iterator col_iterator;
typedef vector<row>::iterator row_iterator;
typedef unsigned int u_int;

char live = '+';
char dead = ' ';

void copy_grid(matrix &old_grid, matrix &new_grid) {
  system("clear");
  for (u_int row = 0; row < old_grid.size(); ++row) {
    for (u_int column = 0; column < old_grid[row].size(); ++column) {
      char current = old_grid[row][column];
      new_grid[row][column] = current;
      cout << current;
    }
    cout << endl;
  }
}

bool alive(u_int row, u_int column, matrix &grid) {
  try {
    return grid.at(row).at(column) == live;
  } catch (out_of_range& e) {
    return false;
  }
}

void update(u_int row, u_int column, matrix &old_grid, matrix &new_grid) {
  bool current = alive(row, column, old_grid);                      // current cell
  int alive_neighbors = 0;
  alive_neighbors += alive(row - 1, column - 1, old_grid) ? 1 : 0;  // up left
  alive_neighbors += alive(row - 1, column, old_grid) ? 1 : 0;      // up center
  alive_neighbors += alive(row - 1, column + 1, old_grid) ? 1 : 0;  // up right
  alive_neighbors += alive(row, column - 1, old_grid) ? 1 : 0;      // center left
  alive_neighbors += alive(row, column + 1, old_grid) ? 1 : 0;      // center right
  alive_neighbors += alive(row + 1, column - 1, old_grid) ? 1 : 0;  // down left
  alive_neighbors += alive(row + 1, column, old_grid) ? 1 : 0;      // down center
  alive_neighbors += alive(row + 1, column + 1, old_grid) ? 1 : 0;  // down right

  if (current && alive_neighbors < 2) {
    // rule 1: Any live cell with fewer than two live neighbours dies, as if
    //    caused by under-population.
    new_grid[row][column] = dead;
  } else if (current && alive_neighbors >= 2 && alive_neighbors <= 3) {
    // rule 2: Any live cell with two or three live neighbours lives on to the
    //    next generation.
    new_grid[row][column] = live;
  } else if (current && alive_neighbors > 3) {
    // rule 3: Any live cell with more than three live neighbours dies, as if
    //    by overcrowding.
    new_grid[row][column] = dead;
  } else if (!current && alive_neighbors == 3) {
    // rule 4: Any dead cell with exactly three live neighbours becomes a live
    //    cell, as if by reproduction.
    new_grid[row][column] = live;
  } else {
    // no rule, leave as is
    new_grid[row][column] = old_grid[row][column];
  }
}

int main() {
  const int num_rows = 35;
  const int num_cols = 50;
  matrix grid(num_rows, num_cols);

  // initialize grid
  // TODO: instead of just random, create cool figures
  for (row_iterator row = grid.begin(); row != grid.end(); ++row) {
    for (col_iterator column = row->begin(); column != row->end(); ++column) {
      if ( (rand() % 100 > 60)) {
        *column = live;
      }
      else {
        *column = dead;
      }
    }
  }
  // main loop
  matrix new_grid = matrix(num_rows, num_cols);
  while (true) {
    sleep(1);
    for (u_int row = 0; row < grid.size(); ++row) {
      for (u_int column = 0; column < grid[row].size(); ++column) {
        update(row, column, grid, new_grid);
      }
    }
    copy_grid(new_grid, grid);
  }
  return 0;
}
