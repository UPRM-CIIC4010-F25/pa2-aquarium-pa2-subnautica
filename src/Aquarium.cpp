#include "Aquarium.h"
#include <cstdlib>
#include <cmath>


string AquariumCreatureTypeToString(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::BiggerFish:
            return "BiggerFish";
        case AquariumCreatureType::NPCreature:
            return "BaseFish";
            case AquariumCreatureType::JellyFish:
            return "JellyFish";
        case AquariumCreatureType::SkeletonFish:
            return "SkeletonFish";
        default:
            return "UnknownFish"; //spell fix
    }
}

// PlayerCreature Implementation
PlayerCreature::PlayerCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 10.0f, 1, sprite) {
    this->m_original_speed = speed;
}


void PlayerCreature::setDirection(float dx, float dy) {
    m_dx = dx;
    m_dy = dy;
    normalize();
}

void PlayerCreature::move() {
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    this->bounce();
}

void PlayerCreature::reduceDamageDebounce() {
    if (m_damage_debounce > 0) {
        --m_damage_debounce;
    }
}

void PlayerCreature::update() {
    this->reduceDamageDebounce();
    this->applyTemporarySpeed();
    this->reduceInvincibilityTimer();
    this->move();
}


void PlayerCreature::draw() const {
    
    ofLogVerbose() << "PlayerCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    if (this->m_damage_debounce > 0) {
        ofSetColor(ofColor::red); // Flash red if in damage debounce
    }
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
    ofSetColor(ofColor::white); // Reset color
 
    if (this->m_damage_debounce > 0 || this->m_invincibility_timer > 0) { // Check for invincibility too
        ofSetColor(ofColor::yellow, 150); // Flash yellow transparent for invincibility
    } else {
        ofSetColor(ofColor::white); // Default
    }
ofSetColor(ofColor::white); // Reset color
}

void PlayerCreature::changeSpeed(int speed) {
    m_speed = speed;
}

void PlayerCreature::loseLife(int debounce) {
    if (m_invincibility_timer > 0) {
 ofLogNotice() << "Player is invincible! Life loss prevented." << std::endl;
return;}
    if (m_damage_debounce <= 0) {
        if (m_lives > 0) this->m_lives -= 1;
        m_damage_debounce = debounce; // Set debounce frames
        ofLogNotice() << "Player lost a life! Lives remaining: " << m_lives << std::endl;
    }
    // If in debounce period, do nothing
    if (m_damage_debounce > 0) {
        ofLogVerbose() << "Player is in damage debounce period. Frames left: " << m_damage_debounce << std::endl;
    }
}
void PlayerCreature::grantInvincibility(int durationFrames) {
 m_invincibility_timer = durationFrames;
 ofLogNotice() << "Player is INVINCIBLE for " << durationFrames << " frames!" << std::endl;
}

void PlayerCreature::reduceInvincibilityTimer() {
 if (m_invincibility_timer > 0) {
--m_invincibility_timer;
ofLogVerbose() << "Invincibility active. Frames left: " << m_invincibility_timer << std::endl;
}
}

// NPCreature Implementation
NPCreature::NPCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 30, 1, sprite) {
    m_dx = (rand() % 3 - 1); // -1, 0, or 1
    m_dy = (rand() % 3 - 1); // -1, 0, or 1
    normalize();

    m_creatureType = AquariumCreatureType::NPCreature;
}

void NPCreature::move() {
    // Simple AI movement logic (random direction)
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }
    bounce();
}

void NPCreature::draw() const {
    ofLogVerbose() << "NPCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    ofSetColor(ofColor::white);
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
}


BiggerFish::BiggerFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(60); // Bigger fish have a larger collision radius
    m_value = 5; // Bigger fish have a higher value
    m_creatureType = AquariumCreatureType::BiggerFish;
}

void BiggerFish::move() {
    // Bigger fish might move slower or have different logic
    m_x += m_dx * (m_speed * 0.5); // Moves at half speed
    m_y += m_dy * (m_speed * 0.5);
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }

    bounce();
}

void BiggerFish::draw() const {
    ofLogVerbose() << "BiggerFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}

JellyFish::JellyFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = 0;
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(25.0f); // Jellyfish have a smaller collision radius
    m_value = 2;
    m_creatureType = AquariumCreatureType::JellyFish;
}

