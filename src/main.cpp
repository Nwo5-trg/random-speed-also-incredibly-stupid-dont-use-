#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <random>

using namespace geode::prelude;

// do not let another mod dev see this code its so bad trust me its only like this cuz i made it in like 5 minutes and didnt care enough :prayer:
// also i give up on dual like omg

std::array<float, 5> speeds = {0.7f, 0.9f, 1.1f, 1.3f, 1.6f};
float currentRandSpeed = 0.0f;
float lastSpeed = 0.0f;
bool isPLayLayer = false;

std::mt19937 rng(std::random_device{}()); 
std::uniform_int_distribution<int> distribution(0, 4);

class $modify(Play, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        isPLayLayer = true;
        return true;
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);
        lastSpeed = 0.0f;
    }
};

class $modify(BaseGame, GJBaseGameLayer) {
    bool init() {
        isPLayLayer = false;
        lastSpeed = 0;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(BaseGame::updateRandom), this, 0.0f, false);
        return GJBaseGameLayer::init();
    }

    void updateRandom(float dt) {
        if (!isPLayLayer) return;
        currentRandSpeed = speeds[distribution(rng)];
        if (lastSpeed == 0.0f) updatePlayerSpeed();
        if (m_player1->m_playerSpeed != lastSpeed || m_player2->m_playerSpeed != m_player1->m_playerSpeed) updatePlayerSpeed();
            
        lastSpeed = m_player1->m_playerSpeed;
    }

    void updatePlayerSpeed() {
        if (auto p1 = m_player1) p1->m_playerSpeed = currentRandSpeed;
        if (auto p2 = m_player2) p2->m_playerSpeed = currentRandSpeed;
    }
};