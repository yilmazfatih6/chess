// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ChessUserWidget.h"
#include "ChessHUD.generated.h"

/**
 *
 */
UCLASS()
class CHESS_API AChessHUD : public AHUD
{
	GENERATED_BODY()

public:

	AChessHUD();

	virtual void BeginPlay() override;

	/** Class of user widget, loaded from Content Browser */
	TSubclassOf<class UUserWidget> WidgetClass;

	/* Reference to created user widget*/
	class UChessUserWidget *Widget;

};


