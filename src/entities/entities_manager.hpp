#ifndef ENTITIES_MANAGER_HPP
#define ENTITIES_MANAGER_HPP

#include "player.hpp"
#include "spit_zombie.hpp"
#include "barrel_zombie.hpp"
#include "warning.hpp"
#include "rocket.hpp"
#include "boom.hpp"
#include "acid_spit.hpp"
#include "barrel.hpp"

#include "obstacles/acid_puddle.hpp"
#include "obstacles/acid_barrel.hpp"
#include "obstacles/rock.hpp"
#include "obstacles/tree.hpp"
#include "obstacles/bush.hpp"
#include "obstacles/small_tree.hpp"
#include "obstacles/car.hpp"

#include "log.hpp"
#include "randomize.hpp"
#include "collision.hpp"

#include <algorithm>
#include <memory>

typedef std::pair<float, int> SpawnData;
typedef std::vector<std::unique_ptr<Entity>> EntitiesVector;

class EntitiesManager
{
public:
    EntitiesManager(std::shared_ptr<Player> pPlayer, bool* pGameRunning) 
        : m_pPlayer(pPlayer), m_pGameRunning(pGameRunning)
    {
        m_vLayers.resize(4);
        m_vLayers[m_pPlayer->GetLayer()].push_back(m_pPlayer.get());
    }

    ~EntitiesManager() { }

    void AddEntity(std::unique_ptr<Entity> pEntity, int nLane = 1) {
        pEntity->AdjustPosition(nLane);

        Renderable* r = dynamic_cast<Renderable*>(pEntity.get());
        if (r != nullptr) {
            m_vLayers[r->GetLayer()].push_back(pEntity.get());
            LOG("Entity added to the layer");
        }

        m_vEntities.push_back(std::move(pEntity));

        LOG("Entity added to the game");
    }

    void MakeBoom(const olc::vf2d& vPosition, BoomType type = BoomType::Basic) {
        this->AddEntity(std::make_unique<Boom>(vPosition, type));
    }

