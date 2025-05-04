#!/usr/bin/env python3
import os
import jinja2
import sys
import json


def load_json_config(config_path):
    with open(config_path, "r") as config_file:
        return json.load(config_file)


def process_templates(root_dir, data):
    for folder, _, files in os.walk(root_dir):
        for file in files:
            if file.endswith(".j2"):
                try:
                    jenv = jinja2.Environment(loader=jinja2.FileSystemLoader(folder))

                    template_full_path = os.path.join(folder, file)
                    output_full_path = template_full_path.replace(".j2", "")

                    env_rel_path = os.path.relpath(template_full_path, folder)
                    template = jenv.get_template(env_rel_path)

                    rendered_content = template.render(data)

                    with open(output_full_path, "w") as output_file:
                        output_file.write(rendered_content)
                    print(f"Processed {template_full_path} -> {output_full_path}")
                except jinja2.exceptions.TemplateSyntaxError as e:
                    print(f"Template syntax error in {template_full_path}: {e}")
                    sys.exit(1)


if __name__ == "__main__":
    settings = load_json_config("chip_definitions.json")
    process_templates(".", settings)
