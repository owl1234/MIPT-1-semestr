#include <stdio.h>
#include "testing.h"

void testing() {
    double x1 = 0, x2 = 0;
    int number_of_test = 1;
    printf("Begin testing the function get_solution\n");
    test_square_eq_solver(1, 2, 1, -1, 0, &x1, &x2, &number_of_test);
    test_square_eq_solver(1, -2, 1, 1, 0, &x1, &x2, &number_of_test);
    test_square_eq_solver(1, -3, 2, 1, 2, &x1, &x2, &number_of_test);
    test_square_eq_solver(5, 1, 5, 0, 0, &x1, &x2, &number_of_test);
    test_square_eq_solver(0, 0, 0, 0, 0, &x1, &x2, &number_of_test);
    test_square_eq_solver(0, 0, 9, 0, 0, &x1, &x2, &number_of_test);
    test_square_eq_solver(0, 8, 0, 0, 0, &x1, &x2, &number_of_test);

    printf("\n\nBegin testing the function that calculates discriminant\n");
    test_discr(1, 2, 3, -8, &number_of_test);
    test_discr(1, 2, 1, 0, &number_of_test);
    test_discr(1, 3, 2, 1, &number_of_test);

    printf("\n\nBegin testing the function that compares the number with 0\n");
    test_is_zero(1, false, &number_of_test);
    test_is_zero(0, true, &number_of_test);
    test_is_zero(1e-9, true, &number_of_test);
}
