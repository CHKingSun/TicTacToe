// Fill out your copyright notice in the Description page of Project Settings.


#include "OwnwePawn.h"

void AOwnerPawn::OnGridChoose(int32 GridPos)
{
	if (OnPiecePosChecked.IsBound())
	{
		OnPiecePosChecked.Broadcast(GridPos, PieceState);
	}
}
