from PIL import Image, ImageDraw, ImageFont
import os

font_name = input("Nom de la font pour générer les lettres (ex: Roboto-Black) : ")

def generate_alphabet_images(font_path, font_size, image_size):
    if not os.path.exists(font_name):
        os.makedirs(font_name)

    font = ImageFont.truetype(font_path, font_size)

    for letter in range(ord('A'), ord('Z') + 1):
        char = chr(letter)
        img = Image.new('RGB', image_size, color='white')
        draw = ImageDraw.Draw(img)
        
        # Calculer la position pour centrer le caractère
        w, h = draw.textsize(char, font=font)
        position = ((image_size[0]-w)/2, (image_size[1]-h)/2)
        
        draw.text(position, char, font=font, fill='black')
        img.save(f"{font_name}/{char}.bmp")

    print(f"Lettres générées avec succès dans le dossier {font_name}")

font_path = f"fonts/{font_name.split('-')[0]}/{font_name}.ttf"
font_size = 45
image_size = (100, 100)

generate_alphabet_images(font_path, font_size, image_size)
