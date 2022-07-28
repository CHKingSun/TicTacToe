// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPawn.h"

#include "TTTUIManager.h"
#include "TTTUserWidget.h"

void APCPawn::OnPCTurn(int32 GridPos, EPieceState PState, EPlayingState NextState)
{
	auto GameUI = Cast<UTTTUserWidget>(UTTTUIManager::Get()->GetWidget(GetGameInstance(), "GameUI"));
	int32 Index = GameUI->GetAvailablePos();
	if (Index == -1) return;
	
	if (OnPiecePosChecked.IsBound())
	{
		OnPiecePosChecked.Broadcast(Index, PieceState, EPlayingState::Playing);
	}
}
