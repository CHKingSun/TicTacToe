// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "ChessBGBorder.generated.h"


/**
 * 
 */
UCLASS()
class TICTACTOE_API UChessBGBorder : public UBorder
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector2D StartOffset;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector2D GridSize;

public:

	FIntVector2 GetIndexByPosition(const FVector2D& Position);

	FVector2D GetGridPosition(const FIntVector2& Index);
};
