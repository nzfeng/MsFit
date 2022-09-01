import glob

WANTED_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

##### I/O

def readEntries(filepath):
	'''
	Read the newline-separated entries in the given .txt file.
	Return a list of these entries, preserving their original forms.
	'''

	f = open(filepath, 'r')
	words = f.read().splitlines() 
	f.close()
	
	return words


def getAllEntries():
	'''
	Get all entries in the MsFit dataset.
	'''
	files = glob.glob('../data/*.txt')

	words = []
	for filepath in files:
		words += readEntries(filepath)

	return words

##### STANDARDIZATION

def depunctuate(string):
	'''
	Remove all non-alphabetical characters from the given string.
	'''
	newWord = ''.join(i for i in string if i in WANTED_CHARS)
	return newWord


def standardize(string):
	return depunctuate(string.upper());