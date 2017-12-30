#pragma once
#include "FBullCowGame.h"
#include <map>

// to make syntax Unreal friendly
#define TMap std::map 
using int32 = int;

FBullCowGame::FBullCowGame() { Reset(); } // default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }
int32 FBullCowGame::GetMaxTries() const 
{
	TMap<int32, int32> WordLengthToMaxTries 
	{
		{ 3, 4 },
		{ 4, 7 },	
		{ 5, 10 },
		{ 6, 16 },
		{ 7, 20 }
	};
	return WordLengthToMaxTries[MyHiddenWord.length()]; 
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	// if the guess isn't an isogram, return an error
	if (!IsIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram;
	}
	// if the guess isn't all lowercase, return error
	else if (!IsLowercase(Guess))
	{
		return EGuessStatus::Not_Lowercase;
	}
	// if the guess length is wrong, return error
	else if (Guess.length() != GetHiddenWordLength())
	{
		return EGuessStatus::Wrong_Length;
	}
	// otherwise, return OK
	else
	{
		return EGuessStatus::OK;
	}
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "planet"; // this MUST be an isogram
	
	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTry = 1;
	bGameIsWon = false;

	return;
}

// receives a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length();

	// loop through all letters in the hidden word	
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		// compare letters agaisnt the hidden guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar])
			{
				if (MHWChar == GChar)
				{
					// increment bulls if they're in the same place
					BullCowCount.Bulls++;
				}
				else
				{
					// increment cows if not
					BullCowCount.Cows++;
				}
			}
		}
	}

	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}

	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1)
	{
		return true;
	}

	// setup our map
	TMap<char, bool> LetterSeen;

	// loop thorugh all the letters of the word
	for (auto Letter : Word)
	{
		Letter = tolower(Letter); // handle mixed case

		// if the letter is in the map
		if (LetterSeen[Letter])
		{
			return false; // we do NOT have and isogram
		}
		else
		{
			LetterSeen[Letter] = true; // add the letter to the map as seen
		}
	}

	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter))
		{
			return false;
		}
	}

	return true;
}
