#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/fmod/fmod.hpp>
#include <random>
#include <vector>
#include <string>

using namespace geode::prelude;

// thanks @fryy_55 for the random stuff
template <typename T>
inline T getRandom(T min, T max) {
    thread_local std::mt19937_64 mt{std::random_device{}()};
    if constexpr (std::is_integral_v<T>) {
        return std::uniform_int_distribution<T>(min, max)(mt);
    } else {
        return std::uniform_real_distribution<T>(min, max)(mt);
    }
}

class $modify(RoastBotLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);
        if (!player->m_isDead) return;

        int percent = this->getCurrentPercentInt(); 
        std::string levelName = this->m_level->m_levelName;

        // 67 fedi idea
        if (percent == 67) {
            auto audioPath = Mod::get()->getResourcesDir() / "67.mp3";
            if (auto fmodEngine = FMODAudioEngine::sharedEngine(); fmodEngine && fmodEngine->m_system) {
                FMOD::Sound* sound = nullptr;
                fmodEngine->m_system->createSound(audioPath.string().c_str(), FMOD_DEFAULT, nullptr, &sound);
                if (sound) fmodEngine->m_system->playSound(sound, nullptr, false, nullptr);
            }
        }

        auto stats = GameStatsManager::sharedState();
        int totalDemons = stats->getStat("5");
        int hardDemons = stats->getStat("14");
        int insaneDemons = stats->getStat("15");
        
        bool isExtreme = (this->m_level->m_stars == 10 && this->m_level->m_demonDifficulty == 6);

        std::string roast;
        std::string iconFile = "med.png";

        if (getRandom(1, 100) == 1) {
            roast = "Alright, I've roasted you enough.\nYou'll beat it soon, keep going 👍";
        } else {
            if (isExtreme && percent > 5) {
                if (totalDemons < 30) {
                    roast = fmt::format("Only {} demons beaten and you're already on {}?\nBold choice, my friend 💀", totalDemons, levelName);
                    iconFile = "idfk.png";
                } else if (hardDemons == 0 && insaneDemons == 0) {
                    roast = "Zero hard demons, zero insane demons...\nand you're tackling " + levelName + "?\nDelusional 💀";
                    iconFile = "idfk.png";
                }
            }

            if (roast.empty()) {
                std::vector<std::string> msgs;
                if (percent < 20) {
                    iconFile = "med.png";
                    msgs = {"Died early? my grandma clicks better 😭", "Bro just play flappy bird 🙏", "Skill issue 💀", "My cat survived that 😂", "You vs the first spike: spike wins in 0.2 seconds 🔥"};
                    if (percent == 16) roast = "16% *again*?\nYou're a legend at being consistently bad 🔥😭";
                } else if (percent < 50) {
                    iconFile = "med.png";
                    msgs = {"Getting somewhere? More like nowhere 😔", "At least you're better than 0% 😂", "Bro thinks he's making progress LMAO 🔥", "You call that clicking? My dog clicks better 😭"};
                } else if (percent < 80) {
                    iconFile = "hard.png";
                    msgs = {"Mid-game choke? Standard 📉", "Imagine making it halfway and still throwing 😔"};
                    if (percent == 49) roast = "Couldn't even reach 50%?\nMassive skill issue, buddy 😭";
                    if (percent == 67) roast = "67% AYYYY 🔥\n(But fedi still says you got a skill issue >:])";
                } else {
                    iconFile = "idfk.png";
                    msgs = {"So close... yet u click like a potato 🙏", "Late-game choke? Quit for the day bro 💀", "holy skill issue"};
                    if (percent == 87) roast = "87 + 87 + 87 + 87, which means u have a skill issue lol 😂";
                    if (percent == 99) roast = "99%... and you still died?\nBro literally clicked 1% wrong somewhere 🔥😭";
                    if (percent == 100) {
                        roast = "You actually did it... good boy";
                        iconFile = "legendary.png";
                    }
                }

                if (roast.empty() && !msgs.empty()) {
                    roast = msgs[getRandom<size_t>(0, msgs.size() - 1)];
                }
            }
        }

        if (!roast.empty()) {
            auto iconPath = Mod::get()->getResourcesDir() / iconFile;
            auto* icon = CCSprite::create(iconPath.string().c_str());
            if (icon) icon->setScale(0.5f);
            Notification::create(roast, icon, 3.0f)->show();
        }
    }
};

// holy fish
// who doesnt love c++
// i do