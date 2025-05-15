#include "AnimMgr.h"

AnimMgr::AnimMgr()
{

}
AnimMgr::~AnimMgr()
{

}
void AnimMgr::allocateAnim(AnimName name_, Cfg::Textures  texID_, int numFrames_, float frameDelay_, bool loops_, bool loopWaits_, float loopWaitDelay_)
{
	animMap.emplace(name_, Animation{});

	animMap[name_].name = name_;
	animMap[name_].texID = texID_;
	animMap[name_].currState = AnimState::Playing;
	animMap[name_].frameDelay = frameDelay_;
	animMap[name_].loops = loops_;
	animMap[name_].loopWaits = loopWaits_;
	animMap[name_].loopWaitDelay = loopWaitDelay_;
	animMap[name_].numFrames = numFrames_;
	animMap[name_].playing = true;
	animMap[name_].previousState = AnimState::Playing;
	animMap[name_].transientState = AnimState::Playing;
}

const Animation* AnimMgr::getAnimationIfExists(AnimName name) const noexcept
{
	auto it = animMap.find(name);
	return (it == animMap.end()) ? nullptr : &it->second;
}

Animation* AnimMgr::getAnimationIfExists(AnimName name) noexcept
{
	auto it = animMap.find(name);
	return (it == animMap.end()) ? nullptr : &it->second;
}

void AnimMgr::setAnimation(AnimName name, Animation anim)
{
	animMap[name] = std::move(anim);
}


AnimDir AnimMgr::getCurrDir()
{
	return currDir;
}

void AnimMgr::setCurrDir(AnimDir dir_)
{
	currDir = dir_;
}

int AnimMgr::getNumAnims()
{
	return (int)animMap.size();
}




void AnimMgr::AddLeftFrames(AnimName name_, Cfg::Textures texID_, int numFrames_, int numRows_, int startCol_, int startRow_, int pitch_, int frameW_, int frameH_, float frameDelay_, AnimSheetType sheetType_, int pad_, int spacer_, bool loops_, bool loopWaits_, float loopDelay_)
{



	auto found = this->animMap.find(name_);
	if (found == animMap.end()) { allocateAnim(name_, texID_, numFrames_, frameDelay_, loops_, loopWaits_, loopDelay_); }

	

	if (animMap[name_].leftFrames.empty())
	{
		animMap[name_].leftFrames.clear();
	

		animMap[name_].leftFrames.reserve(numFrames_);
		animMap[name_].numFrames = numFrames_;


		for (int row = startRow_; row < startRow_ + 1; row++)
		{
			for (int col = startCol_; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].leftFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_*col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_}});
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].leftFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}
			}
		}
		for (int row = startRow_ + 1; row < numRows_; row++)
		{
			for (int col = 0; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].leftFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].leftFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}


			this->currDir = AnimDir::Left;

			this->currIndex = 0;

			this->currAnim = name_;
			switchAnim(name_, AnimDir::Left);
		

	}
}


void AnimMgr::AddRightFrames(AnimName name_, Cfg::Textures texID_, int numFrames_, int numRows_, int startCol_, int startRow_, int pitch_, int frameW_, int frameH_, float frameDelay_, AnimSheetType sheetType_, int pad_, int spacer_, bool loops_, bool loopWaits_, float loopDelay_)
{
	auto found = this->animMap.find(name_);
	if (found == animMap.end()) { allocateAnim(name_, texID_, numFrames_, frameDelay_, loops_, loopWaits_, loopDelay_); }

	if (animMap[name_].rightFrames.empty())
	{
		animMap[name_].rightFrames.clear();
		animMap[name_].rightFrames.reserve(numFrames_);
		animMap[name_].numFrames = numFrames_;

		for (int row = startRow_; row < startRow_ + 1; row++)
		{
			for (int col = startCol_; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].rightFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((col * spacer_) + (col * (2 * pad_)) + pad_), row * frameH_ + ((row * spacer_) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].rightFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}
			}
		}
		for (int row = startRow_ + 1; row < numRows_; row++)
		{
			for (int col = 0; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].rightFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].rightFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}

		this->currDir = AnimDir::Right;

		this->currIndex = 0;

		this->currAnim = name_;
		switchAnim(name_, AnimDir::Right);
	
	}
}

