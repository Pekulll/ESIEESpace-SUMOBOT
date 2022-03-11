# Intermédiaire

Le main.ino contenu dans ce dossier est une version améliorée de celui contenu dans [Basique](https://github.com/Pekulll/ESIEESpace-SUMOBOT/tree/main/Basique). Il implémente des fonctionnalité supplémentaires telles que :
- L'évitement des lignes blanches droites et gauches
- La recherche de l'ennemi à 360° autour du SUMOBOT
- L'ajout d'interruption pour permettre une meilleure réactivité

## Améliorations possibles

Dans son état actuel, ce programme peut être utilisé pour participer à une compétition.
Cependant, le programme implémentant une stratégie d'attaque et de défense très basique, on peut envisager d'ajouter des fonctionnalités supplémentaires plus sophistiquées. Par exemple :
- Calculer la vitesse de l'ennemie, et sa direction, afin d'anticiper sa trajectoire et de l'éviter plus efficacement
- Une meilleure stratégie d'attaque, où le SUMOBOT frappe l'adversaire de manière optimale, afin de réduire toute la résistance du robot adverse