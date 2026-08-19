#
#   splash.py
#
#   Description : Script de "Tetrisation" d'un texte pour fabriquer l'écran d'accueil
#
#   Auteur : JHB
#
#   Date : 01/12/2023
#

from PIL import Image
from casioplot import *

# Fichier source
#
SRC_FILE = "base.png"
SRC_W = 166     # Dimensions réelles de l'image
SRC_H = 45

# Ficier destinatation CG
#
DEST_CG_FILE = "../assets-cg/coloured.png"
DEST_CG_W = 396
DEST_CG_H = 202    # Hauteur casio - barre de menu

# Largeur d'un cube
SQUARE_CG_W  = 6

# Offset sur la source
SRC_CG_STEP = int(SQUARE_CG_W * SRC_W / DEST_CG_W) + 1

DEST_CG_OFFSET_X = 30
DEST_CG_OFFSET_Y = 30

# Ficier destinatation FX
#
DEST_FX_FILE = "../assets-fx/bw.png"
DEST_FX_W = 128
DEST_FX_H = 42

# Largeur d'un cube
SQUARE_FX_W  = 3

# Offset sur la source
SRC_FX_STEP = int(SQUARE_FX_W * SRC_W / DEST_FX_W) + 1

DEST_FX_OFFSET_X = 0
DEST_FX_OFFSET_Y = 0

# Couleurs utilisées
# 
COLOUR_WHITE = (255, 255, 255)
COLOUR_BLACK = (0, 0, 0)
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
#   couleur_fond : Couleur de remplissage au format RGB (ou None si vide) 
#   couleur_bord : Couleur de la bordure au format RGB (ou None si pas de bordure)
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
    
    # For CG calc. 
    #
    dstCG = Image.new('RGB', (DEST_CG_W, DEST_CG_H), COLOUR_BKGROUND)
    width = int(SRC_W / SRC_CG_STEP)
    height = int(SRC_H / SRC_CG_STEP)

    for x in range(width):
        for y in range(height):
            r,g,b,_ = src.getpixel((x*SRC_CG_STEP, y*SRC_CG_STEP))

            colourID = int(x / 9) % 7   # for the pixel

            if COLOUR_WHITE != (r,g,b):
                draw_rectangle(dstCG, x * SQUARE_CG_W + DEST_CG_OFFSET_X,  y * SQUARE_CG_W  + DEST_CG_OFFSET_Y, SQUARE_CG_W, SQUARE_CG_W, colours[colourID])

    # FX9660G calc. 
    #
    dstFX = Image.new('RGB', (DEST_FX_W, DEST_FX_H), COLOUR_WHITE)
    width = int(SRC_W / SRC_FX_STEP)
    height = int(SRC_H / SRC_FX_STEP)

    for x in range(width):
        for y in range(height):
            r,g,b,_ = src.getpixel((x*SRC_FX_STEP, y*SRC_FX_STEP))

            if COLOUR_WHITE != (r,g,b):
                draw_rectangle(dstFX, x * SQUARE_FX_W + DEST_FX_OFFSET_X,  y * SQUARE_FX_W  + DEST_FX_OFFSET_Y, SQUARE_FX_W, SQUARE_FX_W, None, COLOUR_BLACK)
    
    # Enregistrements
    dstCG.save(DEST_CG_FILE, 'png')
    dstFX.save(DEST_FX_FILE, 'png')

# EOF