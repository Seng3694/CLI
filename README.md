# Lightweight CLI Parser #
A CLI parser which is purely written in C and focused to be lightweight. Can be used in C++ applications too.

## How to use ##

```c
//include the library
#include <cli.h>

int main(int argc, char** argv)
{
    //create the CLI struct
    //the integral value is the count of arguments you want to define
    CLI* cli = CLI_Create(3);

    //add 3 arguments:
    //if you add more arguments than you've defined in the create method, 
    //the program will probably crash. The information is known before 
    //compile time, so make sure to doublecheck.
    CLI_AddArgument(cli, 'h', ARG_TYPE_COMMAND);
    CLI_AddArgument(cli, 'v', ARG_TYPE_REQUIRED);
    CLI_AddArgument(cli, 'r', ARG_TYPE_OPTION);
    
    //parse cli args
    cli_errors errors = CLI_Parse(cli, argc, argv);

    if(errors != 0)
    {
        //do your error checking here. As the name indicates, the value 
        //could have more than one error. You can use the "HAS_FLAG" macro
    }

    //if the 'h' command was set
    if(CLI_OptionSet(cli, 'h'))
    {
        printf("Some useful help text to print to the user\n");
        return 0;
    }

    //initialize char* which will point to the value string
    //you should copy the value if you want to persist it
    char* value = NULL;
    if(CLI_TryGetArgument(cli, 'v', &value))
    {
        //this section will only be reached if value was set successfully
        //if 'r' was set, reverse the value
        if(CLI_OptionSet(cli, 'r'))
            printf("%s", strrev(value));
        else
            printf("%s", value);
    }

    //clean up allocated commands and value strings
    CLI_Destroy(cli);
    return 0;
}
```

## Limitations ##
To reduce any overhead there are some limitations:
- Commands do start with a hyphen "-"
- Commands are only one character long "a"
- There are no automatic value conversions
- The count of commands should be known before compile time => no dynamic resize. So if you add your fifth command but you only initialized the CLI struct with four, you will experience unexpected behavior.

## Examples ##
- Example for [C][2] 
- Example for [C++][3]
- And a few [tests][4]

## Build ##

Project uses [CMake][1] to generate platform and compiler-specific build files.

### Building on Windows (Visual Studio) ###
1. Clone the repository
    ```
    git clone https://github.com/Seng3694/CLI
    ```

2. Generate the build files (depending on your Visual Studio Version the `cmake` command differs. In this case `"Visual Studio 15"` will generate the Visual Studio 2017 project files)
    ```
    mkdir bin
    cd bin
    cmake -G"Visual Studio 15" ../CLI
    ```

3. You can either open the `.sln` solution file and build the `ALL_BUILD` target or just do it with cmake:
    ```
    cmake --build . --config Release
    ```

## License ##
This library is licensed under the MIT License. See [LICENSE][5] for more information.

[1]:http://www.cmake.org/
[2]:examples/c/main.c
[3]:examples/cpp/main.cpp
[4]:tests/main.c
[5]:LICENSE