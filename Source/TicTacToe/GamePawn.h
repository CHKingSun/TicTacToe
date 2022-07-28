// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PieceData.h"
#include "GamePawn.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPiecePosCheckedEvent, int32, GridPos, EPieceState, PieceState);


UCLASS()
class TICTACTOE_API AGamePawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EPieceState PieceState;

public:
	FPiecePosCheckedEvent OnPiecePosChecked;

public:
	// Sets default values for this pawn's properties
	AGamePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EPieceState GetPieceSate()const { return PieceState; }
};
