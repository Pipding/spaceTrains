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
    // TODO: Clean up this function

    // How to work out the vector for detecting targets;
    // Grounded position of camera
    // Subtract position of player from above
    // Normalize

    // Get the vector from the camera to its target
    Vector3 groundedCameraPos = {this->camera->camera.position.x, 0.f, this->camera->camera.position.z};

    Vector3 normalizedTargetingVector = Vector3Normalize(Vector3Subtract(this->train->head()->position, groundedCameraPos));

    return normalizedTargetingVector;
}

Ray CombatManager::getTargetingRay() {
    // The ray needs to be a little bit off the ground. If the Y position
    // is 0, the ray doesn't intersect with bounding boxes which are 
    // on the ground. Hence 0.5f
    return {{this->train->head()->position.x, 2.f, this->train->head()->position.z}, this->calculateNormalizedTargetLocationVector()};
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
    // TODO: When there's more than 1 hostile, maybe randomize the type of hostile spawned

    Vector3 newHostilePos = {100.f, 0.f, 100.f};

    Hostile* h = new Hostile(
        newHostilePos,
        *this->hostileTypes[0].getModel(),
        *this->hostileTypes[0].getTexture(),
        &this->train->head()->position,
        this->hostileTypes[0].getMinEngagementDistance(),
        this->hostileTypes[0].getMaxEngagementDistance(),
        this->hostileTypes[0].getMaxSpeed(),
        this->hostileTypes[0].getProjectileModel(),
        this->hostileTypes[0].getProjectileTexture()
    );

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
    if (this->hostiles.size() == 0) {
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

    // Ray targetRay = this->getTargetingRay();
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

    DrawLine3D(this->getTargetingRay().position, Vector3Add(this->getTargetingRay().position, Vector3Scale(this->getTargetingRay().direction, 300.f)), this->hasTarget() ? RED : GREEN);

    std::vector<Ray> targetRays = this->getTargetingRays(7, 8.f);

    for (std::vector<Ray>::iterator it = targetRays.begin(); it != targetRays.end(); ++it) {
        DrawLine3D(it->position, Vector3Add(it->position, Vector3Scale(it->direction, 300.f)), BLUE);
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
    Hostile hostile(
        {0.f, 0.f, 0.f},
        _assets.getModel("ufo"),
        _assets.getTexture("ufo"),
        &this->train->head()->position,
        20.f,
        40.f,
        15.f,
        _assets.getModel("missile"),
        _assets.getTexture("missile")
    );

    this->hostileTypes.push_back(hostile);
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
        10
    );

    this->powerupTypes.push_back(healthPackSmall);
    this->powerupTypes.push_back(healthPackMedium);
    this->powerupTypes.push_back(healthPackLarge);
    this->powerupTypes.push_back(speedBoost);
}