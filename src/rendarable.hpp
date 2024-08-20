#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "olcPixelGameEngine.h"

struct RenderData
{
    olc::vi2d viFramePos;
    olc::vi2d viFrameSize;
    olc::vf2d vfRenderSize;
};

class Renderable
{
public:
    Renderable(const std::string& sFilename, olc::vi2d viFramePos, olc::vi2d viFrameSize, olc::vf2d vfRenderSize, int nLayer = 0)
        : m_pSpritesheet(std::make_unique<olc::Sprite>(sFilename)),
          m_pDecal(std::make_unique<olc::Decal>(m_pSpritesheet.get())),
          m_RenderData({ viFramePos, viFrameSize, vfRenderSize }), m_nLayer(nLayer)
    {

    }

    virtual void Render(const olc::vf2d& pos, olc::PixelGameEngine& pge) const {
        pge.DrawPartialDecal(
            pos, m_RenderData.vfRenderSize, m_pDecal.get(),
            m_RenderData.viFramePos,
            m_RenderData.viFrameSize
        );
    }

    void SetLayer(int nLayer) { m_nLayer = nLayer; }
    int GetLayer() const { return m_nLayer; }

protected:
    std::unique_ptr<olc::Sprite> m_pSpritesheet;
    std::unique_ptr<olc::Decal> m_pDecal;

    RenderData m_RenderData;

    int m_nLayer;
};

#endif // RENDERABLE_HPP
