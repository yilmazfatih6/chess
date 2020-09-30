// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnActor.h"
#include "Board.h"
#include "CollisionBox.h"
#include "ChessController.h"

// Constructor. Gets and assigns mesh to class.
APawnActor::APawnActor()
{
	// Set Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("StaticMesh'/Game/Meshes/Pawn.Pawn'"));
	if (FoundMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(FoundMesh.Object);
	}
}

/*	Detects selectable grids to move the pawn.
@param SelectableGrids: Pointer variable to assign selectable grids.
@param DefaultGrids:	Pointer variable to assign material values of selectable grids.
*/
void APawnActor::DetectSelectableGrids(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials)
{
	/* Spawn CollisionBox to get Selectable Grids by collision check */
	FVector SpawnLocation;
	TArray<AActor*> OverlappedActors;
	TArray<UPrimitiveComponent*> OverlappedComponents;

	/* FORWARD MOVE */
	if (bIsWhite)
		SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 400.0f, 0.0f); // Spawn location if pawn is white
	else
		SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y - 400.0f, 0.0f);  // Spawn location if pawn is black

	AActor* SpawnedActor = GetWorld()->SpawnActor<ACollisionBox>(SpawnLocation, FRotator(0, 0, 0)); // Spawn CollisionBox
	SpawnedActor->GetOverlappingActors(OverlappedActors, TSubclassOf<AActor>()); // Get Overlapped Actors by CollisionBox
	SpawnedActor->GetOverlappingComponents(OverlappedComponents); // Get Overlapped Components by CollisionBox
	if (OverlappedActors.Num() == 1)
		AddAndHighlight(SelectableGrids, DefaultMaterials, &OverlappedActors, &OverlappedComponents);  /*Highlighing overlapped grids.*/
	OverlappedActors.Empty();
	OverlappedComponents.Empty();
	SpawnedActor->Destroy(); // Destroy CollisionBox.
	
	/* DOUBLE FORWARD MOVE */
	if (!bHasEverMoved)
	{
		if (bIsWhite)
			SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 800.0f, 0.0f); // Spawn location if pawn is white
		else
			SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y - 800.0f, 0.0f);  // Spawn location if pawn is black
		AActor* SpawnedActor = GetWorld()->SpawnActor<ACollisionBox>(SpawnLocation, FRotator(0, 0, 0)); // Spawn CollisionBox
		SpawnedActor->GetOverlappingActors(OverlappedActors, TSubclassOf<AActor>()); // Get Overlapped Actors by CollisionBox
		SpawnedActor->GetOverlappingComponents(OverlappedComponents); // Get Overlapped Components by CollisionBox
		if (OverlappedActors.Num() == 1)
			AddAndHighlight(SelectableGrids, DefaultMaterials, &OverlappedActors, &OverlappedComponents);  /*Highlighing overlapped grids.*/
		OverlappedActors.Empty();
		OverlappedComponents.Empty();
		SpawnedActor->Destroy(); // Destroy CollisionBox.
	}
		
	/* RIGHT FORWARD MOVE */
	if (bIsWhite)
		SpawnLocation = FVector(GetActorLocation().X - 400, GetActorLocation().Y + 400.0f, 0.0f); // Spawn location if pawn is white
	else
		SpawnLocation = FVector(GetActorLocation().X + 400, GetActorLocation().Y - 400.0f, 0.0f);  // Spawn location if pawn is black
	SpawnedActor = GetWorld()->SpawnActor<ACollisionBox>(SpawnLocation, FRotator(0, 0, 0));	// Spawn CollisionBox
	SpawnedActor->GetOverlappingActors(OverlappedActors, TSubclassOf<AActor>());			// Get Overlapped Actors by CollisionBox
	SpawnedActor->GetOverlappingComponents(OverlappedComponents);							// Get Overlapped Components by CollisionBox
	if (OverlappedActors.Num() > 1) {
		for (auto Actor : OverlappedActors) {
			if (Actor->IsA(AParentActor::StaticClass())) {
				AParentActor *CastedActor = Cast<AParentActor>(Actor);
				if (CastedActor != nullptr) {
					if (CastedActor->bIsWhite != this->bIsWhite)
						AddAndHighlight(SelectableGrids, DefaultMaterials, &OverlappedActors, &OverlappedComponents); /*Highlighing overlapped grids.*/
				}
			}
		}
	}
	OverlappedActors.Empty();
	OverlappedComponents.Empty();
	SpawnedActor->Destroy(); // Destroy CollisionBox.

	/* LEFT FORWARD MOVE */
	if (bIsWhite)
		SpawnLocation = FVector(GetActorLocation().X + 400, GetActorLocation().Y + 400.0f, 0.0f);	// Spawn location if pawn is white
	else
		SpawnLocation = FVector(GetActorLocation().X - 400, GetActorLocation().Y - 400.0f, 0.0f);	// Spawn location if pawn is black
	SpawnedActor = GetWorld()->SpawnActor<ACollisionBox>(SpawnLocation, FRotator(0, 0, 0));			// Spawn CollisionBox
	SpawnedActor->GetOverlappingActors(OverlappedActors, TSubclassOf<AActor>());					// Get Overlapped Actors by CollisionBox
	SpawnedActor->GetOverlappingComponents(OverlappedComponents);									// Get Overlapped Components by CollisionBox
	if (OverlappedActors.Num() > 1) {
		for (auto Actor : OverlappedActors) {
			if (Actor->IsA(AParentActor::StaticClass())) {
				AParentActor *CastedActor = Cast<AParentActor>(Actor);
				if (CastedActor != nullptr) {
					if (CastedActor->bIsWhite != this->bIsWhite)
						AddAndHighlight(SelectableGrids, DefaultMaterials, &OverlappedActors, &OverlappedComponents); /*Highlighing overlapped grids.*/
				}
			}
		}
	}
	OverlappedActors.Empty();
	OverlappedComponents.Empty();
	SpawnedActor->Destroy(); // Destroy CollisionBox.

}

/*	Highlights every selectable grids. Also adds grid to SelectableGrids array and their default material to DefaultMaterials array.
@param	SelectableGrids : For holding overlapped grids. Overlapped grids will be added to this array.
@param	DefaultMaterials : For holding overlapped grids' materials. Overlapped grids' materials will be added to this array.
@param	OverlappedActors : Used to pass overlapped actors to this function.
@param	OverlappedComponents :  Used to pass overlapped components to this function.
*/
void APawnActor::AddAndHighlight(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials, TArray<AActor*> *OverlappedActors, TArray<UPrimitiveComponent*> *OverlappedComponents)
{
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