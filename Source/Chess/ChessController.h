// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Board.h"
#include "ParentActor.h"
#include "ChessController.generated.h"

/**
 *
 */
UCLASS()
class CHESS_API AChessController : public APlayerController
{
	GENERATED_BODY()

public:
	AChessController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/************** FUNCTIONS **************/
	// When left mouse button is clicked
	UFUNCTION()
		void OnLeftMouseClick();

	// When right mouse button is clicked
	UFUNCTION()
		void OnRightMouseClick();

	UFUNCTION()
		void SelectPiece();

	UFUNCTION()
		void HighlightSelectableGrids();

	UFUNCTION()
		void SetDefaultMaterialsToGrids();

	UFUNCTION()
		void SetDefaultMaterialsToPieces();

	UFUNCTION()
		void SpawnWhiteActors();

	UFUNCTION()
		void SpawnBlackActors();

	UFUNCTION()
		void SwitchPlayer();

	UFUNCTION(BlueprintPure)
		AChessPlayer* GetCurrentPlayer();

	UFUNCTION(BlueprintCallable)
		void MakeMove();

	UFUNCTION(BlueprintCallable)
		void Cancel();

	/************** FUNCTIONS **************/

	/************** VARIABLES **************/
	TArray<class UStaticMeshComponent* > SelectableGrids;
	TArray<class AParentActor* > EliminatePieces;

	TArray<class AActor*> OverlappedActors;

	TArray<class UPrimitiveComponent*> OverlappedComponents;

	TArray<class UMaterial* > DefaultMaterials;

	// Checker if anything is selected. False by default.
	bool IsAnythingSelected = false;

	// Holds selected piece
	AParentActor* SelectedPiece;

	// Holds selected actor
	UStaticMeshComponent* SelectedGrid;

	UPROPERTY()
		FVector GridLocation;

	UPROPERTY(VisibleAnyWhere)
		class UMaterial* SelectedMaterial;

	// Pointer to chess board
	ABoard* ChessBoard;

	/* Pointer to players */
	UPROPERTY()
		AChessPlayer *PlayerOne;		// Pointer to player one
	UPROPERTY(BlueprintReadOnly)
		AChessPlayer *PlayerTwo;		// Pointer to player two
	UPROPERTY()
		AChessPlayer *PreviousPlayer;	// Pointer to previous player
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		AChessPlayer *CurrentPlayer;	// Pointer to current player
	UPROPERTY(BlueprintReadOnly)
		FText ErrorMessage = FText::FromString("No error.");
	
	void DisplayErrorMessage(FText ErrorMessage);

	UPROPERTY(BlueprintReadWrite)
		bool bIsPending = false;
	/************** VARIABLES **************/
};
