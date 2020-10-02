// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordsList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);

    SetupGame();

    // PrintLine(TEXT("%i"), FMath::RandRange(0, 10));    
    // PrintLine(TEXT("The number of possible words in the given word list is: %i"), Words.Num());
    // PrintLine(TEXT("The number of valid words in the Word List are: %i"), GetValidWords(Words).Num());
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver) {
        ClearScreen();
        SetupGame();
    }
    else {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame() {
    PrintLine(TEXT("\tWelcome to the \"Bull-Cows\" Game\n"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Clue :\n The Hidden Word is %i characters long!\n"), HiddenWord.Len());
    PrintLine(TEXT("Guess the Hidden Word and,\nPress Enter to Continue..."));
    // PrintLine(FString::Printf(TEXT("The Hidden Word is: %s"), *HiddenWord));  //debug line.
    PrintLine(TEXT("Total Lives: %i"), Lives);
}

void UBullCowCartridge::EndTheGame() {
    bGameOver = true;
    PrintLine(TEXT("Press Enter to Play Again..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    if (Guess == HiddenWord) {
        PrintLine(TEXT("You Win, Your Guess was Correct!"));
        EndTheGame();
        return;
    }
    if (Guess.Len() != HiddenWord.Len()) {        
        PrintLine(TEXT("The Hidden Word is %i characters long!"), HiddenWord.Len());
        if (!IsIsogram(Guess)) {
            PrintLine(TEXT("Also, No Repeating Letters are Allowed!"));    
        }
        PrintLine(TEXT("Try Guessing Again,\n You have %i Lives remaining!"), Lives);
        return;
    }
    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("The Number of Letters are Correct But,"));
        PrintLine(TEXT("Repeating Letters are not Allowed, Guess Again!"));
        PrintLine(TEXT("LIVES REMAINING: %i"), Lives);
        return;
    }

    PrintLine(TEXT("LOST A LIFE!!"));
    --Lives;

    if (Lives <= 0) {
        ClearScreen();
        PrintLine(TEXT("You Lost!"));
        PrintLine(TEXT("The Hidden Word was : %s"), *HiddenWord);
        EndTheGame();    
        return;              
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess Again, You have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& testString) const{
    for (int32 j = 0;j < testString.Len();j++) {
        for (int32 i = j + 1;i < testString.Len();i++) {
            if (testString[i] == testString[j]) {
                return false;
            }
        }
    }
    return true;
}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const {
    TArray<FString> ValidWords;
    
    for (FString WordCheck : WordList) {
        if (WordCheck.Len() >= 4 && WordCheck.Len() <= 8 && IsIsogram(WordCheck)) {
            ValidWords.Emplace(WordCheck);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const{

    FBullCowCount Count;

    // for every index Guess same as index Hidden, BullCount++
    // if not a bull, was it a cow? If yes, CowCount++

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) {
                Count.Cows++;
                break;
            }
        }  
    }
    return Count;
}