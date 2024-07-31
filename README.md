# Raycasting Engine

## Overview
We developed a raycasting engine from scratch using C and SDL, inspired by *Wolfenstein 3D*, featuring 3D projection from 2D space with depth perception created using similar triangles.

<img src="https://i.imgur.com/87N5cS4.png" alt="In-Engine Capture" width="500">


## Minimap
<img src="https://i.imgur.com/nbpZimX.png" alt="Minimap" width="300">
The minimap shows the player's current location (white square) and their field of view, determined by raycasting (red area). It displays wall boundaries and sprites are represented by the smaller yellow boxes. The map updates in real-time.

## 3D Projection
Rays are cast from the player's current position, and each pixel column of the wall sprite is adjusted to create depth. We utilized similar triangles to create depth when rendering the walls. The visual below helps showcase the math used in this process.
<div style="text-align: center;">
    <img src="https://i.imgur.com/yAaXfM2.png" alt="Similar Triangles Example" width="500">
</div>
Rendering sprites follows a similar process, except that sprites maintain a consistent height across since they don't have depth. The distance from the player to each sprite ensures the sprites are rendered in the correct order. Additionally, sprites are only rendered when they're in the player's FOV.

## Conclusion
This project showcases our ability to create complex graphical systems from scratch, demonstrating proficiency in SDL, C programming, and mathematical concepts for game development. The raycasting engine not only highlights our technical skills but also our ability to work collaboratively on challenging tasks. This experience has further solidified our understanding of 3D rendering and game engine mechanics, preparing us for more advanced projects in the future.
