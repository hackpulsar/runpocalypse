#ifndef ENTITIES_MANAGER_HPP
#define ENTITIES_MANAGER_HPP

#include "warning.hpp"
#include "rocket.hpp"
#include "boom.hpp"

typedef std::pair<float, int> SpawnData;

class EntitiesManager
{
public:
    EntitiesManager() = default;

    ~EntitiesManager() {
        for (auto& pEntity : m_vEntities)
            delete pEntity;
    }

    void AddEntity(Entity* pEntity, int nLane = 1) {
        pEntity->AdjustPosition(nLane);
        m_vEntities.push_back(pEntity);
    }

    void MakeBoom(const olc::vf2d& vPosition, BoomType type = BoomType::Basic) {
        this->AddEntity(new Boom(vPosition, type));
    }

    void Update(float fElapsedTime, const int nDifficultyLevel) {
        // Basic spawner timer
        m_fSpawnTimer += fElapsedTime;
        if (m_fSpawnTimer >= m_fSpawnInterval) {
            m_fSpawnTimer = 0.0f;
            //m_fSpawnInterval = 5.0f - nDifficultyLevel * 0.5f;
            
            // Show a warning first and add an enemy to the queue
            int nLane = rand() % 3 + 1;
            AddEntity(new Warning(), nLane);
            m_vSpawnQueue.push_back({WARNING_LASTS, nLane});
        }
        
        // Removing all entities that are marked to self-destruct
        m_vEntities.erase(
            std::remove_if(
                m_vEntities.begin(), m_vEntities.end(),
                [](Entity* pEntity) { return pEntity->IsSelfDestruct(); }
            ),
            m_vEntities.end()
        );
        
        // Spawning entities from the queue
        for (auto& data : m_vSpawnQueue) {
            float& fTime = data.first;
            fTime -= fElapsedTime;
            if (fTime <= 0.0f)
                AddEntity(new Rocket(), data.second);
        }

        // Removing entities from the queue that have already spawned
        m_vSpawnQueue.erase(
            std::remove_if(
                m_vSpawnQueue.begin(), m_vSpawnQueue.end(),
                [](SpawnData data) { return data.first <= 0.0f; }
            ),
            m_vSpawnQueue.end()
        );
        
        // Update
        for (auto& pEntity : m_vEntities) 
            pEntity->Update(fElapsedTime); 
    }

    void Render(olc::PixelGameEngine& pge) const {
        for (auto& pEntity : m_vEntities)
            pEntity->Render(pge);
    }

    const std::vector<Entity*>& GetEntities() const { return m_vEntities; }

    void Reset() {
        for (auto& pEntity : m_vEntities)
            delete pEntity;
        m_vEntities.clear();

        m_vSpawnQueue.clear();

        m_fSpawnTimer = 0.0f;
        m_fSpawnInterval = 5.0f;
    }

private:
    std::vector<Entity*> m_vEntities;
    std::vector<SpawnData> m_vSpawnQueue;

    float m_fSpawnTimer = 0.0f;
    float m_fSpawnInterval = 5.0f;

};

#endif // ENTITIES_MANAGER_HPP
