# ImageGen Examples

## Formes simples

### Cubes
- "Un cube bleu"
- "Un cube rouge et un cube vert"
- "Trois cubes alignés"
- "Un cube doré métallique"

### Sphères
- "Une sphère rouge"
- "Une sphère qui tourne"
- "Deux sphères côte à côte"
- "Une sphère en verre transparent"

### Autres formes
- "Un cylindre jaune"
- "Une pyramide grise"
- "Un torus rose"
- "Un cône bleu"

## Combinaisons

### Duos
- "Un cube rouge avec une sphère bleue"
- "Un cylindre et un torus"
- "Une pyramide à côté d'un cube"

### Groupes
- "Trois sphères colorées"
- "Quatre cubes en rangée"
- "Une scène avec cube, sphère, et cylindre"

## Animations

### Rotations
- "Un cube qui tourne"
- "Une sphère rotante"
- "Un torus qui spinnne"

### Autres mouvements
- "Une sphère qui pulse"
- "Un cube qui bouncing"
- "Une pyramide qui flotte"
- "Un cylindre qui tombe"

### Orbite
- "Une sphère qui orbite"
- "Un torus qui tourne autour"

## Matériaux

### Métallique
- "Un cube en or"
- "Une sphère métallique"
- "Un objet argenté"

### Verre
- "Une sphère en verre"
- "Un cube transparent"
- "Un objet cristallin"

### Matte
- "Un cube matte rouge"
- "Une sphère rugueuse"

## Combinaisons avancées

- "Un cube rouge métallique avec une sphère bleue rotante"
- "Une pyramide doré et un torus en verre qui pulse"
- "Trois cubes colorés avec une sphère qui orbite"
- "Un scène futuriste avec des formes géométriques"
- "Une composition artistique avec cube, sphère, et cylindre"

## Pour tester

```bash
# Simple
python3 cli/main.py "Un cube bleu"

# Avec animation
python3 cli/main.py "Une sphère qui tourne" --animate

# Avec export
python3 cli/main.py "Un torus" --output torus.png

# Complexe
python3 cli/main.py "Un cube rouge et une sphère bleue qui tourne" --animate
```
