#### To add

- [ ] ***src/casioCalculator*** :
  - [ ] classe à instancier avec le type de calculatrice
  - [ ] intègre les spécificités de chaque terminal
  - [ ] intègre l'appel de toutes les api de gint
  - [ ] mode LINUX / Windows (non Casio) pour le DEBUG
- [x] ***src/shared*** : un dossier pour les objets partagés avec d'autres projets
- [x] ***src/shared/tabs*** : Devient plus "générique" et plus liée à cette app
- [x] ***src/shared/keyboard*** : Classe pour la gestion du clavier (// Slyvtt::KeyboardExtra ?)
  - [x] - Gestion abstraite du clavier (Casio / Linux)
  - [x] - tabKeyboard : classe héritée pour le "transfert" des touches entre un onglet et le gestionnaire d'onglets
- [x] ***src/shared/scrCapture*** : Objet / interface pour la mise en place et le retrait auto de la capture via fxlink ( intégration du src de Lephenixnoir)
- [ ] ***src/shared/FSO*** : Un objet basique pour gérer les fichiers (CASIOWIN ou FUGUE ou LINUX)
- [ ] Gestion des scores
  - [ ] Interface pour la gestion de fichiers
  - [ ] Enregistrement / lecture du tuple (score, #level, #lines)

#### Corrections
- [ ] Ajouter des icones
- [ ] Image de l'ecran d'accueil "+ tetris like" (dde blackium)
- [ ] ***tabValue***
  - [ ] Fonctionnement avec la valeur (un pointeur vers ?) plutôt qu'avec une copie
  - [ ] Ajout de getters et setters plus lisibles
  - [ ] Quid du comportement par défaut (gestion d'un boolén)
- [x] Fin de l'app - ancien écran tjrs en mémoire