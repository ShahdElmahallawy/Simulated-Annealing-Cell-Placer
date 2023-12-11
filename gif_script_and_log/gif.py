import matplotlib.pyplot as plt
import numpy as np
from PIL import Image, ImageDraw, ImageFont

def create_grid_image(grid, cell_size=20, font_size=8):
    rows, cols = len(grid), len(grid[0])
    image = Image.new('RGB', (cols * cell_size, rows * cell_size), color='black')
    draw = ImageDraw.Draw(image)
    font = ImageFont.load_default()  # Adjust the font path as needed

    # Define a fixed color for non-empty cells (e.g., a shade of blue)
    non_empty_cell_color = (0, 0, 255)  # Blue color

    for i, row in enumerate(grid):
        for j, val in enumerate(row):
            if val != -1:
                # Use the fixed color for non-empty cells
                draw.rectangle([j * cell_size, i * cell_size, (j + 1) * cell_size, (i + 1) * cell_size], fill=non_empty_cell_color)
                draw.text((j * cell_size + 2, i * cell_size + 2), str(val), fill='white', font=font)
    
    # Optionally, add grid lines
    for i in range(rows + 1):
        draw.line([(0, i * cell_size), (cols * cell_size, i * cell_size)], fill='grey')
    for j in range(cols + 1):
        draw.line([(j * cell_size, 0), (j * cell_size, rows * cell_size)], fill='grey')

    return image

def generate_animation(file_path, output_gif, cell_size=25, font_size=12):
    images = []
    with open(file_path, 'r') as file:
        grid = []
        for line in file:
            if line.strip():
                row = [int(x) if x != "__" else -1 for x in line.split()]
                grid.append(row)
            else:
                images.append(create_grid_image(grid, cell_size=cell_size, font_size=font_size))
                grid = []

    images[0].save(output_gif, save_all=True, append_images=images[1:], duration=500, loop=0)

generate_animation('./gif_script_and_log/grid_states_t3_0.95.txt', './gifs/t3_0.95.gif')
