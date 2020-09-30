// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessController.h"
#include "PawnActor.h"
#include "RookActor.h"
#include "KnightActor.h"
#include "BishopActor.h"
#include "QueenActor.h"
#include "KingActor.h"
#include "CollisionBox.h"
#include "Board.h"
#include "ChessHUD.h"
#include "ChessPlayer.h"

// Constructor
AChessController::AChessController()
{
	bShowMouseCursor = true;						// Show mouse cursor
	DefaultMouseCursor = EMouseCursor::Crosshairs;	// Change cursor apperance to crosshair
	// Set selected material to Wood Walnut
	static ConstructorHelpers::FObjectFinder<UMaterial> Material31(TEXT("Material'/Game/Materials/M_SelectedMaterial.M_SelectedMaterial'"));
	if (Material31.Object != NULL)
	{
		SelectedMaterial = (UMaterial*)Material31.Object;
	}
}

// Begin play
void AChessController::BeginPlay()
{
	Super::BeginPlay();

	ABoard* Board = GetWorld()->SpawnActor<ABoard>(FVector(0, 0, 0), FRotator(0, 0, 0));;

	PlayerTwo = GetWorld()->SpawnActor<AChessPlayer>(FVector(1600.0, 3700.0, 1500), FRotator(20.0f, -90.0f, 0.0f));
	PlayerTwo->bIsWhite = false;
	PlayerOne = GetWorld()->SpawnActor<AChessPlayer>(FVector(1600.0, -500.0, 1500), FRotator(20.0f, 90.0f, 0.0f));
	PlayerOne->bIsWhite = true;
	CurrentPlayer = PlayerOne;

	PlayerOne->SetName(FText::FromString("PlayerOne"));
	PlayerTwo->SetName(FText::FromString("PlayerTwo"));

	// Initialize previous player variable
	PreviousPlayer = PlayerOne;

	// Spawn actors
	SpawnWhiteActors();
	SpawnBlackActors();

}

void AChessController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind actions for mouse events
	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AChessController::OnLeftMouseClick);
	InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AChessController::OnRightMouseClick);
}

/*	Fired when left mouse button pressed.
Detects clicked objects and if it is a RTSCharacter sets them to selected.
**/
void AChessController::OnLeftMouseClick()
{
	if (bIsPending) // Don't run if game is pending
		return;

	/* Clears previous selection */
	if (this->SelectedPiece != nullptr)
	{
		this->SelectedPiece->SetDeselected();
	}

	SetDefaultMaterialsToGrids(); // Set default materials to previously highlighted grid.
	SetDefaultMaterialsToPieces(); // Set default materials to previously highlighted pieces.

	SelectPiece(); // Detect hit object. Then highlight selected actor and selectable grids.
}

/*	Fired when right mouse button pressed.
	Detects clicked area and make a move if selected area is available to make a move.
**/
void AChessController::OnRightMouseClick()
{
	if (bIsPending) // Don't run if game is pending
		return;

	SetDefaultMaterialsToGrids(); // Set default materials to highlighted grid.
	SetDefaultMaterialsToPieces();
	
	// Check if anything is selected. Execute block if it is selected. Otherwise do nothing.
	if (IsAnythingSelected)
	{
		FHitResult TraceResult(ForceInit);	// FHitResult variable to keep track of mouse hit.
		GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, false, TraceResult);	// Get hit result and assign it to TraceResult.
		if (TraceResult.GetComponent() != nullptr)	// Check if any component was hit.
		{
			SelectedGrid = Cast<UStaticMeshComponent>(TraceResult.GetComponent());	// Cast hit component to StaticMeshComponent. If it is not a StaticMeshComponent this will be nullptr.
			if (SelectedGrid != nullptr)	// Check if cast was successful.
			{
				// Check if selected grid between SelectableGrids. Set bCanMakeMove to True if it is between.
				for (auto Grid : SelectableGrids)
				{
					if (Grid->GetComponentLocation() == SelectedGrid->GetComponentLocation())
					{
						GridLocation = SelectedGrid->GetComponentLocation() + FVector(200, 200, 0.5);	// Get hit component's location.
						AChessHUD* CastedHUD = Cast<AChessHUD>(GetHUD());
						// Check if cast was successful
						if (CastedHUD != nullptr)
						{
							SelectedGrid->SetMaterial(0, SelectedMaterial);
							CastedHUD->Widget->DisplayConfirmationButton(GridLocation); // Display confirmation button on the screen.
						}
						return;
					}
				}
				DisplayErrorMessage(FText::FromString("Can't make a move there!"));
				
			}
		}
	}
}

