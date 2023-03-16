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

    ConstructorHelpers::FObjectFinder<UMaterialInterface> wolfMat(TEXT("/Script/Engine.Material'/Game/VR/Material/M_InteractiveWolf.M_InteractiveWolf'"));
    if (wolfMat.Succeeded())
    {
        originalWolfMaterial = wolfMat.Object;
        GetMesh()->SetMaterial(0, wolfMat.Object);
    }

    //// 1. �� ���̷�Ż�޽� ������Ʈ ���
    //gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
    //// 1-1. �θ� ������Ʈ�� Mesh ������Ʈ�� ����
    //gunMeshComp->SetupAttachment(GetMesh());
    //// 1-2. ���̷�Ż�޽� ������ �ε�
    //ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
    //// 2. �о������ �����ߴٸ�
    //if (TempGunMesh.Succeeded())
    //{
    //    // 3. Mesh�� �����ϰ�ʹ�. 
    //    GetMesh()->SetSkeletalMesh(TempGunMesh.Object);
    //    // 4. Transform �� �����ϰ�ʹ�.
    //    GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
    //}

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    dynamicWolfMaterial = UMaterialInstanceDynamic::Create(originalWolfMaterial, this);
    GetMesh()->SetMaterial(0, dynamicWolfMaterial);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (isInteract && !isHighlight)
    {
        BeforeInteract();
    }
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::RootFire()
{
    FRotator rot = GetActorRotation();

    if (isInteract)
    {
        rot.Pitch = 0.;
        AfterInteract();
    }

    GetWorld()->SpawnActor<ABullet>(bulletFactory, GetActorLocation(), rot);
}

void AEnemy::SetInteract(bool value)
{
    isInteract = value;
}

void AEnemy::BeforeInteract()
{
    isHighlight = true;
    SetHighlight();
}

void AEnemy::Interact(FVector start, FVector end)
{
    FRotator rot = (end - start).Rotation();
    SetActorRotation(rot);
}

void AEnemy::AfterInteract()
{
    isHighlight = false;
    SetHighlight();
}

void AEnemy::SetHighlight()
{
    if (highlightTime >= highlightCoolTime)
    {
        highlightTime = 0.f;
        GetWorld()->GetTimerManager().ClearTimer(highlightTimer);

        return;
    }

    highlightTime += GetWorld()->GetDeltaSeconds();

    float brightness;

    if (isHighlight)
    {
        brightness = FMath::Lerp<float>(defaultBrightness, modifiedBrightness, highlightTime / highlightCoolTime);
    }
    else
    {
        brightness = FMath::Lerp<float>(modifiedBrightness, defaultBrightness, highlightTime / highlightCoolTime);
    }

    dynamicWolfMaterial->SetVectorParameterValue(TEXT("Brightness"), FVector4(brightness, brightness, brightness, brightness));
    GetWorld()->GetTimerManager().SetTimer(highlightTimer, this, &AEnemy::SetHighlight, 0.02);
}