void AnimMgr::AddDownFrames(AnimName name_, Cfg::Textures texID_, int numFrames_, int numRows_, int startCol_, int startRow_, int pitch_, int frameW_, int frameH_, float frameDelay_, AnimSheetType sheetType_, int pad_, int spacer_, bool loops_, bool loopWaits_, float loopDelay_)
{

	auto found = this->animMap.find(name_);
	if (found == animMap.end()) { allocateAnim(name_, texID_, numFrames_, frameDelay_, loops_, loopWaits_, loopDelay_); }

	if (animMap[name_].downFrames.empty())
	{
		animMap[name_].downFrames.clear();
		animMap[name_].downFrames.reserve(numFrames_);
		animMap[name_].numFrames = numFrames_;

		for (int row = startRow_; row < startRow_ + 1; row++)
		{
			for (int col = startCol_; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].downFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)}, {frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].downFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}
		for (int row = startRow_ + 1; row < numRows_; row++)
		{
			for (int col = 0; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{

				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].downFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });

				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].downFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}

		this->currDir = AnimDir::Down;

		this->currIndex = 0;

		this->currAnim = name_;
		switchAnim(name_, AnimDir::Down);

	}
}

void AnimMgr::AddUpFrames(AnimName name_, Cfg::Textures texID_, int numFrames_, int numRows_, int startCol_, int startRow_, int pitch_, int frameW_, int frameH_, float frameDelay_, AnimSheetType sheetType_, int pad_, int spacer_, bool loops_, bool loopWaits_, float loopDelay_)
{

	auto found = this->animMap.find(name_);
	if (found == animMap.end()) { allocateAnim(name_, texID_, numFrames_, frameDelay_, loops_, loopWaits_, loopDelay_); }

	if (animMap[name_].upFrames.empty())
	{
		animMap[name_].upFrames.clear();
		animMap[name_].upFrames.reserve(numFrames_);
		animMap[name_].numFrames = numFrames_;

		for (int row = startRow_; row < startRow_ + 1; row++)
		{
			for (int col = startCol_; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].upFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)}, {frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].upFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}
		for (int row = startRow_ + 1; row < numRows_; row++)
		{
			for (int col = 0; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{

				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].upFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });

				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].upFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}

		this->currDir = AnimDir::Up;

		this->currIndex = 0;

		this->currAnim = name_;
		switchAnim(name_, AnimDir::Up);

	}
}

void AnimMgr::AddUpLeftFrames(AnimName name_, Cfg::Textures texID_, int numFrames_, int numRows_, int startCol_, int startRow_, int pitch_, int frameW_, int frameH_, float frameDelay_, AnimSheetType sheetType_, int pad_, int spacer_, bool loops_, bool loopWaits_, float loopDelay_)
{

	auto found = this->animMap.find(name_);
	if (found == animMap.end()) { allocateAnim(name_, texID_, numFrames_, frameDelay_, loops_, loopWaits_, loopDelay_); }

	if (animMap[name_].upLeftFrames.empty())
	{
		animMap[name_].upLeftFrames.clear();
		animMap[name_].upLeftFrames.reserve(numFrames_);
		animMap[name_].numFrames = numFrames_;

		for (int row = startRow_; row < startRow_ + 1; row++)
		{
			for (int col = startCol_; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].upLeftFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)}, {frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].upLeftFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}
		for (int row = startRow_ + 1; row < numRows_; row++)
		{
			for (int col = 0; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{

				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].upLeftFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });

				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].upLeftFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}

		this->currDir = AnimDir::UpLeft;

		this->currIndex = 0;

		this->currAnim = name_;
		switchAnim(name_, AnimDir::UpLeft);

	}
}

void AnimMgr::AddUpRightFrames(AnimName name_, Cfg::Textures texID_, int numFrames_, int numRows_, int startCol_, int startRow_, int pitch_, int frameW_, int frameH_, float frameDelay_, AnimSheetType sheetType_, int pad_, int spacer_, bool loops_, bool loopWaits_, float loopDelay_)
{

	auto found = this->animMap.find(name_);
	if (found == animMap.end()) { allocateAnim(name_, texID_, numFrames_, frameDelay_, loops_, loopWaits_, loopDelay_); }

	if (animMap[name_].upRightFrames.empty())
	{
		animMap[name_].upRightFrames.clear();
		animMap[name_].upRightFrames.reserve(numFrames_);
		animMap[name_].numFrames = numFrames_;

		for (int row = startRow_; row < startRow_ + 1; row++)
		{
			for (int col = startCol_; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].upRightFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)}, {frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].upRightFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}
		for (int row = startRow_ + 1; row < numRows_; row++)
		{
			for (int col = 0; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{

				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].upRightFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });

				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].upRightFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}

		this->currDir = AnimDir::UpRight;

		this->currIndex = 0;

		this->currAnim = name_;
		switchAnim(name_, AnimDir::UpRight);

	}
}