/* Switches between player */
void AChessController::SwitchPlayer()
{
	// Unposses current player
	UnPossess();
	// Switch to next player based on previous player
	if (CurrentPlayer == PlayerTwo)
	{
		PreviousPlayer = PlayerTwo;
		CurrentPlayer = PlayerOne;
		Possess(PlayerOne);
	}
	else if (CurrentPlayer == PlayerOne)
	{
		PreviousPlayer = PlayerOne;
		CurrentPlayer = PlayerTwo;
		Possess(PlayerTwo);
	}
}

/* Returns highlighted grids' material to their default material */
void AChessController::SetDefaultMaterialsToGrids()
{
	for (int32 index = 0; index < SelectableGrids.Num(); index++)
	{
		SelectableGrids[index]->SetMaterial(0, DefaultMaterials[index]);
	}
}

/* Returns highlighted grids' material to their default material */
void AChessController::SetDefaultMaterialsToPieces()
{
	for (int32 index = 0; index < EliminatePieces.Num(); index++)
	{
		EliminatePieces[index]->SetDeselected();
	}
}

/* Select piece from chess set by mouse hit */
void AChessController::SelectPiece()
{
	/* Detects clicked area */
	FHitResult TraceResult(ForceInit); // Will hold hit result
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, false, TraceResult); // Get hit result and assign it to TraceResult

	// If Hit Target was a an Actor.
	if (TraceResult.GetActor() != nullptr)
	{
		/* If selected actor is a Parent Actor */
		if (TraceResult.GetActor()->IsA(AParentActor::StaticClass()))
		{
			SelectedPiece = Cast<AParentActor>(TraceResult.GetActor()); // Cast actor to AChessParentActor
			if (SelectedPiece != nullptr) // Check if cast was successful
			{
				if(SelectedPiece->bIsWhite == CurrentPlayer->bIsWhite)
				{
					SelectedPiece->SetSelected();	// Call selected function from SelectedPiece to change selected actor's material.
					IsAnythingSelected = true;		// Set Checker variable to True
					HighlightSelectableGrids();		// Highlights Selectable Grids
					return;
				}
				else
				{
					DisplayErrorMessage(FText::FromString("Can't select enemy's piece!"));
					/*
					ErrorMessage = FText::FromString("Can't select enemy's piece!");
					AChessHUD* CastedHUD = Cast<AChessHUD>(GetHUD());
					// Check if cast was successful
					if (CastedHUD != nullptr)
					{
						CastedHUD->Widget->DisplayErrorMessage(); // Display confirmation button on the screen.
					}
					*/
				}
			}
		}
	}

	// If no actor was hit set anything to false.
	IsAnythingSelected = false;
}

/* Sets highlighted material to selectable grids */
void AChessController::HighlightSelectableGrids()
{
	SelectableGrids.Empty(); // Empty previous records.
	DefaultMaterials.Empty(); // Empty previous records.
	EliminatePieces.Empty(); // Empty previous records.

	SelectedPiece->DetectSelectableGrids(&SelectableGrids, &DefaultMaterials);

}

// Detects clicked grid. Then moves previously selected actor to grid's location. 
void AChessController::MakeMove()
{
	AActor* CollisionBox = GetWorld()->SpawnActor<ACollisionBox>(GridLocation, FRotator(0, 0, 0)); // Spawn collision box.
	CollisionBox->GetOverlappingActors(OverlappedActors, TSubclassOf<AActor>()); // Get Overlapped Actors by CollisionBox
	if (OverlappedActors.Num() > 1)
	{
		for (auto Actor : OverlappedActors)
		{
			if (Actor->IsA(AParentActor::StaticClass()))
			{
				AParentActor* CastedActor = Cast<AParentActor>(Actor);
				if (CastedActor)
				{
					CastedActor->Eliminate();
				}
			}
		}
	}
	OverlappedActors.Empty();
	CollisionBox->Destroy();

	SelectedPiece->ChangeLocation(FVector(GridLocation.X, GridLocation.Y, SelectedPiece->GetActorLocation().Z + 200));	// Change actor's location to selected grid's location.
	if (!SelectedPiece->bHasEverMoved)
		SelectedPiece->bHasEverMoved = true;
	SetDefaultMaterialsToGrids(); // Set default material to selected grid.
	SetDefaultMaterialsToPieces();
	SwitchPlayer(); // Switch player 
}

