#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(RoastBotLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        if (p0->m_isDead) {
            int p = static_cast<int>(this->getCurrentPercent());
            std::string r = "";

            if (p < 5) {
                r = "Bro died in the first 5 seconds?\nEven my cat can click better than that 😭💀"; 
            } else if (p >= 5 && p < 15) {
                r = "Nah wtf was that?\nJust delete the game and go play Flappy Bird 🙏"; 
            } else if (p == 16) {
                r = "16% again?\nYou're actually a legend for being this bad wtf 😭🔥"; 
            } else if (p > 16 && p < 40) {
                r = "Stuck in the low percentages?\nThat's a skill issue if I ever saw one 💀";
            } else if (p >= 40 && p < 80) {
                r = "Imagine getting this far\nJust to fall asleep on the mouse 🙏";
            } else if (p >= 80 && p < 95) {
                r = "Gah dayum, I can smell the sweat...\nAnd you still died? 💀"; 
            } else if (p >= 95 && p < 98) {
                r = "My grandma could've\nCleared that jump lol 📉";
            } else if (p >= 98) {
                r = "Ok but imagine getting that far\nJust to click like a bot 🙏"; 
            }

            if (!r.empty()) {
                auto icon = CCSprite::createWithSpriteFrameName("difficulty_00_001.png");
                Notification::create(r, icon, 3.0f)->show();
            }
        }

        PlayLayer::destroyPlayer(p0, p1);
    }
};