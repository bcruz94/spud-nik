// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "CPP_DemoCharacter.generated.h"

UCLASS()
class SPUDNIK_POCT_API ACPP_DemoCharacter : public ACharacter, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_DemoCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to dictate where AIs can see the character from
	virtual bool CanBeSeenFrom(
		const FVector &ObserverLocation,
		FVector &OutSeenLocation,
		int32 &NumberOfLoSChecksPerformed,
		float &OutSightStrength,
		const AActor *IgnoreActor = nullptr
	) const;

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void ToggleCrouch();

	UFUNCTION()
	void HugWall();

	UFUNCTION()
	void UnHugWall();

	UFUNCTION(BlueprintCallable)
	void EnableHug(FVector axis, FVector lowVector, FVector highVector);

	UFUNCTION(BlueprintCallable)
	void DisableHug();

	UPROPERTY(EditDefaultsOnly, Category = Mesh, BlueprintReadWrite)
	UStaticMeshComponent *CharacterMesh;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent *Camera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FVector socketOffset;

private:
	void SetLimits(FVector left, FVector right);

	bool bCanHug = false;
	bool bIsHugging = false;
	FVector hugAxis;
	FVector lowLimit;
	FVector highLimit;
};
