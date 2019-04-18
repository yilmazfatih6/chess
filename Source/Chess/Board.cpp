// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

// Sets default values
ABoard::ABoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/********** DEFINITION OF BOX COMPONENT **********/
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));		// Define a box component
	RootComponent = BoxComponent;													// Set it as RootComponent
	BoxComponent->InitBoxExtent(FVector(0, 0, 0));									// Set size to box
	/********** DEFINITION OF BOX COMPONENT **********/

	/********** DEFINITION OF MATERIALS **********/
	// Set light material to Wood Pine
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/M_Wood_Pine.M_Wood_Pine'"));
	if (Material.Object != NULL)
	{
		LightMaterial = (UMaterial*)Material.Object;
	}
	// Set dark material to Wood Walnut
	static ConstructorHelpers::FObjectFinder<UMaterial> Material2(TEXT("Material'/Game/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	if (Material2.Object != NULL)
	{
		DarkMaterial = (UMaterial*)Material2.Object;
	}
	// Set highlighted material to Wood Walnut
	static ConstructorHelpers::FObjectFinder<UMaterial> Material3(TEXT("Material'/Game/Materials/M_SelectedMaterial.M_SelectedMaterial'"));
	if (Material3.Object != NULL)
	{
		HighlightedMaterial = (UMaterial*)Material3.Object;
	}
	/********** DEFINITION OF MATERIALS **********/

	/********** CREATION OF BOARD GRIDS **********/
	// Calculation of coordinates
	for (int32 index = 0; index < 64; ++index)
	{
		// Set Coordinates
		int32 XCoordinate = (index / 8) * 400;
		int32 YCoordinate = (index % 8) * 400;

		// Add static mesh to component 
		StaticMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(*FString("Mesh" + FString::FromInt(index))));
		StaticMeshes[index]->SetupAttachment(BoxComponent); // Atttach mesh to root component

		// Retrieve floor asset 
		static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("StaticMesh'/Game/Meshes/Floor.Floor'"));
		// If floor asset is retrieved successfully 
		if (StaticMeshAsset.Succeeded())
		{
			// Set floor asset to static mesh
			StaticMeshes[index]->SetStaticMesh(StaticMeshAsset.Object);

			// Set relative location values to calculated values
			StaticMeshes[index]->SetRelativeLocation(FVector(XCoordinate, YCoordinate, 0));
		}
	}
	/********** CREATION OF BOARD GRIDS **********/
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();

	/* Set materials for grids based on their location. */
	for (int32 index = 0; index < 64; ++index)
	{
		// Calculations to set materials 
		int32 First = (index / 8) % 2;
		int32 Second = (index % 8) % 2;
		bool Result = XOR(First, Second);
		if (Result)
		{
			StaticMeshes[index]->SetMaterial(0, LightMaterial);
			DefaultMaterial = LightMaterial;
		}
		else
		{
			StaticMeshes[index]->SetMaterial(0, DarkMaterial);
			DefaultMaterial = DarkMaterial;
		}
	}
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoard::HighlightGrid(UStaticMeshComponent* Grid)
{
	Grid->SetMaterial(0, HighlightedMaterial);
}

