// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ULKSHOOTER_API AKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/* Camera Boom */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	/* Camera */
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;


	/* Input */

	void MoveForwardBackward(float value);
	void MoveLeftRight(float value);
	void TurnLeftRight(float value);
	void TurnUpDown(float value);

	/* Weapon */
	void FireLMB();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Combat")
	class USoundCue* FireSoundEffect;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Combat")
	class UParticleSystem* MuzzleFlash;
};
