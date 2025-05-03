# Find all .j2 template files in the current directory and subdirectories, process them and produce the resulting files without the .j2 extension.
# The resulting files are saved in the same directory as the template files.
# The script uses the Jinja2 templating engine to process the templates.


import os
import jinja2
import sys


def load_json_config(config_path):
    """Load JSON configuration from a file."""
    import json

    with open(config_path, "r") as config_file:
        return json.load(config_file)


def process_templates(root_dir, data):
    # Walk through the directory and process each .j2 file
    # print(f"Processing templates in directory: {root_dir}")
    for folder, _, files in os.walk(root_dir):
        for file in files:
            if file.endswith(".j2"):
                try:
                    # print(f"Found template file: f={file}, root={folder}")

                    # Create a Jinja2 environment with the file system loader
                    jenv = jinja2.Environment(loader=jinja2.FileSystemLoader(folder))

                    template_full_path = os.path.join(folder, file)
                    output_full_path = os.path.splitext(template_full_path)[
                        0
                    ]  # Remove .j2 extension

                    # Load the template
                    env_rel_path = os.path.relpath(template_full_path, folder)
                    # print(f"Loading template: {template_full_path} (relative: {env_rel_path})")
                    template = jenv.get_template(env_rel_path)

                    # Render the template with an empty context (you can modify this to pass variables)
                    rendered_content = template.render(data)

                    # Write the rendered content to the output file
                    with open(output_full_path, "w") as output_file:
                        output_file.write(rendered_content)
                    print(f"Processed {template_full_path} -> {output_full_path}")

                except jinja2.exceptions.TemplateSyntaxError as e:
                    print(f"Template syntax error in {template_full_path}: {e}")
                    sys.exit(1)


if __name__ == "__main__":
    # Load src/registers.json and src/settings.json
    settings_path = os.path.join(os.path.dirname(__file__), "src", "settings.json")
    settings = load_json_config(settings_path)

    # Process templates in the current directory
    process_templates(".", settings)
