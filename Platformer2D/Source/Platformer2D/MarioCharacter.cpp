// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioCharacter.h"
#include "MarioPlayerState.h"
#include "MarioController.h"
#include "MarioCamera.h"
#include "Door.h"
#include "MarioConstants.h"
#include "GameConstants.h"
#include "PlatformerGameModeBase.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Fireball.h"
#include "Kismet/GameplayStatics.h"
#include "Vine.h"
#include "MarioPipe.h"

AMarioCharacter::AMarioCharacter() :
	PlayerState(nullptr),
	CurrentCamera(nullptr),
	CurrentDoor(nullptr),
	CurrentVine(nullptr),
	CurrentPipe(nullptr),
	RunningSegmentTimer(0.0f),
	DamagedTimer(0.0f),
	IsRunningBlocked(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision settings
	GetCapsuleComponent()->SetCollisionProfileName("Mario");
	GetCapsuleComponent()->SetCapsuleRadius(MarioConstants::CapsuleRadius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSmall);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMarioCharacter::OnHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMarioCharacter::OnOverlapBegan);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMarioCharacter::OnOverlapEnded);

	// Ground settings
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->MaxStepHeight = MarioConstants::MaxStepHeight;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));

	// Jumping settings
	GetCharacterMovement()->GravityScale = MarioConstants::GravityScale;
	GetCharacterMovement()->AirControl = MarioConstants::AirControl;
	GetCharacterMovement()->JumpZVelocity = MarioConstants::JumpZVelocity;
	JumpMaxHoldTime = MarioConstants::JumpMaxHoldTimeWalking;

	// Walking settings
	GetCharacterMovement()->GroundFriction = MarioConstants::GroundFriction;
	GetCharacterMovement()->MaxWalkSpeed = MarioConstants::MaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = MarioConstants::MaxAcceleration;
	GetCharacterMovement()->MinAnalogWalkSpeed = MarioConstants::MinWalkSpeed;

	// Set the Actor tag
	Tags.Add("MarioCharacter");
}

// Called when the game starts or when spawned
void AMarioCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get and cast the MarioPlayerState and assign it to the member variable
	PlayerState = GetPlayerState<AMarioPlayerState>();
	if (JumpSound != nullptr)
		UGameplayStatics::PrimeSound(JumpSound);

	if (DiedSound != nullptr)
		UGameplayStatics::PrimeSound(DiedSound);

	if (CoinSound != nullptr)
		UGameplayStatics::PrimeSound(CoinSound);

	if (OneUpSound != nullptr)
		UGameplayStatics::PrimeSound(OneUpSound);

	if (PowerUpSound != nullptr)
		UGameplayStatics::PrimeSound(PowerUpSound);

	if (PowerDownSound != nullptr)
		UGameplayStatics::PrimeSound(PowerDownSound);

	if (StompSound != nullptr)
		UGameplayStatics::PrimeSound(StompSound);

	// Restart the Character before spawning
	Restart();
}

