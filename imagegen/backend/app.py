"""
Backend API Server - FastAPI
Serves generated scenes and handles exports
"""

from fastapi import FastAPI, HTTPException
from fastapi.responses import FileResponse, HTMLResponse, JSONResponse, RedirectResponse
from fastapi.middleware.cors import CORSMiddleware
import json
import uuid
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Optional
import os


app = FastAPI(
    title="ImageGen API",
    description="Generate 3D scenes from text descriptions",
    version="1.0.0"
)

# CORS setup
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Storage
scenes_dir = Path("./data/scenes")
scenes_dir.mkdir(parents=True, exist_ok=True)


# ============================================================================
# Models
# ============================================================================

class SceneData:
    """In-memory scene storage"""
    scenes: Dict[str, dict] = {}


# ============================================================================
# Routes - CHAT (with embedded HTML)
# ============================================================================

@app.get("/chat")
async def chat_page():
    """Chat interface with raytracer integration"""
    html = """<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🎨 ImageGen ChatBot</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 10px;
        }
        .container {
            width: 100%;
            max-width: 1000px;
            height: 90vh;
            background: white;
            border-radius: 15px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
            display: flex;
            flex-direction: column;
            overflow: hidden;
        }
        .header {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 20px;
            text-align: center;
        }
        .header h1 { font-size: 1.8em; margin-bottom: 5px; }
        .header p { font-size: 0.9em; opacity: 0.9; }
        .chat-container {
            flex: 1;
            overflow-y: auto;
            padding: 20px;
            display: flex;
            flex-direction: column;
            gap: 15px;
            background: #f8f9fa;
        }
        .message {
            display: flex;
            gap: 10px;
            animation: slideIn 0.3s ease-out;
        }
        @keyframes slideIn {
            from { opacity: 0; transform: translateY(10px); }
            to { opacity: 1; transform: translateY(0); }
        }
        .message.user { justify-content: flex-end; }
        .message.bot { justify-content: flex-start; }
        .message-avatar {
            width: 35px;
            height: 35px;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 1.5em;
            flex-shrink: 0;
        }
        .user .message-avatar { background: #667eea; color: white; }
        .bot .message-avatar { background: #764ba2; color: white; }
        .message-content {
            max-width: 70%;
            padding: 12px 15px;
            border-radius: 12px;
            word-wrap: break-word;
        }
        .user .message-content {
            background: #667eea;
            color: white;
            border-bottom-right-radius: 2px;
        }
        .bot .message-content {
            background: white;
            border: 1px solid #e0e0e0;
            border-bottom-left-radius: 2px;
        }
        .message-image {
            max-width: 100%;
            border-radius: 10px;
            margin-top: 10px;
            cursor: pointer;
            transition: transform 0.2s;
        }
        .message-image:hover { transform: scale(1.02); }
        .message-info {
            font-size: 0.8em;
            opacity: 0.7;
            margin-top: 5px;
        }
        .message-info a {
            color: #667eea;
            text-decoration: none;
            margin-left: 10px;
        }
        .message-info a:hover {
            text-decoration: underline;
        }
        .loading { display: flex; gap: 5px; align-items: center; }
        .loading-dot {
            width: 8px;
            height: 8px;
            border-radius: 50%;
            background: #667eea;
            animation: bounce 1.4s infinite;
        }
        .loading-dot:nth-child(2) { animation-delay: 0.2s; }
        .loading-dot:nth-child(3) { animation-delay: 0.4s; }
        @keyframes bounce {
            0%, 80%, 100% { opacity: 0.5; transform: translateY(0); }
            40% { opacity: 1; transform: translateY(-10px); }
        }
        .input-area {
            padding: 20px;
            border-top: 1px solid #e0e0e0;
            background: white;
            display: flex;
            gap: 10px;
        }
        .input-group { display: flex; gap: 10px; flex: 1; }
        textarea {
            flex: 1;
            padding: 12px;
            border: 2px solid #e0e0e0;
            border-radius: 10px;
            font-size: 1em;
            font-family: inherit;
            resize: none;
            max-height: 100px;
        }
        textarea:focus {
            outline: none;
            border-color: #667eea;
            background: #f8f9ff;
        }
        button {
            padding: 12px 25px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 10px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s;
            white-space: nowrap;
        }
        button:hover { transform: translateY(-2px); box-shadow: 0 10px 20px rgba(102, 126, 234, 0.3); }
        button:disabled { opacity: 0.5; cursor: not-allowed; }
        .examples {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 10px;
            margin-top: 10px;
        }
        .example-chip {
            padding: 8px 12px;
            background: #f0f0f0;
            border: 1px solid #ddd;
            border-radius: 20px;
            cursor: pointer;
            font-size: 0.85em;
        }
        .example-chip:hover {
            background: #e3f2fd;
            border-color: #667eea;
            color: #667eea;
        }
        .modal {
            display: none;
            position: fixed;
            z-index: 1000;
            left: 0;
            top: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.7);
            align-items: center;
            justify-content: center;
        }
        .modal.show { display: flex; }
        .modal-content {
            background: white;
            border-radius: 15px;
            padding: 20px;
            max-width: 90%;
            max-height: 90%;
            overflow: auto;
            position: relative;
            text-align: center;
        }
        .modal-content img {
            max-width: 100%;
            max-height: 80vh;
            border-radius: 10px;
        }
        .modal-buttons {
            margin-top: 15px;
            display: flex;
            gap: 10px;
            justify-content: center;
        }
        .modal-buttons a, .modal-buttons button {
            padding: 10px 20px;
            border-radius: 8px;
            text-decoration: none;
            display: inline-block;
        }
        .close-modal {
            position: absolute;
            top: 15px;
            right: 15px;
            font-size: 28px;
            font-weight: bold;
            cursor: pointer;
            color: #666;
        }
        .close-modal:hover { color: #000; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🎨 ImageGen ChatBot</h1>
            <p>Décrivez une image → Générez-la en 3D avec Raytracer</p>
        </div>

        <div class="chat-container" id="chatContainer">
            <div class="message bot">
                <div class="message-avatar">🤖</div>
                <div class="message-content">
                    Bonjour! 👋 Je suis votre assistant pour générer des images 3D.<br><br>
                    Décrivez l'image que vous voulez créer et je la génère avec le raytracer!<br><br>
                    <strong>Exemples:</strong>
                    <div class="examples">
                        <div class="example-chip" onclick="sendExample('Un cube bleu')">Un cube bleu</div>
                        <div class="example-chip" onclick="sendExample('Une sphère rouge qui tourne')">Sphère rotante</div>
                        <div class="example-chip" onclick="sendExample('Un cube rouge et une sphère bleue')">Cube + Sphère</div>
                        <div class="example-chip" onclick="sendExample('Une pyramide dorée')">Pyramide</div>
                    </div>
                </div>
            </div>
        </div>

        <div class="input-area">
            <div class="input-group">
                <textarea 
                    id="input" 
                    placeholder="Décrivez l'image à générer..."
                    onkeypress="if(event.key==='Enter' && !event.shiftKey) sendMessage()"
                ></textarea>
                <button onclick="sendMessage()">📤 Générer</button>
            </div>
        </div>
    </div>

    <div class="modal" id="imageModal" onclick="if(event.target === this) closeModal()">
        <div class="modal-content">
            <span class="close-modal" onclick="closeModal()">&times;</span>
            <img id="modalImage" src="" alt="Generated image">
            <div class="modal-buttons">
                <a id="downloadBtn" href="" download style="background: #667eea; color: white; padding: 10px 20px; border-radius: 8px; text-decoration: none;">📥 Télécharger</a>
            </div>
        </div>
    </div>

    <script>
        const chatContainer = document.getElementById('chatContainer');
        const input = document.getElementById('input');
        const imageModal = document.getElementById('imageModal');
        const modalImage = document.getElementById('modalImage');
        const downloadBtn = document.getElementById('downloadBtn');

        async function sendMessage() {
            const text = input.value.trim();
            if (!text) return;

            addMessage('user', text);
            input.value = '';
            input.focus();

            const typingId = addTyping();

            try {
                const response = await fetch('/api/generate', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ description: text })
                });

                const result = await response.json();
                removeTyping(typingId);

                if (result.success) {
                    let content = `✨ Scene generated!<br>`;
                    content += `<strong>Objects:</strong> ${result.objects_count}<br>`;
                    
                    if (result.image_url) {
                        content += `<img 
                            src="${result.image_url}" 
                            class="message-image" 
                            onclick="openModal('${result.image_url}', '${result.filename}')"
                            title="Click to enlarge"
                        >`;
                        content += `<div class="message-info">
                            📊 ${result.render_time}ms
                            <a href="${result.image_url}" download="${result.filename}">⬇️ Download</a>
                        </div>`;
                    }

                    addMessage('bot', content, true);
                } else {
                    addMessage('bot', `❌ Error: ${result.error}`);
                }
            } catch (error) {
                removeTyping(typingId);
                addMessage('bot', `❌ Error: ${error.message}`);
            }
        }

        function sendExample(text) {
            input.value = text;
            input.focus();
        }

        function addMessage(type, content, isHtml = false) {
            const div = document.createElement('div');
            div.className = `message ${type}`;
            
            const avatar = document.createElement('div');
            avatar.className = 'message-avatar';
            avatar.textContent = type === 'user' ? '👤' : '🤖';
            
            const contentDiv = document.createElement('div');
            contentDiv.className = 'message-content';
            if (isHtml) contentDiv.innerHTML = content;
            else contentDiv.textContent = content;
            
            div.appendChild(avatar);
            div.appendChild(contentDiv);
            chatContainer.appendChild(div);
            chatContainer.scrollTop = chatContainer.scrollHeight;
        }

        function addTyping() {
            const div = document.createElement('div');
            div.className = 'message bot';
            div.id = 'typing-' + Date.now();
            
            const avatar = document.createElement('div');
            avatar.className = 'message-avatar';
            avatar.textContent = '🤖';
            
            const contentDiv = document.createElement('div');
            contentDiv.className = 'message-content loading';
            contentDiv.innerHTML = '<div class="loading-dot"></div><div class="loading-dot"></div><div class="loading-dot"></div>';
            
            div.appendChild(avatar);
            div.appendChild(contentDiv);
            chatContainer.appendChild(div);
            chatContainer.scrollTop = chatContainer.scrollHeight;
            
            return div.id;
        }

        function removeTyping(id) {
            const el = document.getElementById(id);
            if (el) el.remove();
        }

        function openModal(src, filename) {
            modalImage.src = src;
            downloadBtn.href = src;
            downloadBtn.download = filename;
            imageModal.classList.add('show');
        }

        function closeModal() {
            imageModal.classList.remove('show');
        }

        document.addEventListener('keydown', (e) => {
            if (e.key === 'Escape') closeModal();
        });

        input.focus();
    </script>
</body>
</html>"""
    return HTMLResponse(html)


