/* Start Header ------------------------------------------------------

File Name:      Readme.txt

Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  13/Dec/2016
- End Header -------------------------------------------------------*/

keyboard controls:
 w : move active object forward
 s : move active objectbackward
 a : move the active object left
 d : move the active object right 
 q : move active object down
 e : move active object up
 r : rotate active object left
 t : rotate active object right



 Antweak bar controls for:
 - toggling wireframe mode
 - Toggling heatmap/ phong rendering mode
 - switch actively controlled object to the camera
 - switch actively controlled object to object 1
 - switch actively controlled object to object 2

HeatMap Guide:
blue color : closest opposing triangle is in front of the vertex.
red color : closest opposing triangle is behind the vertex.
green color: closest opposing triangle is very close to the vertex.
Black : No opposing collision surface was found
Color Intensity is used to gauge the distance from the opposing surfrace, the brighter the color, the further away it is for blue and red
