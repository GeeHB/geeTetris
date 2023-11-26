
// Code d'une fonction *globale* pour appeler un syscall
static int SysCallCode[] = { 0xD201422B, 0x60F20000, 0x80010070 };
// Nom et paramètres de la fonction
static int (*SysCall)(int R4, int R5, int R6, int R7, int FNo ) = (void *)&SysCallCode;
// (Ces deux lignes ne sont à placer dans votre code qu'une seule fois)

int RTC_getTicks(void)
{
    // On donne le numéro du syscall (0x3b) et on transmet les arguments
    // (ici il n'y en a pas donc on met des 0, ça n'a aucune importance)
    return (*SysCall)(0, 0, 0, 0, 0x3b);
}

