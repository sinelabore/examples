# SysML v2 Textual Notation Examples

This folder contains small, self-contained examples written in **SysML v2 textual notation**, intended to be fed into the sinelabore toolchain for **code generation** and **execution/simulation**.

What you will find in each subdirectory:

`*.sysml` models: the SysML v2 textual input.
`main.cpp` and `system.h`: a minimal C++ driver / glue code for running the generated code.
`Makefile` (and sometimes `codegen.cfg`): build configuration to generate and compile the example.

Included examples:

`traffic_light_system`: a simple traffic light control setup (central controller + decentralized controller) and how the generated parts interact.
`tst_action_basic`: a basic SysML v2 **action** example executed from C++.
`tst_if_elseif_else_chain`: decision logic example (if / else-if / else chain).
`tst_while_until`: loop / repetition control-flow example (while / until).

If you just want to start quickly, try running `make` in one of the example subfolders (for example, `traffic_light_system`).


# Code Generation Considerations
The code generator aims to keep the generated code as close as possible to the SysML v2 model, preserving the same structure and intent of the actions and control/data relationships.

Even though this can lead to code that looks verbose, modern optimizing compilers simplify the model-close generated code effectively, so you still get good runtime performance while preserving the original structure.

This can be proven using Compiler Explorer (https://godbolt.org), as shown in the next picture: you can see how few assembly lines are generated from the complete C++ code produced by the `tst_action_basic` example.

![Sinelabore working principle](doc/compiler_explorer.jpg)

Compiler Explorer (https://godbolt.org) is an online playground for C/C++ (and other languages) that lets you compile the same code with many compiler versions and optimization settings. After compilation, it shows what the compiler produces (for example, generated assembly and intermediate representations) so you can directly inspect what optimizations are applied. This is handy to verify that the verbose, model-close generated code still turns into efficient machine code on modern toolchains.