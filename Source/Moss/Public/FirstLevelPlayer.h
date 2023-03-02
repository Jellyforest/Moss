#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "FirstLevelPlayer.generated.h"

UCLASS()
class MOSS_API AFirstLevelPlayer : public APawn
{
	GENERATED_BODY()

public:
	AFirstLevelPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Turn(const FInputActionValue& Values);
	void Move(const FInputActionValue& Values);

public:
	UPROPERTY()
	class AMainCharacter* mainCharacter;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Input;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Mouse;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	FRotator prevRot;
	FRotator curRot;
};
