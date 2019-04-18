// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollisionBox.generated.h"

UCLASS()
class CHESS_API ACollisionBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollisionBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Box component to spawn
	UPROPERTY()
		class UBoxComponent* CollisionBox;

};
