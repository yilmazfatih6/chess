// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParentActor.generated.h"

UCLASS()
class CHESS_API AParentActor : public AActor
{
	GENERATED_BODY()

/******** FUNCTIONS START *******/
public:	
	// Sets default values for this actor's properties
	AParentActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Call when actor is selected by mouse click.
	UFUNCTION()
		void SetSelected();

	// Called when actor is deselected.
	UFUNCTION()
		void SetDeselected();

	// Sets eliminate material to piece.
	UFUNCTION()
		void SetEliminateMaterial();

	// Changes actors location. Gets one parameter which holds location to move.
	UFUNCTION()
		void ChangeLocation(FVector MoveLocation);

	// Eliminates piece from the game.
	UFUNCTION()
		void Eliminate();

	// Detect selectable grids to move the actor. This function does nothing in the parent class. Therefore it is overridden for every chess piece which performs different sets of actions. 
	virtual void DetectSelectableGrids(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials);
/******** FUNCTIONS END*******/

/******** PROPERTIES START *******/
public:
	// Holds information about whether or not this piece is white. Use to check piece's color.
	UPROPERTY(EditAnywhere)
		bool bIsWhite;

	// Variable to check if pawn has ever moved.
	UPROPERTY(EditAnywhere)
		bool bHasEverMoved;

	// Static mesh for the class. Null by default overridden for every chess piece.
	UPROPERTY(VisibleAnyWhere)
		class UStaticMeshComponent* StaticMesh;

	// White material for the piece.
	UPROPERTY(VisibleAnyWhere)
		class UMaterial* WhiteMaterial;

	// Black material for the piece.
	UPROPERTY(VisibleAnyWhere)
		class UMaterial* BlackMaterial;

	// Material to set when piece is selected.
	UPROPERTY(VisibleAnyWhere)
		class UMaterial* SelectedMaterial;

	// Material to set when piece could be eliminated.
	UPROPERTY(VisibleAnyWhere)
		class UMaterial* EliminateMaterial;
/******** PROPERTIES END *******/

};
