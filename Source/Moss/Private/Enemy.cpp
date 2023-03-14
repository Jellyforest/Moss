// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "Bullet.h"
#include "components/capsulecomponent.h"



// Sets default values
AEnemy::AEnemy()
{
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyPreset"));

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    // 1.���̷�Ż�޽� ������ �ε�
    ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimalVarietyPack/Wolf/Meshes/SK_Wolf.SK_Wolf'"));
    //1.������ �ε� �����ϸ�
    if (tempMesh.Succeeded())
    {
        //1-2. �������Ҵ�
        GetMesh()->SetSkeletalMesh(tempMesh.Object);
        //1-3. �޽� ��ġ �� ȸ�� ����
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
        // 1-4. �޽� ũ�� ����
        GetMesh()->SetRelativeScale3D(FVector(0.84f));
    }
    // EnemyFSM ������Ʈ �߰�
    fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

    // �ִϸ��̼� �������Ʈ �Ҵ��ϱ�
    ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/VR/Blueprints/ABP_EnemyWolf.ABP_EnemyWolf_C'"));
    if (tempClass.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(tempClass.Class);
    }

    // 1. �� ���̷�Ż�޽� ������Ʈ ���
    gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
    // 1-1. �θ� ������Ʈ�� Mesh ������Ʈ�� ����
    gunMeshComp->SetupAttachment(GetMesh());
    // 1-2. ���̷�Ż�޽� ������ �ε�
    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
    // 2. �о������ �����ߴٸ�
    if (TempGunMesh.Succeeded())
    {
        // 3. Mesh�� �����ϰ�ʹ�. 
        GetMesh()->SetSkeletalMesh(TempGunMesh.Object);
        // 4. Transform �� �����ϰ�ʹ�.
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
    }

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::RootFire()
{
    // �Ѿ� �߻� ó��
    FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
    GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition.GetLocation(), GetActorRotation());

}
