// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTOE_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FString WidgetName;

public:
	const FString& GetWidgetName()const { return WidgetName; }
	void SetWidgetName(const FString& WName) { WidgetName = WName; }
};
