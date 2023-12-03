#include "CombatManager.h"

static AssetManager& _assets = AssetManager::getInstance();
static AudioManager& _audio = AudioManager::getInstance();
static GameStateManager& _gameState = GameStateManager::getInstance();
static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

CombatManager::CombatManager(FollowCam* camera, Train* train, ScoreManager* scoreMan) {
    this->camera = camera;
    this->train = train;
    this->scoreManager = scoreMan;
    this->camera->parent = this->train->head();

    this->projectiles[train] = std::vector<Projectile*>();

    this->populateHostileTypes();
    this->populatePowerupTypes();
}

Train* CombatManager::getTrain() {
    return this->train;
}

void CombatManager::setTarget(Hostile* newTarget) {
    this->activeTarget = newTarget;
}

void CombatManager::unsetTarget() {
    this->activeTarget = nullptr;
}

// TODO: Not sure what an inline function is but this seems like a candidate...?
bool CombatManager::hasTarget() {
    return this->activeTarget != nullptr;
}

void CombatManager::addHostile(Hostile* hostile) {
    this->hostiles.push_back(hostile);
}

Hostile* CombatManager::getActiveTarget() {
    return this->activeTarget;
}

bool CombatManager::getTargetLocked() {
    return this->targetLocked;
}

Vector3 CombatManager::calculateNormalizedTargetLocationVector() {
    /**
     * 1. Get the position of the camera, minus the vertical component
     * 2. Subtract the position of the Train from the position calculated in step 1
     *      This gives you a vector facing the same direction as the camera
     * 3. Normalize and return
    */
    Vector3 groundedCameraPos = {this->camera->camera.position.x, 0.f, this->camera->camera.position.z};
    return Vector3Normalize(Vector3Subtract(this->train->head()->position, groundedCameraPos));
}

Ray CombatManager::getTargetingRay() {
    /**
     * Since the game is played on a 2D plane we only need rays to be cast at a height
     * which intersects the enemies, so the Y coordinate is hard-coded at 5.f
    */
    return {{this->train->head()->position.x, 5.f, this->train->head()->position.z}, this->calculateNormalizedTargetLocationVector()};
}

std::vector<Ray> CombatManager::getTargetingRays(int count, float spacing) {
    std::vector<Ray> rays(count);
    rays[0] = this->getTargetingRay();
    Vector3 right = Vector3CrossProduct(rays[0].direction, {0.f, 1.f, 0.f});

    for (int i = 1; i < count; i++ ) {
        if (i % 2 == 0) {
            rays[i] = {Vector3Add(rays[0].position, Vector3Scale(right, (i * spacing))), rays[0].direction};
        } else {
            rays[i] = {Vector3Subtract(rays[0].position, Vector3Scale(right, (i * spacing))), rays[0].direction};
        }
    }

    return rays;
}

int CombatManager::getPlayerHealth() {
    return this->train->getHealth();
}

void CombatManager::spawnHostile() {
    // Spawn the new hostile 200 units away from the player in a random-ish direction
    bool positiveX = GetRandomValue(0, 10) % 2 == 0;
    bool positiveY = GetRandomValue(0, 10) % 2 == 0;
    Vector3 newHostilePos = {positiveX ? 200.f : -200.f, 0.f, positiveY ? 200.f : -200.f};

    // Pick a random enemy type to spawn
    Hostile* h = new Hostile(newHostilePos, &this->train->head()->position, &this->hostileTypes[GetRandomValue(0, this->hostileTypes.size() - 1)]);

    this->hostiles.push_back(h);
    this->projectiles[h] = std::vector<Projectile*>();
}

void CombatManager::spawnPowerup(Vector3 pos) {
    int powerUpIndex = GetRandomValue(0, this->powerupTypes.size() - 1 );

    Powerup* p = new Powerup(
        pos,
        *this->powerupTypes[powerUpIndex].getModel(),
        *this->powerupTypes[powerUpIndex].getTexture(),
        this->powerupTypes[powerUpIndex].getType(),
        this->powerupTypes[powerUpIndex].getMagnitude()
    );

    this->powerups.push_back(p);
}

