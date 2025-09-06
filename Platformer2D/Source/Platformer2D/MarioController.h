// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MarioController.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API AMarioController : public APlayerController
{
	GENERATED_BODY()

public:
	AMarioController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input")
	class UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input")
	class UInputAction* RunInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input")
	class UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input")
	class UInputAction* DuckInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input")
	class UInputAction* UpInputAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input")
	class UInputAction* ClickInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced input")
	class UInputAction* PauseInputAction;


	float GetMoveValue();
	bool IsRunPressed();
	bool IsJumpPressed();
	bool IsDuckPressed();
	bool IsUpPressed();
	bool IsClickPressed();
	bool IsPausedPressed();

protected:
	void OnMove(const struct FInputActionValue& Value);
	void OnMoveReleased(const struct FInputActionValue& Value);

	void OnRunPressed(const struct FInputActionValue& Value);
	void OnRunReleased(const struct FInputActionValue& Value);

	void OnJumpPressed(const struct FInputActionValue& Value);
	void OnJumpReleased(const struct FInputActionValue& Value);

	void OnDuckPressed(const struct FInputActionValue& Value);
	void OnDuckReleased(const struct FInputActionValue& Value);

	void OnUpPressed(const struct FInputActionValue& Value);

	void OnClickPressed(const struct FInputActionValue& Value);
	void OnClickReleased(const struct FInputActionValue& Value);

	void OnPausePressed(const struct FInputActionValue& Value);

private:
	class AMarioCharacter* MarioCharacter;
	class AMarioPlayerState* MarioPlayerState;
	class UEnhancedInputLocalPlayerSubsystem* InputSubsystem;
};
