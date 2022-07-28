// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "TTTUserWidget.generated.h"


UENUM()
enum class ETTTGameState : uint8
{
	NotInitialize,
	Pause,
	Playing,
	Waiting,
};


/**
 * 
 */
UCLASS()
class TICTACTOE_API UTTTUserWidget : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	ETTTGameState GameState;

public:
	UTTTUserWidget();
	
	UFUNCTION(BlueprintCallable)
	void StartGame(ETTTGameState NewGameState);
};
