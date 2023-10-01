// Fill out your copyright notice in the Description page of Project Settings.


#include "KCharacter.h"

#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
AKCharacter::AKCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* Camera boom */
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 180.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	/* Camera */
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;




}

// Called when the game starts or when spawned
void AKCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent)
	{
		return;
	}


	PlayerInputComponent->BindAxis("MoveForwardBackward", this, &AKCharacter::MoveForwardBackward);
	PlayerInputComponent->BindAxis("MoveLeftRight", this, &AKCharacter::MoveLeftRight);


	PlayerInputComponent->BindAxis("TurnLeftRight", this, &AKCharacter::TurnLeftRight);
	PlayerInputComponent->BindAxis("TurnUpDown", this, &AKCharacter::TurnUpDown);


}

void AKCharacter::MoveForwardBackward(float value)
{
	if (value == 0)
	{
		return;
	}

	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YawControllerRotation(0.0, ControllerRotation.Yaw, 0.0);
	const FRotationMatrix RotationYawMatrix = FRotationMatrix(YawControllerRotation);
	const FVector DirectionVec = RotationYawMatrix.GetUnitAxis(EAxis::X);

	AddMovementInput(DirectionVec, value);

	//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + DirectionVec * 500, 5, FColor::Red, 0, 5, 0, 3);

}

void AKCharacter::MoveLeftRight(float value)
{
	if (value == 0)
	{
		return;
	}

	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YawControllerRotation(0.0, ControllerRotation.Yaw, 0.0);
	const FRotationMatrix RotationYawMatrix = FRotationMatrix(YawControllerRotation);
	const FVector DirectionVec = RotationYawMatrix.GetUnitAxis(EAxis::Y);

	AddMovementInput(DirectionVec, value);

	//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + DirectionVec * 500, 5, FColor::Green, 0, 5, 0, 3);

}

void AKCharacter::TurnLeftRight(float value)
{
	AddControllerYawInput(value);
}

void AKCharacter::TurnUpDown(float value)
{
	AddControllerPitchInput(value);
}

