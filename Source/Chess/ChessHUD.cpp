// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessHUD.h"
#include "ChessController.h"

AChessHUD::AChessHUD()
{
	/** Load class from asset in Content Browser of the UserWidget we created (UGUserWidget) */
	static ConstructorHelpers::FClassFinder<UChessUserWidget> WidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprints/HUD_BP'"));
	if (WidgetAsset.Succeeded())
	{
		/** Assign the class of the loaded asset to the WigetClass variable, which is a "subclass" of UUserWidget : Which our asset class is */
		WidgetClass = WidgetAsset.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO LOAD HUD BLUEPRINT!"));
	}
}

void AChessHUD::BeginPlay()
{
	Super::BeginPlay();

	/** Make sure asset was loaded and class was set */
	if (WidgetClass)
	{
		/** Create the widget of our UUserWidget type (UChessUserWidget) from the class we loaded from the Content Browser */
		Widget = CreateWidget<UChessUserWidget>(GetWorld(), WidgetClass);

		/** Make sure widget was created */
		if (Widget)
		{
			/** Add it to the viewport */
			Widget->AddToViewport();
		}
	}
}