void AnimMgr::AddDownLeftFrames(AnimName name_, Cfg::Textures texID_, int numFrames_, int numRows_, int startCol_, int startRow_, int pitch_, int frameW_, int frameH_, float frameDelay_, AnimSheetType sheetType_, int pad_, int spacer_, bool loops_, bool loopWaits_, float loopDelay_)
{

	auto found = this->animMap.find(name_);
	if (found == animMap.end()) { allocateAnim(name_, texID_, numFrames_, frameDelay_, loops_, loopWaits_, loopDelay_); }

	if (animMap[name_].downLeftFrames.empty())
	{
		animMap[name_].downLeftFrames.clear();
		animMap[name_].downLeftFrames.reserve(numFrames_);
		animMap[name_].numFrames = numFrames_;

		for (int row = startRow_; row < startRow_ + 1; row++)
		{
			for (int col = startCol_; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].downLeftFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)}, {frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].downLeftFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}
		for (int row = startRow_ + 1; row < numRows_; row++)
		{
			for (int col = 0; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{

				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].downLeftFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });

				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].downLeftFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}

		this->currDir = AnimDir::DownLeft;

		this->currIndex = 0;

		this->currAnim = name_;
		switchAnim(name_, AnimDir::DownLeft);

	}
}

void AnimMgr::AddDownRightFrames(AnimName name_, Cfg::Textures texID_, int numFrames_, int numRows_, int startCol_, int startRow_, int pitch_, int frameW_, int frameH_, float frameDelay_, AnimSheetType sheetType_, int pad_, int spacer_, bool loops_, bool loopWaits_, float loopDelay_)
{

	auto found = this->animMap.find(name_);
	if (found == animMap.end()) { allocateAnim(name_, texID_, numFrames_, frameDelay_, loops_, loopWaits_, loopDelay_); }

	if (animMap[name_].downRightFrames.empty())
	{
		animMap[name_].downRightFrames.clear();
		animMap[name_].downRightFrames.reserve(numFrames_);
		animMap[name_].numFrames = numFrames_;

		for (int row = startRow_; row < startRow_ + 1; row++)
		{
			for (int col = startCol_; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{
				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].downRightFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)}, {frameW_,frameH_} });
				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].downRightFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}
		for (int row = startRow_ + 1; row < numRows_; row++)
		{
			for (int col = 0; col < pitch_ && (row * pitch_ + col) - (startRow_ * pitch_ + startCol_) < numFrames_; col++)
			{

				if (sheetType_ == AnimSheetType::Padded)
				{
					animMap[name_].downRightFrames.emplace_back(sf::IntRect{ {col * frameW_ + ((spacer_ * col) + (col * (2 * pad_)) + pad_), row * frameH_ + ((spacer_ * row) + (row * (2 * pad_)) + pad_)},{frameW_,frameH_} });

				}
				else if (sheetType_ == AnimSheetType::Normal)
				{
					animMap[name_].downRightFrames.emplace_back(sf::IntRect{ {col * frameW_, row * frameH_},{frameW_,frameH_} });
				}

			}
		}

		this->currDir = AnimDir::DownRight;

		this->currIndex = 0;

		this->currAnim = name_;
		switchAnim(name_, AnimDir::DownRight);

	}

}


Cfg::Textures AnimMgr::getTexID()
{
	return animMap[currAnim].texID;
}

sf::IntRect AnimMgr::currFrame()
{
	return animMap[currAnim].getFrame(currDir, currIndex);
}

sf::Vector2f AnimMgr::currOffset()
{

	return animMap[currAnim].getOffset(currDir, currIndex);
}

sf::Vector2f AnimMgr::currSize()
{
	return animMap[currAnim].getWorldSize(currDir, currIndex);
}

void AnimMgr::reset()
{
	currIndex = 0;
	frameElapsed = 0.f;
	loopWaitElapsed = 0.f;
}