// Called every frame
void AMarioCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the Flipbook
	UpdateFlipbook();

	// Safety check the PlayerState
	if (PlayerState != nullptr)
	{
		// Ensure that the player isn't Dead
		if (PlayerState->State != EMarioState::Dead)
		{
			// If Mario falls of the edge, he is no longer on the ground, the movement component 
			// catches this situation, update the IsOnGround value
			if (GetMovementComponent()->IsFalling() && PlayerState->IsOnGround)
				PlayerState->IsOnGround = false;

			// Handle the situation where Mario is Invincible, this 
			// occurs right after Mario takes damage
			if (IsInvincible())
			{
				PlayerState->InvincibilityTimer -= DeltaTime;

				if (PlayerState->InvincibilityTimer <= 0.0f)
				{
					PlayerState->InvincibilityTimer = 0.0f;
					GetCapsuleComponent()->SetCollisionProfileName("Mario");

					GetSprite()->SetVisibility(true);
				}
				else
				{
					DamagedTimer -= DeltaTime;
					if (DamagedTimer <= 0.0f)
					{
						DamagedTimer = MarioConstants::InvincibleVisibilityDuration;
						GetSprite()->ToggleVisibility();
					}
				}
			}

			// Handle the situation where Mario is Running 
			if (PlayerState->IsRunning && IsRunningBlocked == false)
			{
				if (GetMovementComponent()->GetLastInputVector().X != 0.0f && PlayerState->IsOnGround)
				{
					// Increment the segment timer
					RunningSegmentTimer += DeltaTime;

					// Is the segment timer greater than the duration constant?
					if (RunningSegmentTimer > MarioConstants::RunSegmentIncrementDuration)
					{
						// Reset the segment timer and increment the running meter
						RunningSegmentTimer = 0.0f;
						PlayerState->RunningMeter++;

						// Cap the running meter to the max constant and update the flipbook so that Mario's arms are out
						if (PlayerState->RunningMeter >= MarioConstants::MaxRunningMeter)
						{
							PlayerState->RunningMeter = MarioConstants::MaxRunningMeter;
							UpdateFlipbook();
						}

						// Set the jump max time to factor in the running meter, running faster means Mario jumps higher and thus farther
						JumpMaxHoldTime = MarioConstants::JumpMaxHoldTimeRunning + MarioConstants::JumpIncreasePerSegment * (float)PlayerState->RunningMeter;

						// Set character movement's max walk speed to factor in the running meter
						GetCharacterMovement()->MaxWalkSpeed = MarioConstants::MaxWalkSpeed + (PlayerState->RunningMeter * MarioConstants::RunSpeedPerSegment);

						// Set the flipbook's playback rate based on the running meter
						float playRate = 1.0f + PlayerState->RunningMeter * 0.75f;
						GetSprite()->SetPlayRate(playRate);
					}
				}
			}
			else
			{
				// If there's still some running meter built up, but the player is 
				// not running, gradually decrease the running meter
				if (PlayerState->RunningMeter > 0)
				{
					// Increment the segment timer
					RunningSegmentTimer += DeltaTime;

					// Is the segment timer greater than the duration constant?
					if (RunningSegmentTimer > MarioConstants::RunSegmentDecrementDuration)
					{
						// Reset the segment timer and decrement the running meter
						RunningSegmentTimer = 0.0f;
						PlayerState->RunningMeter--;

						// Ensure the running meter doesn't go lower than zero
						if (PlayerState->RunningMeter <= 0)
						{
							PlayerState->RunningMeter = 0;

							// Reset the Jump max time and the character's max walk speed to the walking default
							JumpMaxHoldTime = MarioConstants::JumpMaxHoldTimeWalking;
							GetCharacterMovement()->MaxWalkSpeed = MarioConstants::MaxWalkSpeed;

							// Reset the flipbook's playback rate to 1.0f
							GetSprite()->SetPlayRate(1.0f);
						}
					}
					else
					{
						// Set the jump max time to factor in the running meter, running faster means Mario jumps higher and thus farther
						JumpMaxHoldTime = MarioConstants::JumpMaxHoldTimeRunning + MarioConstants::JumpIncreasePerSegment * (float)PlayerState->RunningMeter;

						// Set character movement's max walk speed to factor in the running meter
						GetCharacterMovement()->MaxWalkSpeed = MarioConstants::MaxWalkSpeed + (PlayerState->RunningMeter * MarioConstants::RunSpeedPerSegment);

						// Set the flipbook's playback rate based on the running meter
						float playRate = 1.0f + PlayerState->RunningMeter * 0.75f;
						GetSprite()->SetPlayRate(playRate);
					}
				}
			}

			// Reset the running blocked flag to false
			IsRunningBlocked = false;

			// Mario has fallen off the edge of the level and has died
			if (GetActorLocation().Z < GameConstants::DestroyActorAtZ)
			{
				MarioHasDied(false);
			}

			//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Running meter: %i"), PlayerState->RunningMeter));
			//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Coins: %i"), PlayerState->Coins));
		}
	}
}

