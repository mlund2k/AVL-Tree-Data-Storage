# AVL-Tree-Data-Storage


This project utilizes a self created AVL Tree class data structure to store and sort large data.


This program was compiled using WSL (Windows Subsystem for Linux) on Windows 10 and 11 and runs perfectly in such environments.


**Steps to run:**
- Copy files into one directory
- Set the location of the files as your directory within your terminal of choice
- Execute ```make``` to compile the project
- The following steps are optional, and can be used to create custom data for testing the AVL tree class:
    - Execute ```./createdata.exe numOps minSize (d|D)(m|M)``` to create test data to use in the following program. Specify both a number of operations to create and a minimum tree size in the respective arguments, as well as specifying the optional arguments listed below:
        - d disables delete operations
        - D enables delete operations
        - m disables deleteMin operations
        - M enables deleteMin operations
        - For example, ```./createdata.exe 10 5 DM```
    - The output can be copied into a json for later use, alternatively examples are provided in the project.
- Execute ```./AVLcommands.exe insertexample.json``` to read in a json input file of commands and store the properly formatted data to a new AVL tree data structure. The newly formed tree is output in JSON format and can then be copied to a .json if desired.


If compiling raises a fatal error on ```#include <json/json.h>```, make sure to run the following commands to install json on your computer:
```sudo apt-get install libjsoncpp-dev```
```sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json```
Credits: https://github.com/stardust95/TinyCompiler/issues/2#issuecomment-410946235
