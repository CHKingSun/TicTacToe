// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTUserWidget.h"

#include "TTTUIManager.h"

UTTTUserWidget::UTTTUserWidget() : GameState(ETTTGameState::NotInitialize)
{
}

void UTTTUserWidget::Reset()
{
	GameState = ETTTGameState::NotInitialize;
}

void UTTTUserWidget::StartGame(EPlayingState NewPlayState)
{
	if (GameState != ETTTGameState::NotInitialize)
	{
		return;
	}

	GameState = ETTTGameState::Start;
	if (NewPlayState == EPlayingState::Playing)
	{
		PlayerPlaying();
	} else if (NewPlayState == EPlayingState::Waiting)
	{
		PlayerWaiting();
	}
}

void UTTTUserWidget::PauseGame()
{
	GameState = ETTTGameState::Pause;

	UTTTUIManager::Get()->GetWidget(GetGameInstance(), "PauseUI");
}

void UTTTUserWidget::ResumeGame()
{
	GameState = ETTTGameState::Start;
}

void UTTTUserWidget::PlayerPlaying()
{
	PlayState = EPlayingState::Playing;
}

void UTTTUserWidget::PlayerWaiting()
{
	PlayState = EPlayingState::Waiting;
}