void AMarioCharacter::Respawn(FVector location)
{
	Restart();

	SetActorHiddenInGame(false);
	SetActorLocation(location);

	ResetJumpState();

	IsRunningBlocked = false;

	CurrentDoor = nullptr;
	CurrentVine = nullptr;
	CurrentPipe = nullptr;

	if (PlayerState->State == EMarioState::Dead)
	{
		ApplyTransformChange(EMarioForm::Small, true);

		AMarioController* marioController = Cast<AMarioController>(Controller);
		if (marioController != nullptr)
		{
			marioController->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));

			PlayerState->Direction = EMarioDirection::Right;
		}
	}

	// Set the state back to Idle
	ApplyStateChange(EMarioState::Idle);

	// Clear the movement forces
	GetCharacterMovement()->ClearAccumulatedForces();
}

void AMarioCharacter::Run()
{
	if (PlayerState != nullptr)
		PlayerState->IsRunning = true;

	// Reset the the Running segment timer to zero, this 
	// timer determines when to increase the running meter
	RunningSegmentTimer = 0.0f;

	// Set the jump max time to the running constant
	JumpMaxHoldTime = MarioConstants::JumpMaxHoldTimeRunning;
}

void AMarioCharacter::StopRunning()
{
	// Set the PlayerState's IsRunning flag to false
	if (PlayerState != nullptr)
		PlayerState->IsRunning = false;

	// Reset the the Running segment timer to zero
	RunningSegmentTimer = 0.0f;
}

void AMarioCharacter::Duck()
{
	ApplyStateChange(EMarioState::Ducking);
}

void AMarioCharacter::StopDucking()
{
	AMarioController* marioController = Cast<AMarioController>(Controller);
	if (marioController != nullptr)
	{
		if (marioController->GetMoveValue() == 0.0f)
		{
			ApplyStateChange(EMarioState::Idle);
		}
		else
		{
			ApplyStateChange(EMarioState::Walking);
		}
	}
}

void AMarioCharacter::TryDoor()
{
	if (CurrentDoor != nullptr)
	{
		CurrentDoor->DoorAction();
	}
	else if (CurrentVine != nullptr)
	{
		CurrentVine->DoorAction();
	}
	else if (CurrentPipe != nullptr)
	{
		if (PowerDownSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, PowerDownSound, GetActorLocation());

		CurrentPipe->DoorAction();
	}
}

void AMarioCharacter::ApplyStateChange(EMarioState newState)
{
	if (PlayerState == nullptr)
		return;

	// Ensure the new mario state is different than the current state
	if (PlayerState->State == newState)
		return;

	// Assign the new state
	EMarioState oldState = PlayerState->State;
	PlayerState->State = newState;

	if (newState == EMarioState::Jumping)
	{
		GetCharacterMovement()->bNotifyApex = true;
	}

	// Handle the duck resize and reposition when mario is big
	if (PlayerState->Form == EMarioForm::Super)
	{
		if (newState == EMarioState::Ducking)
		{

			GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSmall);
			GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, -17.0f));

			FVector location = GetActorLocation();
			location.Z -= 11.0f;
			SetActorLocation(location);
		}
		else
		{
			if (oldState == EMarioState::Ducking)
			{
				GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSuper);
				GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, -27.0f));

				FVector location = GetActorLocation();
				location.Z += 11.0f;
				SetActorLocation(location);
			}
		}
	}
	else if (PlayerState->Form == EMarioForm::Fire)
	{
		if (newState == EMarioState::Ducking)
		{

			GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSmall);
			GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

			FVector location = GetActorLocation();
			location.Z -= 11.0f;
			SetActorLocation(location);
		}
		else
		{
			if (oldState == EMarioState::Ducking)
			{
				GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSuper);
				GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

				FVector location = GetActorLocation();
				location.Z += 11.0f;
				SetActorLocation(location);
			}
		}
	}
	else if (PlayerState->Form == EMarioForm::Gun)
	{
		if (newState == EMarioState::Ducking)
		{

			GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSmall);
			GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

			FVector location = GetActorLocation();
			location.Z -= 11.0f;
			SetActorLocation(location);
		}
		else
		{
			if (oldState == EMarioState::Ducking)
			{
				GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSuper);
				GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

				FVector location = GetActorLocation();
				location.Z += 11.0f;
				SetActorLocation(location);
			}
		}
	}
	else if (PlayerState->Form == EMarioForm::Rocket)
	{
		if (newState == EMarioState::Ducking)
		{

			GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSmall);
			GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));

			FVector location = GetActorLocation();
			location.Z -= 11.0f;
			SetActorLocation(location);
		}
		else
		{
			if (oldState == EMarioState::Ducking)
			{
				GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSuper);
				GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

				FVector location = GetActorLocation();
				location.Z += 11.0f;
				SetActorLocation(location);
			}
		}
	}
	else if (PlayerState->Form == EMarioForm::Raccoon)
	{
		if (newState == EMarioState::Ducking)
		{

			GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSmall);
			GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));

			FVector location = GetActorLocation();
			location.Z -= 11.0f;
			SetActorLocation(location);
		}
		else
		{
			if (oldState == EMarioState::Ducking)
			{
				GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSuper);
				GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

				FVector location = GetActorLocation();
				location.Z += 11.0f;
				SetActorLocation(location);
			}
		}
	}
	// Lastly, update the flipbook sprites
	UpdateFlipbook();
}

