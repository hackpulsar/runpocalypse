#ifndef ANIMATABLE_HPP
#define ANIMATABLE_HPP

#include "rendarable.hpp"
#include "settings.hpp"

#include <stack>
#include <unordered_map>

struct AnimationData
{
    olc::vi2d vFirstFrame;
    olc::vi2d vLastFrame;
    int nFramesCount;

    olc::vf2d vRenderSize;
    olc::vi2d vActualSize;

    bool bCycle;
};

class Animatable : public Renderable
{
public:
    Animatable(const std::string& sFilename, RenderData renderData) 
        : Renderable(sFilename, renderData.viFramePos, renderData.viFrameSize, renderData.vfRenderSize)
    { }

    void Update(float fElapsedTime) {
        if (m_AnimationsStack.empty()) return;

        m_fAnimationDelay += ANIMATION_DELAY * fElapsedTime;
        if (m_fAnimationDelay >= ANIMATION_DELAY_CAP) {
            m_fAnimationDelay = 0.0f;

            m_nFrame++;
            if (m_nFrame >= m_AnimationsStack.top()->nFramesCount) {
                if (m_AnimationsStack.top()->bCycle == false)
                    m_AnimationsStack.pop();
                m_nFrame = 0;
            }
        }
    }

    void Render(const olc::vf2d& pos, olc::PixelGameEngine& pge) const override {
        if (m_AnimationsStack.empty() == false) {
            auto pCurrentAnimation = m_AnimationsStack.top();

            pge.DrawPartialDecal(
                pos, pCurrentAnimation->vRenderSize, m_pDecal.get(), 
                olc::vi2d(
                    pCurrentAnimation->vFirstFrame.x + int(m_nFrame), 
                    pCurrentAnimation->vFirstFrame.y
                ) * pCurrentAnimation->vActualSize, 
                pCurrentAnimation->vActualSize
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

    const AnimationData* GetCurrentAnimation() const {
        if (m_AnimationsStack.empty() == false)
            return m_AnimationsStack.top();
        return nullptr;}

protected:
    std::unordered_map<std::string, AnimationData> m_mAnimations;
    std::stack<AnimationData*> m_AnimationsStack;
    float m_fAnimationDelay = 0.0f;
    int m_nFrame = 0;
    
};

#endif // ANIMATABLE_HP
