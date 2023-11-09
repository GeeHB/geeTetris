#
#   splash.py
#
#   Description : Script de "Tetrisation" d'un texte pour fabriquer l'écran d'accueil
#
#   Auteur : JHB
#
#   Date : 09/11/2023
#

from PIL import Image
from casioplot import *

# Fichier source
#
SRC_FILE = "base.png"
SRC_W = 166     # Dimensions réelles de l'image
SRC_H = 45

# Ficier destinatation
DEST_FILE = "../assets-cg/coloured.png"
DEST_W = 396
DEST_H = 202    # Hauteur casio - barre de menu

DEST_OFFSET_X = 0
DEST_OFFSET_Y = 30

# Couleurs utilisées
# 
COLOUR_WHITE = (255, 255, 255)
COLOUR_BKGROUND = (245, 245, 245)

# Couleurs des pieces
COLOUR_RED = (255, 0, 0)
COLOUR_GREEN = (0, 255, 0)
COLOUR_YELLOW = (255, 255, 0)
COLOUR_BLUE = (0, 0, 255)
COLOUR_PURPLE = (255, 0, 255)
COLOUR_CYAN = (0, 255, 255)
COLOUR_ORANGE = (255, 128, 0)

# Dessin d'un rectangle
#
#   dst : image destination
#   x,y : coin superieur gauche
#   largeur, hauteur : dimensions
#   couleur_bord : Couleur de la bordure au format RGB (ou None si pas de bordure)
#   couleur_fond : Couleur de remplissage au format RGB (ou None si vide) 
#
def draw_rectangle(dest, x, y, largeur, hauteur, couleur_fond = None, couleur_bord = None):
    # Le contour ?
    if couleur_bord is not None:
        for px in range(largeur):
            dest.putpixel((x + px, y), couleur_bord)
            dest.putpixel((x + px, y + hauteur - 1), couleur_bord)

        for py in range(hauteur-2):
            dest.putpixel((x, y + py + 1), couleur_bord)
            dest.putpixel((x + largeur - 1, y + py + 1), couleur_bord)

    # Remplissage ?
    if couleur_fond is not None:
        for px in range(largeur - 1):
            for py in range(hauteur - 1):
                dest.putpixel((x + px, y + py), couleur_fond)


if __name__ == "__main__":      
    
    # Tableau de couleurs
    colours = []
    colours.append(COLOUR_RED)
    colours.append(COLOUR_GREEN)
    colours.append(COLOUR_YELLOW)
    colours.append(COLOUR_BLUE)
    colours.append(COLOUR_PURPLE)
    colours.append(COLOUR_CYAN)
    colours.append(COLOUR_ORANGE)
 
    src = Image.open(SRC_FILE)
    dst = Image.new('RGB', (DEST_W, DEST_H), COLOUR_BKGROUND)
    
    width = int(SRC_W / 2)
    height = int(SRC_H / 2)

    for x in range(width):
        for y in range(height):
            r,g,b,_ = src.getpixel((x*2, y*2))

            colourID = int(x / 9) % 7   # for the pixel

            if COLOUR_WHITE != (r,g,b):
                draw_rectangle(dst, x * 5 + DEST_OFFSET_X,  y * 5  + DEST_OFFSET_Y, 5, 5, colours[colourID])
    
    # Enregistrement
    dst.save(DEST_FILE, 'png')

# EOF