// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTUserWidget.h"

#include "ChessBGBorder.h"
#include "OwnwePawn.h"
#include "PCPawn.h"
#include "TTTUIManager.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"


FEventReply UTTTUserWidget::OnBGMouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (GameState != ETTTGameState::Start || PlayState != EPlayingState::Playing) return FEventReply(false);
	
	FVector2D Pos = MouseEvent.GetScreenSpacePosition() - MyGeometry.GetAbsolutePosition();
	float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	FIntVector2 IndexVector = ImgBG->GetIndexByPosition(Pos / Scale);
	if (IndexVector.X < 0 || IndexVector.X >= ChessCol || IndexVector.Y < 0 || IndexVector.Y >= ChessRow) return FEventReply(true);;
	uint32 Index = IndexVector.X + IndexVector.Y * ChessCol;
	if (PieceData[Index].State != EPieceState::Empty)
	{
		OnGridInValidTips();
		return  FEventReply(true);
	}
	
	if (OnGridChoose.IsBound())
	{
		OnGridChoose.Broadcast(Index);
	}
	
	return FEventReply(true);
}

FEventReply UTTTUserWidget::OnBGMouseMove(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (GameState != ETTTGameState::Start || PlayState != EPlayingState::Playing) return FEventReply(false);
	// FVector2D Pos = MouseEvent.GetScreenSpacePosition() - MyGeometry.GetAbsolutePosition();
	// UKismetSystemLibrary::PrintString(GetWorld(), Pos.ToString());
	
	return FEventReply(true);
}

bool UTTTUserWidget::CheckEnd()
{
	constexpr static int32 Indices[][3] {
		{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
		{0, 3, 6}, {1, 4, 7}, {2, 5, 8},
		{0, 4, 8}, {2, 4 ,6}
	};

	for (const auto& Index : Indices)
	{
		if (PieceData[Index[0]].State != EPieceState::Empty && PieceData[Index[0]].State == PieceData[Index[1]].State && PieceData[Index[1]].State == PieceData[Index[2]].State)
		{
			if (PlayState == EPlayingState::Playing)
			{
				// 赢
				OnGameEnd(1);
			} else
			{
				// 输
				OnGameEnd(-1);
			}
			return true;
		}
	}

	for (const auto& Data : PieceData)
	{
		if (Data.State == EPieceState::Empty) return false;
	}

	// 平局
	OnGameEnd(0);
	return true;
}

void UTTTUserWidget::RemoveFromParent()
{
	for (const auto& [_, Image] : ImgPieces)
	{
		Image->RemoveFromParent();
	}
	
	Super::RemoveFromParent();
}

UTTTUserWidget::UTTTUserWidget() : GameState(ETTTGameState::NotInitialize), ChessCol(3), ChessRow(3)
{
}

void UTTTUserWidget::Reset()
{
	GameState = ETTTGameState::NotInitialize;

	ImgBG->OnMouseButtonUpEvent.BindDynamic(this, &UTTTUserWidget::OnBGMouseButtonUp);
	ImgBG->OnMouseMoveEvent.BindDynamic(this, &UTTTUserWidget::OnBGMouseMove);

	PieceData.Init({}, ChessCol * ChessRow);
	uint32 Index = 0;  // Index = X + Y * ChessCol
	for (auto& Data : PieceData)
	{
		Data.Index = Index;
		Data.State = EPieceState::Empty;
		++Index;
	}

	for (auto& [_, Image] : ImgPieces)
	{
		if (Image) Image->RemoveFromParent();
	}
	ImgPieces.Empty();
}

void UTTTUserWidget::StartGame(EPlayingState NewPlayState)
{
	if (GameState != ETTTGameState::NotInitialize)
	{
		return;
	}

	Reset();

	GameState = ETTTGameState::Start;
	if (NewPlayState == EPlayingState::Playing)
	{
		PlayerPlaying();
	} else if (NewPlayState == EPlayingState::Waiting)
	{
		PlayerWaiting();
	}
}

void UTTTUserWidget::PauseGame()
{
	GameState = ETTTGameState::Pause;

	UTTTUIManager::Get()->GetWidget(GetGameInstance(), "PauseUI");
}

void UTTTUserWidget::ResumeGame()
{
	GameState = ETTTGameState::Start;
}

void UTTTUserWidget::PlayerPlaying()
{
	PlayState = EPlayingState::Playing;

	TxtMsg->SetText(FText::FromString(L"你的回合"));
}

void UTTTUserWidget::PlayerWaiting()
{
	PlayState = EPlayingState::Waiting;

	TxtMsg->SetText(FText::FromString(L"对面回合"));
}

void UTTTUserWidget::OnPiecePosChecked(int32 GridPos, EPieceState PieceState, EPlayingState NextState)
{
	FIntVector2 IndexVector = FIntVector2(GridPos % ChessCol, GridPos / ChessCol);
	auto Pos = ImgBG->GetGridPosition(IndexVector);

	if (PieceData[GridPos].State != EPieceState::Empty) return;
	
	auto ImgGrid = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), FName(FString("ImgGrid") + FString::FromInt(GridPos)));
	auto PanelSlot = Cast<UCanvasPanelSlot>(RootPanel->AddChild(ImgGrid));
	PanelSlot->SetAutoSize(true);
	// PanelSlot->SetAlignment(FVector2D(0.5, 0.5));
	PanelSlot->SetPosition(Pos);
	UTexture2D* Tex = nullptr;
	if (PieceState == EPieceState::Black) Tex = TexBlack;
	else if (PieceState == EPieceState::White) Tex = TexWhite;
	ImgGrid->SetBrushFromTexture(Tex, true);
	ImgGrid->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	PieceData[GridPos].State = PieceState;
	ImgPieces.Add(GridPos, ImgGrid);

	if (CheckEnd()) return;

	if (NextState == EPlayingState::Playing) PlayerPlaying();
	else if (NextState == EPlayingState::Waiting) PlayerWaiting();
}

void UTTTUserWidget::BindPawn(AOwnerPawn* Owner, APCPawn* PC)
{
	OnGridChoose.AddDynamic(Owner, &AOwnerPawn::AOwnerPawn::OnGridChoose);

	Owner->OnPiecePosChecked.Clear();
	Owner->OnPiecePosChecked.AddDynamic(this, &UTTTUserWidget::OnPiecePosChecked);
	Owner->OnPiecePosChecked.AddDynamic(PC, &APCPawn::OnPCTurn);

	PC->OnPiecePosChecked.Clear();
	PC->OnPiecePosChecked.AddDynamic(this, &UTTTUserWidget::OnPiecePosChecked);
}

int32 UTTTUserWidget::GetAvailablePos()
{
	for (const auto& Data : PieceData)
	{
		if (Data.State == EPieceState::Empty) return Data.Index;
	}

	return -1;
}
