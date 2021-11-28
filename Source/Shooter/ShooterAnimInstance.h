// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UShooterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);
	
	virtual void NativeInitializeAnimation() override;

protected:

	//Handle turn in place variables
	void TurnInPlace();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta =(AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta =(AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta =(AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta =(AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	//Offset Yaw used for strafing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement, meta =(AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	//Offset yaw the frame before we stopped moving
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement, meta =(AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta =(AllowPrivateAccess = "true"))
	bool bAiming;

	//Yaw of the character this frame
	float CharacterYaw;

	//Yaw of the character the previous frame
	float CharacterYawLastFrame;

	//the pitch of the aim rotation, used for aim offset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Turn In Place", meta =(AllowPrivateAccess = "true"))
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Turn In Place", meta =(AllowPrivateAccess = "true"))
	float RootYawOffset;

	//true when reloading, used to prevent aim offset while reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Turn In Place", meta =(AllowPrivateAccess = "true"))
	bool bReloading;
	
	//Rotation curve value this frame
	float RotationCurve;

	//Rotation curve value last frame
	float RotationCurveLastFrame;
};