void AMarioCharacter::ApplyTransformChange(EMarioForm newForm, bool noAnimation)
{
	if (PlayerState == nullptr)
		return;

	// Ensure the new mario form is different than the current form
	if (PlayerState->Form == newForm)
		return;

	// Assign the new form and then set the IsTransforming flag to true
	EMarioForm oldForm = PlayerState->Form;
	PlayerState->Form = newForm;

	// Update the capsule half height, based on the current form
	if (oldForm == EMarioForm::Small && newForm == EMarioForm::Super)
	{
		if (PowerUpSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, PowerUpSound, GetActorLocation());

		GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSuper);
		GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, -27.0f));

		FVector location = GetActorLocation();
		location.Z += 11.0f;
		SetActorLocation(location);
	}
	else if ((oldForm == EMarioForm::Fire || oldForm == EMarioForm::Gun || oldForm == EMarioForm::Rocket) && newForm == EMarioForm::Super)
	{
		if (PowerDownSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, PowerDownSound, GetActorLocation());

		GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSuper);
		GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, -27.0f));

		FVector location = GetActorLocation();
		location.Z += 11.0f;
		SetActorLocation(location);
	}
	else if ((oldForm == EMarioForm::Super || oldForm == EMarioForm::Fire || oldForm == EMarioForm::Gun || oldForm == EMarioForm::Rocket) && newForm == EMarioForm::Small)
	{
		if (PowerDownSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, PowerDownSound, GetActorLocation());

		GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSmall);
		GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, -17.0f));

		FVector location = GetActorLocation();
		location.Z -= 11.0f;
		SetActorLocation(location);
	}
	else if ((oldForm == EMarioForm::Super || oldForm == EMarioForm::Small) && (newForm == EMarioForm::Fire || newForm == EMarioForm::Gun || newForm == EMarioForm::Rocket || newForm == EMarioForm::Raccoon))
	{
		if (PowerUpSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, PowerUpSound, GetActorLocation());

		GetCapsuleComponent()->SetCapsuleHalfHeight(MarioConstants::CapsuleHalfHeightSuper);
		GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		FVector location = GetActorLocation();
		location.Z += 11.0f;
		SetActorLocation(location);
	}

	if (noAnimation == false)
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->SpawnMarioTransform(GetSprite()->GetComponentLocation(), oldForm, newForm, PlayerState->Direction);
		}
	}

	// Lastly, update the flipbook sprites
	UpdateFlipbook();
}

