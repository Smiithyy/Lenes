#include "MyFirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Constructor
//Comment to test
AMyFirstPersonCharacter::AMyFirstPersonCharacter()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create a CameraComponent
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    
    // Ensure the attachment is to a USceneComponent, which UCapsuleComponent is derived from
    if (USceneComponent* RootComp = GetRootComponent())
    {
        FirstPersonCameraComponent->SetupAttachment(RootComp);
    }
    else
    {
        // If there's no valid root component, handle the error accordingly
        UE_LOG(LogTemp, Warning, TEXT("No valid root component found for attaching the camera."));
    }

    // Position the camera and set the FOV
    FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight)); 
    FirstPersonCameraComponent->bUsePawnControlRotation = true;
    FirstPersonCameraComponent->FieldOfView = 90.0f;

    // Set the default movement speeds
    SprintMultiplier = 1.5f; 
    BaseWalkSpeed = 600.0f;  

    // Set the turn and look up rates
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Set the character's default movement speed
    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

// Called when the game starts or when spawned
void AMyFirstPersonCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMyFirstPersonCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind movement events
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyFirstPersonCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyFirstPersonCharacter::MoveRight);

    // Bind camera rotation events
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AMyFirstPersonCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AMyFirstPersonCharacter::LookUpAtRate);

    // Bind jump actions
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyFirstPersonCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyFirstPersonCharacter::StopJump);

    // Bind crouch actions
  PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyFirstPersonCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyFirstPersonCharacter::StopCrouch);

    // Bind sprint actions
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyFirstPersonCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyFirstPersonCharacter::StopSprint);
}

// Move the character forward/backward
void AMyFirstPersonCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

// Move the character right/left
void AMyFirstPersonCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

// Turn the character at a certain rate
void AMyFirstPersonCharacter::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// Look up/down at a certain rate
void AMyFirstPersonCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Start the character jumping
void AMyFirstPersonCharacter::StartJump()
{
    bPressedJump = true;
}

// Stop the character jumping
void AMyFirstPersonCharacter::StopJump()
{
    bPressedJump = false;
}

// Start the character crouching
void AMyFirstPersonCharacter::StartCrouch()
{
    UE_LOG(LogTemp, Warning, TEXT("StartCrouch called"));
    Crouch();
}

void AMyFirstPersonCharacter::StopCrouch()
{
    UE_LOG(LogTemp, Warning, TEXT("StopCrouch called"));
    UnCrouch();
}

// Start sprinting
void AMyFirstPersonCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * SprintMultiplier;
}

// Stop sprinting
void AMyFirstPersonCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}