void CombatManager::update(float deltaTime) {
    if (this->hostiles.size() < 3) {
        this->spawnHostile();
    }

    // Update hostiles. This isn't a typical 'for' loop because hostiles are stored on the heap
    // and therefore we're responsible for deleting them. If we delete them without removing them
    // from the hostiles vector, we'll crash. If we remove something from the projectiles vector
    // in a for loop, we'll skip an element. Therefore we use a while loop instead
    // This form of loop sourced from StackOverflow: https://stackoverflow.com/a/13102374
    std::vector<Hostile*>::iterator hostileIt = this->hostiles.begin();

    while (hostileIt != this->hostiles.end()) {

        // Update any projectiles heading for this hostile
        std::vector<Projectile*>::iterator projectileIt = this->projectiles[*hostileIt].begin();

        while (projectileIt != this->projectiles[*hostileIt].end()) {
            // Update the projectile
            dynamic_cast<IUpdatable*>(*projectileIt)->update(deltaTime);

            // If the projectile is not alive after updating, delete it from heap and vector
            if (!(*projectileIt)->isAlive()) {
                // Assign damage
                (*hostileIt)->receiveDamage((*projectileIt)->getDamage());
                _audio.play((*projectileIt)->getdestroySFX());
                delete (*projectileIt);
                projectileIt = this->projectiles[*hostileIt].erase(projectileIt);
            }
            else {
                ++projectileIt;
            }
        }

        // If the Hostile is not alive, delete it from heap and vector
        if (!(*hostileIt)->isAlive()) {
            // TODO: Unsetting target needs a general cleanup
            this->targetLocked = false;
            this->camera->unsetTarget();
            this->camera->parent = this->train->resetActiveComponent();
            this->camera->resetmouseRotationAdjustment();

            this->spawnPowerup((*hostileIt)->position);

            delete (*hostileIt);
            hostileIt = this->hostiles.erase(hostileIt);

            // Update player score
            this->scoreManager->Add(1);
        }
        else {
            (*hostileIt)->update(deltaTime);
            if ((*hostileIt)->canShoot()) {
                Projectile* firedProjectile = (*hostileIt)->shoot(&this->train->head()->position);
                _audio.play(firedProjectile->getlaunchSFX());
                this->projectiles[this->train].push_back(firedProjectile);
            }
            ++hostileIt;
        }
    }

    // Update any projectiles targeted at the player
    std::vector<Projectile*>::iterator it = this->projectiles[this->train].begin();

    while (it != this->projectiles[this->train].end()) {
        // Update the projectile
        (*it)->update(deltaTime);

        // If the projectile is not alive after updating, delete it from heap and vector
        if (!(*it)->isAlive()) {
            // Assign damage
            this->train->receiveDamage((*it)->getDamage());
            _audio.play((*it)->getdestroySFX());
            delete (*it);
            it = this->projectiles[this->train].erase(it);
        }
        else {
            ++it;
        }
    }
    
    // Check for collision with powerups
    std::vector<Powerup*>::iterator powerUpIt = this->powerups.begin();

    while (powerUpIt != this->powerups.end()) {
        // Update the powerup
        (*powerUpIt)->update(deltaTime);

        // Check if the powerup is colliding with the player

        if (CheckCollisionBoxes(this->train->head()->getBounds(), (*powerUpIt)->getBounds())) {
            (*powerUpIt)->setIsAlive(false);
        }

        // If the powerup is not alive after updating, give the train its bonus and then delete it from the heap
        if (!(*powerUpIt)->getIsAlive()) {
            this->train->receivePowerup(*powerUpIt);
            delete (*powerUpIt);
            powerUpIt = this->powerups.erase(powerUpIt);
        }
        else {
            ++powerUpIt;
        }
    }

    // If the player is dead, set the game state to GameOVer
    if ( !train->isAlive() ) {
        _gameState.setState(GameState::GameOver);
    }

    std::vector<Ray> targetRays = this->getTargetingRays(7, .8f);

    // Don't bother scanning for new targets if there's already a locked target
    if (!targetLocked) {

        std::vector<Ray>::iterator rayIt = targetRays.begin();

        Hostile* hit = nullptr;

        // Iterate through all rtargeting rays to check for collisions
        while (rayIt != targetRays.end() && hit == nullptr) {
            // Iterate through all hostiles and check if the targeting ray is hitting one of them
            for (Hostile* hostile : this->hostiles ) {
                RayCollision coll = GetRayCollisionBox((*rayIt), hostile->getBounds());

                if (coll.hit) {
                    hit = hostile;
                }
            }
            ++rayIt;
        }

        if (hit == nullptr) {
            this->unsetTarget();
        } else {
            this->setTarget(hit);
        }
    }
}

