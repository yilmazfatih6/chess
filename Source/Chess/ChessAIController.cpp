// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAIController.h"
#include "PawnActor.h"
#include "RookActor.h"
#include "KnightActor.h"
#include "BishopActor.h"
#include "QueenActor.h"
#include "KingActor.h"

AChessAIController::AChessAIController()
{
	
}

/* Spawns black pieces and makes necessary */
void AChessAIController::SpawnPieces()
{
	// Spawn pawns and add to SpawnedActors array
	for (int i = 1; i <= 8; i++) {
		Pieces.Add(GetWorld()->SpawnActor<APawnActor>(FVector((i * 400.0f) - 200.0f, 2600.0f, 200), FRotator::ZeroRotator));
	}

	// Spawn rooks and add to SpawnedActors array
	Pieces.Add(GetWorld()->SpawnActor<ARookActor>(FVector(400, 3200, 500) - 200, FRotator::ZeroRotator));
	Pieces.Add(GetWorld()->SpawnActor<ARookActor>(FVector(3200, 3200, 500) - 200, FRotator::ZeroRotator));

	// Spawn bishops and add to SpawnedActors array
	Pieces.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(1200, 3200, 500) - 200, FRotator::ZeroRotator));
	Pieces.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(2400, 3200, 500) - 200, FRotator::ZeroRotator));

	// Spawn knights and add to SpawnedActors array
	Pieces.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(800, 3200, 500) - 200, FRotator::ZeroRotator));
	Pieces.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(2800, 3200, 500) - 200, FRotator::ZeroRotator));

	// Spawn queen and add to SpawnedActors array
	Pieces.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(2000, 3200, 500) - 200, FRotator::ZeroRotator));

	// Spawn king and add to SpawnedActors array
	Pieces.Add(GetWorld()->SpawnActor<AKingActor>(FVector(1600, 3200, 500) - 200, FRotator::ZeroRotator));

	// Set color and material for pieces
	for (auto Piece : Pieces) {
		Piece->bIsWhite = false;
		Piece->SetDeselected();
	}
}