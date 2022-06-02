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

# Interface controls
Controls for navigating the crossword:

* `Space`: Toggle across/down.
* `Tab`: Skip to the next open square in the next uncompleted word.
* `Esc`: Enter more than one letter in a box.
* Arrow keys: move one box in the corrresponding direction (skipping black squares)

For any box in the grid, black or white:

* Right-click: Toggle black/white.

# Organization
* The `interface` directory contains code that pertains purely to building the GUI to the screen. Specifically, this means building and laying out the container widgets for the panels, menus, etc. However, objects in the `puzzle` directory still have their own functions defining how they are rendered. I don't envision there being enough demand for flexibility & precise control of rendering crossword puzzles that it calls for de-coupling the core puzzle engine from rendering (at least not yet.)
* The `puzzle` directory contains code for the `PuzzleGrid` object, which represents an ordinary crossword puzzle grid. There is also the `PuzzleGraph` object, which is used when the puzzle has non-square cells and thus requires more navigation overhead. Basically, a `PuzzleGraph` is represented as a graph that can be traversed a manner similar to a halfedge mesh (the cells being faces.)
* A `PuzzleGrid` is represented as single `Gtk::DrawingArea`, rather than as a `Gtk::Grid` comprised of many individual widgets representing the squares. I figured that although I would have to write many functions myself relating to square selection, etc., at least I would be able to exactly control how mouse/key commands behave, and how the grid is rendered, rather than having to deal with dozens of widgets and the interaction between their signal handlers.
* The `utilities` directory contains utility functions like importing datasets.

# TODO
* Support other major crossword types, such as British style, Japanese style, diagramless, etc.
* Enable cell shapes besides squares
* Cell shapes with curved sides (not anytime soon)
* Imagine how cool it would be to make crosswords out of tilings of the hyperbolic plane or the sphere! It may be very hard to find words to fit. (It's already hard enough making ordinary grid crosswords with white squares above a certain %.)
* 3D crosswords not on my to-do list anytime soon, since I think crosswords are meant to be 2D where they can be played on a piece of paper.
* Make crossword area scrollable, in case the grid is too large to be displayed in its entirety.
* Switch from GTK to Qt, which has better cross-platform support, and platform-specific UI.
* Co-optimize both the grid and fill quality (I belive this is very hard)
* Take some ideas from the industry-standard [Crossword Compiler](https://www.crossword-compiler.com/). For example, Grid Insight; viewing "forced words", etc.

# Puzzle ideas
* Include hexagonal cells as 3-way intersections
* Other tilings
* Tilings of the sphere/hyperbolic disk?
* Full 15x15 block (or at least n x n)
* All clues are questions (something clever); puzzle design is a question mark; entries are themed to be question-related

# License
Currently MsFit is a private project.

# Credits
Software engineering-wise, I took organizational inspiration from [Geometry Central](https://github.com/nmwsharp/geometry-central) and [Polyscope](https://github.com/nmwsharp/polyscope).