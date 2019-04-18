// Fill out your copyright notice in the Description page of Project Settings.

#include "KnightActor.h"
#include "CollisionBox.h"
#include "Board.h"
#include "ChessController.h"

// Sets default values
AKnightActor::AKnightActor()
{
	// Set Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/Meshes/Knight.Knight'"));
	if (FoundMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(FoundMesh.Object);
	}
}


/*	Detects selectable grids to move the pawn.
@param SelectableGrids: Pointer variable to assign selectable grids.
@param DefaultGrids:	Pointer variable to assign material values of selectable grids.
*/
void AKnightActor::DetectSelectableGrids(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials)
{
	// Checks for grid overlap. After check it calls AddAndHighlight to add grid to SelectableGrids array and highlights it.
	FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, 0);;
	CheckOverlap(SpawnLocation + FVector(800.0f, 400.f, 0.0f), SelectableGrids, DefaultMaterials);
	CheckOverlap(SpawnLocation + FVector(-800.0f, 400.f, 0.0f), SelectableGrids, DefaultMaterials);
	CheckOverlap(SpawnLocation + FVector(800.0f, -400.f, 0.0f), SelectableGrids, DefaultMaterials);
	CheckOverlap(SpawnLocation + FVector(-800.0f, -400.f, 0.0f), SelectableGrids, DefaultMaterials);
	CheckOverlap(SpawnLocation + FVector(400.0f, 800.f, 0.0f), SelectableGrids, DefaultMaterials);
	CheckOverlap(SpawnLocation + FVector(-400.0f, 800.f, 0.0f), SelectableGrids, DefaultMaterials);
	CheckOverlap(SpawnLocation + FVector(400.0f, -800.f, 0.0f), SelectableGrids, DefaultMaterials);
	CheckOverlap(SpawnLocation + FVector(-400.0f, -800.f, 0.0f), SelectableGrids, DefaultMaterials);
}

/*	Check overlap for possibly moveable locations. If overlap happens calls AddAndHighlight function.
@param SpawnLocation:	Location to spawn collision box.
@param SelectableGrids: Pointer variable to assign selectable grids.
@param DefaultGrids:	Pointer variable to assign material values of selectable grids.
*/
void AKnightActor::CheckOverlap(FVector SpawnLocation, TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials)
{
	TArray<AActor*> OverlappedActors; // Holds overlapped actors.
	TArray<UPrimitiveComponent*> OverlappedComponents; // Holds overlapped components.
	AActor* SpawnedActor = GetWorld()->SpawnActor<ACollisionBox>(SpawnLocation, FRotator(0, 0, 0)); // Spawn collision box.
	SpawnedActor->GetOverlappingActors(OverlappedActors, TSubclassOf<ABoard>()); // Get Overlapped Actors by CollisionBox
	SpawnedActor->GetOverlappingComponents(OverlappedComponents); // Get Overlapped Components by CollisionBox

	if (OverlappedComponents.Num() == 0 && OverlappedActors.Num() == 0)
	{
		SpawnedActor->Destroy(); // Destroy CollisionBox.
		return;
	}
	
	AddAndHighlight(SelectableGrids, DefaultMaterials, &OverlappedActors, &OverlappedComponents); /*Highlighing overlapped grids.*/
	SpawnedActor->Destroy(); // Destroy CollisionBox.
}

/*	Highlights every selectable grids. Also adds grid to SelectableGrids array and their default material to DefaultMaterials array.
@param	SelectableGrids : For holding overlapped grids. Overlapped grids will be added to this array.
@param	DefaultMaterials : For holding overlapped grids' materials. Overlapped grids' materials will be added to this array.
@param	OverlappedActors : Used to pass overlapped actors to this function.
@param	OverlappedComponents :  Used to pass overlapped components to this function.
*/
void AKnightActor::AddAndHighlight(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials, TArray<AActor*> *OverlappedActors, TArray<UPrimitiveComponent*> *OverlappedComponents)
{
	/*	Checks if there exists a piece on the target grid.
		If exists checks for if it is enemy's piece or not.
		If it is not enemies piece it quits function and prevents player to make a move there.
	*/
	if (OverlappedActors->Num() > 1)
	{
		// Foreach overlapped actor run the loop
		for (auto Actor : *OverlappedActors)
		{
			// Check if current actor is a chess piece
			if (Actor->IsA(AParentActor::StaticClass()))
			{
				AParentActor* CastedActor = Cast<AParentActor>(Actor); // Cast actor to ChessBoard class
				// Check if cast was successful
				if (CastedActor != nullptr)
				{
					// Check if player's color and actor's color is the same. 
					if (bIsWhite == CastedActor->bIsWhite)
					{
						// If their colors are the same break the function to prevent highlight.
						return;
					}
				}
			}
		}
	}


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
					if (AttachmentActor->IsA(ABoard::StaticClass()))
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
