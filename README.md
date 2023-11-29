# Asset credits

| Asset name        | Type      | Source                            | Link                                                                                                  |
|-------------------|-----------|-----------------------------------|-------------------------------------------------------                                                |
| missile_fire.wav  | Audio     | freesound.org user MATRIXXX_      | https://freesound.org/people/MATRIXXX_/sounds/441373/                                                 |
| bullet_fire.wav   | Audio     | freesound.org user MATRIXXX_      | https://freesound.org/people/MATRIXXX_/sounds/414885/                                                 |
| ducky.obj         | Model     | cgtrader.com user CGStuffStudios  | https://www.cgtrader.com/free-3d-models/sports/toy/rubber-duck-b31f3585-0347-4532-bd92-7ddea6107d0d   |
| ducky_albedo.png  | Texture   | cgtrader.com user CGStuffStudios  | https://www.cgtrader.com/free-3d-models/sports/toy/rubber-duck-b31f3585-0347-4532-bd92-7ddea6107d0d   |
| space_wham.ttf    | Font      | dafont.com user Zephram           | https://www.dafont.com/space-wham.font                                                                |


# TODO

## MUST
- Implement more than one powerup type and have them randomly spawn
- Add sound effect when a projectile hits
- Add engine SFX
- Improve lock-on UI
- Make train component selection UI vertical
- Add train models
- Add turret models
- Make turret models rotate to face the direction the camera faces
- Improve enemy AI and shooting behaviour
- Comments in all .h files
- Organise code
- Improve "Game Over" screen

## SHOULD
- Limit rotation of the engine based on the car behind it
- Limit lock-on by distance
- ICombatant should be virtual/abstract in the same way IUpdatable is
- Add some more rays for target detection so it's less finicky
- Add UI to show when a target can be locked
- Make repeat fire work if the fire button is held down
- Update selected component UI so it's less confusing
- Create IMouseListener so things can take mouse input properly
    - Update FollowCam to implement IMouseListener

## COULD
- Add UI to show damage being dealt to enemies
- Add UI to show damage being dealt to player
- Add a health bar
- Add a menu system
- Implement an UpdateManager

## WON'T
- Update target display UI to show a visual representation of the current target
- Add DR and DT to Hostiles
- Add DR and DT to the train
