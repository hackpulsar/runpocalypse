#ifndef MANU_HPP
#define MANU_HPP

#include "olcPixelGameEngine.h"
#include "settings.hpp"

#include <fstream>

struct Button
{
    olc::vf2d vPosition;
    olc::vf2d vSize;
    std::string sText;
    olc::Pixel color = olc::WHITE;
};

class Menu 
{
public:
    Menu() {
        m_pBackgroundSprite = std::make_unique<olc::Sprite>("./bin/res/background.png");
        m_pBackgroundDecal = std::make_unique<olc::Decal>(m_pBackgroundSprite.get());

        m_pButtonSprite = std::make_unique<olc::Sprite>("./bin/res/button.png");
        m_pButtonDecal = std::make_unique<olc::Decal>(m_pButtonSprite.get());

        m_PlayButton = new Button { 
            { SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 }, 
            { 64, 16 }, "Play" 
        };
        m_ExitButton = new Button { 
            { SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 25 }, 
            { 64, 16 }, "Exit" 
        };

        std::ifstream file("highscore.txt", std::ios::in);
        if (file.is_open()) {
            file >> m_nHighscore;
            file.close();
        }
        else {
            std::ofstream file("highscore.txt", std::ios::out);
            if (file.is_open()) {
                file << 0;
                file.close();
            }
        }
    }

    void Update(olc::PixelGameEngine* pge) {
         // Moving background
        m_fBackgroundPositionY += BACKGROUND_SPEED * pge->GetElapsedTime();
        if (m_fBackgroundPositionY >= float(SCREEN_HEIGHT))
            m_fBackgroundPositionY = 0;


        m_fLogoRotation += 0.1f * m_fLogoRotationDirection * pge->GetElapsedTime();
        if (m_fLogoRotation >= 0.3f) {
            m_fLogoRotationDirection = -1.0f;
        }
        else if (m_fLogoRotation <= -0.3f) {
            m_fLogoRotationDirection = 1.0f;
        }
        
        olc::vi2d vMousePosition = pge->GetMousePos();
        if (vMousePosition.x >= m_PlayButton->vPosition.x && vMousePosition.x <= m_PlayButton->vPosition.x + m_PlayButton->vSize.x &&
            vMousePosition.y >= m_PlayButton->vPosition.y && vMousePosition.y <= m_PlayButton->vPosition.y + m_PlayButton->vSize.y) {
            m_PlayButton->color = olc::GREY;
            if (pge->GetMouse(0).bPressed) {
                m_bReadyToStart = true;
            }
        }
        else {
            m_PlayButton->color = olc::YELLOW;
        }

        if (vMousePosition.x >= m_ExitButton->vPosition.x && vMousePosition.x <= m_ExitButton->vPosition.x + m_ExitButton->vSize.x &&
            vMousePosition.y >= m_ExitButton->vPosition.y && vMousePosition.y <= m_ExitButton->vPosition.y + m_ExitButton->vSize.y) {
            m_ExitButton->color = olc::GREY;
            if (pge->GetMouse(0).bPressed) {
                exit(0);
            }
        }
        else {
            m_ExitButton->color = olc::YELLOW;
        }
    }

    void Render(olc::PixelGameEngine* pge) {
        pge->Clear(olc::RED);

        pge->DrawDecal(olc::vf2d(0, (int)m_fBackgroundPositionY), m_pBackgroundDecal.get(), olc::vf2d(SCALE, SCALE));
        pge->DrawDecal(olc::vf2d(0, (int)m_fBackgroundPositionY - SCREEN_HEIGHT), m_pBackgroundDecal.get(), olc::vf2d(SCALE, SCALE));
        
        // Logo
        pge->DrawRotatedStringDecal({ SCREEN_WIDTH / 2 + 1, SCREEN_HEIGHT / 2 - 50 + 1 }, "RUNPOCALYPSE", m_fLogoRotation, { 48, 4 }, olc::BLACK, { 2, 2 });
        pge->DrawRotatedStringDecal({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50 }, "RUNPOCALYPSE", m_fLogoRotation, { 48, 4 }, olc::YELLOW, { 2, 2 });
        
        // Buttons

        auto playButtonPos =  m_PlayButton->vPosition + m_PlayButton->vSize / 2 - olc::vf2d((8 * 4) / 2, 8 / 2);
        pge->DrawDecal(m_PlayButton->vPosition, m_pButtonDecal.get(), olc::vf2d(1, 1), olc::Pixel(255, 255, 255, 200));
        pge->DrawStringDecal(playButtonPos + olc::vf2d(1, 1), m_PlayButton->sText, olc::BLACK, { 1, 1 });
        pge->DrawStringDecal(playButtonPos, m_PlayButton->sText, m_PlayButton->color, { 1, 1 });
        
        auto exitButtonPos = m_ExitButton->vPosition + m_ExitButton->vSize / 2 - olc::vf2d((8 * 4) / 2, 8 / 2);
        pge->DrawDecal(m_ExitButton->vPosition, m_pButtonDecal.get(), olc::vf2d(1, 1), olc::Pixel(255, 255, 255, 200));
        pge->DrawStringDecal(exitButtonPos + olc::vf2d(1, 1), m_ExitButton->sText, olc::BLACK, { 1, 1 });
        pge->DrawStringDecal(exitButtonPos, m_ExitButton->sText, m_ExitButton->color, { 1, 1 });

        // Highscore text
        pge->DrawStringDecal({ SCREEN_WIDTH - (8 * 13) / 2 - 4 + 1, SCREEN_HEIGHT - 10 + 1 }, "Longest:" + std::to_string(m_nHighscore) + "m", olc::BLACK, { 0.5f, 0.5f });
        pge->DrawStringDecal({ SCREEN_WIDTH - (8 * 13) / 2 - 4, SCREEN_HEIGHT - 10 }, "Longest:" + std::to_string(m_nHighscore) + "m", olc::WHITE, { 0.5f, 0.5f });
    }
    
    void SetReadyToStart(bool bReady) { m_bReadyToStart = bReady; }
    bool IsReadyToStart() const { return m_bReadyToStart; }

private:
    std::unique_ptr<olc::Sprite> m_pBackgroundSprite;
    std::unique_ptr<olc::Decal> m_pBackgroundDecal;

    std::unique_ptr<olc::Sprite> m_pButtonSprite;
    std::unique_ptr<olc::Decal> m_pButtonDecal;

    float m_fBackgroundPositionY = 0;

    Button* m_PlayButton;
    Button* m_ExitButton;

    bool m_bReadyToStart = false;

    float m_fLogoRotation = 0.0f;
    int m_fLogoRotationDirection = 1.0f;

    int m_nHighscore = 0;

};

#endif // MANU_HPP
