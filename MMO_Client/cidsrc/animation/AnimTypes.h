#ifndef ANIMTYPES_H__
#define ANIMTYPES_H__

enum class AnimName
{
	Idle,
	Fly,
	IdleToRun,
	Running,
	RunToIdle,
	ShootSetup,
	Shooting,
	LiftOff,
	LiftOffAndShooting,
	Rising,
	RisingAndShooting,
	Falling,
	FallingAndShooting,
	JumpPeakRising,
	JumpPeakRisingAndShooting,
	JumpPeakFalling,
	JumpPeakFallingAndShooting,
	Landing,
	LandingAndShooting,
	RunningAndShooting,
	Invariant
};

enum class AnimState
{
	Playing,
	Transient,
	LoopWaiting,
	Stopped,
	Invariant
};

enum class AnimDir
{
	Up,
	Down,
	Left,
	Right,
	UpRight,
	UpLeft,
	DownLeft,
	DownRight,
	Invariant
};

enum class AnimSheetType
{
	Normal,
	Padded,
	Blocks,
	Vertical
};

#endif