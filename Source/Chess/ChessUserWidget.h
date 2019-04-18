// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "ChessUserWidget.generated.h"

/**
 *
 */
UCLASS()
class CHESS_API UChessUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void DisplayConfirmationButton(FVector GridLocation);

	UFUNCTION(BlueprintImplementableEvent)
		void DisplayErrorMessage();
};
