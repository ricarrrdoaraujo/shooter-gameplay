// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
    if (ShooterCharacter == nullptr)
    {
        ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
    }
    if (ShooterCharacter)
    {
        // get the lateral speed of the character from velocity
        FVector Velocity{ShooterCharacter->GetVelocity()};
        Velocity.Z = 0;
        Speed = Velocity.Size();

        //is the character in the air?
        bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

        // is the character accelerating?
        if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }
    }
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
    ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