    void Update(float fElapsedTime, const int nDifficultyLevel) {
        // Basic spawner timer
        if (*m_pGameRunning) m_fEnemiesSpawnTimer += fElapsedTime;
        if (m_fEnemiesSpawnTimer >= m_fEnemiesSpawnInterval) {
            m_fEnemiesSpawnTimer = 0.0f;
            
            m_fEnemiesSpawnInterval = 10.f - float(nDifficultyLevel);
            
            // Show a warning first and add an enemy to the queue
            int nLane, nTries = 0;
            do {
                nLane = Randomize::GetRandom(1, 4);
                nTries++;
                //LOG("Lane for enemy: " + std::to_string(nLane));
            } while (m_bLanes.test(nLane - 1) && nTries < 4);

            if (nTries < 4) {
                m_bLanes.set(nLane - 1);

                AddEntity(std::make_unique<Warning>(), nLane);
                m_vSpawnQueue.push_back({WARNING_LASTS, nLane});
            }
        }

        if (*m_pGameRunning) m_fObstaclesSpawnTimer += fElapsedTime;
        if (m_fObstaclesSpawnTimer >= m_fObstaclesSpawnInterval) {
            m_fObstaclesSpawnTimer = 0.0f;
            
            m_fObstaclesSpawnInterval = 8.f - float(nDifficultyLevel) / 2.f;

            int nLane = Randomize::GetRandom(1, 4);
            LOG("Lane for an obstacle: " + std::to_string(nLane));
            int nObstacleType = Randomize::GetRandom(0, 8);
            if (nObstacleType == 0)
                AddEntity(std::make_unique<AcidPuddle>(), nLane);
            else if (nObstacleType == 1)
                AddEntity(std::make_unique<AcidBarrel>(), nLane);
            else if (nObstacleType == 2)
                AddEntity(std::make_unique<Rock>(), nLane);
            else if (nObstacleType == 3)
                AddEntity(std::make_unique<Tree>(), nLane);
            else if (nObstacleType == 4)
                AddEntity(std::make_unique<Bush>(), nLane);
            else if (nObstacleType == 5)
                AddEntity(std::make_unique<SmallTree>(), nLane);
            else if (nObstacleType >= 6)
                AddEntity(std::make_unique<Car>(), nLane);
            else
                LOG("Unknown obstacle type");
            LOG("Obstacle added to the game: " + std::to_string(nLane));
        }

        // Removing all entities from the layers that are marked to self-destruct
        for (auto& layer : m_vLayers) {
            layer.erase(
                std::remove_if(
                    layer.begin(), layer.end(),
                    [](Entity* pEntity) { return pEntity->IsSelfDestruct(); }
                ),
                layer.end()
            );
        }
        
        // Removing all entities that are marked to self-destruct
        m_vEntities.erase(
            std::remove_if(
                m_vEntities.begin(), m_vEntities.end(),
                [](std::unique_ptr<Entity>& pEntity) { return pEntity->IsSelfDestruct(); }
            ),
            m_vEntities.end()
        );

        // Spawning entities from the queue
        for (auto& data : m_vSpawnQueue) {
            float& fTime = data.first;
            fTime -= fElapsedTime;
            if (fTime <= 0.0f) {
                int nEnemyType = Randomize::GetRandom(0, 2);

                if (nEnemyType == 0)
                    AddEntity(std::make_unique<Rocket>(), data.second); 
                else if (nEnemyType == 1)
                    AddEntity(std::make_unique<SpitZombie>(), data.second);
                else if (nEnemyType == 2)
                    AddEntity(std::make_unique<BarrelZombie>(), data.second);
                else
                    LOG("Unknown enemy type");
            }
        }

        // Removing entities from the queue that have already spawned
        m_vSpawnQueue.erase(
            std::remove_if(
                m_vSpawnQueue.begin(), m_vSpawnQueue.end(),
                [](SpawnData data) { return data.first <= 0.0f; }
            ),
            m_vSpawnQueue.end()
        );

        for (auto& pEntity : m_vEntities) {
            Zombie* sz = dynamic_cast<Zombie*>(pEntity.get());
            if (sz != nullptr) {
                if (*m_pGameRunning == false) sz->ToggleEndGame();

                if (sz->ReadyToShoot()) {
                    sz->Shoot();

                    if (dynamic_cast<BarrelZombie*>(pEntity.get()) != nullptr)
                        this->AddEntity(std::make_unique<Barrel>(), sz->GetLane());
                    else
                        this->AddEntity(std::make_unique<AcidSpit>(), sz->GetLane());
                    break; // ones per one cycle to avoid messing up the container while iterating
                }
            }
        }
        
        // Update
        m_pPlayer->Update(fElapsedTime);

        m_bLanes.reset();
        for (auto& pEntity : m_vEntities) {
            pEntity->Update(fElapsedTime);

            Obstacle* o = dynamic_cast<Obstacle*>(pEntity.get());
            if (o == nullptr) m_bLanes.set(pEntity->GetLane() - 1);
            else {
                for (auto& e : m_vLayers[2]) {
                    Zombie* z = dynamic_cast<Zombie*>(e);
                    if (z != nullptr && Collision::AABB(*z, *o)) {
                        z->ToggleColided();
                    } 
                }
            }
        }
    }

    void Render(olc::PixelGameEngine& pge) const {
        // layers.
        for (const auto& layer : m_vLayers) {
            for (const auto& pEntity : layer) {
                Renderable* r = dynamic_cast<Renderable*>(pEntity);
                if (r != nullptr)
                    pEntity->Render(pge);
            }
        }
    }

    const EntitiesVector& GetEntities() const { return m_vEntities; }

    void Reset() {
        m_vEntities.clear();

        for (auto& layer : m_vLayers)
            layer.clear();
        m_vLayers.clear();
        m_vLayers.resize(4);

        m_vLayers[m_pPlayer->GetLayer()].push_back(m_pPlayer.get());

        m_vSpawnQueue.clear();

        m_fEnemiesSpawnTimer = 0.0f;
        m_fEnemiesSpawnInterval = 10.0f;

        LOG("Entities manager reset");
    }

private:
    EntitiesVector m_vEntities;
    std::shared_ptr<Player> m_pPlayer;

    std::vector<std::vector<Entity*>> m_vLayers;

    std::vector<SpawnData> m_vSpawnQueue;
    std::bitset<4> m_bLanes;

    float m_fEnemiesSpawnTimer = 0.0f;
    float m_fObstaclesSpawnTimer = 0.0f;

    float m_fEnemiesSpawnInterval = 10.0f;
    float m_fObstaclesSpawnInterval = 8.0f;

    bool* m_pGameRunning;

};

#endif // ENTITIES_MANAGER_HPP
