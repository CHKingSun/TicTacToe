// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPawn.h"

#include "TTTUIManager.h"
#include "TTTUserWidget.h"


void APCPawn::OnPlayStateChange(EPlayingState NewPlayState)
{
	Super::OnPlayStateChange(NewPlayState);

	if (NewPlayState != EPlayingState::Waiting) return;

	auto GameUI = Cast<UTTTUserWidget>(UTTTUIManager::Get()->GetWidget(GetGameInstance(), "GameUI"));
	int32 Index = GameUI->GetAvailablePos(PieceState);
	if (Index == -1) return;
	
	if (OnPiecePosChecked.IsBound())
	{
		OnPiecePosChecked.Broadcast(Index, PieceState);
	}
}
