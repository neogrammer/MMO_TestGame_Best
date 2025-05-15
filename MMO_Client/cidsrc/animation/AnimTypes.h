#ifndef ANIMTYPES_H__
#define ANIMTYPES_H__

enum class AnimName
{
	Idle,
	Running,
	Shooting,
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
//
//enum class AnimDir
//{
//	Up,
//	Down,
//	Left,
//	Right,
//	UpRight,
//	UpLeft,
//	DownLeft,
//	DownRight,
//	Invariant
//};

enum class AnimSheetType
{
	Normal,
	Padded,
	Blocks,
	Vertical
};

#endif