void CombatManager::draw() {

    // Draw each hostile and any projectiles targeting them
    for (std::vector<Hostile*>::iterator hostileIt = this->hostiles.begin(); hostileIt != this->hostiles.end(); ++hostileIt) {
        (*hostileIt)->draw();

        for (std::vector<Projectile*>::iterator projectileIt = this->projectiles[(*hostileIt)].begin(); projectileIt != this->projectiles[(*hostileIt)].end(); ++projectileIt) {
            (*projectileIt)->draw();
        }
    }

    // Draw any projectiles targeted at the player
    for (std::vector<Projectile*>::iterator it = this->projectiles[this->train].begin(); it != this->projectiles[this->train].end(); ++it) {
        (*it)->draw();
    }

    // Draw powerups
    for (std::vector<Powerup*>::iterator it = this->powerups.begin(); it != this->powerups.end(); ++it) {
        (*it)->draw();
    }
    
    // Everything below here should only be drawn if debug is enabled
    if (!_debug.getDrawBoundingBoxes()) {
        return;
    }

    std::vector<Ray> targetRays = this->getTargetingRays(7, .8f);

    for (std::vector<Ray>::iterator it = targetRays.begin(); it != targetRays.end(); ++it) {
        DrawLine3D(it->position, Vector3Add(it->position, Vector3Scale(it->direction, 300.f)), this->hasTarget() ? RED : GREEN);
    }
}

void CombatManager::onKeyPressed(int key) {
    if (key == KEY_LEFT_SHIFT) {
        if (this->hasTarget()) {
            this->targetLocked = !this->targetLocked;

            if (this->targetLocked) {
                this->camera->setTarget(this->getActiveTarget());
            } else {
                this->camera->unsetTarget();
                this->camera->parent = this->train->resetActiveComponent();
            }
        }
    } else if (key == KEY_SPACE) {
        if (this->hasTarget() && this->targetLocked) {
            if (!this->train->canShoot()) return;

            Projectile* firedProjectile = this->train->shoot(&this->getActiveTarget()->position);
            _audio.play(firedProjectile->getlaunchSFX());
            this->projectiles[this->getActiveTarget()].push_back(firedProjectile);
        }
    } else if (key == KEY_UP) {
        if (this->targetLocked) {
            this->camera->parent = this->train->incrementActiveComponent();
        }
    } else if (key == KEY_DOWN) {
        if (this->targetLocked) {
            this->camera->parent = this->train->decrementActiveComponent();
        }
    }
}

void CombatManager::onKeyReleased(int key) { }


// =======================================================
// Convenience functions. Really just exctracting these
// for code readability
// =======================================================