void AMarioCharacter::ApplyBounce()
{
	if (StompSound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(this, StompSound, GetActorLocation());

	LaunchCharacter(FVector(0.0f, 0.0f, 600.0f), false, true);
	GetPlayerState<AMarioPlayerState>()->IsOnGround = false;
}

void AMarioCharacter::HandleDamage(bool forceDead)
{
	if (PlayerState != nullptr && (IsInvincible() == false || forceDead))
	{
		if (PlayerState->Form == EMarioForm::Small || forceDead)
		{
			//fixes double death glitch and stops mario from being spawn camped
			DamagedTimer = MarioConstants::InvincibleVisibilityDuration;
			PlayerState->InvincibilityTimer = MarioConstants::InvincibleTime;
			GetCapsuleComponent()->SetCollisionProfileName("MarioInvincible");
			MarioHasDied(true);
		}
		else
		{
			DamagedTimer = MarioConstants::InvincibleVisibilityDuration;
			PlayerState->InvincibilityTimer = MarioConstants::InvincibleTime;
			GetCapsuleComponent()->SetCollisionProfileName("MarioInvincible");

			if (PlayerState->Form == EMarioForm::Super)
			{
				ApplyTransformChange(EMarioForm::Small);
			}
			else if (PlayerState->Form == EMarioForm::Fire || PlayerState->Form == EMarioForm::Gun)
			{
				ApplyTransformChange(EMarioForm::Super);
			}
			else
			{
				ApplyTransformChange(EMarioForm::Super);
			}
		}
	}
}

bool AMarioCharacter::IsInvincible()
{
	return GetPlayerState<AMarioPlayerState>()->InvincibilityTimer > 0.0f;
}

void AMarioCharacter::SpawnFireball()
{
	if (PlayerState->Form == EMarioForm::Fire)
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->SpawnFireball();
		}
	}
	else if (PlayerState->Form == EMarioForm::Gun)
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->SpawnBullet();
		}
	}
	else if (PlayerState->Form == EMarioForm::Rocket)
	{
		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->SpawnRocket();
		}
	}
}

void AMarioCharacter::SetJumpHoldTime()
{
	JumpMaxHoldTime = MarioConstants::JumpMaxHoldTimeRunning;
}

void AMarioCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("World"))
	{
		// Check if Mario collided with the World along the x-axis
		if (Hit.Normal.X <= -0.8f || Hit.Normal.X >= 0.8f)
		{
			IsRunningBlocked = true;
		}
	}
	else if (OtherActor->ActorHasTag("Pickup"))
	{
		if (OtherActor->ActorHasTag("MushroomPickup"))
		{
			if (PlayerState != nullptr && PlayerState->Form == EMarioForm::Small)
			{
				ApplyTransformChange(EMarioForm::Super);
			}
		}
		else if (OtherActor->ActorHasTag("OneUpPickup"))
		{
			if (PlayerState != nullptr && PlayerState->Lives > 0)
			{
				PlayerState->Lives++;
			}
		}
		else if (OtherActor->ActorHasTag("FlowerPickup"))
		{
			if (PlayerState != nullptr)
				ApplyTransformChange(EMarioForm::Fire);
		}
		else if (OtherActor->ActorHasTag("Gun"))
		{
			if (PlayerState != nullptr)
				ApplyTransformChange(EMarioForm::Gun);
		}
		else if (OtherActor->ActorHasTag("RocketLauncher"))
		{
			if (PlayerState != nullptr)
				ApplyTransformChange(EMarioForm::Rocket);
		}
		else if (OtherActor->ActorHasTag("LeafPickup"))
		{
			if (PlayerState != nullptr)
				ApplyTransformChange(EMarioForm::Raccoon);
		}

		// Destroy the pickup
		OtherActor->Destroy();
	}
}