# ============================================================================
# Routes - Images
# ============================================================================

@app.get("/api/images/{filename}")
async def get_image(filename: str):
    """Serve generated images"""
    from backend.raytracer_integration import raytracer
    
    filepath = os.path.join(raytracer.output_dir, filename)
    
    if not os.path.exists(filepath):
        raise HTTPException(status_code=404, detail="Image not found")
    
    return FileResponse(filepath, media_type="image/x-portable-pixmap")


# ============================================================================
# Routes - Health
# ============================================================================

@app.get("/api/health")
async def health():
    """Health check endpoint"""
    return {
        "status": "ok",
        "timestamp": datetime.now().isoformat(),
        "service": "imagegen-api"
    }


# ============================================================================
# Routes - Generate (with Raytracer)
# ============================================================================

@app.post("/api/generate")
async def generate_image(request: dict):
    """Generate image from description"""
    try:
        import sys
        import os
        
        # Add backend to path
        backend_path = os.path.dirname(__file__)
        if backend_path not in sys.path:
            sys.path.insert(0, backend_path)
        
        from nlp_engine import NLPEngine
        from scene_generator import SceneGenerator
        from raytracer_integration import raytracer
        
        description = request.get('description', '')
        if not description:
            raise ValueError("Description required")
        
        # Parse with NLP
        nlp = NLPEngine()
        entities = nlp.parse_description(description)
        
        if not entities:
            return {"success": False, "error": "Could not parse description"}
        
        # Generate 3D scene
        scene_gen = SceneGenerator()
        scene = scene_gen.generate_scene(entities, animate=False)
        
        # Render with raytracer
        result = raytracer.generate(description, 800, 600)
        
        if result['success']:
            return {
                "success": True,
                "description": description,
                "objects_count": len(entities),
                "image_url": result['image_url'],
                "filename": result.get('filename', 'image.ppm'),
                "render_time": result['render_time'],
                "format": result['image_format'],
                "timestamp": result['timestamp']
            }
        else:
            return {"success": False, "error": result.get('error', 'Raytracer error')}
    
    except Exception as e:
        return {"success": False, "error": str(e)}


