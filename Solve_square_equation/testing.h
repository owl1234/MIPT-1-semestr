#ifndef TESTING_H
#define TESTING_H

/**
*   \brief This function tests function get_solution()
*
*   @param a,b,c Coefficients of the square equation
*   @param right_answer1,right_answer2 Correct roots of the equation
*   @param x1,x2 Pointers to variables that solutions are written to
*   @param number_of_test Test number
*
*   @return Nothing
*/

void test_square_eq_solver(double a, double b, double c, double right_answer1,
                           double right_answer2, double* x1, double* x2, int* number_of_test);

/**
*   \brief This function tests function get_discr()
*
*   @param a,b,c Coefficients of the square equation
*   @param right_answer Correct discriminant
*   @param number_of_test Test number
*
*   @return Nothing
*/

void test_discr(double a, double b, double c, double right_answer, int* number_of_test);

/**
*   \brief This function tests function is_zero()
*
*   @param x The number that you want to compare
*
*   @return True or False (equivalent to 0 or not)

*/


void test_is_zero(double x, bool right_answer, int* number_of_test);

#endif
