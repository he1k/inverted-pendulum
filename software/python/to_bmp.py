from PIL import Image
import os

SRC_FOLDER = "../img"
DEST_FOLDER = "../img_bmp" 
LANDSCAPE_MODE = False

PORTRAIT_RESOLUTION  = (240, 320)
LANDSCAPE_RESOLUTION = (320, 240) 

TARGET_RESOLUTION = PORTRAIT_RESOLUTION
if LANDSCAPE_MODE:
  TARGET_RESOLUTION = (320,240)
  print("Outputting in landscape")
else:
  TARGET_RESOLUTION = PORTRAIT_RESOLUTION
  print("Outputting in portrait")

os.makedirs(DEST_FOLDER, exist_ok=True)

def resize_and_convert(image_path, output_folder, target_size):
    """Resizes an image to the target size while maintaining aspect ratio and saves it as a 24-bit BMP."""
    
    # Open image
    img = Image.open(image_path).convert("RGB")  # Ensure RGB mode
    
    # Resize while maintaining aspect ratio
    img.thumbnail(target_size, Image.LANCZOS)
    
    # Create a new image with the exact target size (black background)
    new_img = Image.new("RGB", target_size, (0, 0, 0))  # Black background
    
    # Paste resized image at the center
    x_offset = (target_size[0] - img.width) // 2
    y_offset = (target_size[1] - img.height) // 2
    new_img.paste(img, (x_offset, y_offset))

    # Save as 24-bit BMP
    if LANDSCAPE_MODE:
      output_path = os.path.join(output_folder, os.path.splitext(os.path.basename(image_path))[0] + "-landscape.bmp")
    else:
      output_path = os.path.join(output_folder, os.path.splitext(os.path.basename(image_path))[0] + "-portrait.bmp") 
    new_img.save(output_path, format="BMP")
    print(f"✔ Converted: {image_path} → {output_path}")

# Process all images in the input folder
for filename in os.listdir(SRC_FOLDER):
    if filename.lower().endswith((".jpg", ".jpeg", ".png", ".bmp", ".gif")):
        resize_and_convert(os.path.join(SRC_FOLDER, filename), DEST_FOLDER, TARGET_RESOLUTION)

print("\n All images have been converted and saved in the 'output_bmp' folder!")