AChessPlayer* AChessController::GetCurrentPlayer()
{
	return CurrentPlayer;
}

void AChessController::Cancel()
{
	SetDefaultMaterialsToGrids();
	SetDefaultMaterialsToPieces();
	SelectedPiece->SetDeselected();
	SelectableGrids.Empty();
	EliminatePieces.Empty();
	SelectedPiece = nullptr;
}

void AChessController::DisplayErrorMessage(FText ErrorMessage)
{
	this->ErrorMessage = ErrorMessage;
	AChessHUD* CastedHUD = Cast<AChessHUD>(GetHUD());
	if (CastedHUD != nullptr) // Check if cast was successful
		CastedHUD->Widget->DisplayErrorMessage(); // Display confirmation button on the screen.
}

/* Spawns white pieces and makes necessary */
void AChessController::SpawnWhiteActors()
{
	TArray<class AParentActor*>SpawnedActors; // Pointer to hold spawned actors

	// Spawn pawns and add to SpawnedActors array
	for (int i = 1; i <= 8; i++) {
		SpawnedActors.Add(GetWorld()->SpawnActor<APawnActor>(FVector((i*400.0f) - 200.0f, 600.0f, 0.5f), FRotator::ZeroRotator));
	}

	// Spawn rooks and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<ARookActor>(FVector(400, 400, 500) - 200, FRotator::ZeroRotator));
	SpawnedActors.Add(GetWorld()->SpawnActor<ARookActor>(FVector(3200, 400, 500) - 200, FRotator::ZeroRotator));

	// Spawn bishops and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(1200, 400, 500) - 200, FRotator::ZeroRotator));
	SpawnedActors.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(2400, 400, 500) - 200, FRotator::ZeroRotator));

	// Spawn knights and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(800, 400, 500) - 200, FRotator::ZeroRotator));
	SpawnedActors.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(2800, 400, 500) - 200, FRotator::ZeroRotator));

	// Spawn queen and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(2000, 400, 500) - 200, FRotator::ZeroRotator));

	// Spawn king and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<AKingActor>(FVector(1600, 400, 500) - 200, FRotator::ZeroRotator));

	PlayerOne->Pieces = SpawnedActors;

	// Set color and material for pieces
	for (auto Piece : PlayerOne->Pieces) {
		Piece->bIsWhite = true;
		Piece->SetDeselected();
	}

	SpawnedActors.Empty();
}

/* Spawns black pieces and makes necessary */
void AChessController::SpawnBlackActors()
{
	TArray<class AParentActor*>SpawnedActors; // Pointer to hold spawned actors

	// Spawn pawns and add to SpawnedActors array
	for (int i = 1; i <= 8; i++) {
		SpawnedActors.Add(GetWorld()->SpawnActor<APawnActor>(FVector((i*400.0f) - 200.0f, 2600.0f, 200), FRotator::ZeroRotator));
	}
	
	// Spawn rooks and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<ARookActor>(FVector(400, 3200, 500) - 200, FRotator::ZeroRotator));
	SpawnedActors.Add(GetWorld()->SpawnActor<ARookActor>(FVector(3200, 3200, 500) - 200, FRotator::ZeroRotator));

	// Spawn bishops and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(1200, 3200, 500) - 200, FRotator::ZeroRotator));
	SpawnedActors.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(2400, 3200, 500) - 200, FRotator::ZeroRotator));

	// Spawn knights and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(800, 3200, 500) - 200, FRotator::ZeroRotator));
	SpawnedActors.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(2800, 3200, 500) - 200, FRotator::ZeroRotator));

	// Spawn queen and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(2000, 3200, 500) - 200, FRotator::ZeroRotator));

	// Spawn king and add to SpawnedActors array
	SpawnedActors.Add(GetWorld()->SpawnActor<AKingActor>(FVector(1600, 3200, 500) - 200, FRotator::ZeroRotator));

	PlayerTwo->Pieces = SpawnedActors;

	// Set color and material for pieces
	for (auto Piece : PlayerTwo->Pieces) {
		Piece->bIsWhite = false;
		Piece->SetDeselected();
	}

	SpawnedActors.Empty();
}