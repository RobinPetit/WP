# INFOF-209
Wizard Poker

## Compilation

It is better to do an out-of-source build:

    mkdir build && cd build
	cmake ..
	make


## Usual workflow

When you want to contribute, here is an idea of the usual workflow:

```bash
    git pull
	git checkout -b nameOfYourModifications
	# Do things in you local branch
	git add filesThatYouJustModified.cpp
	git commit
	# Do more commits...
	git push --set-upstream origin nameOfYourModifications # The first push
	git push # The following pushes do not need to add the local branch to origin
```

Then open a pull request on GitHub for the branch nameOfYourModifications,
and if you coworker(s) agree(s), let your coworker(s) merge it (with the GitHub interface, for example).  
**Note:** if the changes in the working directory form a logical unit (all the changes
fix/improve the same feature), then the following command can be useful:

```
    git commit -a
```

It adds all changes to the stagging area (`git add`) and then commits.
But if you did multiple changes that are unrelated (e.g. fix a doc typo and add a method)
,then do separated commits.


## Remarks on Debriefing 1

Here are the remarks I noted while the debriefing on Tuesday 02/02/2016 :

+ the SRD is not complete and too short;
+ the formatting has to be improved (rewrite the flyleaf);
+ in the deck management diagram, explain the notion of default deck (which is not empty);
+ write preconditions and postconditions for **every** use case diagram;
+ in the gameplay diagram, the case where the monster does not die is missing;
+ ambiguity of the class *Compte*.

## Commit formatting

A commit should start with a title that is a short explanation
(52 characters maximum, if possible) of the change, then a blank line,
then some paragraphs that explain the *how* and the *why* of the change
(above all the *why*), if the change is not trivial.
The explanations lines should have a length of maximum 72 characters.  
**Note:** if you are using Vim as default command-line text editor,
Vim will help you to follow these rules with automatic colors and
line wraps.

## Proposal of coding conventions
Comments starting by `//~` are not meant to be in
the final code, they are just indicative about the coding conventions.  
Comments starting by `///` are Doxygen documentation.
This is not mandatory, but this is good practice.  
The header huard is written as follow: an underscore,
then the name of the class (or the file) in uppercase with an underscore
separating each word, then `[_CLIENT_HPP|_SERVER_HPP|_COMMON_HPP]`
depending the directory of the file.
The header guards can be changed, this is
just a proposal.

* include/MyClass.hpp

```cpp
    #ifndef _MYCLASS_CLIENT_HPP
    #define _MYCLASS_CLIENT_HPP

    //~ Standard headers first
    #include <string>
    //~ Then external libs
    #include <SFML/Graphics.hpp>
    //~ Finally project headers, only the ones that can't be forward-declared
	//~ Never forward-declare standard classes, and forward-declare when the
	//~ declaration is "relatively" trivial. Do not structure your code
	//~ to forward-declare when this is usually not possible
    #include "MyOtherClass.hpp"
    
    //~ No trailing dot on regular comments (saving a keystroke is good)
    // Forward declarations
    class IncompleteClass;
    
    //~ But trailing dot on docstrings
    //~ (they are meant to be formatted as document or web page)
    /// Brief explanation of the class.
    class MyClass : public MyOtherClass
    {
        //~ Four spaces as indentation
        public:
            /// Constructor.
            /// \param myInt An useful integer.
            //~ Explicit specifier to avoid implicit conversion from int to MyClass
			// \TODO Things to do in the future.
            MyClass(int myInt=42) explicit;

            /// Constructor.
            /// \param arg Another useful constructor argument.
			// \FIXME Bug to fix as soon as possible.
            MyClass(const IncompleteClass& arg) explicit;

            /// Destructor.
            virtual ~MyClass() = default;

            /// Getter.
            /// \return myInt.
            int getMyInt() const;
            
            /// Setter.
            /// \param newMyInt The new value of myInt.
            void setMyInt(int newMyInt);

        private://~ Use protected only when this is meant to be redefined by subclasses
            //~ Docstring alignment is not mandatory but is pretty
            int m_myInt;        ///< An useful integer.
            std::string m_myStr;///< A good C++ string.
    };

    #endif//_MYCLASS_CLIENT_HPP
    //~ Trailling new line
```


* src/MyClass.cpp

```cpp
    #include <algorithm>
    #include <SFML/Window.hpp>
    #include "IncompleteClass.hpp"
    #include "MyClass.hpp"//~ The corresponding header at last.

    //~ Not `using namespace std;`!
    MyClass::MyClass(int myInt):
        MyOtherClass(3),
        m_myInt(myInt),
        m_myStr("LULZ")
    {
        //~ Use algorithms and standard features whenever possible!
        auto it = std::find(m_myStr.cbegin(), m_myStr.cend(), "Z");
        if(it == m_myStr.cend())
            std::cout << "Z not found in m_myStr!\n";//~ std::endl is not always necessary

        //Locally use a namespace can be useful sometimes
        using namespace std::placeholders;
        const std::string var{"X"};
        //~ I said whenever possible, everything is possible with STL!
        it = std::find_if(m_myStr.cbegin(), m_myStr.cend(), std::bind(checkWithTwoArgs, _1, var));
        if(it == m_myStr.cend())
            std::cout << "checkWithTwoArgs return true for none of the characters of m_myStr and var\n";
    }

    MyClass::MyClass(const IncompleteClass& arg):
        MyOtherClass(arg.getInt() / 2),
        m_myInt(arg.getInt()),
        m_myStr(arg.getStr())
    {
        float var{0.5f};//Comment on same line if short enough
        for(std::size_t i{0}; i < m_myStr.size(); ++i)
            //But if the explanation comment is too long, write it on the previous line
            if(m_myInt > 42)
                doSomething();
            else
                var -= var / 2.f;

        do
        {
            var *= 1.42f;
            doSomething();
        } while(var > 5.f);

        switch(m_myInt)
        {
            case 0:
                inCaseOfZero();
                break;

            case 1:
                inCaseOfOne();
                break;

            default:
                inDefaultCase();
        }
    }

    MyClass::getMyInt() const
    {
        return m_myInt();
    }

    MyClass::setMyInt(int newMyInt)
    {
        m_myInt = newMyInt;
    }
```

