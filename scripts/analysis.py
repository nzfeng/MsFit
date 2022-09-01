from utils import *

# Ideas:
#	- Enumerate legal 2- and 3-letter combos ("units"?) in syllables (idk what the proper term is -- not phonemes/morphemes)
# 	- Avoid repeating "units" within, say, a 10 x 10 block.

def getLetterCombos():
	'''
	Generate all legal 2- and 3-letter combinations in English words.
	'''

	words = getAllEntries()
	# TODO: analyse current dataset, or try to get an existing list


if __name__=="__main__":
	getLetterCombos()