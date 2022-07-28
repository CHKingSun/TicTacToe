// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePawn.h"
#include "PieceData.h"
#include "PCPawn.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTOE_API APCPawn : public AGamePawn
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnPCTurn(int32 GridPos, EPieceState PState, EPlayingState NextState);
};
