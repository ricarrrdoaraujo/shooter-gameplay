// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon():
    ThrowWeaponTime(0.7f),
    bFalling(false)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Keep the weapon upright
    if(GetItemState() == EItemState::EIS_Falling && bFalling)
    {
       const FRotator MeshRotation{0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
        GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
    }
}

void AWeapon::ThrowWeapon()
{
    FRotator MeshRotation{0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
    GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

    FVector MeshForward{GetItemMesh()->GetForwardVector()};
    const FVector MeshRight{GetItemMesh()->GetRightVector()};
    //Direction in which we throw the weapon
    FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);

    //FMath::FRandRange to randomize
    float RandomRotation{ FMath::FRandRange(-30.f, 30.f) };
    ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f,0.f,1.f));
    ImpulseDirection *= 2000.f;
    GetItemMesh()->AddImpulse(ImpulseDirection);

    bFalling = true;
    GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);
}

void AWeapon::StopFalling()
{
    bFalling = false;
    SetItemState(EItemState::EIS_Pickup);
}
