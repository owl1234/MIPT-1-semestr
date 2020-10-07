#include <stdio.h>
#include "solving.h"
#include "runs_tests.h"

int main() {
    printf("To solve a quadratic equation, write 1, else 0: ");

    int flag = 0;
    scanf("%d", &flag);

    if(flag) {
        printf("Begin the solving..........\n");
        solving();
    } else {
        printf("Begin the testing..........\n");
        testing();
    }

    return 0;
}
