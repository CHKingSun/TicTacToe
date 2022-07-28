// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTUIManager.h"
#include "UserWidgetBase.h"
#include "Kismet/GameplayStatics.h"


UTTTUIManager* UTTTUIManager::Singleton = nullptr;


UTTTUIManager* UTTTUIManager::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = NewObject<UTTTUIManager>();
		Singleton->AddToRoot();
	}

	return Singleton;
}

void UTTTUIManager::BindWidgetClass(const FString& WidgetName, TSubclassOf<UUserWidgetBase> InWidgetClass)
{
	auto& [WidgetClass, Widget] = AllWidgets.FindOrAdd(WidgetName);

	WidgetClass = InWidgetClass;
}

UUserWidgetBase* UTTTUIManager::GetWidget(UGameInstance* Instance, const FString& WidgetName)
{
	const auto WidStruct = AllWidgets.Find(WidgetName);

	if (WidStruct == nullptr || (*(WidStruct->WidgetClass)) == nullptr) return nullptr;

	UUserWidgetBase* Widget = WidStruct->Widget;
	if (WidStruct->Widget == nullptr)
	{
		Widget = WidStruct->Widget = CreateWidget<UUserWidgetBase>(Instance, WidStruct->WidgetClass, FName(WidgetName));
		Widget->SetWidgetName(WidgetName);
	}

	if (!Widget->IsInViewport())
	{
		Widget->AddToViewport();  // 默认添加到屏幕
		auto PlayerController = Instance->GetPrimaryPlayerController();
		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(Widget->TakeWidget()));
		}
	}

	Widget->SetVisibility(ESlateVisibility::Visible);
	return Widget;
}

void UTTTUIManager::RemoveWidget(const FString& WidgetName)
{
	if (const auto WidStruct = AllWidgets.Find(WidgetName))
	{
		if (const auto& Wid = WidStruct->Widget)
		{
			Wid->RemoveFromParent();
		}
		WidStruct->Widget = nullptr;
	}
}

void UTTTUIManager::HideWidget(const FString& WidgetName)
{
	if (const auto WidStruct = AllWidgets.Find(WidgetName))
	{
		if (const auto& Wid = WidStruct->Widget)
		{
			Wid->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
