// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioCamera.h"
#include "GameConstants.h"
#include "MarioCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AMarioCamera::AMarioCamera() :
	Camera(nullptr),
	SphereComponent(nullptr),
	TileMapConfinesBox(nullptr),
	MarioCharacter(nullptr),
	OriginalZ(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("CameraSphereComponent");
	SphereComponent->SetVisibility(false);
	RootComponent = SphereComponent;

	TileMapConfinesBox = CreateDefaultSubobject<UBoxComponent>("CameraConfinesBox");
	TileMapConfinesBox->SetBoxExtent(FVector(GameConstants::ResolutionWidth * 0.5f, 10.0f, GameConstants::ResolutionHeight * 0.5f));
	TileMapConfinesBox->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Player Camera Component");
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->SetOrthoWidth(GameConstants::ResolutionWidth);
	Camera->SetAspectRatio(GameConstants::ResolutionAspectRatio);
	Camera->SetConstraintAspectRatio(true);
	Camera->SetRelativeLocation(FVector(0.0f, GameConstants::CameraDepthY, -80.0f));
	Camera->SetRelativeRotation(FQuat(FRotator(0.0f, GameConstants::CameraRotationYaw, 0.0f)));
	Camera->SetupAttachment(RootComponent);

	Tags.Add("MarioCamera");
}

// Called when the game starts or when spawned
void AMarioCamera::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	MarioCharacter = playerController->GetPawn<AMarioCharacter>();

	playerController->SetViewTargetWithBlend(this);

	OriginalZ = Camera->GetComponentLocation().Z - GameConstants::TileSize;
}

// Called every frame
void AMarioCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MarioCharacter != nullptr)
	{
		// Get mario and the camera locations
		FVector marioLocation = MarioCharacter->GetActorLocation();
		FVector cameraLocation = Camera->GetComponentLocation();

		// Set the X value
		cameraLocation.X = FMath::FInterpTo(cameraLocation.X, marioLocation.X, DeltaTime, 8.0f) + ShakeOffset.X;

		// Set the Z value
		cameraLocation.Z = InitialLocation.Z + ShakeOffset.Z;

		// Set the camera location
		SetCameraLocation(FVector2D(cameraLocation.X, cameraLocation.Z));
	}
}

void AMarioCamera::ApplyCameraShake()
{
	GetWorldTimerManager().SetTimer(ShakeTimer, this, &AMarioCamera::OnShake, 0.025f, true);
	InitialLocation = Camera->GetComponentLocation();
}

void AMarioCamera::SetCameraLocation(FVector2D location)
{
	float viewWidth = GameConstants::ResolutionWidth * 0.5f;
	float viewHeight = GameConstants::ResolutionHeight * 0.5f;

	FBoxSphereBounds confinesBoxBounds = TileMapConfinesBox->Bounds;
	float maxCameraX = confinesBoxBounds.Origin.X + confinesBoxBounds.BoxExtent.X - viewWidth;
	float minCameraX = confinesBoxBounds.Origin.X - confinesBoxBounds.BoxExtent.X + viewWidth;
	float maxCameraZ = confinesBoxBounds.Origin.Z + confinesBoxBounds.BoxExtent.Z - viewHeight;
	float minCameraZ = confinesBoxBounds.Origin.Z - confinesBoxBounds.BoxExtent.Z + viewHeight;

	FVector cameraLocation = Camera->GetComponentLocation();
	cameraLocation.X = FMath::Clamp(location.X, minCameraX, maxCameraX);
	cameraLocation.Z = FMath::Clamp(location.Y, minCameraZ, maxCameraZ);
	Camera->SetWorldLocation(cameraLocation);
}

void AMarioCamera::SetTileMapSize(int32 columns, int32 rows)
{
	float width = (float)columns * GameConstants::TileSize;
	float height = (float)rows * GameConstants::TileSize;
	float extentX = width * 0.5f;
	float extentZ = height * 0.5f;

	// Set the TileMap confine box's extent
	TileMapConfinesBox->SetBoxExtent(FVector(extentX, 10.0f, extentZ));

	// Center the Camera on the TileMap
	SphereComponent->SetWorldLocation(FVector(extentX, 0.0f, extentZ));
}

void AMarioCamera::OnShake()
{
	ShakeCount++;

	ShakeOffset.X = FMath::RandRange(-2.0f, 2.0f);
	ShakeOffset.Z = FMath::RandRange(-5.0f, 5.0f);

	if (ShakeCount >= GameConstants::MaxCameraShakes)
	{
		GetWorldTimerManager().ClearTimer(ShakeTimer);
		ShakeTimer.Invalidate();
		ShakeOffset = FVector::Zero();
		ShakeCount = 0;
		SetCameraLocation(FVector2D(Camera->GetComponentLocation().X, OriginalZ));
	}
}
