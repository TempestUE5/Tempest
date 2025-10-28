// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

#include "DrownMeNot/EnhancedInputAbilitySystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrownMeNot/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<UEnhancedInputAbilitySystem>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(AscReplicationMode);

    BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));

    GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = true;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

    HurricaneOrbMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HurricaneOrbMeshComponent"));

    HurricaneOrbMeshComponent->SetMobility(EComponentMobility::Movable);

    UStaticMesh* HurricaneOrbMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Weapons/HurricaneOrb/SM_HurricaneOrb.SM_HurricaneOrb"));

    if (HurricaneOrbMesh) {
        HurricaneOrbMeshComponent->SetStaticMesh(HurricaneOrbMesh);
    }

    HurricaneOrbMeshComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
    Super::BeginPlay();

    if (!IsPlayerControlled()) {
        if (SpawnSound) {
            UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
        }

        if (SpawnEffect) {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                SpawnEffect,
                GetActorLocation(),
                GetActorRotation()
            );
        }
    }

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
    HurricaneOrbMeshComponent->AttachToComponent(GetMesh(), AttachmentRules, FName("HurricaneOrbSocket_R"));
}

void ACharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    if (AbilitySystemComponent) {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}


void ACharacterBase::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    if (AbilitySystemComponent) {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ACharacterBase::RestoreHealthToFull() {
	if (BasicAttributeSet) {
		float MaxHealth = BasicAttributeSet->GetMaxHealth();
		BasicAttributeSet->SetHealth(MaxHealth);
	}
}

