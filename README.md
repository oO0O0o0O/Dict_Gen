Dict Gen in a password dictionary generator for personalized wordlist attacks. It accepts a list of keywords related to a target,
and performs operations to generate a list of possible passwords. The main operations available are:

    Combination: Keywords are combined together with delimeters to create new keywords.
    Substitution: Characters are swapped for homographs.
    Appending: Characters are appended/prepended to keywords.

Keywords are ordered by number of operations performed, such that less modified passwords are listed first. The objective of this program is not to always generate the correct password,
it is to leverage relevant information to generate passwords with the highest likelihood of being correct, and to present these passwords
in order of likelihood.

Usage:

    dict_gen.exe [options] [keywords]

    If a keywords are not provided, will read from stdin until EOF.

Options:

    -c *mode*, --case-mode *mode*
        Specifies the type of case changing to perform. *Lower* means all output is lowercase. *Camel* adds a first letter uppercase version of each keyword. 
        *Upper* adds an uppercase version of each keyword. *Lite* performs camel and upper operations. *Full* does full recusive backtracking for all combinations
        of character capitalizations. Default is *lower*.

    -s *mode*, --substitution-mode *mode*
        Specifies the type of substitution to perform using a built-in table. *None* means no substituions are performed. *Common* means 
        only the most common substitutions are performed. *Shuffle* means all possible combinations of common substitutions are performed,
        including characters not being substituted. *All* means all combinations of valid substitutions are performed. Default is *Common*.

    -cl *length*, --combination-length *length*
        Specifies the maximum number of keywords to include in a combination. Must be postive. One indicates no combinations should be
        performed. Default is *2*.
    
    -cd *depth*, --combination-depth *depth*
        Specifies how many delimeters to use. *Common* means only the most common delimeters. *Most* means all punctation. 
        *All* means every character. Default is *Common*.
    
    -al *length*, --append-length *length*
        Specifies the maximum number of characters to append/prepend. Must be positive. Zero indicates no appending/prepending. Default is *3*.
    
    -am *mode*, --append-mode *mode*
        Specifies the content to append/prepend. *Common* means only common characters/strings. *Special* means only numbers and special characters.
        *All* means all characters. Default is *Common*.
    
    -at *type*, --append-type *type*
        Specifies whether to append/prepend/both. Use with *append*, *prepend*, or *both*. Default is *Append*.
    
    -x, --extreme
        Uses the max for all options and 10 for combination-length and append-length. Very resource intensive - you should probably just not use this.

Order of Operations:
1.  Combination
2.  Case Changing
3.  Substitution
4.  Appending

Combination Details:
-   Settings: length, depth
-   Follows max-combination length (default 2)
-   Grouped list of demonitators used
-   Depths (default Common)
    -   Common: common delimeters used
    -   Most: all punctuation used
    -   All: all characters used

Case Changing Details:
-   Settings: mode
-   Keywords are case insensitive, always set to lowercase
-   Modes (Default Lower)
    - Lower: everything kept at lowercase
    - Camel: add first letter uppercase version of each keyword
    - Upper: add full uppercase version of each keyword
    - Lite: perform camel and upper
    - Full: full recusive backtracking for capitilizing

Substitution Details:
-   Settings: mode
-   Utilizes premade character mappings
-   Default is homoglyphs
-   Utilizes recursive backtracking for All and Deep modes
-   Modes (Default Shuffle)
    -   Common: only most common substitutions are made
    -   Shuffle: all combinations of most common substitutions are made; characters do not have to be substituted
    -   All: all possible substitution combinations are made; characters that can be substituted, are
-   Planned features
    -   Add support for string substitutions

Appending Details:
-   Settings: length, mode, type
-   Follows max-append length (default 3)
-   Uses different premade wordlists
-   Uses type to specify append, prepend, both, or none (default Append)
-   Uses mode to specify content to include (default Common)
-   Uses recursive backtracking
-   Modes (Default Common)
    -   Common: only common characters/strings are included
    -   Special: only numbers and special characters are included
    -   All: all characters are included
-   Types (Default Append)
    -   Append: only add to end
    -   Prepend: only add to beginning
    -   Both: add to end and beginning

Substitutions File Format:

    a4
    4a
    0o
    o0

-   In character followed by out character
-   Each pair on separate line
-   No spaces, punctuation, etc. between characters

Future:
-   Add support for string substitutions
-   Add cracking/guessing when a password is generated
-   Add support for shuffled delimeters when combining
-   Extend camel case to combined keywords (perform before/during combination? check for delimeters?)
-   Identify and fix memory leaks
-   Improve error logs