@app.get("/api/raytracer/status")
async def raytracer_status():
    """Check raytracer status"""
    import sys
    import os
    backend_path = os.path.dirname(__file__)
    if backend_path not in sys.path:
        sys.path.insert(0, backend_path)
    
    from raytracer_integration import get_raytracer_status
    return get_raytracer_status()


# ============================================================================
# Routes - Scenes (legacy)
# ============================================================================

@app.post("/api/scenes", status_code=201)
async def create_scene(scene: dict):
    """Create new scene"""
    scene_id = str(uuid.uuid4())[:8]
    scene["id"] = scene_id
    scene["created_at"] = datetime.now().isoformat()
    
    SceneData.scenes[scene_id] = scene
    
    scene_file = scenes_dir / f"{scene_id}.json"
    with open(scene_file, 'w') as f:
        json.dump(scene, f, indent=2)
    
    return {
        "id": scene_id,
        "name": scene.get("name", "Untitled"),
        "created_at": scene["created_at"],
        "url": f"/view/{scene_id}",
        "api_url": f"/api/scenes/{scene_id}"
    }


@app.get("/api/scenes")
async def list_scenes():
    """List all scenes"""
    scenes = []
    for scene_file in scenes_dir.glob("*.json"):
        with open(scene_file) as f:
            scene = json.load(f)
            scenes.append({
                "id": scene.get("id"),
                "name": scene.get("name"),
                "created_at": scene.get("created_at")
            })
    
    return {"scenes": scenes, "count": len(scenes)}