void CombatManager::populateHostileTypes() {

    // Define the different Hostile types the CombatManager can spawn

    // This one is slow & strong. High damage, high reload time
    Hostile bomber(
        {0.f, 0.f, 0.f},                // Position
        _assets.getModel("ufo"),        // Model
        _assets.getTexture("ufo"),      // Texture
        &this->train->head()->position, // Target
        60.f,                           // Min engagement distance
        80.f,                           // Max engagement distance
        20.f,                           // Max speed
        20,                             // Power (damage dealt on hit)
        5000,                           // ReloadTime (milliseconds)
        80,                             // Max hitpoints
        _assets.getModel("missile"),    // Projectile model
        _assets.getTexture("missile")   // Projectile texture
    );

    // This one is fast & weak. Gets in and peppers the player with shots
    Hostile scout(
        {0.f, 0.f, 0.f},                // Position
        _assets.getModel("ufo"),        // Model
        _assets.getTexture("ufo"),      // Texture
        &this->train->head()->position, // Target
        20.f,                           // Min engagement distance
        40.f,                           // Max engagement distance
        80.f,                           // Max speed
        2,                              // Power (damage dealt on hit)
        1000,                           // ReloadTime (milliseconds)
        25,                             // Max hitpoints
        _assets.getModel("bullet"),     // Projectile model
        _assets.getTexture("bullet")    // Projectile texture
    );

    // This one is middle of the road
    Hostile interceptor(
        {0.f, 0.f, 0.f},                // Position
        _assets.getModel("ufo"),        // Model
        _assets.getTexture("ufo"),      // Texture
        &this->train->head()->position, // Target
        40.f,                           // Min engagement distance
        80.f,                           // Max engagement distance
        35.f,                           // Max speed
        10,                              // Power (damage dealt on hit)
        2000,                           // ReloadTime (milliseconds)
        100,                             // Max hitpoints
        _assets.getModel("missile"),    // Projectile model
        _assets.getTexture("missile")   // Projectile texture
    );

    this->hostileTypes.push_back(bomber);
    this->hostileTypes.push_back(scout);
    this->hostileTypes.push_back(interceptor);
}

void CombatManager::populatePowerupTypes() {

    // Define the different Powerups the CombatManager can spawn
    Powerup healthPackSmall(
        {0.f, 0.f, 0.f},
        _assets.getModel("health_powerup"),
        _assets.getTexture("health_powerup"),
        PowerupType::HealthPack,
        10
    );

    Powerup healthPackMedium(
        {0.f, 0.f, 0.f},
        _assets.getModel("health_powerup"),
        _assets.getTexture("health_powerup"),
        PowerupType::HealthPack,
        25
    );

    Powerup healthPackLarge(
        {0.f, 0.f, 0.f},
        _assets.getModel("health_powerup"),
        _assets.getTexture("health_powerup"),
        PowerupType::HealthPack,
        50
    );

    Powerup speedBoost(
        {0.f, 0.f, 0.f},
        _assets.getModel("speed_powerup"),
        _assets.getTexture("speed_powerup"),
        PowerupType::SpeedBoost,
        0.1f
    );

    this->powerupTypes.push_back(healthPackSmall);
    this->powerupTypes.push_back(healthPackMedium);
    this->powerupTypes.push_back(healthPackLarge);
    this->powerupTypes.push_back(speedBoost);
}

void CombatManager::populateTraincarTypes() {
    TrainCar missileCar(
        _assets.getModel("train_wagon"),        // Model
        _assets.getTexture("wagon"),            // Texture
        nullptr,                                // Pointer to "parent" TrainComponent
        3.f,                                    // Follow distance - how close this component gets to its parent
        20,                                     // Power - How much damage this TrainCar deals
        3000,                                   // ReloadTime (milliseconds)
        _assets.getModel("missile"),            // Model used by projectiles fired by this TrainCar
        _assets.getTexture("missile"),          // Texture used by projectiles fired by this TrainCar
        "missile_fire",                         // Name of the sound to play when this TrainCar fires a projectile
        "explosion"                             // Name of the sound to play when a projectile fired by this TrainCar is destroyed
    );

    TrainCar laserCar(
        _assets.getModel("train_wagon"),        // Model
        _assets.getTexture("wagon"),            // Texture
        nullptr,                                // Pointer to "parent" TrainComponent
        7.5f,                                   // Follow distance - how close this component gets to its parent
        5,                                      // Power - How much damage this TrainCar deals
        200,                                    // ReloadTime (milliseconds)
        _assets.getModel("bullet"),             // Model used by projectiles fired by this TrainCar
        _assets.getTexture("bullet"),           // Texture used by projectiles fired by this TrainCar
        "bullet_fire",                          // Name of the sound to play when this TrainCar fires a projectile
        "laser_hit"                             // Name of the sound to play when a projectile fired by this TrainCar is destroyed
    );

    trainCarTypes["missile_car"] = missileCar;
    trainCarTypes["laser_car"] = laserCar;
}