void AMarioCharacter::OnOverlapBegan(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Pickup"))
	{
		if (OtherActor->ActorHasTag("CoinPickup"))
		{
			if (CoinSound != nullptr)
				UGameplayStatics::PlaySoundAtLocation(this, CoinSound, GetActorLocation());

			// Safety check the PlayerState then increment the Coins
			if (PlayerState != nullptr)
				PlayerState->Coins++;
		}
		else if (OtherActor->ActorHasTag("MushroomPickup"))
		{
			if (PlayerState != nullptr && PlayerState->Form == EMarioForm::Small)
				ApplyTransformChange(EMarioForm::Super);
		}
		else if (OtherActor->ActorHasTag("OneUpPickup"))
		{
			if (PlayerState != nullptr && PlayerState->Lives > 0)
			{
				if (OneUpSound != nullptr)
					UGameplayStatics::PlaySoundAtLocation(this, OneUpSound, GetActorLocation());

				PlayerState->Lives++;
			}
		}
		else if (OtherActor->ActorHasTag("FlowerPickup"))
		{
			if (PlayerState != nullptr)
				ApplyTransformChange(EMarioForm::Fire);
		}
		else if (OtherActor->ActorHasTag("Gun"))
		{
			if (PlayerState != nullptr)
				ApplyTransformChange(EMarioForm::Gun);
		}
		else if (OtherActor->ActorHasTag("RocketLauncher"))
		{
			if (PlayerState != nullptr)
				ApplyTransformChange(EMarioForm::Rocket);
		}
		else if (OtherActor->ActorHasTag("LeafPickup"))
		{
			if (PlayerState != nullptr)
				ApplyTransformChange(EMarioForm::Raccoon);
		}

		// Destroy the pickup
		OtherActor->Destroy();
	}
	//else if (OtherActor->ActorHasTag("Enemy"))
	//{
	//	if (OtherActor->ActorHasTag("PiranhaPlant"))
	//	{
	//		HandleDamage();
	//	}
	//	else if (OtherActor->ActorHasTag("Goomba"))
	//	{
	//		HandleDamage();
	//	}
	//	else if (OtherActor->ActorHasTag("Boo"))
	//	{
	//		HandleDamage();
	//	}
	//	else if (OtherActor->ActorHasTag("Thwomp"))
	//	{
	//		HandleDamage();
	//	}
	//	else if (OtherActor->ActorHasTag("Podoboo"))
	//	{
	//		HandleDamage();
	//	}
	//	else if (OtherActor->ActorHasTag("Disc"))
	//	{
	//		HandleDamage();
	//	}
	//	else if (OtherActor->ActorHasTag("BoomBoom"))
	//	{
	//		HandleDamage();
	//	}
	//}
	//else if (OtherActor->ActorHasTag("Lava") || OtherActor->ActorHasTag("Spikes"))
	//{
	//	HandleDamage();
	//}
	else if (OtherActor->ActorHasTag("Door"))
	{
		CurrentDoor = Cast<ADoor>(OtherActor);
	}
	else if (OtherActor->ActorHasTag("Vine"))
	{
		CurrentVine = Cast<AVine>(OtherActor);
	}
	else if (OtherActor->ActorHasTag("Pipe"))
	{
		CurrentPipe = Cast<AMarioPipe>(OtherActor);
	}
}

void AMarioCharacter::OnOverlapEnded(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Door"))
	{
		CurrentDoor = nullptr;
	}
	else if (OtherActor->ActorHasTag("Vine"))
	{
		CurrentVine = nullptr;
	}
	else if (OtherActor->ActorHasTag("Pipe"))
	{
		CurrentPipe = nullptr;
	}
}

void AMarioCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	if (JumpSound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());

	if (PlayerState != nullptr)
		PlayerState->IsOnGround = false;

	ApplyStateChange(EMarioState::Jumping);
}

void AMarioCharacter::NotifyJumpApex()
{
	if (PlayerState != nullptr && PlayerState->IsOnGround == false)
		ApplyStateChange(EMarioState::Falling);
}

void AMarioCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (PlayerState != nullptr)
	{
		PlayerState->IsOnGround = true;

		AMarioController* marioController = Cast<AMarioController>(Controller);
		if (marioController != nullptr)
		{
			if (marioController->IsDuckPressed())
			{
				ApplyStateChange(EMarioState::Ducking);
			}
			else
			{
				if (marioController->GetMoveValue() == 0.0f)
				{
					ApplyStateChange(EMarioState::Idle);
				}
				else
				{
					ApplyStateChange(EMarioState::Walking);
				}
			}
		}
	}
}

void AMarioCharacter::MarioHasDied(bool spawnDeadMario)
{
	if (PlayerState->State != EMarioState::Dead)
	{
		if (DiedSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, DiedSound, GetActorLocation());

		DamagedTimer = MarioConstants::InvincibleVisibilityDuration;
		PlayerState->InvincibilityTimer = MarioConstants::InvincibleTime;

		ApplyStateChange(EMarioState::Dead);

		SetActorHiddenInGame(true);

		APlatformerGameModeBase* gameMode = GetWorld()->GetAuthGameMode<APlatformerGameModeBase>();
		if (gameMode != nullptr)
		{
			gameMode->MarioHasDied(spawnDeadMario);
		}
	}
}

