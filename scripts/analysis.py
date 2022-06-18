from utils import *
import copy

# TODO: Port this to the core C++ code, so that patterns are not static, but can also be computed according to whatever
# wordlists the user has additionally imported.

ALL_LETTERS = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", 
			   "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"]

ALL_LETTER_PAIRS = ["%s%s" %(i,j) for i in ALL_LETTERS for j in ALL_LETTERS]

def unusedLetterPairs(wordSet, location="beginning"):
	'''
	Out of all 26^2 possible two-letter combinations, return the set of pairs that *don't* appear at the 
	[beginning | middle | end] of any word in the English language.
	'''

	start = 0
	end = 2
	if location == "end":
		start = -2
		end = None
	# TODO: middle

	unusedPairs = copy.deepcopy(ALL_LETTER_PAIRS)
	for word in wordSet:
		std = standardize(word)
		pair = std[start:end]
		if pair in unusedPairs: unusedPairs.remove(pair)

	return unusedPairs

def printPairsInMapForm(pairs):
	'''
	Given a list of unused letter pairs, determine map from 
	[letter + position] -> [unused letter in the empty position].
	'''
	mapToLetters = {}
	for pair in pairs:
		key0 = pair[0] + "."
		key1 = "." + pair[1]

		if key0 in mapToLetters: mapToLetters[key0].add(pair[1]);
		else: mapToLetters[key0] = {pair[1]};

		if key1 in mapToLetters: mapToLetters[key1].add(pair[0]);
		else: mapToLetters[key1] = {pair[0]};

	for key in mapToLetters:
		unusedLetters = ""
		for letter in mapToLetters[key]:
			unusedLetters += letter
		print("{\"%s\", \"%s\"}," %(key, unusedLetters))

def printPairsInRegexForm(pairs):
	'''
	Given a list of unused letter pairs, print the regex pattern for allowable characters
	in the empty position. 
	'''
	mapToLetters = {}
	for pair in pairs:
		key0 = pair[0] + "."
		key1 = "." + pair[1]

		if key0 in mapToLetters: mapToLetters[key0].add(pair[1]);
		else: mapToLetters[key0] = {pair[1]};

		if key1 in mapToLetters: mapToLetters[key1].add(pair[0]);
		else: mapToLetters[key1] = {pair[0]};

	for key in mapToLetters:
		unusedLetters = mapToLetters[key]
		nUnused = len(unusedLetters)

		regex_pattern = ""
		if (nUnused == 0):
			regex_pattern = "."
		elif (nUnused == 26):
			# https://stackoverflow.com/a/940840
			regex_pattern = "[a^]"
		elif (nUnused <= 13):
			regex_pattern = "[^"
			for letter in unusedLetters:
				regex_pattern += letter
			regex_pattern += "]"
		else:
			usedLetters = set(ALL_LETTERS) - unusedLetters
			regex_pattern = "["
			for letter in usedLetters:
				regex_pattern += letter
			regex_pattern += "]"

		print("{\"%s\", \"%s\"}," %(key, regex_pattern))


def main():
	# allWords = getAllWords();
	# print("Total number of words: %d" %len(allWords))

	allWords = getFilteredWords()
	print('{:<} {:>12}'.format("Number of filtered words:", len(allWords)))
	unusedBeginningPairs = unusedLetterPairs(allWords, "beginning")
	unusedEndPairs = unusedLetterPairs(allWords, "end")
	print('{:<} {:>7}'.format("Number unused beginning pairs:", len(unusedBeginningPairs)))
	print('{:<} {:>10}'.format("Number unused ending pairs:", len(unusedEndPairs)))

	printPairsInRegexForm(unusedBeginningPairs)
	printPairsInRegexForm(unusedEndPairs)

if __name__=="__main__":
	main()