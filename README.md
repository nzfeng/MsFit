# MsFit
MsFit (pronounced "misfit") is a crossword construction tool.

# Getting started
```
git clone --recursive https://github.com/nzfeng/MsFit
```

# Dependencies
Installing gtkmm4 on Mac is as simple as
```
brew install gtkmm4
```
[This page](https://wiki.gnome.org/Projects/gtkmm/UsingCMake) was helpful in getting gtkmm to compile with CMake. [This StackOverflow answer](https://stackoverflow.com/a/60942488) was also helpful.

gtkmm documentation is [here](https://developer-old.gnome.org/gtkmm-tutorial/stable/index.html.en).
Better documentation (more up-to-date with gtkmm4) is [here](https://developer-old.gnome.org/gtkmm-tutorial/unstable/gtkmm-tutorial.html).

Possibly [RE2](https://github.com/google/re2).

Possibly[dense_hash_map/dense_hash_set](https://github.com/sparsehash/sparsehash);

# Interface controls

* `Shift-P`: Toggle pencil.

## Grid crossword
Controls for navigating a grid crossword:

* `Space`: Toggle across/down.
* `Tab`: Skip to the next open square in the next uncompleted word.
* `Esc`: Enter more than one letter in a box.
* Arrow keys: move one box in the corrresponding direction (skipping black squares)

For any box in the grid, black or white:

* Right-click: Toggle black/white.

## Graph crossword
* `Space`: Toggle the current word. Aims to go roughly in CCW order around the currently selected square.
* `Tab`:
* `Esc`: Enter more than one letter in a cell.
* Right-click: Toggle black/white cell.

# Organization
* The `interface` directory contains code that pertains purely to building the GUI to the screen. Specifically, this means building and laying out the container widgets for the panels, menus, etc. However, objects in the `puzzle` directory still have their own functions defining how they are rendered. I don't envision there being enough demand for flexibility & precise control of rendering crossword puzzles that it calls for de-coupling the core puzzle engine from rendering (at least not yet.)
* The `puzzle` directory contains code for the `PuzzleGrid` object, which represents an ordinary crossword puzzle grid. There is also the `PuzzleGraph` object, which is used when the puzzle has non-square cells and thus requires more navigation overhead. Basically, a `PuzzleGraph` is represented as a graph that can be traversed a manner similar to a halfedge mesh (the cells being faces.)
* A `PuzzleGrid` is represented as single `Gtk::DrawingArea`, rather than as a `Gtk::Grid` comprised of many individual widgets representing the squares. I figured that although I would have to write many functions myself relating to square selection, etc., at least I would be able to exactly control how mouse/key commands behave, and how the grid is rendered, rather than having to deal with dozens of widgets and the interaction between their signal handlers.
* The `engine` directory contains code that helps the user fill the puzzle.
* The `utilities` directory contains generally useful functions, like ones to assist in drawing things to screen.

# Regex
* `std::regex`: uses BT (backtracking), flexible but possibly slow, especially with the `|` character
* `RE2` uses FSA (finite state machine), seems to be consistently one of the fastest libraries in benchmarks
* `Glib`: a choice since we're already using `Glib::ustring`s
* `PCRE`: for `C`, would need wrappers

# Datasets
<!-- * [Broda](https://peterbroda.me/crosswords/wordlist/) -->
* Dictionary, American idiom dictionary, Wikipedia entries (try to scrape POS, tags)
* Ranking of parts of speech: noun > verb > adjective > adverb > prepositions > article > conjunction > pronouns
* Conjugation: plural (if noun); root, past, gerund (if verb)
* I will probably manually curate my own dataset. A decently sized wordlist would be around 10^5 words, which is at least feasible to manually curate and tag. Dataset will also only take a few MB saved as a text file with UTF-8 chars. A little more overhead if storing in a database (which I will probably do.)
* Word tags: part of speech, conjugation, topic (a list of tags), foreign, idiom, proper
* To determine semantic content (like "grid" relating to the topic "crossword"), will likely leverage others' NLP tools to generate topic tags. Copy Wikipedia tags?
* [WordNet](https://wordnet.princeton.edu/). Useful structure/classifications, also doesn't seem to have pop culture, etc. Has a C API.
* The more I research, the more I'm leaning towards just compiling a wordlist (no structure or tags.) (Only sorting is done by word length)

# Fill algorithm
Ultimately the fill algorithm will be some variant of BFS or DFS... Here are some options:
* naive DFS
* naive BFS; memory issues might prevent this from being much better than DFS
* dancing links

Ideas for fill tools:
* Build database of English phonemes (? I mean legal letter combinations), to help narrow the search space based on feasible combos of letters. However, this may erroneously eliminate compound words (e.g. "pj" in "flapjack".)
* When generating fills for a word, and it would complete in an intersecting word, only generate words that would complete both words.
* When generating fills for a word, only generate fills that would leave at least n possibilities for every intersecting word.
* Filter auto-fill results based on starting letter, whether starting vowel/consonant
* Avoid repeated substrings, letters, and phonemes

# TODO
* Export `.puz` file format
* Support other major crossword types, such as British style, Japanese style, diagramless, etc.
* Enable cell shapes besides squares
* Cell shapes with curved sides (not anytime soon)
* Imagine how cool it would be to make crosswords out of tilings of the hyperbolic plane or the sphere! It may be very hard to find words to fit. (It's already hard enough making ordinary grid crosswords with white squares above a certain %.)
* 3D crosswords not on my to-do list anytime soon, since I think crosswords are meant to be 2D where they can be played on a piece of paper.
* Make crossword area scrollable, in case the grid is too large to be displayed in its entirety.
* Switch from GTK to Qt, which has better cross-platform support, and platform-specific UI.
* Co-optimize both the grid and fill quality (I belive this is very hard)
* Take some ideas from the industry-standard [Crossword Compiler](https://www.crossword-compiler.com/). For example, Grid Insight; viewing "forced words", etc.
* Go through all filtered words and add any commonly-used derivatives.
* Store all unfiltered WordNet words (and Broda words) from the version I used, so I can check against them when I add entries in the future. 

* Automatically show "illegal" grids, i.e. 2-letter words, cells shared by only one word, etc. (can toggle this option)
* Automatically detect duplicate entries. Also have option to display repeated substrings.
* Keyboard shortcuts for everything.
* Options to sort fill list: Alphabetical, search bar, order by average # of fills for all crossing words.
* For "get most constrained" option: implement k-ply lookahead; i.e. pick the entry that has the fewest fill options (that allow at least 1 fill for all crossing words) after removing fills that result in 0 fills if chosen. I.e. attempting to prune dead-end sub-trees. k should balance accuracy vs. computation time per evaluation.

# Puzzle ideas
* Include hexagonal cells as 3-way intersections
* Other tilings
* Tilings of the sphere/hyperbolic disk?
* I liked this puzzle: https://crosshare.org/crosswords/oC5KN16iBGMncNxU4ie5/2x3x4-3. TODO: Option for multiple chars per cell (i.e. use `.*` in regex.)
* [Polyhedral Patterns](https://geometry.stanford.edu/papers/jtvwp-pp-15/jtvwp-pp-15.pdf)

# Limitations
* Only English is supported.

# License
Currently MsFit is a private project.

# Credits
Software engineering-wise, I took organizational inspiration from [Geometry Central](https://github.com/nmwsharp/geometry-central) and [Polyscope](https://github.com/nmwsharp/polyscope).