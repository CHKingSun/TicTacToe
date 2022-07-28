// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTUserWidget.h"

UTTTUserWidget::UTTTUserWidget() : GameState(ETTTGameState::NotInitialize)
{
}

void UTTTUserWidget::StartGame(ETTTGameState NewGameState)
{
	if (GameState != ETTTGameState::NotInitialize)
	{
		return;
	}

	GameState = NewGameState;
}
