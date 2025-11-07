# Tempest in Unreal Engine

[DEMO VIDEO](https://youtu.be/u4kn-k1czP4)

## Overview

Tempest is a game developed by [Daniel Gerhardt](https://github.com/danieljgerhardt) and [Amy Liu](https://github.com/mialana). It was created in UE5 in a little under a month, and features usage of the Unreal Gameplay Ability System and Niagara Particle Simulation to build game systems and mechanics.

The game is an arena brawler. The player faces repeated waves of enemies and is tasked with beating the enemies and surviving.

The player has a basic sword attack, executed with left click, which requires no resources. Moreover, the rest of their [Gameplay Abilities](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-gameplay-abilities-in-unreal-engine) -- `WaterGun`, `Dash`, `Blast`, `Pulse`, and `Hurricane` -- take mana. Mana is restored by standing in water regions of the map. Manifested as physical actors in the level scene and referencing the [`UGameplayEffect`](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-effects-for-the-gameplay-ability-system-in-unreal-engine) class, these regions will be used up by standing on them for ten seconds, and return fifteen seconds after being used up. Abilities are indicated in the bottom left with their key, and on use will trigger a cooldown period, also visually show in the UI. The water gun and hurricane ability are held abilities that require holding the key and aiming for proper use. The enemies feature their own set of abilities, namely `FireSpray` and `Fireball`.

## Credits
- Character animations were created by exporting the `Mannequin` skeleton from Unreal, then importing into [Mixamo](https://www.mixamo.com/).
- **Skeleton Mesh** assets for player and enemies were created by 1. Generating mesh and textures on [MeshyAI](https://www.meshy.ai/) and 2. Importing into [Autodesk Maya](https://www.autodesk.com/products/maya/free-trial) alongside the `Mannequin` skeleton. We then manually manipulated the skeleton to fit the custom character's body proportions, bound skin weights to the joints, and exported as FBX back into Unreal.
