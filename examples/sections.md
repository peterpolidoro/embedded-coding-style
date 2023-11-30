<p align="center">
<a href="README.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION2.md"><b>NEXT&nbsp;»</b></a>
</p>

# 1. White Space
Many software developers pay little attention to the white space in their source code, such as spaces, tabs, new-lines, etc. The common thinking is that compilers (C, C++, etc.) ignore white space anyway, so why bother? But, as a *professional* software developer you should not ignore white space, because it can cause all sorts of problems, some of them illustrated in the figure below:

<p align="center">
<img src="img/whitespacey.png" title="Examples of problematic white space in source code">
</p>
<p align="center">
<em>Examples of problematic white space in source code</em>
</p>

The guidelines in this category are intended for avoiding difficult to see errors and consistent code rendering on a wide variety of devices and platforms like: different screens, printers, and various code editors working on different computer platforms. These guidelines should apply to all ASCII documents, not only strictly to source code.

> **NOTE:**<br>
The problems caused by white space in the source code are particularly insidious, because you don't see the culprit. Also, inconsistent use of white space can lead to source files that are supposed to be identical might have different size on disk and can show as different in various version control systems. The strict control over the software versions is critical for any safety-certification and consequently inconsistencies in white space are unacceptable.


## 1.1 No Trailing White Space
**Amplification**<br>
The source code should have no trailing spaces or tabs after the last printable character in each line.

**Rationale**<br>
Trailing white space after the last printable character in line can cause bugs. For example, trailing white space after the C/C++ macro-continuation character '\' can confuse the C pre-processor and can result in a program error, as indicated by the bug icons.


## 1.2 End-of-Line Convention
**Amplification**<br>
All source code should consistently use only one end-of-line (EOL) convention as opposed to some lines ending with a different EOL convention than the others. For improved portability, this Coding Style recommends consistent use of the Unix end-of-line convention, with only one LF character (0x0A) terminating each line. Specifically, the DOS/Windows end-of-line convention with CR, LF character pair (0x0D, 0x0A) terminating each line is not recommended.

**Rationale**<br>
Inconsisten End-of-Line convention might cause compilation problems on Unix-like systems, such as Linux. (Specifically, the C preprocessor doesn’t correctly parse the multi-line macros.) On the other hand, most DOS/Windows compilers seem to tolerate the Unix EOL convention without problems.


## 1.3 No Tabs
**Amplification**<br>
No tab characters (0x9) are allowed anywhere in the source code. Gropus of spaces should be used instead of the tabs.

**Rationale**<br>
Tabs are rendered differently on different devices and bring only insignificant memory savings. Preferably, tabs should be disabled at the editor level. At the very least, they should be replaced by spaces before saving the file.

## 1.4 Line Width Limit
**Amplification**<br>
Lines of code or comments should never exceed **80** columns

**Rationale**<br>
In programming practice, the source code is very often copied-and-pasted and then modified, rather than created from scratch. For this to work effectively, it’s very advantageous to be able to see simultaneously and *side-by-side* both the original and the modified copy, without line wrapping that destroys the logical layout.

Also, differencing the code is a routinely performed action of any VCS (Version Control System) whenever you check-in or merge the code. Limiting the line widths allows to use the horizontal screen real estate much more efficiently for side-by-side-oriented text windows instead of much less convenient and error-prone top-to-bottom differencing.

<p align="center">
<img src="img/line-wrap.jpg" title="Example of code differencing with long lines wrapping">
</p>
<p align="center">
<em>Example of code differencing with long lines wrapping</em>
</p>

> **Note**<br>
In this guideline the specific line width limit (80 in this case) is not of the key property. The really important is the consensus to set a limit for the length of lines.    


