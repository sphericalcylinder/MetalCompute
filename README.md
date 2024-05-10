# easy-metal-cpp
A C++ wrapper for the Apple metal-cpp library to make it easier to run compute kernels on the GPU

# Usage
It's not built yet so you can't use it


# About
I recently created a C++ application and wanted to use Apple's metal-cpp library to add gpu
proccessing capability, but I found out that it was hard to do. It eventually worked, but I don't want
to deal with that again, which is why I'm creating this. It's March 8 right now and I'm not
going to do any work until probably July, August, or September. I'm just getting this out
there right now so I can have a solid start.


## Goals
The goals for this project (which will probably change) are as follows:

- Concise and easy to read code
- A working API (duh)
- Minimal overhead
- Extensive documentation (probably Doxygen)
- and more!! (i cant think)

## Choices
I have to make some choices for the format of this project such as:

- OOP or functional (SFML vs OpenGL)
- more but that's it for now


## Style (I'm picky)
I have a very defined style and will fight people over it. If you want to contribute but can't stand my requirements, just submit it as-is and I will modify it to match.
Here's the code style requirements for pull requests or changes:

- 4 spaces for indentation
- Braces don't start on a new line
```cpp
int main()
{ // << NOT THIS
    return 0;
}

int main() { // << Yes this
    return 0;
}
```
- JavaDoc C-style type documentation (without autobrief or banner)
```cpp
int variable = 2; ///< inline variable comments and function brief comments like this

/// out of line like this
char a = 'a';

/**
 * don't use the first line ^
 * 
 * use the '@' symbol not \
 * < put the stars there
 *
 * @param[in] please always use param and return
 * for functions
 * @return thank you
 *
 * always one extra line before the end of the comment
 * 
*/
// The star of the ending bit should not have a space before it
```
- Reasonable line length (most cases under 100 characters long)
- Reasonable spacing between code sections
- Spaces before and after = (assignment) `int x = 3` not `int x=3`
- Spaces after the comma in explicitly defined arrays and vectors `int a[3] = [1, 2, 3];` (some cases not doing this is ok)
- Use the std namespace (it's ok if you don't do this one i'll fix it later)
- Spaces between commas seperating parameters in function calls `func(a, b, c);` not `func(a,b,c)` unless it's an emergency
- i'll think of more when I see ugly code

# Contributing

If you haven't read the last section, read it.
Feel free to contribute to this project! I'm not a god at programming and will make mistakes, we all do. If you want to add a feature, submit a pull request or issue and I'll review it (hopefully). There's not much else to say really.

# Issues

I'm not a GitHub Repository Manager<sup>TM</sup> and don't have a solid issue format. Just state the issue, provide error messages if applicable, and do other stuff. Basically be reasonable.


### The end :)
