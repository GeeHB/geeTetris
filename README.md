## *geeTetris* - Un jeu de tetris pour la Casio GRAPH90+E / FX-CG50

### Présentation

`geeTetris` est un jeu de tetris pour la calculatrice Casio Graph90+E développé en C++ sur `gint`. Il ne nécessite aucune librairie spécifique supplémentaires à l'exception de -lstdc++.

Il offre différentes options : affichage de l'ombre de la pièce, démarrage avec un handicap de lignes 'sales', démarrage sur un niveau plus rapide. Et il permet de jouer avec la calculatrice en mode horizontal (position standard) ou en mode vertical.


### Utilisation du clavier

Quel que soit le mode d'affichage choisi, vertical ou horizontal, les touches suivantes fonctionneront à l'identique à tout moment de la partie :

| Touche | Action |  
| --------------- |----------------|
| ![Rotation](assets/key_xot.png) | Changement de mode d'affichage. Par défaut l'affichage est en mode horizontal (calculatrice en mode normal), un appui transformera instantanément les affichages du jeu vers le mode vertical. Et réciproquement suite à un second appui.|
| ![Exit](assets/key_exit.png) | Sortie du jeu |


#### Contrôles en mode horizontal

| Touche | Action |  
| --------------- |----------------|
| ![Gauche](assets/key_left.png) | Déplacement de la pièce vers la gauche |
| ![Droite](assets/key_right.png) | Déplacement de la pièce vers la droite |
| ![Bas](assets/key_down.png) | Descente de la pièce d'un rang |
| ![Rotation](assets/key_up.png) | Rotation de la pièce |
| ![Descente](assets/key_0.png) | Faire tomber la pièce le plus bas possible |

#### Contrôles en mode vertical

Les contrôles "suivent" le mouvement de la calculatrice :

| Touche | Action |  
| --------------- |----------------|
| ![Gauche](assets/key_down.png) | Déplacement de la pièce vers la gauche |
| ![Droite](assets/key_up.png) | Déplacement de la pièce vers la droite |
| ![Bas](assets/key_right.png) | Descente de la pièce d'un rang |
| ![Rotation](assets/key_left.png) | Rotation de la pièce |
| ![Descente](assets/key_0.png) | Faire tomber la pièce le plus bas possible |

### Captures

Ecran d'accueil de `geeTetris` :
![Accueil](assets/welcome.png)

Choix du nombre de ligne 'sales' à ajouter en début de partie :

![Dirty lines](assets/dirty.png)

Jeu en mode normal :

![Horz](assets/horz.png)

... la même partie continue en affichage vertical :
![Vertical](assets/vert.png)