void AMarioCharacter::UpdateFlipbook()
{
	if (PlayerState == nullptr)
		return;

	UPaperFlipbook* currentFlipbook = GetSprite()->GetFlipbook();
	UPaperFlipbook* newFlipbook = nullptr;

	// Get the flipbook based on Mario's Form and State
	if (PlayerState->Form == EMarioForm::Small)
	{
		newFlipbook = GetSmallMarioFlipbook();
	}
	else if (PlayerState->Form == EMarioForm::Super)
	{
		newFlipbook = GetSuperMarioFlipbook();
	}
	else if (PlayerState->Form == EMarioForm::Fire)
	{
		newFlipbook = GetFireMarioFlipbook();
	}
	else if (PlayerState->Form == EMarioForm::Gun)
	{
		newFlipbook = GetGunMarioFlipbook();
	}
	else if (PlayerState->Form == EMarioForm::Rocket)
	{
		newFlipbook = GetRocketMarioFlipbook();
	}
	else if (PlayerState->Form == EMarioForm::Raccoon)
	{
		newFlipbook = GetRaccoonMarioFlipbook();
	}

	// Safety check that the new flipbook is not null and different than the current flipbook
	if (currentFlipbook != newFlipbook && newFlipbook != nullptr)
	{
		GetSprite()->SetFlipbook(newFlipbook);
		GetSprite()->PlayFromStart();

		if (PlayerState->IsRunning)
		{
			float playRate = 1.0f + PlayerState->RunningMeter * 0.2f;
			GetSprite()->SetPlayRate(playRate);
		}
		else
		{
			GetSprite()->SetPlayRate(1.0f);
		}
	}
}

UPaperFlipbook* AMarioCharacter::GetSmallMarioFlipbook()
{
	if (PlayerState == nullptr)
		return nullptr;

	UPaperFlipbook* flipbook = nullptr;

	if (PlayerState->State == EMarioState::Idle)
	{
		flipbook = SmallIdleFlipbook;
	}
	else if (PlayerState->State == EMarioState::Walking)
	{
		const FVector PlayerVelocity = GetVelocity();
		const FVector PlayerAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

		if (PlayerVelocity.X * PlayerAcceleration.X >= 0)
		{
			if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
			{
				flipbook = SmallRunFlipbook;
			}
			else
			{
				flipbook = SmallWalkFlipbook;
			}
		}
		else
		{
			flipbook = SmallTurnFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Jumping || PlayerState->State == EMarioState::Falling)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = SmallRunJumpFlipbook;
		}
		else
		{
			flipbook = SmallJumpFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Ducking)
	{
		flipbook = SmallIdleFlipbook;
	}

	return flipbook;
}

UPaperFlipbook* AMarioCharacter::GetSuperMarioFlipbook()
{
	if (PlayerState == nullptr)
		return nullptr;

	UPaperFlipbook* flipbook = nullptr;

	if (PlayerState->State == EMarioState::Idle)
	{
		flipbook = SuperIdleFlipbook;
	}
	else if (PlayerState->State == EMarioState::Walking)
	{
		const FVector PlayerVelocity = GetVelocity();
		const FVector PlayerAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

		if (PlayerVelocity.X * PlayerAcceleration.X >= 0)
		{
			if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
			{
				flipbook = SuperRunFlipbook;
			}
			else
			{
				flipbook = SuperWalkFlipbook;
			}
		}
		else
		{
			flipbook = SuperTurnFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Jumping)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = SuperRunJumpFlipbook;
		}
		else
		{
			flipbook = SuperJumpFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Falling)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = SuperRunJumpFlipbook;
		}
		else
		{
			flipbook = SuperFallingFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Ducking)
	{
		flipbook = SuperDuckFlipbook;
	}

	return flipbook;
}

UPaperFlipbook* AMarioCharacter::GetFireMarioFlipbook()
{
	if (PlayerState == nullptr)
		return nullptr;

	UPaperFlipbook* flipbook = nullptr;

	if (PlayerState->State == EMarioState::Idle)
	{
		flipbook = FireIdleFlipbook;
	}
	else if (PlayerState->State == EMarioState::Walking)
	{
		const FVector PlayerVelocity = GetVelocity();
		const FVector PlayerAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

		if (PlayerVelocity.X * PlayerAcceleration.X >= 0)
		{
			if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
			{
				flipbook = FireRunFlipbook;
			}
			else
			{
				flipbook = FireWalkFlipbook;
			}
		}
		else
		{
			flipbook = FireTurnFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Jumping)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = FireRunJumpFlipbook;
		}
		else
		{
			flipbook = FireJumpFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Falling)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = FireRunJumpFlipbook;
		}
		else
		{
			flipbook = FireFallingFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Ducking)
	{
		flipbook = FireDuckFlipbook;
	}

	return flipbook;
}

