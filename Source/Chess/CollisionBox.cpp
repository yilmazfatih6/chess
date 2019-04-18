// Fill out your copyright notice in the Description page of Project Settings.
#include "CollisionBox.h"
#include "Components/BoxComponent.h"

// Sets default values
ACollisionBox::ACollisionBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Creates a box component and assigns it as root component. Also defines box size as 100,100,100.
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->InitBoxExtent(FVector(125, 125, 500));
}

// Called when the game starts or when spawned
void ACollisionBox::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACollisionBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