# Enforcement
To enforce the guidelines in this category it is highly recommended to use an **automated** white space cleanup utility to check and cleanup the source code after editing or, at the very least, before checking it into a version control system. Quantum Leaps provides such a cleanup utility called **[QClean](https://www.state-machine.com/qtools/qclean.html)**.

<p align="center">
<a href="README.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION2.md"><b>NEXT&nbsp;»</b></a>
</p>
<p align="center">
<a href="SECTION1.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION3.md"><b>NEXT&nbsp;»</b></a>
</p>

# 2. C/C++ Layout
The fundamental principle underlying the guidelines in this section is that good layout needs to show the _structure_ of the code clearly and accurately. If one technique shows the structure better and the other simply looks better, the one that represents the structure is preferred. Additionally, the preference is for layouts that can prevent errors due to misinterpretation as well as those that are more robust in view of likely changes, even if the layouts might not look as "neat" as the alternatives.

# 2.1 Pure-Block Layout
The general layout of C/C++ code shall be based on the _pure-block_ layout [[CODE2]](README.md#07-references)), which looks as follows:
```
<declarator | control-statement> {
    <statement>;
    . . .
}
```
where  `<declarator | control-statement>` may be one of the following: 
- function declarator
- structure/class declarator
- enumeration
- structure/array initializer
- control-statement (`if`, `else`, `while`, `do`, `for`, `switch` and `case`)

**Rationale**<br>
The pure-block layout has three main benefits:
- it clearly shows the code structure;
- it eliminates possible misinterpretation with incorrect scope of the nested statements or other blocks within a block;
- it is robust with respect of future modifications, such as adding or removing nested blocks.

Variations of the pure-block layout are used in several other coding styles ([K&R:78], [PPP:11], [FDS:11])

The concrete examples of this layout will be provided in the following sub-sections, but the main point is that _all_ elements in the source code are formatted _consistently_ according to the same pure-block layout.


# 2.2 Use of Braces
The statement following a `<control-statement>` (statement beginning with: `if`, `else`, `while`, `do`, `for`, `switch` and `case`) must be compound, that is, use of braces (`{...}`) is **obligatory**.

This requirement corresponds to MISRA-C rule 59. Although not strictly required by the C/C++ syntax, this rule is imposed to remove any doubts as far as statement nesting is concerned, and to allow easy addition/removal of nested statements with minimal differences showing when differencing the code using a VCS. For example, consider adding a statement to a while loop:

```
/* not a compound statement—NOT RECOMMENDED */    /* compound statement-RECOMMENDED */
while (i > 0)                                     while (i > 0) {
    *t++ = *s++;                                      *t++ = *s++;
                                                  }
```

after modification:

```
/* not a compound statement—NOT RECOMMENDED */    /* compound statement-RECOMMENDED */
while (i > 0)                                     while (i > 0) {
    *t++ = *s++;                                      *t++ = *s++;
    --i;                                              --i;
                                                  }
```

With the not-compound statement case you either make a mistake by forgetting the braces (although the indentation clearly indicates the intention), or you must introduce the braces, which then show as a big difference when merging the code or checking it into the VCS.
If the `<declarator | control-statement>` is so complex that it cannot fit in one line, then it should be formatted in the following manner: 

```
<declarator | control-statement ...>
    <... declarator | control-statement ...>
    <... declarator | control-statement>
{
    <statement>;
    . . .
}
```

The arguments of a declarator should be split into separate lines leaving separating comma ‘,’ as the last character in the line. The condition of the control statement should be split into separate lines starting new lines with a binary operator. This formatting intends to avoid confusing continuation of the control statement with its body. For example:

```
if (!decodeData(&s, &dataType, 
        &format, &version)
    && (format != 2U)    /* expecting only format 2 */
    && (version != 1U))  /* expecting only version 1 */
{
    log("data corrupt");
}
```

### 3.2.1 The `if` Statement

```
if (x < 0) {
    z = 25; 
} 
if (--cnt == 0) { 
    z = 10; 
    cnt = 1000; 
}
else {
    z = 200; 
}
if (x > y) { 
    foo(x, y); 
    z = 100; 
}
else {
    if (y < x) { 
        foo(y, x); 
        z = 200; 
    } 
    else { 
        x = 0; 
        y = 0; 
    } 
}
```

### 3.2.2 The `for` Statement
```
for (i = 0; i < MAX_ITER; ++i) {
    *p2++ = *p1++;
    xx[i] = 0; 
}
```

### 3.2.3 The `while` Statement
```
while (--ctr != 0) {
    *p2++ = *p1++; 
    *p3++ = 0; 
}
```

### 3.2.4 The `do..while` Statement
```
do { 
    --ctr; 
    *p2++ = *p1++; 
} while (cnt > 0);
```

# 3.2.5. The `switch` Statement
```
switch (key) {
    case KEY_BS: {
        if (--me->ctr == 0) { 
            me->ctr = PERIOD; 
        } 
        break;
    }
    case KEY_CR: {
        ++me->crCtr;
        /* intentionally fall through */
    }
    case KEY_LF: {
        ++p;
        break;
    }
    default: {
        Q_ERROR(); /* should never be reached */
        break;
    }
}
```

Any fall though cases must be documented with comments confirming intentional fall through rather then an omission. 


### 3.2.6 Function Definition
```
void clrBuf(char *buf[], int len) { 
    char *b = &buf[0]; 
    while (len-- != 0) { 
        *b++ = '\0'; 
    } 
}
```

### 3.2.7 C++ Class Declaration
```
class Foo : public Bar { 
public: 
    Foo(int8_t x, int16_t y, int32_t z)    // ctor
        : Bar(x, y), m_z(z)
    {}
    virtual ~Foo();                        // xtor
    virtual int32_t doSomething(int8_t x); // operation

protected: 
    virtual void *bar();

private:
    friend class Tar; 
    friend void *fxyz(int16_t i);

    int8_t   m_x;
    uint16_t m_y;
    int32_t  m_z;
};
```
# 3.1 Expressions
The following binary operators are written with no space around them: 

|   Operator | Description                | Example           |
| ---------- | -------------------------- | ----------------- |
| `->`       | Structure pointer operator | `me->foo`         |
| `.`        | Structure member operator  | `s.foo`           |
| `[]`       | Array subscripting         | `a[i]`            |
| `()`       | Function call              | `foo(x, y, z);`   |


Parentheses after function names have no space before them. A space should be introduced after each comma to separate each actual argument in a function. Expressions within parentheses are written with no space after the opening parenthesis and no space before the closing parenthesis. Terminating semicolons should follow the instructions immediately with no space(s) before them: 
`strncat(t, s, n);`

The unary operators are written with no spaces between them and their operands: 
```
!p     ~b    ++i     j--     (void *)ptr    *p     &x   -k
```

The binary operators are preceded and followed by one (1) space, as is the ternary operator: 
```
c1 == c2     x + y     i += 2     n > 0 ? n : -n
```
The keywords: `if`, `while`, `for`, `switch` and `return` are followed by one (1) space: 
```
return foo(me->x) + y;
```

In case of compound expressions, parenthesizing should be used whenever the precedence is not "obvious". In general, over parenthesizing is recommended to remove any doubt and guessing. In the extreme case of MISRA-C Rules [MISRA 98], no dependence should be placed on C’s operator precedence whatsoever (MISRA-C rule 47), so every expression must be parenthesized. In addition, MISRA rules require that the operands of && and || shall be primary expressions (MISRA-C rule 34).
Following are examples of parenthesizing consistent with the strict MISRA-C rules: 
(a < b) && (b < c) /* operands of && are primary expressions (MISRA rule 34) */
x = (a * b) + c;                  /* don’t rely on precedence of ‘*’over ‘+’ */

<p align="center">
<a href="SECTION1.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION3.md"><b>NEXT&nbsp;»</b></a>
</p>

<p align="center">
<a href="SECTION1.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION3.md"><b>NEXT&nbsp;»</b></a>
</p>

The guidelines in this category are intended for visualizing the _logical code structure_ accurately and consistently, improve code readaility, minimize misinterpretation and withstand modifications. 

# 2. Layout
The fundamental principle underlying the guidelines in this section is that good layout needs to show the code's _structure_. If one technique shows the structure better and the other simply looks better, the one that represents the structure is preferred. Additionally, the preference is for layouts that can prevent errors due to misinterpretation as well as those that are more robust in view of likely changes, even if the layouts might not look as "neat" as the alternatives.


## Pure-Block Layout
Pure-block layout is based on the visualization of blocks of source code according to the following schematic view (see also [[CODE2]](README.md#07-references)):

```
XXXXXXXXXXXXXXXXXXXXXX
    YYYYYYYYYYYYY
    YYYYYYYYYYYYYYY
ZZZ
```
Where, line `XXXXXXX` begins the control block. The indented lines `YYYY` schematically show the statements hierarchically nested within the block. And finally, the line `ZZZ` explicitly ends the block. The alignment between `XXXXXXX` and `ZZZ` provides solid visual closure of the block.

The pure-block layout has three main benefits:
- it clearly shows the code structure;
- it eliminates possible misinterpretation with incorrect scope of the nested statements within a block;
- it is robust with respect of future modifications, such as adding or removing nested statements.

The concrete examples of this layout will be provided in the following guidelines, but the main point is that _all_ elements in the source code are formatted _consistently_ according to the same pure-block layout.


## 2.1 Indentation
Each level of scope within a block shall be indented by four (__4__) spaces.

__Rationale__<br>
Usability studies [[CODE2]](README.md#07-references) show that indentation of less than two spaces can be insufficient (especially for longer blocks of code), while indentation beyond 5 spaces does not improve code readability while increasing the need for wrapping long lines. Indentation of 4 spaces seems to be the "sweat spot".


## 2.2 Obligatory Braces
Use braces (`{...}`) to emulate the pure-block layout in C and C++.

__MISRA-C__<br>
This guideline overlaps the MISRA-C:2012 Rule 16.6 (Required) "The body of an iteration-statement and a selection-statement shall be a compound statement".

__Amplification__<br>
All blocks of code in a C or C++ must be surrounded by braces (`{...}`), even if not strictly required by the syntax.

__Rationale__<br>
In C and C++ the pure-block layout is not strictly enforced by the language syntax. For example, the following statement is syntactically correct, but is not a pure-block:

```
if (x < 0) 
    x = -x;
// missing the explicit end of the block
```
But the pure-block can be emulated by consistently applying braces (`{...}`) in all situations.

__Exception__<br>
The `if` statement immediately following an `else` need not be enclosed in a separate set of braces.

__Examples__<br>
```
/* compliant pure-block layout */
if (x < 0) {}
    x = -x;
}

if (x < 0) z = 25; /* NON-COMPLIANT, not a pure-block (no braces) */
else {
    z = 35; /* Compliant, braces used */
}

if (x < 0) {
    z = 25; /* Compliant, pure-block (braces used) */
}
else
    z = 35; /* NON-COMPLIANT, not a pure-block (no braces) */
    t = 25  /* incorrect indentation leads to misinterpretation */ 


if (flag1) {
    action1();
}
else { /* not necessary, see the next example */
	if (flag2) {
        action2();
    }
    else {
   	    //...
    }
}

if (flag1) {
    action1();
}
else if (flag2) { /* Compliant by exception */
    action2();
}
else {
	//...
}

/* NON-COMPLIANT, not a pure-block (no braces) */
while (millis() - start < TIMEOUT) ;

/* NON-COMPLIANT, not a pure-block (no braces) */
while (millis() - start < TIMEOUT)
    ;

/* Compliant, pure-block (braces used) */
while (millis() - start < TIMEOUT) {
}
```


# Enforcement


<p align="center">
<a href="SECTION1.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION3.md"><b>NEXT&nbsp;»</b></a>
</p>
<p align="center">
<a href="SECTION2.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION4.md"><b>NEXT&nbsp;»</b></a>
</p>

# 3. Commenting
Code implements an algorithm; the comments communicate the code’s operation to yourself and others. Adequate comments allow you to understand the system’s operation without having to read the code itself. 
Comments can be generally divided into three categories: 

- Elaborate high-level comments for major software components like: modules, classes, and exported APIs. 
- Brief, fine granularity comments explaining algorithmic details of performed operations. 
- References and notes attached to code in form of comments. 

Comments in the first category explain high level interfaces and the code structure. With help of automatic documentation generating tools like JavaDoc for Java or DOC++ for C/C++, these comments can easily be turned into online (HTML) documentation. The Standard does not require that every top-level comment be verified in view of converting it into on-line documentation. However, adhering to the following simple rules will make such automatic documentation extraction much easier, if we want to take advantage of it: 
Top-level comments should always come before the commented object.
No right-edge in comment boxes should be used, because keeping the right-edge of the box aligned is counterproductive:

```
/* INCORRECT: */ 
/*******************************************************************************
* this is class Foo                                                            *
* . . .                                                                        *
*******************************************************************************/
class Foo { 
    ... 
};

// CORRECT: 
//==============================================================================
// class Foo performs the following 
// ... 
//
class Foo { 
    ... 
};
```

Comments in the second category are typically low-level, algorithmic details. They should be placed as close to the pertaining code as possible, preferably in the same line. Although close to the code, the comments should be visually separated from code as much as possible by right-justifying them. Following section gives examples of aligning such comments: 
References and notes should be attached in form of the "notes" comment at the end of the module as shown in the next section describing the module layout.
 
Write comments in clear English. Use simple sentences: noun, verb, object. Use active voice. Be complete. Good comments capture everything important about the problem at hand. Ideally, it should be possible to get a sense of the system’s operation by reading only the comments.
 
For portability, not use C++ comments (`//`) in C, although many C99 compilers recognize C++ comments. (But the point is that some don’t!). Conversely, avoid using C comments (`/*..*/`) in C++.


## 3.4 Module Layout
The module is organized as follows: 

- Header comment block
- `#include` statements
- Public Section Specification
- Package Section Specification
- Local Section Specification
- Implementation
- Notes

### 3.4.1 Header Comment
Each module starts with a header comment in the following format: 

```
/*==============================================================================
* Product:  . . 
* Version:  . . 
* Updated:  Dec 17 2004
*
* Copyright (C) 2002-2004 Quantum Leaps. All rights reserved.
*
* <licensing terms> (if any)
*
* <Company contact information>
==============================================================================*/
```

### 3.4.2. Included Header Files
```
#include "rtk.h"      /* Real-Time Kernel */
#include "qassert.h"  /* embedded-systems-friendly assertions */
```

### 3.4.3. Public Section Specification
Definitions of public (global) variables should appear at the top of the module: 

```
/* Public-scope objects ---------------------------------------------------*/
QActive *UI_Mgr; /* pointer to the User Interface Manager active object */
. . .
```

### 3.4.4 Package Section Specification
The public (global) variables should be followed by all package-scope variables: 

```
/* Package-scope objects -----------------------------------------------------*/
QEvent const QEP_stdEvt[] = {
    { Q_EMPTY_SIG, 0},
    { Q_INIT_SIG,  0},
    { Q_ENTRY_SIG, 0},
    { Q_EXIT_SIG,  0}
};
```

### 3.4.5 Local Section Specification
The package-scope variables should be followed by local (module-scope) declarations and local variables (module-scope). All local-scope variables should be defined static.

```
/* Local-scope objects -------------------------------------------------------*/
static uint32_t l_svMask;    /* Space Vehicle mask indicating allocated SVs */
. . .
```

### 3.4.6 Implementation Section
The implementation section contains function definitions (in C) or class method definitions (in C++). Regardless of language, keep functions small. The ideal size is less than a page; in no case should a function ever exceed two pages. Break large functions into several smaller ones.

The only exception to this rule is the very rare case where you must handle very many events in one state handler. However, even in this case you should try to use state nesting (behavioral inheritance) to move some of the events to higher-level state handlers (See also [Samek 02, Section 4.3.1]).

Define a prototype for every function, even the static helper functions called only from within their own module. (The good place for such static prototypes is the local-scope section.) Prototypes let the compiler catch the all-too-common errors of incorrect argument types and improper numbers of arguments. They are cheap insurance.
In general, function names should follow the variable naming conventions (see below). Remember that functions are the “verbs” in programs - they do things. Incorporate the concept of “action words” into the variables’ names. For example, use “readAD” instead of “ADdata”.

Elaborate function comments (or class method comments in C++) should be placed in the header files, since the header files form the API documentation. These comments, generally, should not be repeated in the implementation section because this would create double points of maintenance. The comments at function definitions should rather refer to implementation details and revision history. At a minimum, the functions should be visually separated from each other.

```
/*............................................................................*/
void swap(int *x, int *y) {
    /* @pre pointers must be valid */
    Q_REQUIRE((x != (int *)0) && (y != (int *)0));
    int temp = *x; /* store value of x in a temporary */
    *x = *y;
    *y = temp;
}
/*..........................................................................*/
int pow(int base, unsigned int exponent) {
    uint_fast8_t result = 1U;
    for (; exponent > 0U; exponent >>= 1U) {
        if ((exponent & 1U) != 0U) { /* exponent is odd? */
            result *= base;
        }
        base *= base;
    }
    return result;
}
```

### 3.4.7 Notes Section
The Notes Section is the ideal place for longer explanations that apply to multiple sections of code or would otherwise distract the reader when placed directly at the code. In the pertinent code you place just a reference to the Note:

```
/*............................................................................*/
void interrupt dumpISR() {
    . . . /* dump channel data to RAM, see NOTE03 */
}

At the end of the module, you place the Notes Section:
/*==============================================================================
* . . .
*
* NOTE03: 
* this system has two hard real-time constraints: 
* #1 processing of channel DUMP data must complete before next DUMP 
*    for the channel. This constraint is ensured in Assertion (NOTE03.1). 
* 
* #2 no accumulated data can be missed due to new DUMP for a channel 
*    before the previous data has been read. This constraint is ensured 
*    in Assertion (NOTE03.2). 
*/
```

# 4. Exact-Width Integer Types

Avoid the use of "raw" C/C++ types, as these declarations vary depending on the machine (MISRA-C rule 13). The recommended strategy is to always use a C99 `<stdint.h>` header file [C99, Section 7.18]. In case this standard header file is not available (e.g., in a pre-standard compiler), you should create it and place in the compiler’s include directory. At a minimum, this file should contain the typedefs for the following exact-width integer data types [C99 Section 7.18.1.1]:

| exact size  | signed     | unsigned    |
| ----------- | ---------- | ----------- |
| 8 bits      | `int8_t`   | `uint8_t`   |
| 16 bits     | `int16_t`  | `uint16_t`  |
| 32 bits     | `int32_t`  | `uint32_t   | 

The main goal of the <stdint.h> indirection layer is promotion of code portability across different platforms. To achieve this goal the C99-style types listed above should be consistently used instead of the "raw" C/C++ types, such as long or unsigned char, and inventing different aliases for the C/C++ types is forbidden. 


# 5 Names

## 5.1 Reserved Names
The ANSI C specification restricts the use of names that begin with an underscore and either an uppercase letter or another underscore (regular expression: _[A-Z_][0-9A-Za-z_]). Much compiler runtime code also starts with leading underscores. 

These names are also reserved by ANSI for its future expansion: 

| Regular expression               | purpose                     |
| -------------------------------- | --------------------------- |
| `E[0-9A-Z][0-9A-Za-z]*`          | POSIX errno values          |     
| `is[a-z][0-9A-Za-z]*`            | character classification    |
| `to[a-z][0-9A-Za-z]*`            | character manipulation      |
| `LC_[0-9A-Za-z_]*`               | locale                      |
| `SIG[_A-Z][0-9A-Za-z_]*`         | POSIX signals               |
| `str[a-z][0-9A-Za-z_]*`          | string manipulation         |
| `mem[a-z][0-9A-Za-z_]*`          | memory manipulation         | 
| `wcs[a-z][0-9A-Za-z_]*`          | wide character manipulation |

To improve portability and avoid name conflicts, never use a name with a leading underscore or one of the name patterns reserved for future expansion.


## 5.2 Naming Conventions
This section does not intend to impose strict "Hungarian-type" notation. However, the following simple rules in naming various identifiers are strongly recommended:

- No identifier should be longer than 31 characters (this is a stricter version of MISRA-C rule 11).
- Type names (typedef, sturct and class) should start with an upper-case letter e.g., struct Foo. Optionally, the type name can be prefixed with the module identifier, e.g., typedef uint16_t QSignal, class QActive.
- Ordinary C functions and C++ class member functions start with a lower-case letter. 
- Member functions of classes coded in C (see Section 6) are prefixed with the class name and an underscore, so per the previous rule must begin with an upper-case letter. (QActive_start()). Besides clearly distinguishing the member functions, this rule minimizes link-time name conflicts with other functions (including third-party library functions).
- Global functions are prefixed with a module name and an underscore (e.g., QF_start()). Package-scope functions, visible only from a closely related group of source files—the package, are additionally suffixed with an underscore (QF_add_()). Besides clearly distinguishing global and package-scope functions, this rule minimizes link-time name conflicts with other functions (including third-party library functions).
- Ordinary variables should start with a lower-case letter (foo).
- Global variables should be prefixed with the module name and an underscore (e.g., QK_readySet).
- Local variables (visible within one module only) should start with “l_”, e.g., l_bitmask. All local variables should be declared static at the file scope (MISRA-C rule 23).
- C++ class attributes (data members) should should start with “m_”, e.g. int8_t m_foo. This convention allows easy distinction between the class data members and other variables like, for example, member function arguments. 
- Constants (numeric macros or enumerations) should be in upper-case with underscores "_" between each word or abbreviation (FOO_BAR). Global constants should be prefixed with the module name/identifier (Q_USER_SIG).
- All other parts of identifiers composed form multiple words should be constructed with capitalizing letters at word boundaries like: fooBarTar, and not foo_bar_tar.
- Generally, the more broad the scope the more descriptive the name should be. For a very limited scope, it is recommended to use single letter identifiers. For example: 
   + `i, j, k, m, n,` for integers like loop counters
   + `p, q, r, s, t, u, v, w,` for pointers or floating point numbers

<p align="center">
<a href="SECTION1.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION3.md"><b>NEXT&nbsp;»</b></a>
</p>

<p align="center">
<a href="SECTION1.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION3.md"><b>NEXT&nbsp;»</b></a>
</p>


# 4 Files
The module is organized as follows: 

- Header comment block
- `#include` statements
- Public Section Specification
- Package Section Specification
- Local Section Specification
- Implementation
- Notes

### 3.4.1 Header Comment
Each module starts with a header comment in the following format: 

```
/*==============================================================================
* Product:  . . 
* Version:  . . 
* Updated:  Dec 17 2004
*
* Copyright (C) 2002-2004 Quantum Leaps. All rights reserved.
*
* <licensing terms> (if any)
*
* <Company contact information>
==============================================================================*/
```

### 3.4.2. Included Header Files
```
#include "rtk.h"      /* Real-Time Kernel */
#include "qassert.h"  /* embedded-systems-friendly assertions */
```

### 3.4.3. Public Section Specification
Definitions of public (global) variables should appear at the top of the module: 

```
/* Public-scope objects ---------------------------------------------------*/
QActive *UI_Mgr; /* pointer to the User Interface Manager active object */
. . .
```

### 3.4.4 Package Section Specification
The public (global) variables should be followed by all package-scope variables: 

```
/* Package-scope objects -----------------------------------------------------*/
QEvent const QEP_stdEvt[] = {
    { Q_EMPTY_SIG, 0},
    { Q_INIT_SIG,  0},
    { Q_ENTRY_SIG, 0},
    { Q_EXIT_SIG,  0}
};
```

### 3.4.5 Local Section Specification
The package-scope variables should be followed by local (module-scope) declarations and local variables (module-scope). All local-scope variables should be defined static.

```
/* Local-scope objects -------------------------------------------------------*/
static uint32_t l_svMask;    /* Space Vehicle mask indicating allocated SVs */
. . .
```

### 3.4.6 Implementation Section
The implementation section contains function definitions (in C) or class method definitions (in C++). Regardless of language, keep functions small. The ideal size is less than a page; in no case should a function ever exceed two pages. Break large functions into several smaller ones.

The only exception to this rule is the very rare case where you must handle very many events in one state handler. However, even in this case you should try to use state nesting (behavioral inheritance) to move some of the events to higher-level state handlers (See also [Samek 02, Section 4.3.1]).

Define a prototype for every function, even the static helper functions called only from within their own module. (The good place for such static prototypes is the local-scope section.) Prototypes let the compiler catch the all-too-common errors of incorrect argument types and improper numbers of arguments. They are cheap insurance.
In general, function names should follow the variable naming conventions (see below). Remember that functions are the “verbs” in programs - they do things. Incorporate the concept of “action words” into the variables’ names. For example, use “readAD” instead of “ADdata”.

Elaborate function comments (or class method comments in C++) should be placed in the header files, since the header files form the API documentation. These comments, generally, should not be repeated in the implementation section because this would create double points of maintenance. The comments at function definitions should rather refer to implementation details and revision history. At a minimum, the functions should be visually separated from each other.

```
/*............................................................................*/
void swap(int *x, int *y) {
    /* @pre pointers must be valid */
    Q_REQUIRE((x != (int *)0) && (y != (int *)0));
    int temp = *x; /* store value of x in a temporary */
    *x = *y;
    *y = temp;
}
/*..........................................................................*/
int pow(int base, unsigned int exponent) {
    uint_fast8_t result = 1U;
    for (; exponent > 0U; exponent >>= 1U) {
        if ((exponent & 1U) != 0U) { /* exponent is odd? */
            result *= base;
        }
        base *= base;
    }
    return result;
}
```

### 3.4.7 Notes Section
The Notes Section is the ideal place for longer explanations that apply to multiple sections of code or would otherwise distract the reader when placed directly at the code. In the pertinent code you place just a reference to the Note:

```
/*............................................................................*/
void interrupt dumpISR() {
    . . . /* dump channel data to RAM, see NOTE03 */
}

At the end of the module, you place the Notes Section:
/*==============================================================================
* . . .
*
* NOTE03: 
* this system has two hard real-time constraints: 
* #1 processing of channel DUMP data must complete before next DUMP 
*    for the channel. This constraint is ensured in Assertion (NOTE03.1). 
* 
* #2 no accumulated data can be missed due to new DUMP for a channel 
*    before the previous data has been read. This constraint is ensured 
*    in Assertion (NOTE03.2). 
*/
```

# 4. Exact-Width Integer Types

Avoid the use of "raw" C/C++ types, as these declarations vary depending on the machine (MISRA-C rule 13). The recommended strategy is to always use a C99 `<stdint.h>` header file [C99, Section 7.18]. In case this standard header file is not available (e.g., in a pre-standard compiler), you should create it and place in the compiler’s include directory. At a minimum, this file should contain the typedefs for the following exact-width integer data types [C99 Section 7.18.1.1]:

| exact size  | signed     | unsigned    |
| ----------- | ---------- | ----------- |
| 8 bits      | `int8_t`   | `uint8_t`   |
| 16 bits     | `int16_t`  | `uint16_t`  |
| 32 bits     | `int32_t`  | `uint32_t   | 

The main goal of the <stdint.h> indirection layer is promotion of code portability across different platforms. To achieve this goal the C99-style types listed above should be consistently used instead of the "raw" C/C++ types, such as long or unsigned char, and inventing different aliases for the C/C++ types is forbidden. 


# 5 Names

## 5.1 Reserved Names
The ANSI C specification restricts the use of names that begin with an underscore and either an uppercase letter or another underscore (regular expression: _[A-Z_][0-9A-Za-z_]). Much compiler runtime code also starts with leading underscores. 

These names are also reserved by ANSI for its future expansion: 

| Regular expression               | purpose                     |
| -------------------------------- | --------------------------- |
| `E[0-9A-Z][0-9A-Za-z]*`          | POSIX errno values          |     
| `is[a-z][0-9A-Za-z]*`            | character classification    |
| `to[a-z][0-9A-Za-z]*`            | character manipulation      |
| `LC_[0-9A-Za-z_]*`               | locale                      |
| `SIG[_A-Z][0-9A-Za-z_]*`         | POSIX signals               |
| `str[a-z][0-9A-Za-z_]*`          | string manipulation         |
| `mem[a-z][0-9A-Za-z_]*`          | memory manipulation         | 
| `wcs[a-z][0-9A-Za-z_]*`          | wide character manipulation |

To improve portability and avoid name conflicts, never use a name with a leading underscore or one of the name patterns reserved for future expansion.


## 5.2 Naming Conventions
This section does not intend to impose strict "Hungarian-type" notation. However, the following simple rules in naming various identifiers are strongly recommended:

- No identifier should be longer than 31 characters (this is a stricter version of MISRA-C rule 11).
- Type names (typedef, sturct and class) should start with an upper-case letter e.g., struct Foo. Optionally, the type name can be prefixed with the module identifier, e.g., typedef uint16_t QSignal, class QActive.
- Ordinary C functions and C++ class member functions start with a lower-case letter. 
- Member functions of classes coded in C (see Section 6) are prefixed with the class name and an underscore, so per the previous rule must begin with an upper-case letter. (QActive_start()). Besides clearly distinguishing the member functions, this rule minimizes link-time name conflicts with other functions (including third-party library functions).
- Global functions are prefixed with a module name and an underscore (e.g., QF_start()). Package-scope functions, visible only from a closely related group of source files—the package, are additionally suffixed with an underscore (QF_add_()). Besides clearly distinguishing global and package-scope functions, this rule minimizes link-time name conflicts with other functions (including third-party library functions).
- Ordinary variables should start with a lower-case letter (foo).
- Global variables should be prefixed with the module name and an underscore (e.g., QK_readySet).
- Local variables (visible within one module only) should start with “l_”, e.g., l_bitmask. All local variables should be declared static at the file scope (MISRA-C rule 23).
- C++ class attributes (data members) should should start with “m_”, e.g. int8_t m_foo. This convention allows easy distinction between the class data members and other variables like, for example, member function arguments. 
- Constants (numeric macros or enumerations) should be in upper-case with underscores "_" between each word or abbreviation (FOO_BAR). Global constants should be prefixed with the module name/identifier (Q_USER_SIG).
- All other parts of identifiers composed form multiple words should be constructed with capitalizing letters at word boundaries like: fooBarTar, and not foo_bar_tar.
- Generally, the more broad the scope the more descriptive the name should be. For a very limited scope, it is recommended to use single letter identifiers. For example: 
   + `i, j, k, m, n,` for integers like loop counters
   + `p, q, r, s, t, u, v, w,` for pointers or floating point numbers

<p align="center">
<a href="SECTION1.md"><b>«&nbsp;PREVIOUS</b></a>&nbsp;|&nbsp;
<a href="README.md"><b>TOC</b></a>&nbsp;|&nbsp;
<a href="SECTION3.md"><b>NEXT&nbsp;»</b></a>
</p>

[<b>« PREVIOUS</b>](README.md) <span style="float:right">[<b>NEXT »</b>](SECTION7.md)</span>

# 6. Object Oriented Programming in C
The following guidelines are intended to help in adoption of best practices from Object Oriented Programming into C programming. Contrary to widespread beliefs, it’s quite easy to implement encapsulation, single inheritance, and even polymorphism (late binding) in procedural languages, such as C [Samek 97]. Knowing how to implement encapsulation, inheritance, or even polymorphism in C can be very beneficial, because it leads to better code organization and better code reuse.

At the C programming level, encapsulation and inheritance become two simple design patterns. The following section describes the recommended ways of implementing these patterns.


## 6.1. Encapsulation
Encapsulation is the ability to package data with functions into classes. This concept should actually come as very familiar to any C programmer because it’s quite often used even in the traditional C. For example, in the Standard C runtime library, the family of functions that includes fopen(), fclose(), fread(), and fwrite() operates on objects of type FILE. The FILE structure is thus encapsulated because client programmers have no need to access the internal attributes of the FILE struct and instead the whole interface to files consists only of the aforementioned functions. You can think of the FILE structure and the associated C-functions that operate on it as the FILE class. The following bullet items summarize how the C runtime library implements the FILE "class": 

- Attributes of the class are defined with a C struct (the FILE struct). 
- Methods of the class are defined as C functions. Each function takes a pointer to the attribute structure (FILE *) as an argument. Class methods typically follow a common naming convention (e.g., all FILE class methods start with prefix f). 
- Special methods initialize and clean up the attribute structure (fopen() and fclose()). These methods play the roles of class constructor and destructor, respectively. 

This is exactly how QP/C and QP-nano frameworks implement classes. For instance, the following snippet of QP/C code declares the QActive (active object) "class". Please note that all class methods start with the class prefix ("QActive" in this case) and all take a pointer to the attribute structure as the first argument "me": 

```
typedef struct QActiveTag {
    QHsm super; /* inherit QHsm */
    . . .
/* private: */
    uint8_t prio; /* private priority of the active object */
} QActive; /* Active Object base struct */

/* public operations... */
int QActive_start(QActive * const me, uint8_t prio,
                  QEvent *qSto[], uint16_t qLen,
                  void *stkSto, uint32_t stkSize, 
                  QEvent const *ie);
void QActive_postFIFO(QActive * const me, QEvent const *e);
void QActive_postLIFO(QActive * const me, QEvent const *e);

/* protected operations ...*/
void QActive_ctor(QActive * const me, QPseudoState initial);
void QActive_xtor(QActive * const me);
void QActive_stop(QActive * const me);    /* stopps thread; nothing happens after */
void QActive_subscribe(QActive const * const me, QSignal sig);
*/
```

## 6.2 Inheritance
Inheritance is the ability to define new classes based on existing classes in order to reuse and organize code. QP/C and QP-nano implement single inheritance by literally embedding the parent class attribute structure as the first member of the child class structure. As shown in the following figure, this arrangement lets you treat any pointer to the Child class as a pointer to the Parent: 


In particular, such memory alignment of the Child attributes with the Parent attributes allows you to always pass a pointer to the Child class to a C function that expects a pointer to the Parent class. (To be strictly correct in C, you should explicitly upcast this pointer.) Therefore, all methods designed for the Parent class are automatically available to Child classes; that is, they are inherited. 

For example, in the code snippet from the previous section class QActive inherits from class QHsm. Please note the first protected attribute "super" of type QHsm in the QActive struct definition.



[<b>« PREVIOUS</b>](README.md) <span style="float:right">[<b>NEXT »</b>](SECTION7.md)</span>
[<b>« PREVIOUS</b>](SECTION6.md) <span style="float:right">[<b>NEXT »</b>](SECTION8.md)</span>

# 7. Design by Contract

Design by Contract is a very powerful set of techniques introduced by Bertrand Meyer [Meyer 97]. The techniques are based on the concept of contract that formally captures assumptions and delicate dependencies between software components. The central idea is to enforce fulfillment of these contracts by instrumenting code and explicit checking against contract violations. Contracts may be in form of preconditions, postconditions and invariants. In C/C++ they are implemented in form of assertions. Some of the listed benefits of Design by Contract include [Samek 03]:

- A better understanding of software construction
- A systematic approach to building bug free systems
- An effective framework for debugging, testing and, more generally, Software Quality Assurance (SQA) 
- A method for documenting software components
- Better understanding and control of code reuse
- A technique for dealing with abnormal cases, leading to a sage and effective language constructs for exception handling. 

In deeply embedded systems, assertion failure must be treated differently than on desktop computers. Typically, standard actions of printing out assertion failure statement and exit are not the right approach. For that reason a customized behavior in case of contract violation is coded in "qassert.h" include file [QL 04]. This header file defines the following assertions:

| Macro               | Purpose                       |
| ------------------- | ----------------------------- |
| `Q_ASSERT()`        | General-purpose assertions    |
| `Q_ALLEGE()`        | Assertions with side-effects in testing the expression,<br> when the side effects are desired even if assertions are disabled |
| `Q_REQUIRE()`       | For asserting preconditions   |
| `Q_ENSURE()`        | For asserting postconditions  |
| `Q_INVARIANT()`     | For asserting invariants      |
| `Q_ERROR()`         | For asserting incorrect paths through the code |
| `Q_ASSERT_STATIC()` | For static, compile-time assertions |

[<b>« PREVIOUS</b>](SECTION6.md) <span style="float:right">[<b>NEXT »</b>](SECTION8.md)</span>
[<b>« PREVIOUS</b>](SECTION7.md)

# 8. Related Documents and References

| Reference        | Location                                                        |
| ---------------- | --------------------------------------------------------------- |
| [C99]            | ISO/IEC 9899 C Approved Standards http://www.open-std.org/jtc1/sc22/open/n2794 |
| [Humphrey 95]    | Humphrey, Watts, S., A Discipline in Software Engineering, Addison-Wesley, Reading 1995. |
| [Labrosse 00]    | Labrosse, Jean J., AN2000: C Coding Standard, http://www.micrium.com |
| [Meyer 97]       | Meyer, Bertrand, Object-Oriented Software Construction 2nd Edition, Prentice Hall, 1997. ISBN: 0-136-29155-4 |
| [MISRA 98,04,12] | Motor Industry Software Reliability Association (MISRA), MISRA Limited, MISRA-C:1998 Guidelines for the Use of the C Language in Vehicle Based Software, April 1998, ISBN 0-9524156-9-0.<br>See also https://www.misra.org.uk  |
| [Samek 97]       | Portable Inheritance and Polymorphism in C, ESP, December 1997           |
| [Samek 02]       | Samek, Miro, Practical Statecharts in C/C++, CMP Books 2002.             |
| [Samek 03]       | Samek, Miro, “An Exception or a Bug?” C/C++ Users Journal, August, 2003, pages 36-40 |
| [Stroustrup 10]  | Bjarne Stroustrup. "PPP Style Guide", September 2010,<br>http://www.stroustrup.com/Programming/PPP-style-rev3.pdf |
| [QL]             | https://www.state-machine.com  |


[<b>« PREVIOUS</b>](SECTION7.md)