UPaperFlipbook* AMarioCharacter::GetGunMarioFlipbook()
{
	if (PlayerState == nullptr)
		return nullptr;

	UPaperFlipbook* flipbook = nullptr;

	if (PlayerState->State == EMarioState::Idle)
	{
		flipbook = GunIdleFlipbook;
	}
	else if (PlayerState->State == EMarioState::Walking)
	{
		const FVector PlayerVelocity = GetVelocity();
		const FVector PlayerAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

		if (PlayerVelocity.X * PlayerAcceleration.X >= 0)
		{
			if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
			{
				flipbook = GunRunFlipbook;
			}
			else
			{
				flipbook = GunWalkFlipbook;
			}
		}
		else
		{
			flipbook = GunTurnFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Jumping)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = GunRunJumpFlipbook;
		}
		else
		{
			flipbook = GunJumpFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Falling)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = GunRunJumpFlipbook;
		}
		else
		{
			flipbook = GunFallingFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Ducking)
	{
		flipbook = GunDuckFlipbook;
	}

	return flipbook;
}

UPaperFlipbook* AMarioCharacter::GetRocketMarioFlipbook()
{
	if (PlayerState == nullptr)
		return nullptr;

	UPaperFlipbook* flipbook = nullptr;

	if (PlayerState->State == EMarioState::Idle)
	{
		flipbook = RocketIdleFlipbook;
	}
	else if (PlayerState->State == EMarioState::Walking)
	{
		const FVector PlayerVelocity = GetVelocity();
		const FVector PlayerAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

		if (PlayerVelocity.X * PlayerAcceleration.X >= 0)
		{
			if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
			{
				flipbook = RocketRunFlipbook;
			}
			else
			{
				flipbook = RocketWalkFlipbook;
			}
		}
		else
		{
			flipbook = RocketTurnFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Jumping)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = RocketRunJumpFlipbook;
		}
		else
		{
			flipbook = RocketJumpFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Falling)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = RocketRunJumpFlipbook;
		}
		else
		{
			flipbook = RocketFallingFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Ducking)
	{
		flipbook = RocketDuckFlipbook;
	}

	return flipbook;
}

UPaperFlipbook* AMarioCharacter::GetRaccoonMarioFlipbook()
{
	if (PlayerState == nullptr)
		return nullptr;

	UPaperFlipbook* flipbook = nullptr;

	if (PlayerState->State == EMarioState::Idle)
	{
		flipbook = RaccoonIdleFlipbook;
	}
	else if (PlayerState->State == EMarioState::Walking)
	{
		const FVector PlayerVelocity = GetVelocity();
		const FVector PlayerAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

		if (PlayerVelocity.X * PlayerAcceleration.X >= 0)
		{
			if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
			{
				flipbook = RaccoonRunFlipbook;
			}
			else
			{
				flipbook = RaccoonWalkFlipbook;
			}
		}
		else
		{
			flipbook = RaccoonTurnFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Jumping)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = RaccoonRunJumpFlipbook;
		}
		else
		{
			flipbook = RaccoonJumpFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Falling)
	{
		if (PlayerState->IsRunning && PlayerState->RunningMeter == MarioConstants::MaxRunningMeter)
		{
			flipbook = RaccoonRunJumpFlipbook;
		}
		else
		{
			flipbook = RaccoonFallingFlipbook;
		}
	}
	else if (PlayerState->State == EMarioState::Ducking)
	{
		flipbook = RaccoonDuckFlipbook;
	}
	else if (PlayerState->State == EMarioState::Gliding)
	{
		flipbook = RaccoonGlideFlipbook;
	}
	else if (PlayerState->State == EMarioState::Flying)
	{
		flipbook = RaccoonFlyFlipbook;
	}

	return flipbook;
}