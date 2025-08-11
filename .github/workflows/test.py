import requests
from bs4 import BeautifulSoup

def extract_table_data(url):
    response = requests.get(url, timeout=10)
    response.raise_for_status()
    soup = BeautifulSoup(response.text, 'html.parser')
    tables = soup.find_all('table')
    if not tables:
        raise ValueError("No table found in the document.")
    table = tables[0]  # Assume the first table contains the data
    rows = []
    for tr in table.find_all('tr'):
        cells = [td.get_text(strip=True) for td in tr.find_all(['td', 'th'])]
        rows.append(cells)
    return rows

def build_coordinate_map(rows):
    coord_map = {}
    for row in rows[1:]:  # Skip header
        if len(row) == 3:
            x, char, y = row
            coord_map[(int(x), int(y))] = char
    return coord_map

def render_grid(coord_map):
    if not coord_map:
        print("No data to render.")
        return
    max_x = max(x for x, _ in coord_map)
    max_y = max(y for _, y in coord_map)
    for y in range(max_y, -1, -1):
        line = ''.join(coord_map.get((x, y), ' ') for x in range(max_x + 1))
        print(line)


def print_character_grid_from_doc(doc_url):
    try:
        rows = extract_table_data(doc_url)
        coord_map = build_coordinate_map(rows)
        render_grid(coord_map)
    except Exception as e:
        print(f"Error: {e}")
        
# test the function with an example
print_character_grid_from_doc("https://docs.google.com/document/d/e/2PACX-1vTER-wL5E8YC9pxDx43gk8eIds59GtUUk4nJo_ZWagbnrH0NFvMXIw6VWFLpf5tWTZIT9P9oLIoFJ6A/pub")