# To do list

## Highest priority, in order

* Implement all basic features
* Implement autotools
  * Install scripts
* Add wordlists
  * Include wordlists in installation
  * Figure out how program will know where wordlists are installed
    * Probably https://stackoverflow.com/a/14655291 answers that question

## Lowest priority, in order

* Find more things to add to this list
* Maybe import wordlists from xkcdpass
  * Ask xkcdpass owner for permission
* Handle integer CLI arguments if below zero
* Refactor and split up code for easier readability and maintainability
* Automate package generation
  * Primarily a .deb file
* Figure out dependencies and requirements
  * Which versions of what is required
  * Put into autotools
  * Put into README.md
* Write tests
  * For multiple platforms and architectures
* Make cxkcdpass work on Windows
* Make cxkcdpass work on any and all platforms and architectures
* Cross platform builds maybe?
* Profiling and optimization
* Reduce binary sizes
* Support for compressed wordlists
* Wordlist generating scripts
  * Sources such as Wikipedia, etc
* CI/CD ? Automatically publish to package repositories?
