#### v0.4
* xxx nov. 2023
* Ajouts
    * gestion des scores
        * `src/sList` : liste dynamique pour les scores
    * mode TRACE via `src/shared/trace.h`
        * messages sur l'écran
        * Accès à la capture d'écran (`shift` `7`)        
    * classes partagées:
        * `src/shared/bFile` : Accès au FS
        * `src/shared/window` : 'fenêtre' 
* Corrections
    * [x] BUG : Mauvais calcul de l'accélération des pieces
    * [ ] BUG : La vitesse initiale ne tient pas compte du niveau de départ
    * [x] BUG : score valide même si 'exit' 
        - Simplification des statut des parties
    * [x] BUG : Sortie du jeu => retour à la police par défaut
    * [x] BUG : `src/shared/keyboard`` : Pas de gestion des "modifiers" du clavier

#### v0.3 - version intermédiaire (test et validation des nouvelles classes)
* 21 nov. 2023
* Ajouts
    * Réorganisation des sources
        * dossier `src/shared` pour les classes portables
            * `src/shared/keyboard` : Accès au clavier
            * `src/shared/scrCapture` : capture via fxlink ( intégration du src de **Lephenixnoir**)
        * `tabValue` : Fonctionnement avec un pointeur vers la valeur à modifier (plutôt qu'une copie)
        * `tetrisGame` : ne gère que le jeu et plus les affichages
    * `playArea` : Couche d'abstraction à l'écran
        * centralise tous les affichages
        * permet un fonctionnement hors calculatrice (Linux / Windows)
        * Rotation des affichages
        * A terme operationnel pour d'autres calcu
    * Ajout de la fonction *pause* en mode jeu (demande de **blackium**)
    * Affichages en mode horizontal ou vertical
* Corrections
    * BUG : lignes complètes toujours visibles

#### v0.2
* 1er nov. 2023
* Ajouts
    * classe tabManager : gestionnaire d'onglets
    * classes tab (et dérivées) : onglets
    * `tabTetris` - onglet pour `tetrisGame`

#### v0.1 - Première version fonctionnelle du jeu nu
* 15 oct. 2023
* classe `tetrisGame` operationnelle

