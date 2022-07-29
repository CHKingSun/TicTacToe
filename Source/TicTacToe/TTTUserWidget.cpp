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
		ShowInvalidTips(L"该格子已被占领！");
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

	FVector2D Pos = (MouseEvent.GetScreenSpacePosition() - MyGeometry.GetAbsolutePosition()) / UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	ImgCursor->SetRenderTranslation(Pos);
	if (!ImgCursor->IsVisible()) ImgCursor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FIntVector2 IndexVector = ImgBG->GetIndexByPosition(Pos);

	uint32 Index = IndexVector.X + IndexVector.Y * ChessCol;
	bool bShowImgGrid = IndexVector.X >= 0 && IndexVector.X < ChessCol && IndexVector.Y >= 0 && IndexVector.Y < ChessRow
					&& PieceData[Index].State == EPieceState::Empty;
	if (bShowImgGrid)
	{
		ImgHint->SetRenderTranslation(ImgBG->GetGridPosition(IndexVector));
		if (!ImgHint->IsVisible()) ImgHint->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	} else if (ImgHint->IsVisible())
	{
		ImgHint->SetVisibility(ESlateVisibility::Hidden);
	}
	
	return FEventReply(true);
}

bool UTTTUserWidget::CheckEnd(bool Notify)
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
				if (Notify) OnGameEnd(1);
			} else
			{
				// 输
				if (Notify) OnGameEnd(-1);
			}
			return true;
		}
	}

	for (const auto& Data : PieceData)
	{
		if (Data.State == EPieceState::Empty) return false;
	}

	// 平局
	if (Notify) OnGameEnd(0);
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

UTTTUserWidget::UTTTUserWidget() : ChessRow(3), ChessCol(3), GameState(ETTTGameState::NotInitialize), CanUndo(true)
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

	PlaySteps.Empty();
}

void UTTTUserWidget::StartGame(EPlayingState NewPlayState, EGameDifficulty NewDifficulty)
{
	if (GameState != ETTTGameState::NotInitialize)
	{
		return;
	}

	Reset();

	GameState = ETTTGameState::Start;
	Difficulty = NewDifficulty;
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

	// UTTTUIManager::Get()->GetWidget(GetGameInstance(), "PauseUI");
}

void UTTTUserWidget::ResumeGame()
{
	GameState = ETTTGameState::Start;
}

void UTTTUserWidget::PlayerPlaying()
{
	PlayState = EPlayingState::Playing;

	TxtMsg->SetText(FText::FromString(L"你的回合"));
	if (OnPlayStateChange.IsBound()) OnPlayStateChange.Broadcast(PlayState);
}

void UTTTUserWidget::PlayerWaiting()
{
	PlayState = EPlayingState::Waiting;

	TxtMsg->SetText(FText::FromString(L"对面回合"));
	if (OnPlayStateChange.IsBound()) OnPlayStateChange.Broadcast(PlayState);
}

void UTTTUserWidget::OnPiecePosChecked(int32 GridPos, EPieceState PieceState)
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

	PlaySteps.Add({GridPos, PlayState});
	if (PlayState == EPlayingState::Playing) CanUndo = true;

	if (CheckEnd()) return;

	if (PlayState == EPlayingState::Waiting) PlayerPlaying();
	else if (PlayState == EPlayingState::Playing) PlayerWaiting();
}

void UTTTUserWidget::BindPawn(AOwnerPawn* Owner, APCPawn* PC)
{
	OnGridChoose.AddDynamic(Owner, &AOwnerPawn::OnGridChoose);
	OnPlayStateChange.AddDynamic(PC, &APCPawn::OnPlayStateChange);

	Owner->OnPiecePosChecked.Clear();
	Owner->OnPiecePosChecked.AddDynamic(this, &UTTTUserWidget::OnPiecePosChecked);

	if (Owner->GetPieceSate() == EPieceState::Black)
	{
		ImgCursor->SetBrushFromTexture(TexBlack, true);
	} else
	{
		ImgCursor->SetBrushFromTexture(TexWhite, true);
	}

	PC->OnPiecePosChecked.Clear();
	PC->OnPiecePosChecked.AddDynamic(this, &UTTTUserWidget::OnPiecePosChecked);
}

int32 UTTTUserWidget::GetAvailablePos(EPieceState InState)
{
	TArray<int32> AvailableIndices;
	AvailableIndices.Reserve(PieceData.Num());
	for (const auto& Data : PieceData)
	{
		if (Data.State == EPieceState::Empty) AvailableIndices.Add(Data.Index);
	}

	if (AvailableIndices.Num() == 0) return -1;

	if (Difficulty == EGameDifficulty::Diffcult)
	{
		for (const auto& Index : AvailableIndices)
		{
			PieceData[Index].State = InState;
			if (CheckEnd(false))
			{
				PieceData[Index].State = EPieceState::Empty;
				return Index;
			}
			PieceData[Index].State = EPieceState::Empty;
		}
	}

	if (Difficulty == EGameDifficulty::Normal || Difficulty == EGameDifficulty::Diffcult)
	{
		const EPieceState AnimyState = InState == EPieceState::Black ? EPieceState::White : EPieceState::Black;
		for (const auto& Index : AvailableIndices)
		{
			PieceData[Index].State = AnimyState;
			if (CheckEnd(false))
			{
				PieceData[Index].State = EPieceState::Empty;
				return Index;
			}
			PieceData[Index].State = EPieceState::Empty;
		}
	}

	return AvailableIndices[FRandomStream(UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld())).RandHelper(AvailableIndices.Num())];
}

bool UTTTUserWidget::CheckUndo()
{
	if (GameState != ETTTGameState::Start) return false;
	
	if (!CanUndo)
	{
		ShowInvalidTips(L"只允许撤回一步哦");
		return false;
	}

	int32 LastStep = -1;
	for (int32 Index = PlaySteps.Num() - 1; Index >= 0; --Index)
	{
		if (PlaySteps[Index].Value == EPlayingState::Playing)
		{
			LastStep = Index;
			break;
		}
	}

	if (LastStep == -1)
	{
		ShowInvalidTips(L"你还未下棋！");
		return false;
	}

	return true;
}

void UTTTUserWidget::UndoLastStep()
{
	CanUndo = false;
	for(int32 Index = PlaySteps.Num() - 1; Index >= 0; --Index)
	{
		int32 Pieceindex = PlaySteps[Index].Key;

		UImage* Image = nullptr;
		if (ImgPieces.RemoveAndCopyValue(Pieceindex, Image)) Image->RemoveFromParent();
		PieceData[Pieceindex].State = EPieceState::Empty;
		
		if (PlaySteps[Index].Value == EPlayingState::Playing) break;
	}
}
