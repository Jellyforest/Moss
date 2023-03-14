// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"git 
#include "MainCharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class MOSS_API UMainCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	float speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float direction = 0;

	//�������� ���ŵǴ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	//���߿���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	bool isInAir = false;
	
	//����� ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category=PlayerAnim)
	class UAnimMontage* attackAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* finishAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* magicAnimMontage;

	//���� �ִϸ��̼� ����Լ�
	void PlayAttackAnim();
	void PlayFinishAnim();
	void PlayMagicAnim();

	UFUNCTION()
	void AnimNotify_FinishEnd();
	UFUNCTION()
	void AnimNotify_MagicEnd();

	bool isMagicEnd=true;
};