void AnimMgr::animate(float dt_)
{
	if (animMap[currAnim].playing)
	{
		if (!animMap[currAnim].loopWaits)
		{
			frameElapsed += dt_;
			if (frameElapsed >= animMap[currAnim].frameDelay)
			{
				frameElapsed = 0.f;
				if (++currIndex >= animMap[currAnim].numFrames)
				{

					if (animMap[currAnim].loops)
						currIndex = 0;
					else
						currIndex = animMap[currAnim].numFrames - 1;
				}
			}
		}
		else
		{
			// does loop wait
			

			if (currIndex == animMap[currAnim].numFrames - 1)
			{
				// currently loopWaiting, increment the timer
				loopWaitElapsed += dt_;

				if (loopWaitElapsed >= animMap[currAnim].loopWaitDelay)
				{
					loopWaitElapsed = 0.f;
					frameElapsed = 0.f;
					if (animMap[currAnim].loops)
						currIndex = 0;
					else
						currIndex = animMap[currAnim].numFrames - 1;
				}
			}
			else
			{
				
				frameElapsed += dt_;
				if (frameElapsed >= animMap[currAnim].frameDelay)
				{
					++currIndex;
				
					frameElapsed = 0.f;

					if (currIndex >= animMap[currAnim].numFrames)
					{
						loopWaitElapsed = 0.f;
						currIndex = animMap[currAnim].numFrames - 1;
					}
				}
			}
		}
	}
}

int AnimMgr::getCurrIndex()
{
	return currIndex;
}

int AnimMgr::getCurrNumFrames()
{
	return animMap[currAnim].getNumFrames();
}

void AnimMgr::resizeOffsets(AnimName animName_, AnimDir animDir_, int numFrames_)
{
	if (animMap.find(animName_) != animMap.end())
	{
		switch (animDir_)
		{
		case AnimDir::Left:
			animMap[animName_].leftOffsets.resize(numFrames_);
			break;
		case AnimDir::Right:
			animMap[animName_].rightOffsets.resize(numFrames_);
			break;
		case AnimDir::Up:
			animMap[animName_].upOffsets.resize(numFrames_);
			break;
		case AnimDir::Down:
			animMap[animName_].downOffsets.resize(numFrames_);
			break;
		case AnimDir::UpLeft:
			animMap[animName_].upLeftOffsets.resize(numFrames_);
			break;
		case AnimDir::UpRight:
			animMap[animName_].upRightOffsets.resize(numFrames_);
			break;
		case AnimDir::DownLeft:
			animMap[animName_].downLeftOffsets.resize(numFrames_);
			break;
		case AnimDir::DownRight:
			animMap[animName_].downRightOffsets.resize(numFrames_);
			break;
		default:
			break;
		}
	}
}

void AnimMgr::resizeWorldSizes(AnimName animName_, AnimDir animDir_, int numFrames_)
{
	if (animMap.find(animName_) != animMap.end())
	{
		switch (animDir_)
		{
		case AnimDir::Left:
			animMap[animName_].leftSizes.resize(numFrames_);
			break;
		case AnimDir::Right:
			animMap[animName_].rightSizes.resize(numFrames_);
			break;
		case AnimDir::Down:
			animMap[animName_].downSizes.resize(numFrames_);
			break;
		case AnimDir::Up:
			animMap[animName_].upSizes.resize(numFrames_);
			break;
		case AnimDir::UpLeft:
			animMap[animName_].upLeftSizes.resize(numFrames_);
			break;
		case AnimDir::UpRight:
			animMap[animName_].upRightSizes.resize(numFrames_);
			break;
		case AnimDir::DownLeft:
			animMap[animName_].downLeftSizes.resize(numFrames_);
			break;
		case AnimDir::DownRight:
			animMap[animName_].downRightSizes.resize(numFrames_);
			break;
		default:
			break;
		}
	}
}

AnimName AnimMgr::getCurrAnimName()
{
	return currAnim;
}

