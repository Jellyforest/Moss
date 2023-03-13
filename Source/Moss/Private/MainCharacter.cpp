#include "MainCharacter.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacterAnim.h"
#include "EnemyFSM.h"
#include "Magic.h"
#include "Components/CapsuleComponent.h"
#include <Animation/AnimMontage.h>
#include <Components/ArrowComponent.h>

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterPreset"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	}

	//����ְ����
	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	//boxComp->SetupAttachment(GetMesh());
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	meshComp->SetRelativeLocation(FVector(-42, 7, 1));
	meshComp->SetRelativeRotation(FRotator(0, 90, 0));
	meshComp->SetCollisionProfileName(TEXT("WeaponPreset"));
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowComp"));
	arrowComp->SetupAttachment(RootComponent);
	//boxComp->SetCollisionProfileName(TEXT("Sword"));
	//boxComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	//boxComp->SetRelativeLocation(FVector(-42, 7, 1));
	//boxComp->SetRelativeRotation(FRotator(0, 90, 0));
	//boxComp->SetCollisionProfileName(TEXT("WeaponPreset"));

	//��Ż�ִϸ��̼�
	ConstructorHelpers::FObjectFinder<UAnimSequence>anim(TEXT("AnimSequence'/Game/VR/Animation/MainCharacter/fine_UE.fine_UE'"));
	if (anim.Succeeded())
	{
		Anim = anim.Object;
	}


}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	hp = initialHp;

}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void AMainCharacter::InputJump()
{
	Jump();
}

void AMainCharacter::InputRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 500;
}

void AMainCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 300;

}

void AMainCharacter::OnHitEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Ouch"));
	hp--;
	if (hp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Death"));

	}
}

void AMainCharacter::InputAttack()
{
	auto anim = Cast<UMainCharacterAnim>(GetMesh()->GetAnimInstance());
	if (anim) {
		bool isMontagePlaying = anim->IsAnyMontagePlaying();
		if (isMontagePlaying == false)
		{
			anim->PlayAttackAnim();

		}
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("attack"));
	
	//LineTrace�� ���� ��ġ
	FVector startPos = meshComp->GetComponentLocation();
	//LineTrace�� ���� ��ġ
	FVector endPos = meshComp-> GetComponentLocation ()+ meshComp->GetForwardVector() *100;
	//LineTrace�� �浹 ������ ���� ����
	FHitResult hitInfo;
	//�浹�ɼǼ��� ����
	FCollisionQueryParams params;
	//�ڱ��ڽ��� �浹���� ����
	params.AddIgnoredActor(this);
	//channel ���͸� �̿��� LineTrace�浹 ���� (�浹����, ������ġ, ������ġ, ����ä��, �浹�ɼ�)
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos,ECC_Visibility,params);
	//Linetrace�� �ε�������
	if (bHit)
	{
		//�浹ó�� 
		
			FTransform trans(hitInfo.ImpactPoint);
			}


	//�ε��� ��� ������ �Ǵ�
	auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
	if (enemy)
	{
		auto enemyFSM = Cast<UEnemyFSM>(enemy);
		enemyFSM->OnDamageProcess();
	}

}

//��Ż�ִϸ��̼�
void AMainCharacter::PlayAnim()
{
	auto anim = Cast<UMainCharacterAnim>(GetMesh()->GetAnimInstance());
	if (anim) {
		bool isMontagePlaying = anim->IsAnyMontagePlaying();
		if (isMontagePlaying == false)
		{
			anim->PlayFinishAnim();
		}
	}
}

void AMainCharacter::InputMagic()
{
	GetWorld()->SpawnActor<AMagic>(magicFactory,arrowComp->GetComponentLocation(), arrowComp->GetComponentRotation());

}
