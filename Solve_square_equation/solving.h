#ifndef SOLVING_H
#define SOLVING_H

/**
 * \brief A necessary constant for its convenient use
 */

const double epsilon = 1e-7;

/**
*   \brief This function compares a number with 0
*
*   @param x The number that you want to compare
*
*   @return True or False (equivalent to 0 or not)
*/

bool is_zero(double x);

/**
*   \brief This function calculates discriminant of the
*   square equation
*
*   @param a,b,c Coefficients of the square equation
*
*   @return Discriminant your square equation
*/

double get_discr(double a, double b, double c);

/**
*   \brief This function calculates solutions of the
*   square equation
*
*   @param a,b,c Coefficients of the square equation
*   @param x1,x2 Pointers to variables that solutions are written to
*
*   @return Number of solutions.
*   If the discriminant is less than 0, the solutions not written to a variables.
*   If the discriminant is 0, the solution is written to a variable x1.
*   If the discriminant is greater than 0, the solution is written to a
*   variables x1 and x2
*/

int get_solution(double a, double b, double c, double* x1, double* x2);

/**
*   \brief This function starst solving a quadratic equation
*
*   @return Nothing
*/

void solving();
/*
/**
*   \brief This is the main function in the program
*
*   @return 0, if all is well


int main();*/

#endif
