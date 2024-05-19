# Contributing
I'm very happy that you're contributing! There's just a few things I'd like to say.

## Content
I will accept pretty much anything that you write. I would like a detailed description
so that I know what's going on and not bad code to go with it. Definitely include Doxygen
comments in your code. Look at the Style section for how I'd like those written.

## Style
I think that my style is pretty reasonable but in no way am I forcing you to use it.
If it's not exactly to all of this I won't care and if it's really bad I'll just rewrite
it. Thanks for putting up with me.

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
- i'll think of more
