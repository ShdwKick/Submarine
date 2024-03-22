// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerWidget.h"
#include "Components/SizeBoxSlot.h"
#include "Blueprint/WidgetTree.h"

void USpawnerWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

bool USpawnerWidget::Initialize()
{
	UUserWidget::Initialize();

	if (WidgetTree)
	{
		ResourseIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		WidgetTree->RootWidget = ResourseIcon;
		ResourseIcon->SetRenderScale(FVector2D(0.1, 0.1));
	}
	return true;
}


void USpawnerWidget::SetIcon(UTexture2D* NewIcon)
{
	MyIcon = NewIcon;
	ResourseIcon->SetBrushFromTexture(MyIcon);
}
