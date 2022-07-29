// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PieceData.generated.h"


UENUM()
enum class ETTTGameState : uint8
{
	NotInitialize,
	Start,
	Pause,
};


UENUM(BlueprintType)
enum class EPlayingState : uint8
{
	Playing,
	Waiting,
};


UENUM()
enum class EPieceState : uint8
{
	Empty,  // 没有被下
	Black,  // 黑棋
	White,  // 白棋
};


UENUM(BlueprintType)
enum class EGameDifficulty : uint8
{
	Easy = 0,
	Normal = 1,
	Diffcult = 2,
};


USTRUCT()
struct FPieceStruct
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 Index;

	UPROPERTY()
	EPieceState State;
};
