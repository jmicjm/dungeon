import os

assets_dir = "assets"
output_dir = "src/generated_assets"

outfile_entry_template = """#define {name_size} {size}
static const unsigned char {name}[{name_size}] = {{{data}}};\n
"""


def serialize_file(path: str) -> tuple[str, int]:
    with open(path, "rb") as file:
        data = [f"0x{b:02x}" for b in file.read()]

    result_str = ",".join(data)
    size = len(data)

    return result_str, size


def generate_header(path: str, dirname: str) -> None:
    with open(os.path.join(output_dir, f"{dirname}.h"), "w") as output_file:
        output_file.write("#pragma once\n\n")
        output_file.write("namespace " + dirname.upper() + "_ASSETS\n{\n")
        entries = []

        for root, dirs, files in os.walk(path):
            for filename in files:
                name, ext = os.path.splitext(filename)
                asset_path = os.path.join(root, filename)
                asset_data, asset_size = serialize_file(asset_path)
                asset_name = name.upper()
                asset_name_size = f"{asset_name}_SIZE"

                out_entry = outfile_entry_template.format(**{
                    "data": asset_data,
                    "size": asset_size,
                    "name": asset_name,
                    "name_size": asset_name_size,
                })

                entries.append((asset_path, asset_name, asset_size))
                output_file.write(out_entry)

        paths, names, sizes = tuple(zip(*entries))  # type: list[str], list[str], list[int]
        assets_count = len(entries)
        newline = "\n"
        quot = "\""

        output_file.write(f"static const int ASSETS_COUNT = {assets_count};\n\n")
        output_file.write(f"static const char *ASSET_PATHS[{assets_count}] = {{{newline + f',{newline}'.join(f'{quot}{x}{quot}' for x in paths) + newline}}};\n\n")
        output_file.write(f"static const void *ASSETS_DATA[{assets_count}] = {{{newline + f',{newline}'.join(f'{x}' for x in names) + newline}}};\n\n")
        output_file.write(f"static const int ASSET_SIZES[{assets_count}] = {{{newline + f',{newline}'.join(str(x) for x in sizes) + newline}}};\n")
        output_file.write("}")


def main():
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)

    for dirname in os.listdir(assets_dir):
        path = os.path.join(assets_dir, dirname)
        if os.path.isdir(path):
            generate_header(path, dirname)


if __name__ == "__main__":
    main()