void JellyFish::move() {
    static float time = 0.0f;
    time += 0.1f;
    m_x += m_dx * (m_speed * 0.2f); // Slow drift
    m_y += std::sin(time) * 2.0f;   // vertical movement
    if (m_dx < 0) this->m_sprite->setFlipped(true);
    else this->m_sprite->setFlipped(false);
    bounce();
}

void JellyFish::draw() const {
    ofLogVerbose() << "JellyFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    if (m_sprite) m_sprite->draw(m_x, m_y);
}

SkeletonFish::SkeletonFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = 1; // Always moves right by default
    m_dy = 0;
    normalize();

    setCollisionRadius(40.0f);
    m_value = 3;
    m_creatureType = AquariumCreatureType::SkeletonFish;
}

void SkeletonFish::move() {
    m_x += m_dx * m_speed;
    if (m_dx < 0) this->m_sprite->setFlipped(true);
    else this->m_sprite->setFlipped(false);
    bounce();
}

void SkeletonFish::draw() const {
    ofLogVerbose() << "SkeletonFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    if (m_sprite) m_sprite->draw(m_x, m_y);
}

SpeedPowerUp::SpeedPowerUp(float x, float y, std::shared_ptr<GameSprite> sprite)
    : Creature(x, y, 1, 20.0f, 0, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();
}

void SpeedPowerUp::move() {
    m_x += m_dx * m_speed * 0.5f; 
    m_y += m_dy * m_speed * 0.5f;
    this->bounce();
}

void SpeedPowerUp::draw() const {
    ofLogVerbose() << "SpeedPowerUp at (" << m_x << ", " << m_y << ")" << std::endl;
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
}

void PlayerCreature::applyTemporarySpeed() {
    if (m_speed_boost_timer > 0) {
        --m_speed_boost_timer;
        ofLogVerbose() << "Speed boost active. Frames left: " << m_speed_boost_timer << std::endl;
    } else if (m_speed_boost_timer == 0 && m_speed != m_original_speed) {
        m_speed = m_original_speed;
        ofLogNotice() << "Speed boost expired. Speed reset to " << m_original_speed << "!" << std::endl;
    }
}

ExtraLifePowerUp::ExtraLifePowerUp(float x, float y, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, 1, 20.0f, 0, sprite) {
m_dx = (rand() % 3 - 1);
m_dy = (rand() % 3 - 1);
normalize();
}

void ExtraLifePowerUp::move() {
 m_x += m_dx * m_speed * 0.5f; 
m_y += m_dy * m_speed * 0.5f;
this->bounce();
}

void ExtraLifePowerUp::draw() const {
 ofLogVerbose() << "ExtraLifePowerUp at (" << m_x << ", " << m_y << ")" << std::endl;
 if (m_sprite) {
 m_sprite->draw(m_x, m_y);
}
}


// InvincibilityPowerUp Implementation
InvincibilityPowerUp::InvincibilityPowerUp(float x, float y, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, 1, 20.0f, 0, sprite) {
 m_dx = (rand() % 3 - 1);
 m_dy = (rand() % 3 - 1);
 normalize();
}

void InvincibilityPowerUp::move() {
 m_x += m_dx * m_speed * 0.5f; 
 m_y += m_dy * m_speed * 0.5f;
this->bounce();
}

void InvincibilityPowerUp::draw() const {
ofLogVerbose() << "InvincibilityPowerUp at (" << m_x << ", " << m_y << ")" << std::endl;
 if (m_sprite) {
m_sprite->draw(m_x, m_y);
}
}

// In Aquarium.cpp

void PlayerCreature::increaseSpeed(int value, int durationFrames) {
    if (m_speed_boost_timer <= 0) {
        m_original_speed = m_speed; 
    }
    m_speed += value; 
    m_speed_boost_timer += durationFrames;
    
    ofLogNotice() << "Player speed increased by " << value << " to " << m_speed << "! Duration: " << durationFrames << " frames." << std::endl;
}

// AquariumSpriteManager
AquariumSpriteManager::AquariumSpriteManager(){
    this->m_npc_fish = std::make_shared<GameSprite>("base-fish.png", 70,70);
    this->m_big_fish = std::make_shared<GameSprite>("bigger-fish.png", 120, 120);
    this->m_jelly_fish = std::make_shared<GameSprite>("Jellyfish.png", 50, 50);
    this->m_skeleton_fish = std::make_shared<GameSprite>("Robo Fish.png", 100, 100);
    this->m_speed_powerup_sprite = std::make_shared<GameSprite>("speedboost.png", 50, 50);
    this->m_extra_life_powerup_sprite = std::make_shared<GameSprite>("extra life.png", 60, 60);
    this->m_invincibility_powerup_sprite = std::make_shared<GameSprite>("Invincibility.png", 80, 80);
}

