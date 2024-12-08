Entrainement du réseau de neurones (manuel)

Préparer les images d'entrainement :
- Créer une image BMP pour chaque lettre de l'alphabet (A.bmp, B.bmp, ..., C.bmp).
- Chaque image doit contenir une seule lettre, idéalement en noir sur fond blanc.
- Les images doivent être de taille similaire, le code les redimensionnera à 56x49 pixels.

Entraînement via le réseau pour chaque lettre :

./train train_single A.bmp
./train train_single B.bmp
./train train_single C.bmp
... et ainsi de suite pour toutes les lettres

Répéter l'étape d'entraînement plusieurs fois pour chaque lettre afin d'améliorer la précision.

Puis une fois que toutes lettres sont reconnues :

./train train dossier_lettres/

Tester la reconnaissance :
./train recognize test_image.bmp

Le réseau sauvegarde automatiquement son état après chaque entraînement dans "network_data.bin" et pour les singles dans un autre fichier .bin.

-------------------------------------------------------------------------------------------------

Entrainement du réseau de neurones (automatique)

Préparer les images d'entrainement :
- Choisir la font au format TTF et modifier dans le code de generator.py avec la bonne font
- Générer des lettres au format BMP avec letters/generator.py
- La taille des images générées est définie dans generator.py

Entraînement via le réseau pour chaque lettre :

python3 auto_train.py (exécution une seule fois la première fois)
... et attendre la fin de l'entrainement

Tester la reconnaissance de chaques lettres :

python3 verify_train.py
ou pour une seule lettre
./train recognize test_image.bmp

et si la lettre détectée n'est pas la bonne :
python3 manual_train.py
... puis entrer la lettre (ex: U)

Puis :

pour vérifier une seule lettre
./train train dossier_lettres/

... et ainsi de suite pour toutes les lettres mal détectées (recommencer les 2 processus)

Le réseau sauvegarde automatiquement son état après chaque entraînement dans "network_data.bin" et pour les singles dans un autre fichier .bin.

-------------------------------------------------------------------------------------------------

- backup_data contient des sauvegardes d'anciens entrainements
- python3 manual_train.py permet d'entrainer une seule lettre
- python3 auto_train est utilisé pour entrainer toutes les lettres d'un dossier pour la première fois
- train est la version utilisée par l'utilisateur (train.c)
- *.bin contient les données d'entrainement
- letters/ contient les lettres avec generator.py

--------------------------------------------------------------------------------------------------

Compilation : gcc train.c -o train -lm `pkg-config --cflags --libs sdl2 SDL2_image`

--------------------------------------------------------------------------------------------------

Librairies à installer :
- libsdl2-image-dev (pour la lecture des images)
- PIL (python générer les lettres)

--------------------------------------------------------------------------------------------------

Sauvegarder le réseau de neurones :
- bash checksum.sh