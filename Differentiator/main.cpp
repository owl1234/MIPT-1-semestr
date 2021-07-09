/*
	Author:
						Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>

    Last edition:
        				July 9, 2021, 23:53:28
    
    What was changed?
						1. The latex file has become more informative.           

	To-do list
						1. Add simplifying the formula in which there is a degree, lg and others
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

					
				    "\\title{Research paper on the topic \"Differentiation of complex functions\"}\n"
			 		"\\author{Kolesnikova Xenia}\n"
					"\\date{July 2021}\n"

					"\\usepackage{natbib}\n"
					"\\usepackage{graphicx}\n"

					"\\begin{document}\n"

					"\\maketitle\n"
					"\\centering\n"
					"\\section{\\Large{Simplification before differentiation}}\n"
			);
    
    fprintf(latex, "\\centering\nWe want to differentiate this expression:\n");
	latex_tree(&tree, latex);

    simplify_expression(&tree, latex);

    draw_tree(&tree);

    fprintf(latex, "\\section{\\Large{Differentiation}}\n");

    derivate_tree(&tree, latex);

    fprintf(latex, "Result:\n");
	latex_tree(&tree, latex);
	
	draw_tree(&tree);

	fprintf(latex, "\\section{\\Large{Simplification after differentiation}}");

	//simplify_expression(&tree, latex);

	latex_tree(&tree, latex);
	
	draw_tree(&tree);	

	fprintf(latex, "\\section{\\Large{References}}\n"

					"\\begin{enumerate} \\item Author's repository - https://github.com/x-ENIAC/MIPT\\underline{ }projects\\underline{ }1\\underline{ }sem/tree/master/Differentiator\n"

									  " \\item Redkozubov's lectures\n"

					"\\end{enumerate}\n"
					
					"\\end{document}\n");

    fclose(latex);

    tree_destruct(&tree);
	
	return TREE_OKEY;
}