// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessController.h"
#include "ChessPlayer.generated.h"

UCLASS()
class CHESS_API AChessPlayer : public APawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	AChessPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* FUNCTIONS */
	UFUNCTION(BlueprintPure)
		int32 GetScore();
	UFUNCTION(BlueprintPure)
		FText GetName();
	UFUNCTION(BlueprintCallable)
		void SetName(FText PlayerName);
	UFUNCTION()
		FVector GetCameraPanDirection();
	/* FUNCTIONS */

	/* PROPERTIES */
	UPROPERTY()
		bool bIsWhite;
	UPROPERTY()
		AChessController* PC;
	
	TArray<class AParentActor*> Pieces; // Pointers to all pieces that belong to this player.

protected:

	UPROPERTY()
		class USpringArmComponent* CameraSpringArm;
	UPROPERTY()
		class UCameraComponent* Camera;

private:
	UPROPERTY()
		int32 Score = 0;
	UPROPERTY()
		FText Name = FText::FromString("Playerone");
	/* PROPERTIES */

};
