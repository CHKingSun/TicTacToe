// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "PieceData.h"
#include "TTTUserWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridChooseEvent, int32, GridPos);


/**
 * 
 */
UCLASS()
class TICTACTOE_API UTTTUserWidget : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	int32 ChessRow;
	UPROPERTY()
	int32 ChessCol;
	
	UPROPERTY(BlueprintReadWrite)
	ETTTGameState GameState;

	UPROPERTY(BlueprintReadWrite)
	EPlayingState PlayState;

	UPROPERTY()
	TArray<FPieceStruct> PieceData;

	UPROPERTY(BlueprintReadOnly)
	TMap<int32, class UImage*> ImgPieces;

	UPROPERTY(BlueprintReadWrite)
	class UChessBGBorder* ImgBG;

	UPROPERTY(BlueprintReadWrite)
	class UPanelWidget* RootPanel;

	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* TxtMsg;

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* TexBlack;
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* TexWhite;

	

public:

	UPROPERTY(BlueprintAssignable)
	FGridChooseEvent OnGridChoose;

protected:

	UFUNCTION()
	FEventReply OnBGMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
	FEventReply OnBGMouseMove(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION(BlueprintCallable)
	bool CheckEnd(bool Notify = true);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGridInValidTips();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameEnd(int32 Result);


public:
	UTTTUserWidget();
	
	virtual void RemoveFromParent() override;

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

	UFUNCTION(BlueprintCallable)
	void OnPiecePosChecked(int32 GridPos, EPieceState PieceState, EPlayingState NextState);

	UFUNCTION(BlueprintCallable)
	void BindPawn(class AOwnerPawn* Owner, class APCPawn* PC);

	UFUNCTION()
	int32 GetAvailablePos(EPieceState InState);
};