@app.get("/api/scenes/{scene_id}")
async def get_scene(scene_id: str):
    """Get scene by ID"""
    scene_file = scenes_dir / f"{scene_id}.json"
    
    if not scene_file.exists():
        raise HTTPException(status_code=404, detail="Scene not found")
    
    with open(scene_file) as f:
        return json.load(f)


@app.delete("/api/scenes/{scene_id}")
async def delete_scene(scene_id: str):
    """Delete scene"""
    scene_file = scenes_dir / f"{scene_id}.json"
    
    if not scene_file.exists():
        raise HTTPException(status_code=404, detail="Scene not found")
    
    scene_file.unlink()
    
    if scene_id in SceneData.scenes:
        del SceneData.scenes[scene_id]
    
    return {"deleted": scene_id}


@app.get("/view/{scene_id}")
async def view_scene(scene_id: str):
    """View scene"""
    scene_file = scenes_dir / f"{scene_id}.json"
    
    if not scene_file.exists():
        raise HTTPException(status_code=404, detail="Scene not found")
    
    with open(scene_file) as f:
        scene_data = json.load(f)
    
    html = f"<h1>{scene_data.get('name')}</h1><pre>{json.dumps(scene_data, indent=2)}</pre>"
    return HTMLResponse(html)


@app.get("/")
async def root():
    """Root endpoint"""
    return {
        "name": "ImageGen API",
        "version": "1.0.0",
        "docs": "/docs",
        "chat": "/chat"
    }


# ============================================================================
# Main
# ============================================================================

if __name__ == "__main__":
    import uvicorn
    
    print("🚀 Starting ImageGen API server...")
    print("📱 Docs: http://localhost:5000/docs")
    print("💬 Chat: http://localhost:5000/chat")
    print("🌐 API: http://localhost:5000/api/health")
    
    uvicorn.run(app, host="0.0.0.0", port=5000)