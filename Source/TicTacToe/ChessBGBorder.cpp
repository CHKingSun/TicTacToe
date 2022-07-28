// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBGBorder.h"

FIntVector2 UChessBGBorder::GetIndexByPosition(const FVector2D& Position)
{
	FVector2D Index = (Position - StartOffset) / GridSize;
	return FIntVector2(Index.X < 0 ? -1 : int32(Index.X), Index.Y < 0 ? -1 : int32(Index.Y));
}

FVector2D UChessBGBorder::GetGridPosition(const FIntVector2& Index)
{
	return StartOffset + FVector2D(Index.X, Index.Y) * GridSize;
}
