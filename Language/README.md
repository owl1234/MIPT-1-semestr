This is my realization of compiler my own C-like language. It processes a program written in the language invented by me and generates assembly code suitable for my CPU.

This work consists of several stages:
1. The program text is split into tokens.
2. An array of tokens is converted into an AST (abstract syntax tree) by the recursive descent algorithm. At this stage compilation errors are found (for example, use of a previously not declared variable).
3. The AST is compiled into executable code. Compilation errors are also found here.

Let's take a look at [this](https://github.com/x-ENIAC/MIPT_projects_1_sem/blob/master/Language/example/program.txtg) simple example. The tree of this program:

![program_tree_example](https://github.com/x-ENIAC/MIPT_projects_1_sem/blob/master/Language/example/tree.png)

[Assembly](https://github.com/owl1234/MIPT_projects_1_sem/blob/master/Language/pictures/asm_example.png) and [binary](https://github.com/owl1234/MIPT_projects_1_sem/blob/master/Language/pictures/binary_example.png) code.

### Syntax
*Variable declaration* is `Introducing a new word into the Newspeak Dictionary:`     </br>
*If* is `if Big Brother didn't notice that <cond> then calmly exhale and do`         </br>
*Else* is `otherwise you will be taken to room 101`                                  </br>
*Print* is `Display the face of Big Brother on the telescreen with the inscription:` </br>
