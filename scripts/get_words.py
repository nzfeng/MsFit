from utils import *

def main():
	# Get the remaining WordNet words that I haven't filtered yet, and properly remove duplicates.
	files = glob.glob('./prefilter_data/wordnet_*.txt')

	allWords = []
	for file in files:
	    f = open(file, 'r')
	    lines = f.read().splitlines() 
	    f.close()
	    allWords += lines

	print(len(allWords))
	allWords = removeDuplicates(allWords)
	print(len(allWords))

	# TODO: Once I've filtered all of WordNet, remove all duplicates (and currently-sorted filtered words) from the 
	# remaining Broda words that I haven't filtered yet.

if __name__=="__main__":
	main()