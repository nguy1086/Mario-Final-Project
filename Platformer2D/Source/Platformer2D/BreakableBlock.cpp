// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableBlock.h"
#include "BreakableBlockBit.h"
#include "GameConstants.h"
#include "MarioCharacter.h"
#include "MarioPlayerState.h"
#include "PickupConstants.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h" 

// Sets default values
ABreakableBlock::ABreakableBlock() :
	BoxComponent(nullptr),
	FlipbookComponent(nullptr),
	BreakableBlockBitTemplate(nullptr),
	Start(),
	Target(),
	Original(),
	AnimationTimer(0.0f),
	State(EBreakableBlockState::Unknown)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the BoxComponent and set it as the Root component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BreakableBlockBoxComponent");
	BoxComponent->SetCollisionProfileName("BlockAllDynamic");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->OnComponentHit.AddDynamic(this, &ABreakableBlock::OnHit);
	RootComponent = BoxComponent;

	// Create the FlipbookComponent and attach it to the Root component
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("BreakableBlockFlipbook");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionProfileName("NoCollision");
	FlipbookComponent->SetupAttachment(RootComponent);

	// Add "BreakableBlock" to the Actor tags
	Tags.Add("BreakableBlock");
}

// Called when the game starts or when spawned
void ABreakableBlock::BeginPlay()
{
	Super::BeginPlay();
	
	if (BlockSound != nullptr)
		UGameplayStatics::PrimeSound(BlockSound);

	SetState(EBreakableBlockState::Active);
	FlipbookComponent->PlayFromStart();
}

// Called every frame
void ABreakableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EBreakableBlockState::AnimUp)
	{
		AnimationTimer -= DeltaTime;

		float pct = 1.0f - (AnimationTimer / PickupConstants::ItemBoxAnimationDuration);
		FVector location = FMath::Lerp(Start, Target, pct);
		SetActorLocation(location);

		if (AnimationTimer <= 0.0f)
		{
			AnimationTimer = 0.0f;
			SetState(EBreakableBlockState::AnimDown);
		}
	}
	else if (State == EBreakableBlockState::AnimDown)
	{
		AnimationTimer -= DeltaTime;

		float pct = 1.0f - (AnimationTimer / PickupConstants::ItemBoxAnimationDuration);
		FVector location = FMath::Lerp(Start, Target, pct);
		SetActorLocation(location);

		if (AnimationTimer <= 0.0f)
		{
			AnimationTimer = 0.0f;

			SetState(EBreakableBlockState::Active);
		}
	}
}

bool ABreakableBlock::CanTransformToCoin()
{
	return State == EBreakableBlockState::Active;
}

void ABreakableBlock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor->ActorHasTag("MarioCharacter") && Hit.Normal.Z >= 0.8f))
	{
		// Get the MarioCharacter (Pawn) and the MarioPlayerState pointers
		AMarioCharacter* mario = GetWorld()->GetFirstPlayerController()->GetPawn<AMarioCharacter>();
		AMarioPlayerState* playerState = Cast<AMarioPlayerState>(mario->GetPlayerState());

		if (State == EBreakableBlockState::Active)
		{
			if (playerState->Form == EMarioForm::Small)
			{
				SetState(EBreakableBlockState::AnimUp);
			}
			else
			{
				if (BlockSound != nullptr)
					UGameplayStatics::PlaySoundAtLocation(this, BlockSound, GetActorLocation());

				SetState(EBreakableBlockState::Breaking);
				mario->StopJumping();
			}
		}
	}
	else if (OtherActor->ActorHasTag("Bullet") || 
		OtherActor->ActorHasTag("Rocket") || 
		OtherActor->ActorHasTag("Explosion"))
	{
		SetState(EBreakableBlockState::Breaking);
	}
}

void ABreakableBlock::SetState(EBreakableBlockState state)
{
	if (State != state)
	{
		State = state;

		if (State == EBreakableBlockState::AnimUp)
		{
			Original = GetActorLocation();
			Start = Original;
			Target = Start + FVector(0.0f, 0.0f, 8.0f);

			AnimationTimer = PickupConstants::ItemBoxAnimationDuration;
		}
		else if (State == EBreakableBlockState::AnimDown)
		{
			Start = Target;
			Target = Original;

			AnimationTimer = PickupConstants::ItemBoxAnimationDuration;
		}
		else if (State == EBreakableBlockState::Breaking)
		{
			FVector location = GetActorLocation();
			const int32 kNumBits = 4;
			ABreakableBlockBit* bits[kNumBits] = {};
			FVector locations[kNumBits] = { location + FVector(-8.0f, 0.0f, 8.0f),
											location + FVector(-8.0f, 0.0f, -8.0f),
											location + FVector(8.0f, 0.0f, 8.0f),
											location + FVector(8.0f, 0.0f, -8.0f) };

			FVector impulses[kNumBits] = {  FVector(-150.0f, 0.0f, 350.0f),
											FVector(-150.0f, 0.0f, 300.0f),
											FVector(150.0f, 0.0f, 350.0f),
											FVector(150.0f, 0.0f, 300.0f) };

			EBreakableBlockBitType types[kNumBits] = {  EBreakableBlockBitType::Left,
														EBreakableBlockBitType::Left,
														EBreakableBlockBitType::Right,
														EBreakableBlockBitType::Right };

			// Spawn the four BreakableBlockBit actors in the world
			for (int32 i = 0; i < kNumBits; i++)
			{
				bits[i] = GetWorld()->SpawnActor<ABreakableBlockBit>(BreakableBlockBitTemplate, locations[i], FRotator(0.0f, 0.0f, 0.0f));
				bits[i]->Spawn(types[i], impulses[i]);
			}

			// Lastly, destroy the BreakableBlock actor
			Destroy();
		}
	}
}