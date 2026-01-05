#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/fmod/fmod.hpp>
#include <vector>
#include <string>

using namespace geode::prelude;

class $modify(RoastBotLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);
        if (!player->m_isDead) return;

        int p = static_cast<int>(this->getCurrentPercent());
        std::string levelName = this->m_level->m_levelName;

        // --- AUDIO TRIGGER (67%) ---
        if (p == 67) {
            auto audioPath = Mod::get()->getResourcesDir() / "67.mp3";
            
            log::info("Attempting to play audio from: {}", audioPath.string());
            
            auto fmodEngine = FMODAudioEngine::sharedEngine();
            if (fmodEngine && fmodEngine->m_system) {
                FMOD::System* system = fmodEngine->m_system;
                FMOD::Sound* sound = nullptr;
                FMOD_RESULT result = system->createSound(
                    audioPath.string().c_str(), 
                    FMOD_DEFAULT, 
                    nullptr, 
                    &sound
                );
                
                if (result == FMOD_OK && sound) {
                    FMOD::Channel* channel = nullptr;
                    system->playSound(sound, nullptr, false, &channel);
                    log::info("Playing 67.mp3 audio");
                }
            }
        }

        auto stats = GameStatsManager::sharedState();
        int totalDemons = stats->getStat("5"); 
        int hard = stats->getStat("14");       
        int insane = stats->getStat("15");     
        bool isExtreme = (this->m_level->m_stars == 10 && this->m_level->m_demonDifficulty == 6);

        std::string r = "";
        std::string iconName = "med.png";

        // 1% CHANCE TO BE NICE
        if (rand() % 100 == 0) {
            r = "Ok i flamed u enough, u will beat it soon 👍";
            iconName = "med.png";
        } 
        else {
            // --- JUMPING EXTREMES ROASTS ---
            if (isExtreme && p > 5) {
                if (totalDemons < 30) {
                    r = fmt::format("Only {} demons and you're jumping\nto {}? Delusional 💀", totalDemons, levelName);
                    iconName = "idfk.png";
                } else if (hard == 0 && insane == 0) {
                    r = "0 Hards, 0 Insanes...\nAnd you're jumping to " + levelName + "? Delusional 💀";
                    iconName = "idfk.png";
                }
            }

            if (r.empty()) {
                std::vector<std::string> pool;
                
                if (p < 20) {
                    iconName = "med.png";
                    pool = {
                        "Bro died in the first 5 seconds?\nEven my cat can click better than that 😭💀",
                        "Nah wtf was that?\nJust delete the game and go play Flappy Bird 🙏",
                        "Stuck in the low percentages?\nThat's a skill issue if I ever saw one 💀",
                        "My grandma could've\nCleared that jump lol 📉",
                        "Are u even trying or is the mouse broken? 💀"
                    };
                    if (p == 16) r = "16% again?\nYou're actually a legend for being this bad wtf 😭🔥";
                } 
                else if (p >= 40 && p < 80) {
                    iconName = "hard.png";
                    pool = {
                        "Imagine getting this far\nJust to fall asleep on the mouse 🙏",
                        "Gah dayum, I can smell the sweat...\nAnd you still died? 💀",
                        "You're actually consistent at failing\nI'll give u that 😭",
                        "Mid-level choke? Standard. 📉"
                    };
                    if (p == 49) r = "Buddy o pal,\nu couldn't even make it halfway holy skill issue 😭";
                    if (p == 67) r = "AYEEEEE 67, fedi says u have a skill issue btw >:]";
                } 
                else if (p >= 80) {
                    iconName = "idfk.png";
                    pool = {
                        "Ok but imagine getting that far\nJust to click like a bot 🙏",
                        "I know u just threw your mouse\nI heard it from here 💀",
                        "90%+ death? Just stop playing for today bro 🙏",
                        "That's gotta be a new record for choking 📉"
                    };
                }

                if (r.empty() && !pool.empty()) {
                    r = pool[rand() % pool.size()];
                }
            }
        }

        if (!r.empty()) {
            // Try to load the icon - just attempt it directly
            CCSprite* icon = CCSprite::create((Mod::get()->getResourcesDir() / iconName).string().c_str());
            
            if (icon) {
                icon->setScale(0.5f);
                log::info("Icon loaded: {}", iconName);
            } else {
                log::warn("Could not load icon: {}", iconName);
                // Try alternative names
                if (iconName == "idfk.png") {
                    icon = CCSprite::create((Mod::get()->getResourcesDir() / "hard.png").string().c_str());
                }
                if (icon) icon->setScale(0.5f);
            }
            
            // Show notification (with or without icon)
            Notification::create(r, icon, 3.0f)->show();
            log::info("Notification: {}", r);
        }
    }
};