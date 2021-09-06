// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootIK.generated.h"


typedef struct IKTraceInfo
{
	float Offset;
	FVector ImpactLocation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UFootIK : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootIK();

private:
	UPROPERTY()
	class ACharacter* pCharacter;

	bool bIsActive;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	IKTraceInfo IKFootTrace(float TraceDistance, FName Socket);

	float IKCapsuleHalfHeight;

	void IKUpdate(float fDeltaTime);

	void IKUpdateFootOffset(float DeltaTime, float TargetValue, float* EffectorValue, float InterpSpeed);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
	FName IKSocketNameLeftFoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
	FName IKSocketNameRightFoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Debug")
	bool bIKDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKAdjustOffset;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKTraceDistance;

	void SetIKSocketName(FString SocLeftFoot, FString SocRightFoot);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetIKActive(bool bActive) { bIsActive = bActive; };
};
