# 🎨 ImageGen - Text to 3D with Raytracer

Generate 3D images from text descriptions using NLP and raytracing.

## 🚀 Quick Start (3 commands)

```bash
pip install -r requirements.txt
python backend/app.py
# Then open: http://localhost:5000/chat
```

## 📁 Project Structure

```
imagegen-FINAL/
├── backend/
│   ├── app.py                    # FastAPI server + chat
│   ├── nlp_engine.py             # NLP parser
│   ├── scene_generator.py        # 3D scene generator
│   ├── raytracer_integration.py  # Raytracer integration
│   ├── client.py
│   └── __init__.py
├── cli/
│   └── main.py                   # Command-line interface
├── frontend/
│   ├── chat.html                 # Chat interface
│   └── index.html                # 3D viewer
├── requirements.txt
└── README.md
```

## ✨ Features

- 💬 Chat interface for describing images
- 🧠 NLP engine (shapes, colors, animations, materials)
- 🎨 3D scene generation
- 🚀 Raytracer C integration for high-quality rendering
- 🌐 WebGL fallback viewer

## 🎯 How to Use

1. Run: `python backend/app.py`
2. Open: http://localhost:5000/chat
3. Describe an image: "Un cube bleu"
4. Click "Générer" to generate
5. See the rendered result!

## 📊 Supported

**Shapes**: Cube, Sphere, Cylinder, Pyramid, Torus, Cone, Plane

**Colors**: 15+ colors (red, blue, green, etc.)

**Animations**: Rotation, pulse, bounce, float, orbit, fall

**Materials**: Matte, metallic, glass

## 🔧 Requirements

- Python 3.9+
- pip

## 📝 Examples

```
"Un cube bleu"
"Une sphère rouge qui tourne"
"Un cube rouge et une sphère bleue"
"Une pyramide dorée métallique"
"Un torus qui pulse"
```

## 🎉 Enjoy!

Happy generating! 🚀
