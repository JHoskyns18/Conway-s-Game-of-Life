# Conway's Game of Life

This project is an implementation of Conway's Game of Life, a classic example of cellular automata, written in C. It simulates the evolution of a grid of cells based on a set of simple rules, producing complex and often surprising patterns.

Each cell in a 2D grid can be either alive or dead. The grid evolves generation to generation according to the following rules:

- Underpopulation: A live cell with fewer than two live neighbors dies.
- Survival: A live cell with two or three live neighbors lives on to the next generation.
- Overpopulation: A live cell with more than three live neighbors dies.
- Reproduction: A dead cell with exactly three live neighbors becomes alive.

The project went out of its way to demonstate the aspects of C we learned during the course. For example, it makes extensive use of pointers and double pointers for dynamic memory management and efficient manipulation of 2D arrays.

This project was developed as part of the module 'Scientific Programming in C' during the second year of my undergraduate degree at the University of Exeter, for which I achieved a grade of 92%.
