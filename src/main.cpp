#define OLC_PGE_APPLICATION

#include "player.hpp"

class Application : public olc::PixelGameEngine
{
public:
	Application()
	{
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
        m_pBackgroundSprite = std::make_unique<olc::Sprite>("../res/background.png");
        m_pBackgroundDecal = std::make_unique<olc::Decal>(m_pBackgroundSprite.get());

        m_pPlayer = new Player();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        // Moving background
        m_fBackgroundPositionY += BACKGROUND_SPEED * fElapsedTime;
        if (m_fBackgroundPositionY >= 240.0f)
            m_fBackgroundPositionY = 0;
        
        m_pPlayer->HandleInput(*this);
        m_pPlayer->Update(fElapsedTime);
        
        // Rendering
        Clear(olc::BLACK);

        DrawDecal(olc::vf2d(0, (int)m_fBackgroundPositionY), m_pBackgroundDecal.get(), olc::vf2d(2, 2));
        DrawDecal(olc::vf2d(0, (int)m_fBackgroundPositionY - SCREEN_HEIGHT), m_pBackgroundDecal.get(), olc::vf2d(2, 2));

        m_pPlayer->Render(*this);

		return true;
	}

private:
    std::unique_ptr<olc::Sprite> m_pBackgroundSprite;
    std::unique_ptr<olc::Decal> m_pBackgroundDecal;
    float m_fBackgroundPositionY = 0;

    Player* m_pPlayer;

};


int main()
{
	Application demo;
	if (demo.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, 4, 4))
		demo.Start();

	return 0;
}
