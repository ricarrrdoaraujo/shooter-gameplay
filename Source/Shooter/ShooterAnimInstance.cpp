// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UShooterAnimInstance::UShooterAnimInstance() :
    Speed(0.f),
    bIsInAir(false),
    bIsAccelerating(false),
    MovementOffsetYaw(0.f),
    LastMovementOffsetYaw(0.f),
    bAiming(false),
    TIPCharacterYaw(0.f),
    TIPCharacterYawLastFrame(0.f),
    CharacterRotation(FRotator(0.f)),
    CharacterRotationLastFrame(FRotator(0.f)),
    RootYawOffset(0.f),
    Pitch(0.f),
    bReloading(false),
    OffsetState(EOffsetState::EOS_Hip),
    YawDelta(0.f),
    RecoilWeight(1.f),
    bTurnInPlace(false)
{
    
}

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
    if (ShooterCharacter == nullptr)
    {
        ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
    }
    if (ShooterCharacter)
    {
        bCrouching = ShooterCharacter->GetCrouching();
        bReloading = ShooterCharacter->GetCombatState() == ECombatState::ECS_Reloading;
        
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

        FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
        FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
        MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator( MovementRotation, AimRotation).Yaw;

        if(ShooterCharacter->GetVelocity().Size() > 0.f)
        {
            LastMovementOffsetYaw = MovementOffsetYaw;
        }

        bAiming = ShooterCharacter->GetAiming();
        
        if (bReloading)
        {
            OffsetState = EOffsetState::EOS_Reloading;
        }
        else if (bIsInAir)
        {
            OffsetState = EOffsetState::EOS_InAir;
        }
        else if (ShooterCharacter->GetAiming())
        {
            OffsetState = EOffsetState::EOS_Aiming;
        }
        else
        {
            OffsetState = EOffsetState::EOS_Hip;
        }

    }
    TurnInPlace();
    Lean(DeltaTime);
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
    ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UShooterAnimInstance::TurnInPlace()
{
    if(ShooterCharacter == nullptr) return;

    Pitch = ShooterCharacter->GetBaseAimRotation().Pitch;
    
    if(Speed > 0 || bIsInAir)
    {
        // Dont want to turn in place, character is moving
        RootYawOffset = 0;
        TIPCharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
        TIPCharacterYawLastFrame = TIPCharacterYaw;
        RotationCurveLastFrame = 0.f;
        RotationCurve = 0.f;
    }
    else
    {
        TIPCharacterYawLastFrame = TIPCharacterYaw;
        TIPCharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
        const float TIPYawDelta{TIPCharacterYaw - TIPCharacterYawLastFrame};

        // Root Yaw Offset, updated and clamped to [-180, 180]
        RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

        // GetCurveValue(TEXT("Turning")) - 0 if not playing the animation
        const float Turning{ GetCurveValue(TEXT("Turning")) };

        // 1.0 if turning, 0.0 if not
        if (Turning > 0)
        {
            bTurnInPlace = true; 
            RotationCurveLastFrame = RotationCurve;
            RotationCurve = GetCurveValue(TEXT("Rotation"));
            const float DeltaRotation{RotationCurve - RotationCurveLastFrame};

            // RootYawOffset > 0, TurnLeft
            // RootYawOffset < 0, TurnRight
            RootYawOffset > 0 ?  RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

            const float ABSRootYawOffset{FMath::Abs(RootYawOffset)};
            if (ABSRootYawOffset > 90.f)
            {
                const float YawExcess{ABSRootYawOffset - 90.f};
                RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
            }
        }
        else
        {
            bTurnInPlace = false;
        }

        if (bTurnInPlace)
        {
            if (bReloading)
            {
                RecoilWeight = 1.f;
            }
            else
            {
                RecoilWeight = 0.f;
            }
        }
        else // not turning in place
        {
            if (bCrouching)
            {
                if (bReloading)
                {
                    RecoilWeight = 1.f;
                }
                else
                {
                    RecoilWeight = 0.1f;
                }
            }
            else
            {
                if (bAiming || bReloading)
                {
                    RecoilWeight = 1.f;
                }
                else
                {
                    RecoilWeight = 0.5f;
                }
            }
        }
    }
}

void UShooterAnimInstance::Lean(float DeltaTime)
{
    if (ShooterCharacter == nullptr) return;
    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation = ShooterCharacter->GetActorRotation();

    const FRotator Delta{UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame)};

    const float Target{Delta.Yaw / DeltaTime};

    const float Interp{FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.f)};

    YawDelta = FMath::Clamp(Interp, -90.f, 90.0f);
}