void AnimMgr::resizeBulletPoints(AnimName aName, AnimDir aDir, int numFrames)
{
	if (animMap.find(aName) != animMap.end())
	{
		switch (aDir)
		{
		case AnimDir::Left:
			animMap[aName].leftBulletAnchors.resize(numFrames);
			break;
		case AnimDir::Right:
			animMap[aName].rightBulletAnchors.resize(numFrames);
			break;
		case AnimDir::Down:
			animMap[aName].downBulletAnchors.resize(numFrames);
			break;
		case AnimDir::Up:
			animMap[aName].upBulletAnchors.resize(numFrames);
			break;
			case AnimDir::UpLeft:
				animMap[aName].upLeftBulletAnchors.resize(numFrames);
				break;
			case AnimDir::UpRight:
				animMap[aName].upRightBulletAnchors.resize(numFrames);
				break;
			case AnimDir::DownLeft:
				animMap[aName].downLeftBulletAnchors.resize(numFrames);
				break;
			case AnimDir::DownRight:
			animMap[aName].downRightBulletAnchors.resize(numFrames);
			break;
		default:
			break;
		}
	}
}

void AnimMgr::setBulletAnchorPoint(AnimName aName, AnimDir aDir, int index, int numX, int numY)
{
	if (animMap.find(aName) != animMap.end())
	{
		switch (aDir)
		{
		case AnimDir::Left:
			if (index >= 0 && index < animMap[aName].leftBulletAnchors.size())
				animMap[aName].leftBulletAnchors[index] = { (float)numX,(float)numY };

			break;
		case AnimDir::Right:
			if (index >= 0 && index < animMap[aName].rightBulletAnchors.size())
				animMap[aName].rightBulletAnchors[index] = { (float)numX,(float)numY };

			break;
		case AnimDir::Down:
			if (index >= 0 && index < animMap[aName].downBulletAnchors.size())
				animMap[aName].downBulletAnchors[index] = { (float)numX,(float)numY };
			break;
		case AnimDir::Up:
			if (index >= 0 && index < animMap[aName].upBulletAnchors.size())
				animMap[aName].upBulletAnchors[index] = { (float)numX,(float)numY };
			break;
		case AnimDir::UpLeft:
			if (index >= 0 && index < animMap[aName].upLeftBulletAnchors.size())
				animMap[aName].upLeftBulletAnchors[index] = { (float)numX,(float)numY };
			break;
		case AnimDir::UpRight:
			if (index >= 0 && index < animMap[aName].upRightBulletAnchors.size())
				animMap[aName].upRightBulletAnchors[index] = { (float)numX,(float)numY };
			break;
		case AnimDir::DownLeft:
			if (index >= 0 && index < animMap[aName].downLeftBulletAnchors.size())
				animMap[aName].downLeftBulletAnchors[index] = { (float)numX,(float)numY };
			break;
		case AnimDir::DownRight:
			if (index >= 0 && index < animMap[aName].downRightBulletAnchors.size())
				animMap[aName].downRightBulletAnchors[index] = { (float)numX,(float)numY };
			break;
		default:
			break;
		}
	}
}

sf::Vector2f AnimMgr::getBulletPoint(AnimName aName, AnimDir aDir, int index)
{
	
	return animMap[aName].getBulletPoint(aDir, index);
}

void AnimMgr::setFrameWorldSize(AnimName animName_, AnimDir animDir_, int index_, int width_, int height_)
{
	if (animMap.find(animName_) != animMap.end())
	{
		switch (animDir_)
		{
		case AnimDir::Left:
			if (index_ >= 0 && index_ < animMap[animName_].leftSizes.size())
				animMap[animName_].leftSizes[index_] = { (float)width_,(float)height_ };

			break;
		case AnimDir::Right:
			if (index_ >= 0 && index_ < animMap[animName_].rightSizes.size())
				animMap[animName_].rightSizes[index_] = { (float)width_,(float)height_ };

			break;
		case AnimDir::Down:
			if (index_ >= 0 && index_ < animMap[animName_].downSizes.size())
				animMap[animName_].downSizes[index_] = { (float)width_,(float)height_ };
			break;
		case AnimDir::Up:
			if (index_ >= 0 && index_ < animMap[animName_].upSizes.size())
				animMap[animName_].upSizes[index_] = { (float)width_,(float)height_ };
			break;
		case AnimDir::UpLeft:
			if (index_ >= 0 && index_ < animMap[animName_].upLeftSizes.size())
				animMap[animName_].upLeftSizes[index_] = { (float)width_,(float)height_ };

			break;
		case AnimDir::UpRight:
			if (index_ >= 0 && index_ < animMap[animName_].upRightSizes.size())
				animMap[animName_].upRightSizes[index_] = { (float)width_,(float)height_ };

			break;
		case AnimDir::DownLeft:
			if (index_ >= 0 && index_ < animMap[animName_].downLeftSizes.size())
				animMap[animName_].downLeftSizes[index_] = { (float)width_,(float)height_ };
			break;
		case AnimDir::DownRight:
			if (index_ >= 0 && index_ < animMap[animName_].downRightSizes.size())
				animMap[animName_].downRightSizes[index_] = { (float)width_,(float)height_ };
			break;
		default:
			break;
		}
	}
}

