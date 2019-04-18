// Fill out your copyright notice in the Description page of Project Settings.
#include "RookActor.h"
#include "CollisionBox.h"
#include "Board.h"
#include "Engine/World.h"
#include "ChessController.h"

// Constructor
ARookActor::ARookActor()
{
	// Set Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/Meshes/Rook.Rook'"));
	if (FoundMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(FoundMesh.Object);
	}
}

/*	Detects selectable grids to move the rook.
@param SelectableGrids: Pointer variable to assign selectable grids.
@param DefaultGrids:	Pointer variable to assign material values of selectable grids.
*/
void ARookActor::DetectSelectableGrids(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials)
{
	TArray<AActor*> OverlappedActors; // Holds overlapped actors.
	TArray<UPrimitiveComponent*> OverlappedComponents; // Holds overlapped components.
	FVector SpawnLocation; // Location to spawn the collision box.

	float ChangeXBy = 400.0f, ChangeYBy = 0.0f; // To update X and Y value by this.

	// Search should be made for 4 different directions which are +X, -X, +Y, -Y. Loop 4 times for 4 directions.
	for (int32 i = 0; i < 4; i++)
	{
		SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, 0); // Reset spawn value.

		// Update search coordinates value changers for every step.
		if (i == 1)
			ChangeXBy = -400.0f;
		else if (i == 2)
		{
			ChangeXBy = 0;
			ChangeYBy = 400.0f;
		}
		else if (i == 3)
			ChangeYBy = -400.0f;

		while (1)
		{
			/* These two parameters are used to check if there are other pieces block movevement. */
			bool bOwnPieceDetected = false; // Checker for if there is player's own piece 
			bool bEnemyDetected = false;	// Checker for if there is enemy's piece 	
			/*Spawning CollisionBox*/
			SpawnLocation += FVector(ChangeXBy, ChangeYBy, 0); // Change spawn location for spawning collision box.
			AActor* SpawnedActor = GetWorld()->SpawnActor<ACollisionBox>(SpawnLocation, FRotator(0, 0, 0)); // Spawn collision box.

			/* Getting overlapped actors and components */
			SpawnedActor->GetOverlappingActors(OverlappedActors, TSubclassOf<ABoard>()); // Get Overlapped Actors by CollisionBox
			SpawnedActor->GetOverlappingComponents(OverlappedComponents); // Get Overlapped Components by CollisionBox

			/*Checking if anything was overlapped. If anything is overlapped break the loop. */
			if (OverlappedComponents.Num() == 0 && OverlappedActors.Num() == 0)
			{
				SpawnedActor->Destroy(); // Destroy CollisionBox.
				break;
			}
			/* Checks if there is another piece with the same color, which block player movement */
			else if (OverlappedActors.Num() > 1)
			{
				for (auto Actor : OverlappedActors) // Foreach overlapped actor run the loop
				{
					if (Actor->IsA(AParentActor::StaticClass())) // Check if current actor is a chess piece
					{
						AParentActor* CastedActor = Cast<AParentActor>(Actor); // Cast actor to ChessBoard class
						if (CastedActor != nullptr) // Check if cast was successful
						{
							if (bIsWhite == CastedActor->bIsWhite)
								bOwnPieceDetected = true;
							else if (bIsWhite != CastedActor->bIsWhite)
								bEnemyDetected = true;
						}
					}
				}
			}
			/*if there is another piece which block player movement, break loop to end other grids avaibility research */
			if (bOwnPieceDetected)
			{
				SpawnedActor->Destroy(); // Destroy CollisionBox.
				break;
			}
			
			/*Highlighing overlapped grids.*/
			AddAndHighlight(SelectableGrids, DefaultMaterials, &OverlappedActors, &OverlappedComponents);
			
			/*if there is enemy piece which block further player movement, break loop to end other grids avaibility research */
			if (bEnemyDetected)
			{
				SpawnedActor->Destroy(); // Destroy CollisionBox.
				break;
			}

			SpawnedActor->Destroy(); // Destroy CollisionBox.
		}
	}
}

/*	Highlights every selectable grids. Also adds grid to SelectableGrids array and their default material to DefaultMaterials array.
@param	SelectableGrids : For holding overlapped grids. Overlapped grids will be added to this array.
@param	DefaultMaterials : For holding overlapped grids' materials. Overlapped grids' materials will be added to this array.
@param	OverlappedActors : Used to pass overlapped actors to this function.
@param	OverlappedComponents :  Used to pass overlapped components to this function.
*/
void ARookActor::AddAndHighlight(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials, TArray<AActor*> *OverlappedActors, TArray<UPrimitiveComponent*> *OverlappedComponents)
{
	/*	Checks every overlapped actor. 
		If overlapped actor is a grid adds it to SelectableGrids array and it's material to DefaultMaterials array. Then highlights that grid.
		If overlapped actor is a chess piece changes that piece's material.
	*/
	for (auto Actor : *OverlappedActors)
	{
		// Code to perform if overlapped actor is a board grid.
		if (Actor->IsA(ABoard::StaticClass()))
		{
			ABoard* CastedActor = Cast<ABoard>(Actor); // Cast actor to ChessBoard class
			// Check if cast was successful
			if (CastedActor != nullptr)
			{
				// Foreach overlapped component run the loop
				for (auto Component : *OverlappedComponents)
				{
					// Checks components attached root actor. If attached root actor is a type of ABoard continues.
					AActor *AttachmentActor = Component->GetAttachmentRootActor();
					if (AttachmentActor->IsA(ABoard::StaticClass()))
					{
						UStaticMeshComponent* CastedMesh = Cast<UStaticMeshComponent>(Component); // Cast component to StaticMeshComponent
						if (CastedMesh != nullptr) // Check if cast was successful
						{
							SelectableGrids->Add(CastedMesh); // Add grid to SelectableGrids.
							/* Adding material types of selectable grids to checker array for undoing material change in the future */
							UMaterial* CastedMaterial = Cast<UMaterial>(CastedMesh->GetMaterial(0)); //  Cast to UMaterial
							if (CastedMaterial)
							{
								DefaultMaterials->Add(CastedMaterial); // Add Grid's material to this array.
							}
							CastedActor->HighlightGrid(CastedMesh); // Change grid's material to highlighted material
						}
					}
				}
			}
		}
		// Code to perform if overlapped actor is a chess piece.
		else if (Actor->IsA(AParentActor::StaticClass()))
		{
			AParentActor* CastedParent = Cast<AParentActor>(Actor);
			if (CastedParent != nullptr)
			{
				AChessController* CastedPC = Cast<AChessController>(GetWorld()->GetFirstPlayerController());
				if (CastedPC != nullptr)
				{
					CastedPC->EliminatePieces.Add(CastedParent);
					CastedParent->SetEliminateMaterial();
				}
			}
		}
	}
}