// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "TTTUserWidget.generated.h"


UENUM()
enum class ETTTGameState : uint8
{
	NotInitialize,
	Start,
	Pause,
};


UENUM()
enum class EPlayingState : uint8
{
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

	UPROPERTY(BlueprintReadWrite)
	EPlayingState PlayState;

public:
	UTTTUserWidget();

	UFUNCTION(BlueprintCallable)
	void Reset();
	
	UFUNCTION(BlueprintCallable)
	void StartGame(EPlayingState NewPlayState);

	UFUNCTION(BlueprintCallable)
	void PauseGame();

	UFUNCTION(BlueprintCallable)
	void ResumeGame();

	UFUNCTION(BlueprintCallable)
	void PlayerPlaying();

	UFUNCTION(BlueprintCallable)
	void PlayerWaiting();
};
