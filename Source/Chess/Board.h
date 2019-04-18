// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Board.generated.h"

UCLASS()
class CHESS_API ABoard : public AActor
{
	GENERATED_BODY()

/******** FUNCTIONS START *******/

public:	
	// Sets default values for this actor's properties
	ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void HighlightGrid(UStaticMeshComponent* Grid);

/******** FUNCTIONS END*******/

/******** PROPERTIES START *******/
public:

	// Box component to use as Root Component
	UPROPERTY()
		class UBoxComponent* BoxComponent;

	// Pointer array to hold every grid's address.
	UPROPERTY()
		TArray<class  UStaticMeshComponent*> StaticMeshes;

	//Light colored grid material for the white chess grids.
	UPROPERTY(EditAnywhere)
		class UMaterial* LightMaterial;

	// Dark colored grid material for the black chess grids.
	UPROPERTY(EditAnywhere)
		class UMaterial* DarkMaterial;

	// Grid's material is changed to this when player has ability to move to this grid.
	UPROPERTY(EditAnywhere)
		class UMaterial* HighlightedMaterial;

	// This is null by default. Only used as temporary pointer when grid's original material is changed to highlighted or selected material.
	UPROPERTY()
		class UMaterial* DefaultMaterial;
/******** PROPERTIES END *******/






};
