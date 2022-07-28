// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TTTUIManager.generated.h"


class UUserWidgetBase;


USTRUCT(BlueprintType)
struct FWidgetPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidgetBase> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidgetBase* Widget;

	FWidgetPair() : Widget(nullptr) {}
};


/**
 * 
 */
UCLASS()
class TICTACTOE_API UTTTUIManager : public UObject
{
	GENERATED_BODY()

public:
	UTTTUIManager() {}

	UFUNCTION(BlueprintCallable)
	static UTTTUIManager* Get();

	UFUNCTION(BlueprintCallable)
	void BindWidgetClass(const FString& WidgetName, TSubclassOf<UUserWidgetBase> InWidgetClass);

	UFUNCTION(BlueprintCallable)
	UUserWidgetBase* GetWidget(UGameInstance* Instance, const FString& WidgetName);

	UFUNCTION(BlueprintCallable)
	void RemoveWidget(const FString& WidgetName);

	UFUNCTION(BlueprintCallable)
	void HideWidget(const FString& WidgetName);

protected:
	UPROPERTY()
	TMap<FString, FWidgetPair> AllWidgets;
	
	static UTTTUIManager* Singleton;
};
