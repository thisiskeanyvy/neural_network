import os
import subprocess

def train_letter(image_path):
    letter = os.path.splitext(os.path.basename(image_path))[0]
    while True:
        subprocess.run(["./train", "train_single", image_path], capture_output=True, text=True)
        result = subprocess.run(["./train", "recognize", image_path], capture_output=True, text=True)
        output = result.stdout.strip()
        print(output)
        if output == f"Lettre reconnue : {letter}":
            print(f"Lettre {letter} correctement identifiée.")
            return True
        else:
            print(f"Erreur pour {letter}: identifiée comme {output}. Réessai...")

def process_letters_folder(folder_path):
        filename = input("Lettre à lire (ex: Roboto-Black/A) : ");
        filename += ".bmp"
        image_path = os.path.join(folder_path, filename)
        print(f"Traitement de {filename}...")
        train_letter(image_path)

letters_folder = "letters/"
process_letters_folder(letters_folder)
