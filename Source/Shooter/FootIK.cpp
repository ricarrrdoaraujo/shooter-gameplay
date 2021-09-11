// Fill out your copyright notice in the Description page of Project Settings.


#include "FootIK.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UFootIK::UFootIK() :
	IKAdjustOffset(5.0f),
	IKTraceDistance(55.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFootIK::BeginPlay()
{
	Super::BeginPlay();

	pCharacter = Cast<ACharacter>(GetOwner());
	if (pCharacter == nullptr) return;

	IKCapsuleHalfHeight = pCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SetIKActive(true);
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
}

void UFootIK::SetIKSocketName(FString SocLeftFoot, FString SocRightFoot)
{
	IKSocketNameLeftFoot = FName(*SocLeftFoot);
	IKSocketNameRightFoot = FName(*SocRightFoot);
	
}

// Called every frame
void UFootIK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (pCharacter == nullptr) return;
	if (bIsActive == false) return;
	
	UE_LOG(LogTemp, Warning, TEXT("TickComponent"));
	IKUpdate(DeltaTime);
}

IKTraceInfo UFootIK::IKFootTrace(float TraceDistance, FName Socket)
{
	UE_LOG(LogTemp, Warning, TEXT("IKFootTrace"));
	IKTraceInfo TraceInfo;

	FVector SocketLocation = pCharacter->GetMesh()->GetSocketLocation(Socket);
	FVector LineStart = FVector(SocketLocation.X, SocketLocation.Y, pCharacter->GetActorLocation().Z);
	FVector LineEnd = FVector(SocketLocation.X, SocketLocation.Y, (
		pCharacter->GetActorLocation().Z - IKCapsuleHalfHeight) - TraceDistance);

	FHitResult IKHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (bIKDebug == true) eDebug = EDrawDebugTrace::ForOneFrame;

	bool bHitResult = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		LineStart,
		LineEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		pIgnore,
		eDebug,
		IKHitResult,
		true);

	TraceInfo.ImpactLocation = IKHitResult.ImpactNormal;
	if (IKHitResult.IsValidBlockingHit() == true)
	{
		float ImpactLength = (IKHitResult.ImpactPoint - IKHitResult.TraceEnd).Size();
		TraceInfo.Offset = IKAdjustOffset + (ImpactLength - TraceDistance);
	}
	else
	{
		TraceInfo.Offset = 0.0f;
	}

	return TraceInfo;
}

void UFootIK::IKUpdate(float fDeltaTime)
{
	IKTraceInfo TraceLeft = IKFootTrace(IKTraceDistance, IKSocketNameLeftFoot);
	IKTraceInfo TraceRight = IKFootTrace(IKTraceDistance, IKSocketNameRightFoot);
	UE_LOG(LogTemp, Warning, TEXT("IKUpdate"));

	IKUpdateFootOffset(fDeltaTime, TraceLeft.Offset, &EffectorLocationLeft, 13.0f);
	IKUpdateFootOffset(fDeltaTime, TraceRight.Offset, &EffectorLocationRight, 13.0f);
}

void UFootIK::IKUpdateFootOffset(float DeltaTime, float TargetValue, float* EffectorValue, float InterpSpeed)
{
	float InterpValue = UKismetMathLibrary::FInterpTo(*EffectorValue, TargetValue, DeltaTime, InterpSpeed);
	*EffectorValue = InterpValue;
}

