#define OLC_PGE_APPLICATION

#include "entities/player.hpp"
#include "entities/entities_manager.hpp"
#include "collision.hpp"

class Application : public olc::PixelGameEngine
{
public:
	Application() {
		sAppName = "RUNPOCALYPSE by @hackpulsar";
	}

    ~Application() {
        delete m_pPlayer;
        delete m_pEntitiesManager;
    }

public:
	bool OnUserCreate() override
	{
        m_pBackgroundSprite = std::make_unique<olc::Sprite>("./bin/res/background.png");
        m_pBackgroundDecal = std::make_unique<olc::Decal>(m_pBackgroundSprite.get());

        m_pPlayer = new Player();
        m_pEntitiesManager = new EntitiesManager();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        if (m_bRunning) {
             // Moving background
            m_fBackgroundPositionY += BACKGROUND_SPEED * fElapsedTime;
            if (m_fBackgroundPositionY >= 240.0f)
                m_fBackgroundPositionY = 0;

            // Collision detection between player and entities
            for (const auto& pEntity : m_pEntitiesManager->GetEntities()) {
                Collidable* c = dynamic_cast<Collidable*>(pEntity);
                if (c != nullptr) {
                    if (Collision::AABB(*m_pPlayer, *c)) {
                        pEntity->SelfDestruct();
                        MakeBoom(dynamic_cast<AcidSpit*>(pEntity) != nullptr ? BoomType::Acid : BoomType::Basic);
                        m_bRunning = false;
                    }
                }
            }

            m_pPlayer->HandleInput(*this);
            m_pPlayer->Update(fElapsedTime);
        }
        else {
            if (GetKey(olc::Key::SPACE).bPressed)
                this->Restart();
        }

        m_pEntitiesManager->Update(fElapsedTime, m_nDifficultyLevel);
        
        // Rendering
        this->Render();
        
		return true;
	}

private:
    void Render() {
        Clear(olc::BLACK);

        DrawDecal(olc::vf2d(0, (int)m_fBackgroundPositionY), m_pBackgroundDecal.get(), olc::vf2d(2, 2));
        DrawDecal(olc::vf2d(0, (int)m_fBackgroundPositionY - SCREEN_HEIGHT), m_pBackgroundDecal.get(), olc::vf2d(2, 2));
        
        if (m_bRunning) m_pPlayer->Render(*this);
        m_pEntitiesManager->Render(*this);
        
        if (!m_bRunning) {
            // "GAME OVER" text
            olc::vf2d vTextPosition = { float(SCREEN_WIDTH / 2 - 70), float(SCREEN_HEIGHT / 2 - 10) };
           
            // Shadow
            DrawStringDecal(vTextPosition + olc::vf2d(2, 2), "GAME OVER", olc::BLACK, { 2, 2 });
            // Actual text
            DrawStringDecal(vTextPosition, "GAME OVER", olc::WHITE, { 2, 2 });

            // "Press SPACE to restart" text
            vTextPosition = { float(SCREEN_WIDTH / 2 - 85), float(SCREEN_HEIGHT / 2 + 20) };
            // Shadow
            DrawStringDecal(vTextPosition + olc::vf2d(1, 1), "Press SPACE to restart", olc::BLACK, { 1, 1 });
            // Actual text
            DrawStringDecal(vTextPosition, "Press SPACE to restart", olc::WHITE, { 1, 1 });
        }
    }

    void MakeBoom(BoomType type) {
        m_pEntitiesManager->MakeBoom(
            { 
                m_pPlayer->GetPosition().x + m_pPlayer->GetHitbox().vSize.x / 2 - 32.f, 
                m_pPlayer->GetPosition().y + m_pPlayer->GetHitbox().vSize.y / 2 - 32.f
            },
            type
        );
    }

    void Restart() {
        m_bRunning = true;
        m_pEntitiesManager->Reset();
        m_pPlayer->Reset();
    }

private:
    std::unique_ptr<olc::Sprite> m_pBackgroundSprite;
    std::unique_ptr<olc::Decal> m_pBackgroundDecal;
    float m_fBackgroundPositionY = 0;

    Player* m_pPlayer;
    EntitiesManager* m_pEntitiesManager;

    int m_nDifficultyLevel = 1;

    bool m_bRunning = true;

};


int main()
{
	Application app;
	if (app.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, 4, 4))
		app.Start();

	return 0;
}
