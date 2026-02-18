# 🎨 ImageGen - Text to 3D Image Generator

Generate 3D images from natural language descriptions using NLP and optional raytracer rendering.

## 🚀 Quick Start

```bash
# 1. Install dependencies
pip install -r requirements.txt

# 2. Run server
python run.py

# 3. Open browser
http://localhost:5000/chat
```

## 📁 Project Structure

```
imagegen/
├── backend/                       # Python backend
│   ├── app.py                    # FastAPI server + chat UI
│   ├── nlp_engine.py             # NLP parser
│   ├── scene_generator.py        # 3D scene generator
│   ├── raytracer_integration.py  # Raytracer C integration
│   ├── client.py                 # API client
│   └── __init__.py
├── cli/                          # Command-line interface
│   └── main.py
├── frontend/                     # Web UI
│   ├── chat.html                 # Chat interface
│   └── index.html                # 3D viewer
├── data/                         # Data storage
│   └── scenes/
├── run.py                        # Server launcher
├── requirements.txt              # Python dependencies
└── README.md
```

## ✨ Features

✅ **Chat Interface** - Describe images naturally
✅ **NLP Engine** - Parse shapes, colors, animations, materials
✅ **3D Scene Generation** - Create Three.js scenes
✅ **Raytracer Integration** - High-quality C raytracer rendering
✅ **WebGL Fallback** - Real-time Three.js preview
✅ **REST API** - Full API endpoints

## 📊 Supported Elements

### Shapes (7)
Cube, Sphere, Cylinder, Pyramid, Torus, Cone, Plane

### Colors (15+)
Red, Blue, Green, Yellow, Cyan, Magenta, White, Black, Gray, Orange, Purple, Pink, Brown, Gold, Silver

### Animations (6)
Rotation, Pulse, Bounce, Float, Orbit, Fall

### Materials (3)
Matte, Metallic, Glass

## 🎯 How to Use

### Web Chat Interface

1. Start server: `python run.py`
2. Open: http://localhost:5000/chat
3. Type description: "A red cube with blue sphere"
4. Click "Générer" to generate
5. See 3D rendered image!

### Command Line

```bash
python cli/main.py "A red cube"
python cli/main.py "A rotating sphere" --animate
python cli/main.py "A torus" --output scene.png
```

### API

```bash
curl -X POST http://localhost:5000/api/generate \
  -H "Content-Type: application/json" \
  -d '{"description": "A red cube"}'
```

## 🔧 Configuration

### Raytracer Integration

If you have compiled the raytracer C binary:

```bash
cd ../raytracer_c
make clean && make
```

The system will automatically detect and use it for high-quality rendering.

Path expected: `../raytracer_c/build/bin/raytracer`

If not available, falls back to WebGL rendering.

## 📚 API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/chat` | Chat interface |
| POST | `/api/generate` | Generate image from description |
| GET | `/api/raytracer/status` | Check raytracer availability |
| GET | `/api/health` | Health check |
| POST | `/api/scenes` | Create scene |
| GET | `/api/scenes` | List scenes |
| GET | `/api/scenes/{id}` | Get scene |
| DELETE | `/api/scenes/{id}` | Delete scene |

## 📝 Examples

Try these descriptions:

```
"Un cube bleu"
"Une sphère rouge qui tourne"
"Un cube rouge et une sphère bleue"
"Une pyramide dorée métallique"
"Un torus qui pulse"
"Trois cylindres jaunes alignés"
```

## 🐛 Troubleshooting

### Port 5000 already in use
```bash
python run.py --port 5001
```

### Module not found
```bash
# Ensure virtual environment is activated
source venv/bin/activate
pip install -r requirements.txt
```

### Raytracer not found
- Compile raytracer_c: `cd ../raytracer_c && make clean && make`
- System will fall back to WebGL if unavailable

## 🚀 Performance

- Single-threaded: ~24M pixels/sec
- Small scenes: <100ms
- Medium scenes: 100-500ms
- Complex scenes: 500ms+

## 📄 License

MIT License

---

**Happy generating! 🎨✨**
