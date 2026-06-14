import re
import argparse
from pathlib import Path
from PIL import Image

GLYPH_WIDTH = 6
GLYPH_HEIGHT = 8
CHARS_PER_ROW = 32  # Updated to 32 glyphs per row


def extract_font_bytes(text: str):
    # Remove C comments
    text = re.sub(r"//(.*)\n", "\n", text)

    pattern = r"font\s*\[\s*\]\s*PROGMEM\s*=\s*\{(.*?)\};"

    m = re.search(pattern, text, re.S)
    if not m:
        raise RuntimeError("Could not find font[] PROGMEM array in file.")
    body = m.group(1)

    hex_bytes = re.findall(r"0x([0-9A-Fa-f]{2})", body)
    return [int(h, 16) for h in hex_bytes]


def font_bytes_to_bitmap(font_bytes):
    print("Byte count:", len(font_bytes))

    bytes_per_glyph = GLYPH_WIDTH
    num_glyphs = len(font_bytes) // bytes_per_glyph

    rows = (num_glyphs + CHARS_PER_ROW - 1) // CHARS_PER_ROW
    img_w = CHARS_PER_ROW * GLYPH_WIDTH
    img_h = rows * GLYPH_HEIGHT

    img = Image.new("1", (img_w, img_h), 0)
    pixels = img.load()

    for glyph_index in range(num_glyphs):
        gx = glyph_index % CHARS_PER_ROW
        gy = glyph_index // CHARS_PER_ROW

        base_x = gx * GLYPH_WIDTH
        base_y = gy * GLYPH_HEIGHT

        for col in range(GLYPH_WIDTH):
            byte = font_bytes[glyph_index * bytes_per_glyph + col]
            for bit in range(GLYPH_HEIGHT):
                # LSB is top pixel
                on = (byte >> bit) & 0x01
                if on:
                    x = base_x + col
                    y = base_y + bit
                    pixels[x, y] = 1

    return img


def bitmap_to_font_bytes(img: Image.Image):
    """Convert an 8‑bit or 1‑bit PNG into QMK 6×8 column-major font bytes."""
    img = img.convert("L")  # ensure 8-bit grayscale
    w, h = img.size

    if w % GLYPH_WIDTH != 0 or h % GLYPH_HEIGHT != 0:
        raise ValueError("Image size must be a multiple of 6×8 pixels.")

    glyphs_x = w // GLYPH_WIDTH
    glyphs_y = h // GLYPH_HEIGHT

    pixels = img.load()
    font_bytes = []

    for gy in range(glyphs_y):
        for gx in range(glyphs_x):
            base_x = gx * GLYPH_WIDTH
            base_y = gy * GLYPH_HEIGHT

            for col in range(GLYPH_WIDTH):
                byte = 0
                for bit in range(GLYPH_HEIGHT):
                    px = pixels[base_x + col, base_y + bit]

                    # threshold: treat >=128 as ON
                    if px >= 128:
                        byte |= (1 << bit)

                font_bytes.append(byte)

    return font_bytes


def emit_c_array(font_bytes, output_path):
    # ASCII control names for 0x00–0x1F
    control_names = [
        "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
        "BS", "HT", "LF", "VT", "FF", "CR", "SO", "SI",
        "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
        "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US"
    ]

    # Unicode symbols from your original file for 0x00–0x7F
    unicode_symbols = {
        0x00: "Ψ", 0x01: "☺︎", 0x02: "☻", 0x03: "♥︎", 0x04: "♦︎",
        0x05: "♣︎", 0x06: "♠︎", 0x07: "•", 0x08: "◘", 0x09: "○",
        0x0A: "◙", 0x0B: "♂︎", 0x0C: "♀︎", 0x0D: "♪", 0x0E: "♫",
        0x0F: "☼", 0x10: "►", 0x11: "◄", 0x12: "↕︎", 0x13: "‼︎",
        0x14: "¶", 0x15: "§", 0x16: "▬", 0x17: "↨", 0x18: "↑",
        0x19: "↓", 0x1A: "→", 0x1B: "←", 0x1C: "∟", 0x1D: "↔︎",
        0x1E: "▲", 0x1F: "▼",
        # 0x20–0x7E use ASCII printable chars
        # 0x7F DEL
        0x7F: "⌂"
    }

    with open(output_path, "w", encoding="utf-8") as f:
        f.write("#include \"progmem.h\"\n")
        f.write("\n")
        f.write("static const unsigned char font[] PROGMEM = {\n")

        glyph_count = len(font_bytes) // GLYPH_WIDTH

        for glyph_index in range(glyph_count):
            start = glyph_index * GLYPH_WIDTH
            bytestr = ", ".join(f"0x{b:02X}" for b in font_bytes[start:start+GLYPH_WIDTH])

            # Determine comment label
            if glyph_index < 0x20:
                label = control_names[glyph_index]
            elif 0x20 <= glyph_index <= 0x7E:
                if glyph_index == 0x5C:
                    label = "Backslash"
                else:
                    label = chr(glyph_index)
            elif glyph_index == 0x7F:
                label = "DEL"
            else:
                label = ""

            # Unicode symbol if available
            symbol = unicode_symbols.get(glyph_index, "")

            # Build comment
            comment = f"// 0x{glyph_index:02X}"
            if label:
                comment += f" {label}"
            if symbol:
                comment += f" / {symbol}"

            f.write(f"    {bytestr}, {comment}\n")

        f.write("};\n")

def main():
    parser = argparse.ArgumentParser(description="Convert between QMK font C header and bitmap PNG.")
    parser.add_argument("--mode", choices=["extract", "build"], required=True,
                        help="extract = C→PNG, build = PNG→C")
    parser.add_argument("--input", "-i", required=True)
    parser.add_argument("--output", "-o", required=True)
    args = parser.parse_args()

    if args.mode == "extract":
        text = Path(args.input).read_text(encoding="utf-8", errors="ignore")
        font_bytes = extract_font_bytes(text)
        img = font_bytes_to_bitmap(font_bytes)
        img.save(args.output)
        print(f"Saved bitmap to {args.output}")

    elif args.mode == "build":
        img = Image.open(args.input)
        font_bytes = bitmap_to_font_bytes(img)
        emit_c_array(font_bytes, args.output)
        print(f"Saved C file to {args.output}")


if __name__ == "__main__":
    main()
