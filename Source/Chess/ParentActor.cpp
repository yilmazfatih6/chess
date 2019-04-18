// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentActor.h"

// Sets default values
AParentActor::AParentActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bHasEverMoved = false;

	// Create static mesh and assign it as root
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	StaticMesh->SetCollisionProfileName(TEXT("Pawn"));	// Set Collision
	StaticMesh->SetSimulatePhysics(true);				// Set Physics

	// Set WhiteMaterial
	static ConstructorHelpers::FObjectFinder<UMaterial> GetWhiteMaterial(TEXT("Material'/Game/Materials/M_LightMaterial.M_LightMaterial'"));
	if (GetWhiteMaterial.Succeeded())
	{
		WhiteMaterial = GetWhiteMaterial.Object;
	}

	// Set BlackMaterial
	static ConstructorHelpers::FObjectFinder<UMaterial> GetBlackMaterial(TEXT("Material'/Game/Materials/M_DarkMaterial.M_DarkMaterial'"));
	if (GetBlackMaterial.Succeeded())
	{
		BlackMaterial = GetBlackMaterial.Object;
	}

	// Set SelectedMaterial
	static ConstructorHelpers::FObjectFinder<UMaterial> GetSelectedMaterial(TEXT("Material'/Game/Materials/M_SelectedMaterial.M_SelectedMaterial'"));
	if (GetSelectedMaterial.Succeeded())
	{
		SelectedMaterial = GetSelectedMaterial.Object;
	}

	// Set EliminateMaterial
	static ConstructorHelpers::FObjectFinder<UMaterial> GetEliminateMaterial(TEXT("Material'/Game/Materials/M_EliminateMaterial.M_EliminateMaterial'"));
	if (GetEliminateMaterial.Succeeded())
	{
		EliminateMaterial = GetEliminateMaterial.Object;
	}

}

// Called when the game starts or when spawned
void AParentActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Call when actor is selected by mouse click. Changes actor's material to selected material to indicate it is selected.
void AParentActor::SetSelected()
{
	StaticMesh->SetMaterial(0, SelectedMaterial);
}

// Changes pieces material to eliminate material if this piece can be eliminated by making move.
void AParentActor::SetEliminateMaterial()
{
	UE_LOG(LogTemp, Error, TEXT("ow yeah"));
	StaticMesh->SetMaterial(0, EliminateMaterial);
}

// Called when actor is deselected. Changes actor's material to it's original material.
void AParentActor::SetDeselected()
{
	/*	REMAINDER TO ME: IF I ADD ADDITIONAL CODES HERE
		CREATE ANOTHER FUNCTION CALLED SetDefaultMaterial WHICH ONLY INCLUDES THIS CODES.
	 */
	if (bIsWhite)
	{
		StaticMesh->SetMaterial(0, WhiteMaterial);
	}
	else
	{
		StaticMesh->SetMaterial(0, BlackMaterial);
	}
}

// Called when actor is moved by the player. Gets one parameter which holds location to move.
void AParentActor::ChangeLocation(FVector MoveLocation)
{
	SetActorLocation(MoveLocation);
}

// Detects selectable grids to move the piece. Gets 2 parameters. After function complete, first one holds selectable grid's addresses second one holds those grid's default materials sequentially.
void AParentActor::DetectSelectableGrids(TArray<UStaticMeshComponent*> *SelectableGrids, TArray<UMaterial*> *DefaultMaterials)
{
	UE_LOG(LogTemp, Warning, TEXT("This is parent class for chess pieces. Make calls from your specific piece class!"));
}

// Eliminates piece from game.
void AParentActor::Eliminate()
{
	this->Destroy();
}
