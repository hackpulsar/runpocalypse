#ifndef ANIMATABLE_HPP
#define ANIMATABLE_HPP

#include "olcPixelGameEngine.h"

#include "settings.hpp"
#include <stack>
#include <unordered_map>

struct AnimationData
{
    olc::vi2d m_FirstFrame;
    olc::vi2d m_LastFrame;
    int m_nFramesCount;

    olc::vf2d m_RenderSize;
    olc::vi2d m_ActualSize;

    bool m_bCycle;
};

class Animatable
{
public:
    void Update(float fElapsedTime) {
        if (m_AnimationsStack.empty()) return;

        m_fAnimationDelay += ANIMATION_DELAY * fElapsedTime;
        if (m_fAnimationDelay >= ANIMATION_DELAY_CAP) {
            m_fAnimationDelay = 0.0f;

            m_nFrame++;
            if (m_nFrame >= m_AnimationsStack.top()->m_nFramesCount) {
                if (m_AnimationsStack.top()->m_bCycle)
                    m_nFrame = 0;
                else
                    m_AnimationsStack.pop();
            }
        }
    }

    void Render(const olc::vi2d& pos, olc::PixelGameEngine& pge) const {
        if (m_AnimationsStack.empty() == false) {
            auto pCurrentAnimation = m_AnimationsStack.top();

            pge.DrawPartialDecal(
                pos, pCurrentAnimation->m_RenderSize, m_pDecal.get(), 
                olc::vi2d(pCurrentAnimation->m_FirstFrame.x + int(m_nFrame), pCurrentAnimation->m_FirstFrame.y) * pCurrentAnimation->m_ActualSize.x, 
                pCurrentAnimation->m_ActualSize
            );
        }
    }

    void LoadAnimation(const AnimationData data, const std::string& sName) {
        m_mAnimations[sName] = data;
    }

    void AddAnimation(const std::string& sName) {
        if (m_mAnimations.find(sName) != m_mAnimations.end())
            m_AnimationsStack.push(&m_mAnimations.at(sName));
    }

protected:
    std::unique_ptr<olc::Sprite> m_pSpritesheet;
    std::unique_ptr<olc::Decal> m_pDecal;

    std::unordered_map<std::string, AnimationData> m_mAnimations;
    std::stack<AnimationData*> m_AnimationsStack;
    float m_fAnimationDelay = 0.0f;
    int m_nFrame = 1;
    
};

#endif // ANIMATABLE_HPP
