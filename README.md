# Entraînement du réseau de neurones (manuel)

## Préparer les images d'entraînement
- Créer une image BMP pour chaque lettre de l'alphabet (A.bmp, B.bmp, ..., C.bmp).
- Chaque image doit contenir une seule lettre, idéalement en noir sur fond blanc.
- Les images doivent être de taille similaire, le code les redimensionnera à 56x49 pixels.

## Entraînement via le réseau pour chaque lettre
```bash
./train train_single A.bmp
./train train_single B.bmp
./train train_single C.bmp
... et ainsi de suite pour toutes les lettres
```

Répéter l'étape d'entraînement plusieurs fois pour chaque lettre afin d'améliorer la précision.

Puis, une fois que toutes les lettres sont reconnues :
```bash
./train train dossier_lettres/
```

## Tester la reconnaissance
```bash
./train recognize test_image.bmp
```

Le réseau sauvegarde automatiquement son état après chaque entraînement dans "network_data.bin" et pour les singles dans "train_data.bin".

---

# Entraînement du réseau de neurones (automatique)

## Préparer les images d'entraînement
- Choisir la font au format TTF et modifier dans le code de `generator.py` avec la bonne font.
- Générer des lettres au format BMP avec `letters/generator.py`.
- La taille des images générées est définie dans `generator.py`.

## Entraînement via le réseau pour chaque lettre
```bash
python3 auto_train.py  # exécution une seule fois la première fois
```
... et attendre la fin de l'entraînement.

## Tester la reconnaissance de chaque lettre
```bash
python3 verify_train.py
```
ou pour une seule lettre :
```bash
./train recognize test_image.bmp
```

Et si la lettre détectée n'est pas la bonne :
```bash
python3 manual_train.py
```
... puis entrer la lettre attendue (ex: U).

Puis, pour harmoniser le réseau de neurones et le sauvegarder :
```bash
./train train dossier_lettres/
```
... et ainsi de suite pour toutes les lettres mal détectées (recommencer les 2 processus).

Le réseau sauvegarde automatiquement son état après chaque entraînement dans "network_data.bin" et pour les singles dans "train_data.bin".

---

## Informations supplémentaires
- `backup_data` contient des sauvegardes d'anciens entraînements.
- `python3 manual_train.py` permet d'entraîner une seule lettre.
- `python3 auto_train.py` est utilisé pour entraîner toutes les lettres d'un dossier pour la première fois.
- `python3 verify_train.py` est utilisé pour vériier si les lettres sont bien détectées.
- `train` est la version utilisée par l'utilisateur (`train.c`).
- `*.bin` contient les données d'entraînement.
- `letters/` contient les lettres avec `generator.py`.
- `checksum.sh` est utilisé pour sauvegarder les fichiers `*.bin`.

---

## Compilation
```bash
gcc train.c -o train -lm `pkg-config --cflags --libs sdl2 SDL2_image`
```

---

## Librairies à installer
- `libsdl2-image-dev` (pour la lecture des images en C).
- `PIL` (générer les lettres à partir de fonts en python).

---

## Sauvegarder le réseau de neurones
```bash
bash checksum.sh
```