std::shared_ptr<GameSprite> AquariumSpriteManager::GetSprite(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::BiggerFish:
            return std::make_shared<GameSprite>(*this->m_big_fish);
            
        case AquariumCreatureType::NPCreature:
            return std::make_shared<GameSprite>(*this->m_npc_fish);
            case AquariumCreatureType::JellyFish:
            return std::make_shared<GameSprite>(*this->m_jelly_fish);
        case AquariumCreatureType::SkeletonFish:
            return std::make_shared<GameSprite>(*this->m_skeleton_fish);
        case AquariumCreatureType::SpeedPowerup: 
            return std::make_shared<GameSprite>(*this->m_speed_powerup_sprite);
            case AquariumCreatureType::ExtraLifePowerup: 
        return std::make_shared<GameSprite>(*this->m_extra_life_powerup_sprite);
        case AquariumCreatureType::InvincibilityPowerup: 
        return std::make_shared<GameSprite>(*this->m_invincibility_powerup_sprite);
        default:
            return nullptr;
    }
}


// Aquarium Implementation
Aquarium::Aquarium(int width, int height, std::shared_ptr<AquariumSpriteManager> spriteManager)
    : m_width(width), m_height(height) {
        m_sprite_manager =  spriteManager;
    }



void Aquarium::addCreature(std::shared_ptr<Creature> creature) {
    creature->setBounds(m_width - 20, m_height - 20);
    m_creatures.push_back(creature);
}

void Aquarium::addAquariumLevel(std::shared_ptr<AquariumLevel> level){
    if(level == nullptr){return;} // guard to not add noise
    this->m_aquariumlevels.push_back(level);
}

void Aquarium::update() {
    for (auto& creature : m_creatures) {
        creature->move();
    }
    this->Repopulate();
}

void Aquarium::draw() const {
    for (const auto& creature : m_creatures) {
        creature->draw();
    }
}


void Aquarium::removeCreature(std::shared_ptr<Creature> creature) {
    auto it = std::find(m_creatures.begin(), m_creatures.end(), creature);
    if (it != m_creatures.end()) {
        ofLogVerbose() << "removing creature " << endl;
        auto npcCreature = std::dynamic_pointer_cast<NPCreature>(creature);
        if (npcCreature) {
        int selectLvl = this->currentLevel % this->m_aquariumlevels.size();
        this->m_aquariumlevels.at(selectLvl)->ConsumePopulation(npcCreature->GetType(), npcCreature->getValue());
        }else {
            ofLogVerbose() << "Not an NPCreature " << endl;
        }
        m_creatures.erase(it);
    }
}

void Aquarium::clearCreatures() {
    m_creatures.clear();
}

std::shared_ptr<Creature> Aquarium::getCreatureAt(int index) {
    if (index < 0 || size_t(index) >= m_creatures.size()) {
        return nullptr;
    }
    return m_creatures[index];
}



void Aquarium::SpawnCreature(AquariumCreatureType type) {
     int x = rand() % this->getWidth();
    int y = rand() % this->getHeight();
    int speed = 1 + rand() % 6;

    switch (type) {
        case AquariumCreatureType::NPCreature:
            this->addCreature(std::make_shared<NPCreature>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::NPCreature)));
            break;
        case AquariumCreatureType::BiggerFish:
            this->addCreature(std::make_shared<BiggerFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::BiggerFish)));
            break;
        case AquariumCreatureType::JellyFish:
            this->addCreature(std::make_shared<JellyFish>(x, y, std::max(1, speed/3), this->m_sprite_manager->GetSprite(AquariumCreatureType::JellyFish)));
            break;
        case AquariumCreatureType::SkeletonFish: {
            // spawn at left edge by constructing with x=0 (no setX needed)
            auto sk = std::make_shared<SkeletonFish>(0, y, std::max(2, speed), this->m_sprite_manager->GetSprite(AquariumCreatureType::SkeletonFish));
            this->addCreature(sk);
            break;
        }
        case AquariumCreatureType::SpeedPowerup: { 
             this->addCreature(std::make_shared<SpeedPowerUp>(x, y, this->m_sprite_manager->GetSprite(AquariumCreatureType::SpeedPowerup)));
             break;
        }
        case AquariumCreatureType::ExtraLifePowerup: { 
        this->addCreature(std::make_shared<ExtraLifePowerUp>(x, y, this->m_sprite_manager->GetSprite(AquariumCreatureType::ExtraLifePowerup)));
        break;
 }
        case AquariumCreatureType::InvincibilityPowerup: { 
        this->addCreature(std::make_shared<InvincibilityPowerUp>(x, y, this->m_sprite_manager->GetSprite(AquariumCreatureType::InvincibilityPowerup)));
        break;
}
        default:
            ofLogError() << "Unknown creature type to spawn!";
            break;
    }
    }



