# IMAGE GENERATOR - Architecture Système

## 🎯 Vue d'ensemble

Système complet permettant de décrire des images en texte naturel et les générer avec un moteur 3D/graphique.

```
┌─────────────────────────────────────────────────────────────┐
│                    CLI (Python)                             │
│  "Un cube rouge avec une sphère bleue qui tourne"          │
└────────────────────────┬────────────────────────────────────┘
                         │
                    HTTP POST
                         │
         ┌───────────────▼───────────────┐
         │    Backend API (Python)       │
         │  - NLP Parser                 │
         │  - Scene Generator            │
         │  - Asset Manager              │
         └───────────────┬───────────────┘
                         │
                    JSON Scene
                         │
         ┌───────────────▼──────────────────┐
         │   Frontend Web (Three.js)        │
         │  - WebGL Rendering              │
         │  - Interactive Viewer           │
         │  - Export (PNG, SVG, video)     │
         └────────────────────────────────┘
```

## 📁 Structure du projet

```
imagegen/
├── README.md
├── requirements.txt
├── setup.py
│
├── cli/
│   ├── __init__.py
│   ├── main.py                  # Entry point
│   ├── parser.py                # NLP description parser
│   └── commands.py              # CLI commands
│
├── backend/
│   ├── __init__.py
│   ├── app.py                   # Flask/FastAPI
│   ├── scene_generator.py       # Generate 3D scenes
│   ├── nlp_engine.py            # NLP processing
│   ├── asset_lib.py             # Asset library
│   └── models/
│       ├── scene.py
│       ├── object.py
│       └── material.py
│
├── frontend/
│   ├── index.html
│   ├── css/
│   │   └── style.css
│   ├── js/
│   │   ├── main.js
│   │   ├── scene.js             # Three.js setup
│   │   ├── renderer.js          # Rendering
│   │   └── interactions.js      # Controls
│   └── assets/
│       └── textures/
│
└── examples/
    ├── simple_cube.txt
    ├── animated_scene.txt
    └── complex_landscape.txt
```

## 🔄 Workflow

### 1️⃣ **CLI Input**
```bash
./imagegen "Un cube rouge avec une sphère bleue"
```

### 2️⃣ **NLP Parsing**
- Détecte: "cube", "sphère"
- Couleurs: "rouge", "bleue"
- Modifications: "avec"

### 3️⃣ **Scene Generation**
```json
{
  "objects": [
    {
      "type": "cube",
      "color": [1, 0, 0],
      "position": [-2, 0, 0]
    },
    {
      "type": "sphere",
      "color": [0, 0, 1],
      "position": [2, 0, 0],
      "animation": { "type": "rotation" }
    }
  ],
  "camera": { "position": [0, 0, 5] },
  "lighting": [
    { "type": "directional", "intensity": 1 }
  ]
}
```

### 4️⃣ **Web Rendering**
- Three.js charge la scène
- WebGL rendu temps réel
- Controls interactifs (rotate, zoom, pan)

### 5️⃣ **Export**
```bash
./imagegen "..." --output result.png
./imagegen "..." --output scene.html
./imagegen "..." --output animation.mp4
```

## 🎨 Features

### Phase 1 : Core
- [x] CLI argument parsing
- [x] Simple NLP (shapes, colors, positions)
- [x] Basic scene generation
- [x] Three.js rendering
- [x] Interactive viewer

### Phase 2 : Intermediate
- [ ] Complex shapes (torus, pyramid, etc)
- [ ] Materials (metallic, glass, matte)
- [ ] Lighting (point, directional, ambient)
- [ ] Basic animations

### Phase 3 : Advanced
- [ ] Advanced NLP (relationships, spatial descriptions)
- [ ] Procedural generation
- [ ] Physics simulation
- [ ] Video export
- [ ] Real-time raytracing integration

## 💻 Technologies

| Layer | Tech | Raison |
|-------|------|--------|
| CLI | Python 3.9+ | Flexible, easy NLP |
| Backend | FastAPI | Fast, async, easy |
| NLP | spaCy / NLTK | NLP robuste |
| Frontend | Three.js | WebGL, performant |
| Export | Puppeteer / FFmpeg | Screenshots, vidéos |

## 🚀 Démarrage

```bash
# Setup
pip install -r requirements.txt
npm install (frontend)

# Run backend
python backend/app.py

# Run CLI
./imagegen "Une pyramide dorée"

# Open browser
http://localhost:5000
```

## 📝 Exemples de descriptions

```
Simple:
- "Un cube bleu"
- "Une sphère rouge et un cube vert"
- "Trois cylindres alignés"

Avec styles:
- "Un château en pixel art"
- "Une maison moderne avec toit rouge"
- "Un paysage montagneux"

Avec animations:
- "Une planète qui tourne autour du soleil"
- "Une boîte qui pulse"
- "Des particules qui tombent"

Complexe:
- "Une scène futuriste avec des gratte-ciel en verre et des voitures volantes"
- "Un jardin avec des arbres, des fleurs colorées et un sentier de pierre"
```

## 🎯 Success Criteria

Phase 1 terminée quand:
- ✅ CLI parse descriptions simples
- ✅ Backend génère JSON scenes
- ✅ Frontend Three.js affiche les objets
- ✅ Interactif (rotate, zoom)
- ✅ Peut exporter PNG

## 📚 Ressources

- Three.js Docs: https://threejs.org/docs/
- spaCy NLP: https://spacy.io/
- FastAPI: https://fastapi.tiangolo.com/
- Puppeteer: https://pptr.dev/

