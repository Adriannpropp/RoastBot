#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(RoastBotLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);

        int p = static_cast<int>(this->getCurrentPercent());
        std::string r = "";

        if (p < 5) {
            r = "Bro died in the first 5 seconds? even my cat can click better than that "; 
        } else if (p >= 5 && p < 15) {
            r = "Nah wtf was that? just delete the game and go play Flappy Bird 🙏"; 
        } else if (p == 16) {
            r = "16% again? you’re actually a legend for being this bad at the game wtf "; 
        } else if (p > 16 && p < 40) {
            r = "Stuck in the low percentages? that's a skill issue if I ever saw one ";
        } else if (p >= 40 && p < 65) {
            r = "Mid-way death? you're the definition of mediocre wtf ";
        } else if (p >= 65 && p < 80) {
            r = "Imagine getting this far just to fall asleep on the mouse ";
        } else if (p >= 80 && p < 95) {
            r = "Gah dayum, I can smell the sweat from here... and you still died?"; 
        } else if (p >= 95 && p < 98) {
            r = "My grandma could’ve cleared that jump lol 📉🚢";
        } else if (p >= 98) {
            r = "Ok but imagine getting that far just to click like a bot "; 
        }

        if (!r.empty()) {
            Notification::create(r, NotificationIcon::Error, 3.0f)->show();
        }
    }
};