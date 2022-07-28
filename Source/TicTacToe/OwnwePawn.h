// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePawn.h"
#include "OwnwePawn.generated.h"


/**
 * 
 */
UCLASS()
class TICTACTOE_API AOwnerPawn : public AGamePawn
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnGridChoose(int32 GridPos);
};
