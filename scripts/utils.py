import numpy as np
import glob
import sys
import time
from itertools import chain

import nltk
nltk.download('wordnet')
nltk.download('omw-1.4')
nltk.download('punkt')
nltk.download('averaged_perceptron_tagger')
nltk.download('maxent_ne_chunker')
nltk.download('words')
from nltk.tokenize import SpaceTokenizer
from nltk.corpus import wordnet as wn

##### A bunch of utility functions.

def depunctuate(string, unwanted_chars="_'.,/;[]\-=`~_+{}|:<>?()@#$%^&* "):
	'''
	Remove all instances of each char in <unwanted_chars> from <word>.
	<unwanted_chars> can be either a string or a list.
	'''

	newWord = ''.join(i for i in string if not i in unwanted_chars)
	return newWord

def isAllDigits(string):
	'''
	Return True if the word consists solely of numbers.
	'''
	for letter in string:
		if letter not in "0123456789":
			return False
	return True

def removeDigits(string):
	'''
	Remove all digits from a string.
	'''

	digits = "1234567890"
	newWord = ''.join(i for i in string if not i in digits)
	return newWord

def standardize(string):
	return depunctuate(removeDigits(string)).upper()


def buildStandardizedDictAndWordlist(wordlist):
	'''
	Build map from standardized versions of entries to the original entries.
	Return this map (a dict), and a list of the standardized entries.
	'''

	# Build map from standardized versions of entries to the original entries.
	wordMap = {}
	stdList = []
	for word in wordlist:
		std = standardize(word)
		wordMap[std] = word
		stdList.append(std)

	return (wordMap, stdList)

def mapFromStandardizedWordist(stdList, wordMap):
	'''
	Given a wordlist <stdList> of standardized entries, and a 
	dictionary <wordMap> mapping standardized entries to their original entries,
	return the mapped <stdList>.
	'''

	wordSet = []
	for word in stdList:
		wordSet.append(wordMap[word])

	return wordSet

def removeDuplicates(wordlist):
	'''
	Remove duplicates from the given wordlist, where duplicates are determined by comparing standardized (de-puncutated) 
	versions; preserve the original format of the word. 
	'''

	# Build map from standardized versions of entries to the original entries.
	(wordMap, stdList) = buildStandardizedDictAndWordlist(wordlist)

	# Remove duplicates.
	stdList = set(stdList)

	# Return original versions of entries.
	return mapFromStandardizedWordist(stdList, wordMap)

def removeEntries(wordlist, setlist):
	'''
	Subtract elements in <setlist> from <wordlist>. Preserve the original formatting of the word.
	'''

	# Lists are passed in by reference in Python...
	stdSetlist = [standardize(word) for word in setlist]

	# Build map from standardized versions of entries to the original entries.
	(wordMap, stdList) = buildStandardizedDictAndWordlist(wordlist)

	# Remove elements.
	newList = set(stdList) - set(stdSetlist)

	# Return original versions of entries.
	wordSet = mapFromStandardizedWordist(newList, wordMap)

	return wordSet

##### INPUT/OUTPUT

def getWordNetWords():
	'''
	Get all words (lemmas) from WordNet.
	'''

	# https://stackoverflow.com/a/33538878
	lemmas_in_wn = set(chain(*[ss.lemma_names() for ss in wn.all_synsets()])) # also gets rid of duplicates
	nLemmas = len(lemmas_in_wn)

	# Get rid of numbers.
	temp = []
	for lemma in lemmas_in_wn:
	    if not isAllDigits(lemma): temp.append(lemma)
	lemmas_in_wn = temp

	# Get rid of words less than 3 chars long.
	lemmas = [word for word in lemmas_in_wn if len(word) > 2]

	# Remove duplicates.
	lemmas = removeDuplicates(lemmas)

	return lemmas


def getBrodaWords():
	'''
	Get all entries from Peter Broda's list. Get only the ones that have been scored 50 or above (i.e. words that are at 
	least decent, or unscored.)
	'''

	broda_list = []
	f = open('./raw_data/peter-broda-wordlist__scored.txt',"r")
	lines = f.read().splitlines() 
	f.close()

	for line in lines:
	    (word, score) = line.split(";")
	    score = int(score)
	    if (len(word) >= 3 and score >= 50): broda_list.append(word)

	# Get rid of duplicates.
	broda_list = removeDuplicates(broda_list)

	return broda_list


def getAllWords(datasets=["wordnet", "broda"]):
	'''
	Get words from the specified datasets.
	'''

	allWords = []
	for datasetName in datasets:
		if datasetName == "wordnet": allWords += getWordNetWords()
		elif datasetName == "broda": allWords += getBrodaWords()

	# De-duplicate.
	allWords = removeDuplicates(allWords)
	return allWords	


def getFilteredWords():
	'''
	Get all words that I have manually filtered, preserving their original forms.
	'''

	files = glob.glob('./filtered_data/*/*.txt', recursive = True)

	allWords = []
	for file in files:
	    f = open(file, 'r')
	    lines = f.read().splitlines() 
	    f.close()
	    allWords += lines

	allWords = removeDuplicates(allWords)
	allWords = [word for word in allWords if len(word) >= 3]
	
	return allWords


def saveRawWords(wordset, dataset_name="broda", incr=1000, offset=0, toSort=False):
	'''
	Save a list of raw words into files of <incr> words each.
	'''
	
	if toSort: wordset = sorted(wordset, key=len)

	nFiles = len(wordset) // incr + 1
	for i in range(nFiles):
		with open('./prefilter_data/%s_%d.txt' %(dataset_name, offset+i), 'w') as f:

			start = incr * i
			end = incr * (i+1)
			if (i==nFiles-1): end = None

			for word in wordset[start:end]:
				f.write(word)
				f.write("\n")

def saveFilteredWords():
	'''
	Save final wordlist, to be loaded into MsFit.

	Save de-punctuated, capitalized words from the filtered lists (the files I manually compiled)
	into files sorted by wordlength, in the "data" directory.
	'''

	files = glob.glob('./filtered_data/*/*.txt', recursive = True)

	all_words = []
	for file in files:
		f = open(file, 'r')
		lines = f.read().splitlines() 
		f.close()
		all_words += lines

	print("Total number of words: %d" %len(all_words))

	all_words = [depunctuate(word).upper() for word in all_words]
	print([word for word in all_words if len(word)<3])
	all_words = [word for word in all_words if len(word) >= 3]
	all_words = set(all_words)
	print("Number of unique words: %d" %len(all_words))

	# No categories for now. Just sort by wordlength.
	wordLengths = [len(word) for word in all_words]
	minLength = min(wordLengths)
	maxLength = max(wordLengths)
	print("min length: %d\t max length: %d" %(minLength,maxLength))

	for i in range(minLength,maxLength+1):
		n = len([word for word in all_words if len(word)==i])
		print("# of words of length %d: %d" %(i, n))

	# Save words.
	for i in range(minLength, maxLength+1):
		filename = "../data/words_%d.txt" %i
		words = [word for word in all_words if len(word)==i]

		with open(filename, 'w') as f:
			for word in words:
				f.write(word)
				f.write("\n")