void AnimMgr::setOffset(AnimName animName_, AnimDir animDir_, int index_, int x_, int y_)
{
	if (animMap.find(animName_) != animMap.end())
	{
		switch (animDir_)
		{
		case AnimDir::Left:
			if (index_ >= 0 && index_ < animMap[animName_].leftOffsets.size())
				animMap[animName_].leftOffsets[index_] = { (float)x_,(float)y_ };

			break;
		case AnimDir::Right:
			if (index_ >= 0 && index_ < animMap[animName_].rightOffsets.size())
				animMap[animName_].rightOffsets[index_] = { (float)x_,(float)y_ };

			break;
		case AnimDir::Down:
			if (index_ >= 0 && index_ < animMap[animName_].downOffsets.size())
				animMap[animName_].downOffsets[index_] = { (float)x_,(float)y_ };
			break;
			break;
		case AnimDir::Up:
			if (index_ >= 0 && index_ < animMap[animName_].upOffsets.size())
				animMap[animName_].upOffsets[index_] = { (float)x_,(float)y_ };
			break;
		case AnimDir::UpLeft:
			if (index_ >= 0 && index_ < animMap[animName_].upLeftOffsets.size())
				animMap[animName_].upLeftOffsets[index_] = { (float)x_,(float)y_ };

			break;
		case AnimDir::UpRight:
			if (index_ >= 0 && index_ < animMap[animName_].upRightOffsets.size())
				animMap[animName_].upRightOffsets[index_] = { (float)x_,(float)y_ };

			break;
		case AnimDir::DownLeft:
			if (index_ >= 0 && index_ < animMap[animName_].downLeftOffsets.size())
				animMap[animName_].downLeftOffsets[index_] = { (float)x_,(float)y_ };
			break;
			break;
		case AnimDir::DownRight:
			if (index_ >= 0 && index_ < animMap[animName_].downRightOffsets.size())
				animMap[animName_].downRightOffsets[index_] = { (float)x_,(float)y_ };
			break;
		default:
			break;
		}
	}
}

// call after loading frames or the size will be the frame size
void AnimMgr::setSize(AnimName animName_, AnimDir animDir_, int currIndex_, int sizeX, int sizeY)
{
	switch (animDir_)
	{
	case AnimDir::Left:
		animMap[animName_].leftSizes[currIndex_] = {(float)sizeX,(float)sizeY};

		break;
	case AnimDir::Right:
		animMap[animName_].rightSizes[currIndex_] = { (float)sizeX,(float)sizeY };
		break;
	case AnimDir::Down:
		animMap[animName_].downSizes[currIndex_] = { (float)sizeX,(float)sizeY };
		break;
	case AnimDir::Up:
		animMap[animName_].upSizes[currIndex_] = { (float)sizeX,(float)sizeY };
		break;
	case AnimDir::UpLeft:
		animMap[animName_].upLeftSizes[currIndex_] = { (float)sizeX,(float)sizeY };
		break;
	case AnimDir::UpRight:
		animMap[animName_].upRightSizes[currIndex_] = { (float)sizeX,(float)sizeY };
		break;
	case AnimDir::DownLeft:
		animMap[animName_].downLeftSizes[currIndex_] = { (float)sizeX,(float)sizeY };
		break;
	case AnimDir::DownRight:
		animMap[animName_].downRightSizes[currIndex_] = { (float)sizeX,(float)sizeY };
		break;
	}
	
}

void AnimMgr::switchAnim(AnimName name_, AnimDir dir_)
{
	this->currAnim = name_;
	this->currDir = dir_;
	this->currIndex = 0;
}

sf::IntRect AnimMgr::getTexRect()
{
	return animMap[currAnim].getFrame(currDir, getCurrIndex());
}

void AnimMgr::pauseAnim()
{
	animMap[currAnim].playing = false;
}

void AnimMgr::resumeAnim()
{
	animMap[currAnim].playing = true;
}
