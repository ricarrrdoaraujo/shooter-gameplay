// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SubmachineGun UMETA(DysplayName = "SubmachineGun"),
	EWT_AssaultRifle UMETA(DysplayName = "AssaultRifle"),
	
	EWT_MAX UMETA(DysplayName = "DefaultMax")
};

/**
 * 
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	
	void StopFalling();

private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	//Ammo count for this Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	int32 Ammo;

	//The type of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

public:
	//Adds impulse to the weapon
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const {return Ammo;};

	//called from character class when firing weapon
	void DecrementAmmo();

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; };
};
