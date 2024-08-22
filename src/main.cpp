#define OLC_PGE_APPLICATION

#include "entities/player.hpp"
#include "entities/entities_manager.hpp"
#include "menu.hpp"
#include "collision.hpp"

#include <fstream>

#include "log.hpp"

class Application : public olc::PixelGameEngine
{
public:
	Application() {
		sAppName = "RUNPOCALYPSE by @hackpulsar";
	}

    ~Application() = default;

public:
	bool OnUserCreate() override
	{
        m_pBackgroundSprite = std::make_unique<olc::Sprite>("./bin/res/background.png");
        m_pBackgroundDecal = std::make_unique<olc::Decal>(m_pBackgroundSprite.get());

        m_pFlashSprite = std::make_unique<olc::Sprite>("./bin/res/flash.png");
        m_pFlashDecal = std::make_unique<olc::Decal>(m_pFlashSprite.get());

        m_pPlayer = std::make_shared<Player>(&m_bRunning);
        m_pEntitiesManager = std::make_unique<EntitiesManager>(m_pPlayer, &m_bRunning);

        m_pMenu = std::make_unique<Menu>();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        if (!m_pMenu->IsReadyToStart()) {
            m_pMenu->Update(this);
            m_pMenu->Render(this);
            return true;
        }

        if (m_bRunning) {
             // Moving background
            m_fBackgroundPositionY += BACKGROUND_SPEED * fElapsedTime;
            if (m_fBackgroundPositionY >= float(SCREEN_HEIGHT))
                m_fBackgroundPositionY = 0;

            // Collision detection between player and entities
            for (const auto& pEntity : m_pEntitiesManager->GetEntities()) {
                Collidable* c = dynamic_cast<Collidable*>(pEntity.get());
                if (c != nullptr) {
                    if (Collision::AABB(*m_pPlayer, *c)) {
                        pEntity->SelfDestruct();

                        BoomType type = BoomType::Dust;
                        if (dynamic_cast<AcidSpit*>(pEntity.get()) != nullptr) type = BoomType::Acid;
                        else if (dynamic_cast<AcidPuddle*>(pEntity.get()) != nullptr) type = BoomType::Acid;
                        else if (dynamic_cast<AcidBarrel*>(pEntity.get()) != nullptr) type = BoomType::Acid;
                        else if (dynamic_cast<Barrel*>(pEntity.get()) != nullptr) type = BoomType::Acid;
                        else if (dynamic_cast<Rocket*>(pEntity.get()) != nullptr) type = BoomType::Basic;

                        m_bRunning = false;
                        MakeBoom(type);
                        StopAllObstacles();

                        LOG("Player collided with an entity");

                        std::ifstream file("highscore.txt", std::ios::in);
                        if (file.is_open()) {
                            int nHighScore;
                            file >> nHighScore;

                            if (m_fScore > nHighScore) {
                                file.close();
                                std::ofstream outFile("highscore.txt", std::ios::out);
                                if (outFile.is_open()) {
                                    outFile << (int)m_fScore;
                                    outFile.close();
                                }
                            }
                            else {
                                file.close();
                            }
                            file.close();
                        }

                        break;
                    }
                }
            }

            m_fScore += fElapsedTime * PLAYER_SPEED;

            if ((int)m_fScore % 50 == 0 && m_nDifficultyLevel < DIFFICULTY_CAP) {
                LOG("Difficulty level increased, score: " + std::to_string((int)m_fScore));
                m_nDifficultyLevel++;
                m_fScore += 1.0f;
            }

            m_pPlayer->HandleInput(*this);
        }
        else {
            if (!m_bFlashPhaseOneEnded) {
                m_fFlashTransperency += 2000.0f * fElapsedTime;
                if (m_fFlashTransperency >= 255) {
                    m_fFlashTransperency = 255;
                    m_bFlashPhaseOneEnded = true;
                }
            }
            else if (!m_bFlashPhaseTwoEnded) {
                m_fFlashTransperency -= (150.0f * fElapsedTime);
                if (m_fFlashTransperency <= 0) {
                    m_fFlashTransperency = 0;
                    m_bFlashPhaseTwoEnded = false;
                }
            }

            if (GetKey(olc::Key::SPACE).bPressed)
                this->Restart();
            else if (GetKey(olc::Key::ESCAPE).bPressed) {
                m_pMenu->SetReadyToStart(false);
                this->Restart();
            }
        }
        
        m_pEntitiesManager->Update(fElapsedTime, m_nDifficultyLevel);
        
        // Rendering
        this->Render();
        
		return true;
	}

private:
    void Render() {
        Clear(olc::BLACK);

        DrawDecal(olc::vf2d(0, (int)m_fBackgroundPositionY), m_pBackgroundDecal.get(), olc::vf2d(SCALE, SCALE));
        DrawDecal(olc::vf2d(0, (int)m_fBackgroundPositionY - SCREEN_HEIGHT), m_pBackgroundDecal.get(), olc::vf2d(SCALE, SCALE));
        
        m_pEntitiesManager->Render(*this);

        // Score text
        // Shadow
        DrawStringDecal({ SCREEN_WIDTH - 50 + 1, 20 + 1 }, std::to_string((int)m_fScore) + "m", olc::BLACK, { 1, 1 });
        // Actual text
        DrawStringDecal({ SCREEN_WIDTH - 50, 20 }, std::to_string((int)m_fScore) + "m", olc::WHITE, { 1, 1 });
        
        if (!m_bRunning) {
            // Flash effect
            DrawDecal(olc::vf2d(0, 0), m_pFlashDecal.get(), olc::vf2d{ 1.0f, 1.0f } * 10.f * SCALE, olc::Pixel(255, 255, 255, m_fFlashTransperency));
            
            if (m_bFlashPhaseOneEnded == false) return;

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

            // "or ESC to exit to menu" text
            vTextPosition = { float(SCREEN_WIDTH / 2 - 86), float(SCREEN_HEIGHT / 2 + 30) };
            // Shadow
            DrawStringDecal(vTextPosition + olc::vf2d(1, 1), "or ESC to exit to menu", olc::BLACK, { 1, 1 });
            // Actual text
            DrawStringDecal(vTextPosition, "or ESC to exit to menu", olc::WHITE, { 1, 1 });
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
        m_fScore = 0.0f;
        m_fFlashTransperency = 0.0f;
        m_bFlashPhaseOneEnded = false;
        m_bFlashPhaseTwoEnded = false;
    }

    void StopAllObstacles() {
        for (const auto& pEntity : m_pEntitiesManager->GetEntities()) {
            if (Obstacle* o = dynamic_cast<Obstacle*>(pEntity.get()); o != nullptr) {
                LOG("stop all obstacles");
                o->ToggleEndGame();
            }
        }
    }

private:
    std::unique_ptr<olc::Sprite> m_pBackgroundSprite;
    std::unique_ptr<olc::Decal> m_pBackgroundDecal;
    
    std::unique_ptr<olc::Sprite> m_pFlashSprite;
    std::unique_ptr<olc::Decal> m_pFlashDecal;

    std::unique_ptr<Menu> m_pMenu;

    float m_fBackgroundPositionY = 0;

    std::shared_ptr<Player> m_pPlayer;
    std::unique_ptr<EntitiesManager> m_pEntitiesManager;

    int m_nDifficultyLevel = 1;
    float m_fScore = 1.0f; 

    bool m_bRunning = true;

    float m_fFlashTransperency = 0.0f;
    bool m_bFlashPhaseOneEnded = false;
    bool m_bFlashPhaseTwoEnded = false;

};


int main()
{
	Application app;
	if (app.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, 4, 4))
		app.Start();

	return 0;
}
