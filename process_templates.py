#!/usr/bin/env python3
from pathlib import Path
import jinja2
import sys
import json


def load_json_config(config_path):
    with open(config_path, "r") as config_file:
        return json.load(config_file)


def process_templates(template_dir, data):
    template_dir = Path(template_dir)
    jenv = jinja2.Environment(
        loader=jinja2.FileSystemLoader(template_dir),
        # see https://ttl255.com/jinja2-tutorial-part-3-whitespace-control/
        trim_blocks=True,
        lstrip_blocks=True,
    )
    for template_path in sorted(template_dir.glob("*.j2")):
        output_path = template_path.with_suffix("")
        try:
            rendered_content = jenv.get_template(template_path.name).render(data)
            with open(output_path, "w") as output_file:
                output_file.write(rendered_content)
            print(f"Processed {template_path} -> {output_path}")
        except jinja2.exceptions.TemplateSyntaxError as e:
            print(f"Template syntax error in {template_path}: {e}")
            sys.exit(1)


if __name__ == "__main__":
    project_dir = Path(__file__).resolve().parent
    settings = load_json_config(project_dir / "chip_definitions.json")
    process_templates(project_dir / "lib" / "BQ25798Core", settings)