// repopulation will be called from the levl class
// it will compose into aquarium so eating eats frm the pool of NPCs in the lvl class
// once lvl criteria met, we move to new lvl through inner signal asking for new lvl
// which will mean incrementing the buffer and pointing to a new lvl index
void Aquarium::Repopulate() {
    ofLogVerbose("entering phase repopulation");
    // lets make the levels circular
    int selectedLevelIdx = this->currentLevel % this->m_aquariumlevels.size();
    ofLogVerbose() << "the current index: " << selectedLevelIdx << endl;
    std::shared_ptr<AquariumLevel> level = this->m_aquariumlevels.at(selectedLevelIdx);


    if(level->isCompleted()){
        level->levelReset();
        this->currentLevel += 1;
        selectedLevelIdx = this->currentLevel % this->m_aquariumlevels.size();
        ofLogNotice()<<"new level reached : " << selectedLevelIdx << std::endl;
        level = this->m_aquariumlevels.at(selectedLevelIdx);
        this->clearCreatures();
    }

    
    // now lets find how many to respawn if needed 
    std::vector<AquariumCreatureType> toRespawn = level->Repopulate();
    ofLogVerbose() << "amount to repopulate : " << toRespawn.size() << endl;
    if(toRespawn.size() <= 0 ){return;} // there is nothing for me to do here
    for(AquariumCreatureType newCreatureType : toRespawn){
        this->SpawnCreature(newCreatureType);
    }
}


// Aquarium collision detection
std::shared_ptr<GameEvent> DetectAquariumCollisions(std::shared_ptr<Aquarium> aquarium, std::shared_ptr<PlayerCreature> player) {
    if (!aquarium || !player) return nullptr;
    
    for (int i = 0; i < aquarium->getCreatureCount(); ++i) {
        std::shared_ptr<Creature> npc = aquarium->getCreatureAt(i);
        if (npc && checkCollision(player, npc)) {
            return std::make_shared<GameEvent>(GameEventType::COLLISION, player, npc);
        }
    }
    return nullptr;
};

//  Imlementation of the AquariumScene

