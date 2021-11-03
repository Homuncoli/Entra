# Entra
This repository is an implementation of an entity component system. It provides the source code for a library as well a paper explaining its underlaying structure and implementation details.

## Paper
To build the aforementioned paper "Implementation and usage of Entity Component Systems" use the ECS.tex file to build your desired output format via *pandoc*.

### Example PDF
Either directly via *LaTeX*(recommened)
```bash
pdflatex ECS.tex
```
or via *pandoc*
```bash
pandoc ECS.tex -o ECS.pdf
```

### Example AsciiDoc
```bash
pandoc ECS.tex -o ECS.adoc && asciidoc ECS.adoc
```