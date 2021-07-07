/*
	Author:
						Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>

    Last edition:
        				July 7, 2021, 09:06:34
    
    What was changed?
                        1. Differentiation of fractions is done

	To-do list
						1. Add other basic trigonometric functions: tg, ctg.
						2. Add inverse trigonometric functions: arcsin, arccos, arctg, arcctg (possibly)
						3. Add lg
						4. Add simplifying the formula in which there is a degree
*/

#include <stdio.h>
#include "parsing.h"
#include "make_latex.h"
#include "simplify_expression.h"
#include "take_derivate.h"

#define CHECK_STATUS(status)								\
	if(!(status == TREE_OKEY || status == NODE_OKEY))		\
		printf("awful!!!\n");

int main() {
	Tree tree = {};
	ERRORS_TYPE status = tree_construct(&tree);
	CHECK_STATUS(status)

	tree_fill(&tree, "file.txt");

	draw_tree(&tree);

	FILE* latex = fopen("derivate.tex", "wb");
	fprintf(latex, "\\documentclass{article}\n"
				   "\\usepackage[utf8]{inputenc}\n"

					
				    "\\title{k}\n"
			 		"\\author{heiduk.k.k.s }\n"
					"\\date{April 2021}\n"

					"\\usepackage{natbib}\n"
					"\\usepackage{graphicx}\n"

					"\\begin{document}\n"

					"\\maketitle\n"
			);
    
    fprintf(latex, "\\centering\nWe want to differentiate this expression:\n");
	latex_tree(&tree, latex);

    simplify_expression(&tree, latex);

    draw_tree(&tree);

    derivate_tree(&tree, latex);

    fprintf(latex, "\\centering\nResult:\n");
	latex_tree(&tree, latex);
	
	draw_tree(&tree);

	simplify_expression(&tree, latex);

	latex_tree(&tree, latex);
	
	draw_tree(&tree);	

    fprintf(latex, "\\end{document}\n");

    fclose(latex);

    tree_destruct(&tree);
	
	return TREE_OKEY;
}