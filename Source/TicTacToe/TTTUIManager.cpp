// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTUIManager.h"
#include "UserWidgetBase.h"


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

	if (WidStruct->Widget == nullptr)
	{
		WidStruct->Widget = CreateWidget<UUserWidgetBase>(Instance, WidStruct->WidgetClass, FName(WidgetName));
		WidStruct->Widget->SetWidgetName(WidgetName);
	}

	if (!WidStruct->Widget->IsInViewport()) WidStruct->Widget->AddToViewport();  // 默认添加到屏幕
	return WidStruct->Widget;
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
