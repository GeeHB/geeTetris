#### Ajouts
- [ ] ***src/playArea*** :
  - ~~classe à instancier avec le type de calculatrice~~
  - [ ] intègre les spécificités de chaque terminal
  - [ ] intègre l'appel de toutes les api de gint
  - [ ] mode LINUX / Windows (non Casio) pour le DEBUG
- [x] ***src/shared*** : un dossier pour les objets partagés avec d'autres projets
- [x] ***src/shared/tabs*** : Devient plus "générique" et moins liée à cette app
- [x] ***src/shared/keyboard*** : Classe pour la gestion du clavier (// Slyvtt::KeyboardExtra ?)
  - [x] \- Gestion abstraite du clavier (Casio / Linux)
  - [x] \- ***tabKeyboard*** : classe héritée pour le "transfert" des touches entre un onglet et le gestionnaire d'onglets
- [x] ***src/shared/scrCapture*** : Objet / interface pour la mise en place et le retrait auto de la capture via fxlink ( intégration du src de Lephenixnoir)
- [x] ***src/sList*** : Liste simplement chaînée pour la gestion des scores
- [ ] ***src/shared/bFile*** : Un objet basique pour gérer les fichiers (interface // gint::bfile.h)
  - [x] FX-CG50
  - [ ] autres calcs ...
  - [x] Linux (avec std, filesystem et fstream)
- [ ] Gestion des scores
  - [ ] Utilisation de ***shared/bFile***
  - [ ] Mini-liste pour gérer le tableau des scores : ***sList***
  - [ ] Enregistrement / lecture du tuple (score, #level, #lines)
- [ ] Mode pause (dde de blackium)
#### Corrections

- [ ] Ajouter / modifier les icones
- [ ] Image de l'ecran d'accueil "+ tetris like" (dde de blackium)
- [ ] ***tabValue***
  - [x] Fonctionnement avec la valeur (un pointeur vers ?) plutôt qu'avec une copie
  - [ ] Ajout de getters et setters plus lisibles
  - [x] Quid du comportement par défaut (gestion d'un booléen) <- inchangé
- [x] Fin de l'app - ancien écran tjrs en mémoire
- [ ] BUG : Certaines lignes pleines ne disparaissent pas