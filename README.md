# Asset credits

| Asset name                | Type      | Source                            | Link                                                                                                  |
|---------------------------|-----------|-----------------------------------|-------------------------------------------------------------------------------------------------------|
| bullet_fire.wav           | Audio     | freesound.org user MATRIXXX_      | https://freesound.org/people/MATRIXXX_/sounds/414885/                                                 |
| engine.wav                | Audio     | freesound.org user starkadr       | https://freesound.org/people/Starkadr/sounds/641194/                                                  |
| explosion.wav             | Audio     | freesound.org user V-ktor         | https://freesound.org/people/V-ktor/sounds/435413/                                                    |
| laser_hit.wav             | Audio     | freesound.org user lavik89        | https://freesound.org/people/lavik89/sounds/168984/                                                   |
| missile_fire.wav          | Audio     | freesound.org user MATRIXXX_      | https://freesound.org/people/MATRIXXX_/sounds/441373/                                                 |
| ducky.obj                 | Model     | cgtrader.com user CGStuffStudios  | https://www.cgtrader.com/free-3d-models/sports/toy/rubber-duck-b31f3585-0347-4532-bd92-7ddea6107d0d   |
| train_engine.obj          | Model     | cgtrader.com user PixelYards      | https://www.cgtrader.com/3d-models/vehicle/train/historic-steam-train                                 |
| ufo.obj                   | Model     | Turbosquid user JonLundy3D        | https://www.turbosquid.com/3d-models/free-3ds-model-flying-saucer/1081073                             |
| wagon.obj                 | Model     | Turbosquid user mieszas           | https://www.turbosquid.com/3d-models/3d-wooden-train-cars-model/1066200                               |
| ducky_albedo.png          | Texture   | cgtrader.com user CGStuffStudios  | https://www.cgtrader.com/free-3d-models/sports/toy/rubber-duck-b31f3585-0347-4532-bd92-7ddea6107d0d   |
| train_engine_specular.png | Texture   | cgtrader.com user PixelYards      | https://www.cgtrader.com/3d-models/vehicle/train/historic-steam-train                                 |
| ufo.obj                   | Texture   | Turbosquid user JonLundy3D        | https://www.turbosquid.com/3d-models/free-3ds-model-flying-saucer/1081073                             |
| wagon_albedo.obj          | Texture   | Turbosquid user mieszas           | https://www.turbosquid.com/3d-models/3d-wooden-train-cars-model/1066200                               |
| ds_digi.ttf               | Font      | dafont.com user Dusit Supasawat   | https://www.dafont.com/ds-digital.font                                                                |
| space_wham.ttf            | Font      | dafont.com user Zephram           | https://www.dafont.com/space-wham.font                                                                |
| shift_key_icon.png        | Image     | flaticon.com                      | https://www.flaticon.com/free-icons/shift                                                             |


# TODO

## MUST
- Improve enemy AI and shooting behaviour
    - Hostiles either shouldn't get too far away or there should be an indicator showing where they are
- Fix train acceleration & rotation
- Add multiple hostiles
- Comments in all .h files
- Organise code

## SHOULD
- Update the target lock UI to be drawn in the same way the game over screen is
- Round the speed shown in the UI to some sensible number
- Resize textures
- Play a sound effect when the player collects a powerup
- Make train component selection UI vertical
- Make a diffuse map for the train engine model
- Limit rotation of the engine based on the car behind it
- Limit lock-on by distance
- ICombatant should be virtual/abstract in the same way IUpdatable is
- Make repeat fire work if the fire button is held down
- Update selected component UI so it's less confusing
- Create IMouseListener so things can take mouse input properly
    - Update FollowCam to implement IMouseListener
- Add turret models
- Make turret models rotate to face the direction the camera faces

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
