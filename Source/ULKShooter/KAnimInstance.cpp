// Fill out your copyright notice in the Description page of Project Settings.


#include "KAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KCharacter.h"

void UKAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    KCharacter = Cast<AKCharacter>(TryGetPawnOwner());

}
//Frame
void UKAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);


    if (KCharacter == nullptr) return;

    FVector Velocity = KCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    bIsInAir = KCharacter->GetCharacterMovement()->IsFalling();

    bIsInputAccelerating = KCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

}
