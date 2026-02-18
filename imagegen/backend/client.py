"""
API Client - Communicates with backend server
"""

import requests
import json
from typing import Dict, Any
from pathlib import Path
import sys


class APIClient:
    """Client for backend API"""
    
    def __init__(self, base_url: str = "http://localhost:5000"):
        """
        Initialize API client.
        
        Args:
            base_url: Base URL of backend server
        """
        self.base_url = base_url
        self.timeout = 30
        self._check_connection()
    
    def _check_connection(self):
        """Check if backend is running"""
        try:
            response = requests.get(
                f"{self.base_url}/api/health",
                timeout=2
            )
            if response.status_code == 200:
                print("✅ Connected to backend")
            else:
                self._warn_offline()
        except requests.exceptions.ConnectionError:
            self._warn_offline()
    
    def _warn_offline(self):
        """Warn if backend is offline"""
        print("⚠️  Backend server not running")
        print("   Start with: python backend/app.py")
        print("   Continuing in offline mode...")
    
    def create_scene(self, scene_data: Dict[str, Any]) -> Dict[str, Any]:
        """
        Create scene on backend.
        
        Args:
            scene_data: Scene dictionary
            
        Returns:
            Scene with ID and metadata
        """
        try:
            response = requests.post(
                f"{self.base_url}/api/scenes",
                json=scene_data,
                timeout=self.timeout
            )
            
            if response.status_code == 201:
                return response.json()
            else:
                print(f"⚠️  Server returned: {response.status_code}")
                return scene_data  # Return locally
        except requests.exceptions.ConnectionError:
            print("⚠️  Server offline, using local scene")
            return scene_data
        except Exception as e:
            print(f"⚠️  API error: {e}")
            return scene_data
    
    def get_scene(self, scene_id: str) -> Dict[str, Any]:
        """Get scene by ID"""
        try:
            response = requests.get(
                f"{self.base_url}/api/scenes/{scene_id}",
                timeout=self.timeout
            )
            return response.json()
        except:
            return None
    
    def export_png(self, scene_id: str, output_path: str):
        """Export scene as PNG"""
        try:
            response = requests.post(
                f"{self.base_url}/api/scenes/{scene_id}/export",
                json={"format": "png"},
                timeout=60
            )
            
            if response.status_code == 200:
                with open(output_path, 'wb') as f:
                    f.write(response.content)
                print(f"✅ Exported to {output_path}")
            else:
                print(f"❌ Export failed: {response.status_code}")
        except Exception as e:
            print(f"❌ Export error: {e}")
    
    def export_html(self, scene_id: str, output_path: str):
        """Export scene as standalone HTML"""
        try:
            response = requests.post(
                f"{self.base_url}/api/scenes/{scene_id}/export",
                json={"format": "html"},
                timeout=30
            )
            
            if response.status_code == 200:
                with open(output_path, 'w') as f:
                    f.write(response.text)
                print(f"✅ Exported to {output_path}")
            else:
                print(f"❌ Export failed: {response.status_code}")
        except Exception as e:
            print(f"❌ Export error: {e}")
    
    def list_scenes(self) -> list:
        """List all scenes"""
        try:
            response = requests.get(
                f"{self.base_url}/api/scenes",
                timeout=self.timeout
            )
            return response.json()
        except:
            return []
    
    def delete_scene(self, scene_id: str):
        """Delete scene"""
        try:
            response = requests.delete(
                f"{self.base_url}/api/scenes/{scene_id}",
                timeout=self.timeout
            )
            if response.status_code == 200:
                print(f"✅ Scene deleted: {scene_id}")
            return response.status_code == 200
        except:
            return False


# Usage example
if __name__ == "__main__":
    client = APIClient()
    
    # Create a test scene
    test_scene = {
        "name": "Test Scene",
        "scene": {
            "objects": [
                {
                    "name": "test_cube",
                    "type": "mesh",
                    "geometry": {"type": "BoxGeometry", "width": 1, "height": 1, "depth": 1},
                    "material": {"type": "MeshStandardMaterial", "color": "#ff0000"},
                    "position": [0, 0, 0]
                }
            ],
            "lights": [],
            "camera": {"position": [0, 0, 5]}
        }
    }
    
    result = client.create_scene(test_scene)
    print(f"Created scene: {result.get('id')}")
