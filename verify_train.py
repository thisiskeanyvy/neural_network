import os
import subprocess

def train_letter(image_path):
    letter = os.path.splitext(os.path.basename(image_path))[0]
    while True:
        result = subprocess.run(["./train", "recognize", image_path], capture_output=True, text=True)
        #result = subprocess.run(["./train_auto", "recognize", image_path], capture_output=True, text=True)
        output = result.stdout.strip()
        print(output)
        #if output == letter:
            #print(f"Lettre {letter} correctement identifiée.")
        return True
        #else:
            #print(f"Erreur pour {letter}: identifiée comme {output}. Réessai...")

def process_letters_folder(folder_path):
    for filename in sorted(os.listdir(folder_path)):
        if filename.endswith(".bmp"):
            image_path = os.path.join(folder_path, filename)
            print(f"Traitement de {filename}...")
            train_letter(image_path)

final_folder = input("Dossier de fonts à vérifier : ")
letters_folder = f"letters/{final_folder}"
process_letters_folder(letters_folder)
