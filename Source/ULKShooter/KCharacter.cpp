// Fill out your copyright notice in the Description page of Project Settings.


#include "KCharacter.h"

#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"

#include "DrawDebugHelpers.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"


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



	/* Action */
	PlayerInputComponent->BindAction("FireLMB", IE_Pressed, this, &AKCharacter::FireLMB);


	/* Axis */

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

void AKCharacter::FireLMB()
{

	UE_LOG(LogTemp, Warning, TEXT("FireLMB()"))


	if (FireSoundEffect)
	{
		UGameplayStatics::PlaySound2D(this, FireSoundEffect);
	}


	const USkeletalMeshSocket* GunBarrelSocket = GetMesh()->GetSocketByName("GunBarrelSocket");
	if (GunBarrelSocket)
	{

		const FTransform SocketTransform = GunBarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}



		//Line Trace Hit
		FHitResult HitRst;

		const FVector Start{ SocketTransform.GetLocation() };
		const FQuat StartRotation{ SocketTransform.GetRotation() };
		const FVector RotationAxis{ StartRotation.GetAxisX() };
		const FVector End{ Start + RotationAxis * 500000.f };

		GetWorld()->LineTraceSingleByChannel(HitRst, Start, End, ECollisionChannel::ECC_Visibility);


// 		if (HitRst.bBlockingHit)
// 		{
// 			DrawDebugLine(GetWorld(), Start, End, FColor::Red, 0, 2);
// 			DrawDebugSphere(GetWorld(), HitRst.Location, 10, 10, FColor::Blue, 0, 2);
// 		}


		//Impact Effect

		if (HitRst.bBlockingHit)
		{
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitRst.Location);

			}
		}


		//Beam/Trace Effect
		if (BeamParticles)
		{

			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);

			//For Beam
			FVector BeamEndPoint{ End };

			if (HitRst.bBlockingHit)
			{
				BeamEndPoint = HitRst.Location;
			}


			if (Beam)
			{
				UE_LOG(LogTemp, Warning, TEXT("Beam->SetVectorParameter()"))
				Beam->SetVectorParameter(FName("Target"), BeamEndPoint);

			}

		}


	}




}
