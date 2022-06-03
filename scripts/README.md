A bunch of scripts to help me parse and organize datasets.

* Use Python to extract words from various sources (WordNet, etc.) Output into files organized by word length (get rid of all punctuation.) TODO: Separate wordlist for punctation; user can load in the punctuated wordlist if they want. 
* Manually purge unwanted words from the output wordlists.
* For each scraped dataset, combine their output, organizing by length.
* TODO: Perhaps organize words in each file by some scoring system.

# Update
After attempting to filter some NLP and crossword datasets, I quickly realized that the main challenge of crossword construction is having a good dataset. I think most of the demand for human crossword constructors, after supplying any theme entries, is actually just to guide construction towards having quality fills. Experienced constructors are very in tune with what makes a good crossword, and how to make decisions towards a feasible crossword. However, their process seems to be ad hoc and kind of an art. 

I realized that if I want to make good crosswords, I need to have a good dataset that has been curated manually. At least the estimated number of words and phrases to sift through is on the order of 10^5, so manual curation is at least feasible (and by only one person.) What makes a "good" wordlist is subjective, of course. In my case, I want to focus on common American English entries, with as few names/places/pop culture references as possible. Upon some deliberation, here is how I decided to categorize words:

* `pure`: Loosely defined as "ordinary" words that an American would likely be familiar with. Common knowledge; words you would find in any dictionary. Aim for "lemmatized" words, i.e. the root word and not derivatives. If we wanted to allow derivative words, in theory we could use WordNet or other NLP tools to help us look them up.
* `padding`: Well-known short words, abbreviations, acronyms, and other crossword-ese that may be needed in a pinch to help fill out a puzzle (e.g. "CIA", "FBI", "HOA", "ABC".) Want to stay away from typical crossword-ese like "ERN", "ENO", etc. though. 
* `people`: Includes names of well-known people (scientists, writers, artists, U.S. Presidents, etc.) that an American would likely be familiar with. One could argue that names of musicians/artists could belong in `culture`, but I really want to separate "things" vs. people", which would be useful for people like me who think of the world more in terms of ideas/objects rather than people (and for people who think the opposite way! We could make a more people-focused puzzle for them.)
* `place`: Geographic locations (countries, major cities, major parks). Also includes museums, famous buildings, famous streets.
* `phrase`: Phrases and idioms, with bias towards American ones. Debating on whether to include song lyrics and lines from poems.
* `pop`: Includes terms relating to (pop) culture: culturally significant brand names, song names, movie titles, art, etc. (e.g. "Coca-Cola", "Mona Lisa"). Trivia about a particular topic that wouldn't be known to an absolute beginner is also included in here, even if the topic itself is common knowledge; for example, "baseball" would be in `pure`, but baseball positions like "shortstop" would be considered `pop`.

Process:
* Combine words from a bunch of datasets (WordNet, Broda \[good for phrases\], Oxford English Dictionary.)
* Get rid of duplicates (compare their de-punctuated, capitalized versions.) 
* Output original words to a bunch of files containing 1000 words each, each titled "scripts/raw_data/words_%n.txt".
* Manually go through the entries in each file; when done, add words to a single txt file "scripts/filter_data/raw_wordlist.txt", separated by newline. For each word, add ";\[category\]" after it (optional for now.)
* Save de-punctuated, capitalized words from "raw_wordlist.txt" into files sorted by category and wordlength (in that order of hierarchy) under "data" (this is the final version.)
