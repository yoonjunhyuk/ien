#include "MyCharacter.h"
#include"MyAnimInstance.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> MyAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_MyCharacter.ABP_MyCharacter_C'"));
	if (MyAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MyAnim.Class);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	IsAttacking = false;
	MaxCombo = 3;
	AttackEndCombo();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartCombo();
			MyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::InputJump);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMyCharacter::InputAttack);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::InputTurn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::InputLookUp);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AMyCharacter::InputVertical);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMyCharacter::InputHorizontal);
}

void AMyCharacter::InputTurn(float value)
{
	AddControllerYawInput(value);
}

void AMyCharacter::InputLookUp(float value)
{
	AddControllerPitchInput(value);
}

void AMyCharacter::InputVertical(float value)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), value);
}

void AMyCharacter::InputHorizontal(float value)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), value);
}

void AMyCharacter::InputJump()
{
	Jump();
}

void AMyCharacter::InputAttack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartCombo();
		MyAnim->PlayAttackMontage();
		MyAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndCombo();
}

void AMyCharacter::AttackStartCombo()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AMyCharacter::AttackEndCombo()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}