// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AChessPlayer::AChessPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	CameraSpringArm->bDoCollisionTest = false;
	CameraSpringArm->TargetArmLength = 400.f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 3.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AChessPlayer::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<AChessController>(GetController());
}

// Called every frame
void AChessPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AddActorWorldOffset(GetCameraPanDirection());
}

// Called to bind functionality to input
void AChessPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Returns player score
int32 AChessPlayer::GetScore()
{
	return Score;
}

// Returns player name
FText AChessPlayer::GetName()
{
	return Name;
}

// Sets player name
void AChessPlayer::SetName(FText PlayerName)
{
	Name = PlayerName;
}

FVector AChessPlayer::GetCameraPanDirection()
{
	// Variable to hold mouse position
	float MousePositionX;
	float MousePositionY;
	float CameraDirectionX = 0;
	float CameraDirectionY = 0;

	PC->GetMousePosition(MousePositionX, MousePositionY);
	if (MousePositionX == 0)
	{
		CameraDirectionY = -1;
	}
	return FVector(CameraDirectionX, CameraDirectionY, 0);
}
