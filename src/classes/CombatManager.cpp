#include "CombatManager.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();
static AssetManager& _assets = AssetManager::getInstance();

CombatManager::CombatManager(FollowCam* camera, Train* train) {
    this->camera = camera;
    this->train = train;
    this->camera->parent = this->train->head();

    // Define the different Hostile types the CombatManager can spawn
    Hostile hostile(
        {0.f, 0.f, 0.f},
        _assets.getModel("duck"),
        _assets.getTexture("duck"),
        &this->train->head()->position,
        200.f,
        1000.f,
        300.f
    );

    this->hostileTypes.push_back(hostile);
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
    return {{this->train->head()->position.x, 0.5f, this->train->head()->position.z}, this->calculateNormalizedTargetLocationVector()};
}

int CombatManager::getPlayerHealth() {
    return this->train->getHealth();
}

void CombatManager::spawnHostile() {
    // TODO: When there's more than 1 hostile, maybe randomize the type of hostile spawned

    Vector3 newHostilePos = {1000.f, 0.f, 1000.f};

    Hostile* h = new Hostile(
        newHostilePos,
        *this->hostileTypes[0].getModel(),
        *this->hostileTypes[0].getTexture(),
        &this->train->head()->position,
        this->hostileTypes[0].getMinEngagementDistance(),
        this->hostileTypes[0].getMaxEngagementDistance(),
        this->hostileTypes[0].getMaxSpeed()
    );

    this->hostiles.push_back(h);
}

void CombatManager::update(float deltaTime) {
    if (this->hostiles.size() == 0) {
        this->spawnHostile();
    }

    // Update each hostile
    for (std::vector<Hostile*>::iterator it = this->hostiles.begin(); it != this->hostiles.end(); ++it) {
        dynamic_cast<IUpdatable*>(*it)->update(deltaTime);
    }

    Ray targetRay = this->getTargetingRay();

    // Don't bother scanning for new targets if there's already a locked target
    if (!targetLocked) {
        // Iterate through all hostiles and check if the targeting ray is hitting one of them
        for (Hostile* hostile : this->hostiles ) {
            RayCollision coll = GetRayCollisionBox(targetRay, hostile->getBounds());

            if (coll.hit) {
                this->setTarget(hostile);
            } else {
                this->unsetTarget();
            }
        }
    }
}

void CombatManager::draw() {

    // Draw each hostile
    for (std::vector<Hostile*>::iterator it = this->hostiles.begin(); it != this->hostiles.end(); ++it) {
        (*it)->draw();
    }

    // Everything below here should only be drawn if debug is enabled
    if (!_debug.getDrawBoundingBoxes()) {
        return;
    }

    DrawLine3D(this->getTargetingRay().position, Vector3Add(this->getTargetingRay().position, Vector3Scale(this->getTargetingRay().direction, 300.f)), this->hasTarget() ? RED : GREEN);
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

            int damageDealt = this->train->shoot(&this->getActiveTarget()->position);
            
            this->getActiveTarget()->receiveDamage(damageDealt);
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