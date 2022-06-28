from utils import *


def getRemainingPrefilter(datasetName):
	'''
	Get the remaining words in the given dataset that I haven't filtered yet.
	'''

	files = glob.glob('./prefilter_data/%s*.txt' %datasetName)

	allWords = []
	for file in files:
	    f = open(file, 'r')
	    lines = f.read().splitlines() 
	    f.close()
	    allWords += lines

	allWords = removeDuplicates(allWords)
	return allWords


def filterPreviousWords(dataset):
	'''
	Given a dataset <dataset>, remove all duplicates, already-filtered words, and words I've already looked at & filtered out.
	'''

	print(len(dataset))
	# Assume I've already looked at all of WordNet.
	# TODO: Perhaps better to compare to the specific version of WordNet I looked at, since WordNet gets updated.
	wordnet = getWordNetWords() 
	filtered = getFilteredWords()
	wordlist = removeDuplicates(wordnet+filtered)
	
	dedup = removeEntries(dataset, wordlist)
	print(len(dedup))
	return dedup

def outputAlphabetizedFiltered():
	'''
	Get all filtered words currently, and output them in alphabetized order into a txt file.
	'''

	wordset = getFilteredWords()

	# Build map from standardized versions of entries to the original entries.
	(wordMap, stdList) = buildStandardizedDictAndWordlist(wordset)

	# Remove duplicates.
	stdList = set(stdList)

	stdList = sorted(stdList) # alphabetized

	with open('./alphabetized-entries.txt', 'w') as f:
		for word in stdList:
			f.write(wordMap[word]) # write original entry
			f.write("\n")

def main():
	# # Once I've filtered all of WordNet, remove all duplicates (and currently-sorted filtered words) from the 
	# # remaining Broda words that I haven't filtered yet.
	# restBroda = filterPreviousWords(getRemainingPrefilter("broda"))
	# saveRawWords(restBroda, dataset_name="broda", incr=1000, offset=0, toSort=True)

	# TODO: Need to get Broda words 3-4 letters

	# saveFilteredWords()

	# harringtonRaw = filterPreviousWords(getRemainingPrefilter("harrington-nouns"));
	# saveRawWords(harringtonRaw, dataset_name="harrington-nouns", incr=4000, offset=0)

	outputAlphabetizedFiltered()

if __name__=="__main__":
	main()