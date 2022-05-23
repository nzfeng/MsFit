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

# Interface controls
Controls for navigating the crossword:

* `Space`: Toggle across/down.
* `Tab`: Skip to the next uncompleted word.
* `Esc`: Enter more than one letter in a box.
* Arrow keys: move one box in the corrresponding direction (skipping black squares)

For any box in the grid, black or white:

* `Ctrl`-`X`: Toggle black/white.

# Organization
* The `interface` directory contains code that pertains to building the GUI / rendering anything to the screen.
* The `puzzle` directory contains code for the `PuzzleGraph` object, which represents the puzzle. The puzzle is represented as a graph that can be traversed a manner similar to a halfedge mesh (the cells being faces.)
* The `utilities` directory contains utility functions like importing datasets.

# TODO

* Support other major crossword types, such as British style, Japanese style, diagramless, etc.
* Render the numbers inside boxes
* Enable cell shapes besides squares
* Cell shapes with curved sides (not anytime soon)
* Imagine how cool it would be to make crosswords out of tilings of the hyperbolic plane or the sphere! It may be very hard to find words to fit. (It's already hard enough making ordinary grid crosswords with white squares above a certain %.)
* Make functions inline to reduce execution time (may not be necessary)
* 3D crosswords not on my to-do list anytime soon, since I think crosswords are meant to be 2D where they can be played on a piece of paper.

# License
Currently MsFit is a private project.

# Credits
Software engineering-wise, I took organizational inspiration from [Geometry Central](https://github.com/nmwsharp/geometry-central) and [Polyscope](https://github.com/nmwsharp/polyscope).

Patrick Crosby's [xword software](http://xword.sourceforge.net/) was helpful in getting ideas on how to organize the grid using gtkmm.