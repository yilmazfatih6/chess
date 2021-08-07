// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAIPawn.h"
#include "ChessAIController.h"

// Sets default values
AChessAIPawn::AChessAIPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AChessAIController aiController = AChessAIController();
	//AIControllerClass = (AIController)aiController;
}

// Called when the game starts or when spawned
void AChessAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChessAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

