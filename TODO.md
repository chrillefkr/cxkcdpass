# To do list

## Highest priority, in order

* Comment code!
* Implement all basic features
* Add wordlists
  * Include wordlists in installation
  * Figure out how program will know where wordlists are installed
    * Probably https://stackoverflow.com/a/14655291 answers that question
* Create man pages
  * Doxygen?

## Lowest priority, in order

* Find more things to add to this list
* Maybe import wordlists from xkcdpass
  * Ask xkcdpass owner for permission
* Have multiple methods of reading wordlist, i.e. read(), aio, etc., instead of
mmap for supporting other filetypes than "regular" files on disk
(e.g. pipes, fifo).
* Shell completion
  * bash
    * Fix list of included wordfiles
  * zsh: https://github.com/zsh-users/zsh-completions/blob/master/zsh-completions-howto.org
* CLI argument validation
  * Handle if integers are out of range (e.g. below zero)
  * Handle mutual exclusive arguments
    * `--numwords` and `--acrostic`
    * https://www.gnu.org/software/gengetopt/gengetopt.html#Mode-options ?
* Refactor and split up code for easier readability and maintainability
* Automate package generation
  * Primarily a .deb file
  * brew
* Figure out dependencies and requirements
  * Which versions of what is required
  * Put into autotools
  * Put into README.md
* Write tests
  * For multiple platforms and architectures
* Make cxkcdpass work on Windows
* Make cxkcdpass work on BSD
* Make cxkcdpass WSL friendly
* Make cxkcdpass work on any and all platforms and architectures
* Cross platform builds maybe?
* Profiling and optimization
* Reduce binary sizes
* Support for compressed wordlists
* Wordlist generating scripts
  * Sources such as Wikipedia, etc
* CI/CD ? Automatically publish to package repositories?
* Maybe decrease system calls
  * arc4random_buf ?
* Mutliple wordfile support
* Print entropy stats on verbose
* Configuration file (/etc/cxkcdpass/cxkcdpass.conf) and --configuration ./eh.conf
  * Maybe run `cmdline_parser_file_save()` by hidden argument (e.g. --generate-configuration-file) on installation
* Maybe support random amount of words?
  * --max-words --min-words would override --num-words
