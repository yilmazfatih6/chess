// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ChessAIController.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChessAIController : public AAIController
{
	GENERATED_BODY()

public:
	AChessAIController();

private:
	UFUNCTION()
		void SpawnPieces();

	UPROPERTY()
		TArray<class AParentActor*>Pieces;
	
};
