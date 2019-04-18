// Fill out your copyright notice in the Description page of Project Settings.

#include "BishopActor.h"
#include "CollisionBox.h"
#include "Board.h"
#include "ChessController.h"

// Constructor
ABishopActor::ABishopActor()
{
	// Set Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/Meshes/Bishop.Bishop'"));
	if (FoundMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(FoundMesh.Object);
	}
}


/*	Detects selectable grids to move the pawn.
@param SelectableGrids: Pointer variable to assign selectable grids.
@param DefaultGrids:	Pointer variable to assign material values of selectable grids.
*/
void ABishopActor::DetectSelectableGrids(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials)
{
	TArray<AActor*> OverlappedActors; // Holds overlapped actors.
	TArray<UPrimitiveComponent*> OverlappedComponents; // Holds overlapped components.
	FVector SpawnLocation; // Location to spawn the collision box.
	float ChangeXBy = 400.0f, ChangeYBy = 400.0f; // To update X and Y value by this.

	// Loop for 4 axis to search.
	for (int32 i = 0; i < 4; ++i)
	{
		SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, 0); // Reset spawn value.

		// Change CollisionBox spawn location increaser in every loop.
		if (i == 1)
			ChangeXBy = -400.0f;
		else if (i == 2)
			ChangeYBy = -400.0f;
		else if (i == 3)
			ChangeXBy = +400.0f;

		
		// Spawn CollisionBox till it doesn't overlap anything.
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

			// If CollisionBox doesn't overlap anything break the loop.
			if (OverlappedComponents.Num() == 0 && OverlappedActors.Num() == 0)
			{
				SpawnedActor->Destroy(); // Destroy CollisionBox.
				break;
			} 
			/* Checks if there is another piece with the same color, which block player movement */
			else if (OverlappedActors.Num() > 1)
			{
				// Foreach overlapped actor run the loop
				for (auto Actor : OverlappedActors)
				{
					// Check if current actor is a chess piece
					if (Actor->IsA(AParentActor::StaticClass()))
					{
						AParentActor* CastedActor = Cast<AParentActor>(Actor); // Cast actor to ChessBoard class
						// Check if cast was successful
						if (CastedActor != nullptr)
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
void ABishopActor::AddAndHighlight(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials, TArray<AActor*> *OverlappedActors, TArray<UPrimitiveComponent*> *OverlappedComponents)
{
	/*	Checks every overlapped actor.
		If overlapped actor is a grid adds it to SelectableGrids array and it's material to DefaultMaterials array. Then highlights that grid.
		If overlapped actor is a chess piece changes that piece's material. Add's that pieces to EliminatePieces array so to change their material back to default again.
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
					AActor *AttachmentActor = Component->GetAttachmentRootActor();
					if(AttachmentActor->IsA(ABoard::StaticClass()))
					{ 
						UStaticMeshComponent* CastedMesh = Cast<UStaticMeshComponent>(Component); // Cast component to StaticMeshComponent
						// Check if cast was successful
						if (CastedMesh != nullptr)
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