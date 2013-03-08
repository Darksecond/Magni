Keuzes
======

Voor deze opdracht is er gekozen om zelf een game engine te ontwikkelen.
Hierbij zijn veel keuzes gemaakt. In dit hoofdstuk zullen de verschillende keuzes worden uitgelegd.

Klassenstructuur & Objectmodel
------------------------------

Voor dit project is er gekozen voor een Entity Component System, met aanpassingen.
Dit betekend dat het merendeel van de data in components zit.
Hierbij is de aanpassing gemaakt dat er naast Engines ook Behaviors bestaan.

Engines verwerken grote hoeveelheden data. De renderer is bijvoorbeeld een engine.
Behaviors verwerken data voor één entity.

Een entity bestaat uit meerdere behaviors en componenten. Hierdoor is zijn entities ook heel flexibel en is functionaliteit makkelijk toe te voegen.

In het klassen diagram is de basis van het object model te zien. Hierbij zijn de meeste klassen weggelaten omdat dat hulp klassen zijn.

Resource Management
-------------------

De engine bevat een uitgebreid resource management systeem. Deze is flexibel en makkelijk aan te passen.
Op dit moment leest het systeem gewone bestanden uit mappen uit, maar het is zeer goed mogelijk het systeem aan te passen zodat het resource files gebruikt (bijvoorbeeld ZIP files).

Render pipeline
--------------

De render pipeline bestaat uit een aantal verschillende stappen.

De eerste stap is het renderen van de skybox. Hierbij wordt gebruik gemaakt van een cubemap en een speciale shader.

De tweede stap is het renderen van de belichting op de objecten. Hierbij word opnieuw een speciale shader gebruikt.
Voor het belichtings model is Phong gekozen.

De derde stap is het renderen van de textures op de objecten. Hierbij word met behulp van blending de belichting meegenomen.

De vierde en laatste stap is het renderen van de overlays. Dit is bijvoorbeeld de text.

Audio
-----

Voor audio wordt er gebruik gemaakt van OpenAL en freeALUT. Geluiden worden opgeslagen in wav formaat.
Door het gebruik van OpenAL is positionele audio mogelijk.

Textures
--------

Voor het laden wordt stb_image gebruikt. Dit is een public domain image loading library. Deze maakt het mogelijk om PNG, JPG, TGA, BMP en andere formaten te gebruiken.