void AquariumGameScene::Update(){
    std::shared_ptr<GameEvent> event;

    this->m_player->update();

    if (this->updateControl.tick()) {
        event = DetectAquariumCollisions(this->m_aquarium, this->m_player);
        if (event != nullptr && event->isCollisionEvent()) {
            ofLogVerbose() << "Collision detected between player and NPC!" << std::endl;
            if(event->creatureB != nullptr){
                event->print();

                if (std::dynamic_pointer_cast<SpeedPowerUp>(event->creatureB)) { 
                    ofLogNotice() << "Player collected a Speed PowerUp!";
                    
                    auto powerUp = std::static_pointer_cast<SpeedPowerUp>(event->creatureB);
                    const int BOOST_DURATION_FRAMES = 300;
                    this->m_player->increaseSpeed(powerUp->getSpeedBoostAmount(), BOOST_DURATION_FRAMES); 
                    this->m_aquarium->removeCreature(event->creatureB);
                }
                else if (std::dynamic_pointer_cast<ExtraLifePowerUp>(event->creatureB)) { 
    ofLogNotice() << "Player collected an Extra Life PowerUp!";
    this->m_player->setLives(this->m_player->getLives() + 1);
    this->m_aquarium->removeCreature(event->creatureB);
} 
else if (std::dynamic_pointer_cast<InvincibilityPowerUp>(event->creatureB)) { 
    ofLogNotice() << "Player collected an Invincibility PowerUp!";
    auto powerUp = std::static_pointer_cast<InvincibilityPowerUp>(event->creatureB);
    const int INVINCIBILITY_DURATION_FRAMES = 300;
    this->m_player->grantInvincibility(INVINCIBILITY_DURATION_FRAMES);
    this->m_aquarium->removeCreature(event->creatureB);
}
               if (std::dynamic_pointer_cast<SkeletonFish>(event->creatureB)) {
                    //Skeleton fish gimmick
                    ofLogNotice() << "Hit by SkeletonFish — player loses a life regardless of power.";
                    this->m_player->loseLife(3*60); 
                    this->m_aquarium->removeCreature(event->creatureB);
                    
                } 
                 
                //BASE FISH LOGIC, Always edible for initial points
                else if (std::dynamic_pointer_cast<NPCreature>(event->creatureB)) {
                    ofLogNotice() << "Player is eating BaseFish regardless of power for initial points." << std::endl;
                    this->m_aquarium->removeCreature(event->creatureB);
                    this->m_player->addToScore(1, event->creatureB->getValue());
                    
                    // Power increase check
                    if (this->m_player->getScore() % 25 == 0){
                        this->m_player->increasePower(1);
                        ofLogNotice() << "Player power increased to " << this->m_player->getPower() << "!" << std::endl;
                    }
                }
                //LOGIC FOR ALL NPCs
                else if(this->m_player->getPower() >= event->creatureB->getValue()){
                    // Player is strong enough = EAT
                    this->m_aquarium->removeCreature(event->creatureB);
                    this->m_player->addToScore(1, event->creatureB->getValue());
                    
                    // Power increase check
                    if (this->m_player->getScore() % 25 == 0){
                        this->m_player->increasePower(1);
                        ofLogNotice() << "Player power increased to " << this->m_player->getPower() << "!" << std::endl;
                    }
                } else {
                    // Player is too weak: Ignore collision.
                    ofLogNotice() << "Player is too weak to eat the creature! Collision ignored." << std::endl;
                }
                
                // --- Game Over Check
                if(this->m_player->getLives() <= 0){
                    this->m_lastEvent = std::make_shared<GameEvent>(GameEventType::GAME_OVER, this->m_player, nullptr);
                    return;
                }
                
            } else {
                ofLogError() << "Error: creatureB is null in collision event." << std::endl;
            }
        }
    
        this->m_aquarium->update();
    }
}

void AquariumGameScene::Draw() {
    this->m_player->draw();
    this->m_aquarium->draw();
    this->paintAquariumHUD();

}


void AquariumGameScene::paintAquariumHUD(){
    float panelWidth = ofGetWindowWidth() - 150;
    ofDrawBitmapString("Score: " + std::to_string(this->m_player->getScore()), panelWidth, 20);
    ofDrawBitmapString("Power: " + std::to_string(this->m_player->getPower()), panelWidth, 30);
    ofDrawBitmapString("Lives: " + std::to_string(this->m_player->getLives()), panelWidth, 40);
    for (int i = 0; i < this->m_player->getLives(); ++i) {
        ofSetColor(ofColor::red);
        ofDrawCircle(panelWidth + i * 20, 50, 5);
    }
    ofSetColor(ofColor::white); // Reset color to white for other drawings
}

void AquariumLevel::populationReset(){
    for(auto node: this->m_levelPopulation){
        node->currentPopulation = 0; // need to reset the population to ensure they are made a new in the next level
    }
}

void AquariumLevel::ConsumePopulation(AquariumCreatureType creatureType, int power){
    for(std::shared_ptr<AquariumLevelPopulationNode> node: this->m_levelPopulation){
        ofLogVerbose() << "consuming from this level creatures" << endl;
        if(node->creatureType == creatureType){
            ofLogVerbose() << "-cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            if(node->currentPopulation == 0){
                return;
            } 
            node->currentPopulation -= 1;
            ofLogVerbose() << "+cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            this->m_level_score += power;
            return;
        }
    }
}

bool AquariumLevel::isCompleted(){
    return this->m_level_score >= this->m_targetScore;
}

std::vector<AquariumCreatureType> AquariumLevel::Repopulate() {
    std::vector<AquariumCreatureType> toRepopulate;
    for (std::shared_ptr<AquariumLevelPopulationNode> node : this->m_levelPopulation) {
        // Calculate how many creatures of this type need to be added
        int delta = node->population - node->currentPopulation;
        
        if (delta > 0) {
            // Add the required number of creatures to the list
            for (int i = 0; i < delta; i++) {
                toRepopulate.push_back(node->creatureType);
            }
            // Update the current population count for this node
            node->currentPopulation += delta;
        }
    }
    return toRepopulate;
}



