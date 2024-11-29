#include "MyFirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"

// Constructor
AMyFirstPersonCharacter::AMyFirstPersonCharacter()
{
    // Enable Tick
    PrimaryActorTick.bCanEverTick = true;

    // Create the camera component
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(RootComponent);
    FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Initialize movement variables
    SprintMultiplier = 1.5f;

    // Initialize head bob variables
    BobFrequency = 8.0f; // Increased for quicker oscillations
    BobAmplitude = 5.0f; // Increased for more pronounced motion
    BobRunMultiplier = 2.5f; // More pronounced head bobbing while sprinting
    BobTimer = 0.0f;
}

// Called at the start of the game
void AMyFirstPersonCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Store the default camera position
    DefaultCameraPosition = FirstPersonCameraComponent->GetRelativeLocation();
}

// Handle forward/backward movement
void AMyFirstPersonCharacter::MoveForward(float Value)
{
    AddMovementInput(GetActorForwardVector(), Value);
}

// Handle right/left movement
void AMyFirstPersonCharacter::MoveRight(float Value)
{
    AddMovementInput(GetActorRightVector(), Value);
}

// Start jumping
void AMyFirstPersonCharacter::StartJump()
{
    bPressedJump = true;
}

// Stop jumping
void AMyFirstPersonCharacter::StopJump()
{
    bPressedJump = false;
}

// Start sprinting
void AMyFirstPersonCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed *= SprintMultiplier;
}

// Stop sprinting
void AMyFirstPersonCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed /= SprintMultiplier;
}

// Start crouching
void AMyFirstPersonCharacter::StartCrouch()
{
    Crouch();
}

// Stop crouching
void AMyFirstPersonCharacter::StopCrouch()
{
    UnCrouch();
}

// Apply head bob effect
void AMyFirstPersonCharacter::ApplyHeadBob(float DeltaTime)
{
    // Check if the character is moving
    FVector Velocity = GetVelocity();
    float Speed = Velocity.Size();

    if (Speed > 0.0f) // Only apply head bob when moving
    {
        // Adjust bob frequency and amplitude for sprinting
        float CurrentFrequency = (Speed > 600.0f) ? BobFrequency * BobRunMultiplier : BobFrequency;
        float CurrentAmplitude = (Speed > 600.0f) ? BobAmplitude * BobRunMultiplier : BobAmplitude;

        // Update the bob timer
        BobTimer += DeltaTime * CurrentFrequency;

        // Calculate the bob offset
        FVector BobOffset = FVector(0.0f, 0.0f, FMath::Sin(BobTimer) * CurrentAmplitude);

        // Only modify the camera's position, not its rotation
        FirstPersonCameraComponent->SetRelativeLocation(DefaultCameraPosition + BobOffset);
    }
    else
    {
        // Reset the camera position when not moving
        BobTimer = 0.0f;
        FirstPersonCameraComponent->SetRelativeLocation(DefaultCameraPosition);
    }
}

// Called every frame
void AMyFirstPersonCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Apply head bobbing
    ApplyHeadBob(DeltaTime);
}

// Bind input actions and axes
void AMyFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind movement axes
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyFirstPersonCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyFirstPersonCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

    // Bind actions
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyFirstPersonCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyFirstPersonCharacter::StopJump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyFirstPersonCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyFirstPersonCharacter::StopSprint);
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyFirstPersonCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyFirstPersonCharacter::StopCrouch);
}
