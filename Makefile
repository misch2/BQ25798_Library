.PHONY: jinja2 test

jinja2: 
	@echo "Generating jinja2 templates..."
	@echo "--------------------------------"
	@py process_templates.py
	@echo "--------------------------------"
	@echo "All jinja2 templates have been generated."
	@echo "--------------------------------"

test: jinja2
	platformio test -e native
