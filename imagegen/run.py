#!/usr/bin/env python3
"""
ImageGen API Server Launcher
Run: python run.py
"""
import sys
import os
sys.path.insert(0, os.path.dirname(__file__))

from backend.app import app
import uvicorn

if __name__ == "__main__":
    print("🚀 Starting ImageGen API server...")
    print("📱 Docs: http://localhost:5000/docs")
    print("💬 Chat: http://localhost:5000/chat")
    print("🌐 API: http://localhost:5000/api/health")
    uvicorn.run(app, host="0.0.0.0", port=5000)
