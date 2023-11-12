### What's next ?

#### To add
* ***src/casioCalculator*** : 
    * classe à instancier avec le type de calculatrice
    * intègre les spécificités de chaque terminal
    * intégre l'appel de toutes les api de gint
    * mode LINUX / Windows (non Casio) pour le DEBUG
* ***src/shared/tabs*** : Devient plus "générique" est plus lié à cette app
* ***src/shared*** - Dossier contenant les objets de portée plus large que ce premier projet
* ***src/shared/keyHandler*** : Classe pour la gestion du clavier (// Slyvtt::KeyboardExtra ?)
* ***src/shared/scrCapture*** : Objet / interface pour la mise en place et le retrait auto de la capture via fxlink ( intégration du src de Lephenixnoir)

#### Corrections
* ***tabValue*** 
    * Fonctionnement avec la valeur (un pointeur vers ?) plutôt qu'avec une copie
    * ajout de getters et setters plus lisibles

* Fin de l'app - ancien écran tjrs en mémoire