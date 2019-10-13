// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_DemoCharacter.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

// Sets default values
ACPP_DemoCharacter::ACPP_DemoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Allow Character to crouch.
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment((USceneComponent*)GetCapsuleComponent());

	FVector armLocation(3.1012802f, 0.0f, 9.5177116f);
	FRotator armRotation(0.0f, -50.0f, 0.0f);

	SpringArm->SetRelativeLocationAndRotation(armLocation, armRotation);
	SpringArm->TargetArmLength = 708.0245361f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment((USceneComponent*)SpringArm);

	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment((USceneComponent*)GetCapsuleComponent());
}

// Called when the game starts or when spawned
void ACPP_DemoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_DemoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_DemoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForwardandBack", this, &ACPP_DemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveLeftandRight", this, &ACPP_DemoCharacter::MoveRight);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACPP_DemoCharacter::ToggleCrouch);
}

bool ACPP_DemoCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor) const
{
	static const FName NAME_AILineofSight = FName(TEXT("TestPawnLineOfSight"));
	FHitResult result;
	FVector socketPosition = CharacterMesh->GetStaticMesh()->FindSocket("AI_LineOfSight")->RelativeLocation + GetActorLocation() + socketOffset;

	const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(result, ObserverLocation, socketPosition, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)), FCollisionQueryParams(NAME_AILineofSight, true, IgnoreActor));
	if (!bHitSocket || (result.Actor.IsValid() && result.Actor->IsOwnedBy(this)))
	{
		OutSeenLocation = socketPosition;
		OutSightStrength = 1;
		GEngine->AddOnScreenDebugMessage(1, 0.2f, FColor::Blue, FString::Printf(TEXT("Socket: %s"), *socketPosition.ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("SocketTrue"));
		return true;
	}
	const bool bHit = GetWorld()->LineTraceSingleByObjectType(result, ObserverLocation, GetActorLocation(), FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)), FCollisionQueryParams(NAME_AILineofSight, true, IgnoreActor));
	if (!bHit || (result.Actor.IsValid() && result.Actor->IsOwnedBy(this)))
	{
		OutSeenLocation = GetActorLocation();
		OutSightStrength = 1;
		GEngine->AddOnScreenDebugMessage(1, 0.2f, FColor::Blue, FString::Printf(TEXT("Actor: %s"), *socketPosition.ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("ActorTrue"));
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("BothFalse"));
	OutSightStrength = 0;
	return false;
}

void ACPP_DemoCharacter::MoveForward(float value)
{
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, value);
}

void ACPP_DemoCharacter::MoveRight(float value)
{
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(direction, value);
}

void ACPP_DemoCharacter::ToggleCrouch()
{
	if (CanCrouch())
		Crouch();
	else
		UnCrouch();
}
