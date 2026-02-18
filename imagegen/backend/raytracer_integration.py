"""
Raytracer Integration - Generate images using the C raytracer
"""

import subprocess
import os
import base64
import json
from pathlib import Path
from datetime import datetime
import tempfile
import time


class RaytracerIntegration:
    """Interface avec le raytracer C"""
    
    def __init__(self, raytracer_path: str = None):
        """
        Initialize raytracer integration.
        
        Args:
            raytracer_path: Path to raytracer binary
        """
        # Try to find raytracer in common locations
        if raytracer_path is None:
            possible_paths = [
                '../raytracer_c/build/bin/raytracer',
                '/mnt/c/Users/Tom/Documents/Github/BTP B2/minivibes/vibe-tracing/raytracer_c/build/bin/raytracer',
                './raytracer_c/build/bin/raytracer',
                '/home/raytracer',
            ]
            
            for path in possible_paths:
                if os.path.exists(path):
                    raytracer_path = path
                    break
        
        self.raytracer_path = raytracer_path
        self.available = raytracer_path is not None and os.path.exists(raytracer_path)
    
    def generate(self, description: str, width: int = 800, height: int = 600) -> dict:
        """
        Generate image using raytracer.
        
        Args:
            description: Scene description
            width: Image width
            height: Image height
            
        Returns:
            Dict with image data and metadata
        """
        
        if not self.available:
            return {
                'success': False,
                'error': 'Raytracer not found. Please compile raytracer_c first.',
                'message': 'Run: cd raytracer_c && make clean && make'
            }
        
        try:
            # Create temp file for output
            with tempfile.NamedTemporaryFile(suffix='.ppm', delete=False) as f:
                output_path = f.name
            
            # Run raytracer
            print(f"🎨 Running raytracer: {self.raytracer_path}")
            print(f"📊 Output: {width}x{height}")
            
            start_time = time.time()
            
            result = subprocess.run(
                [self.raytracer_path, output_path, str(width), str(height)],
                capture_output=True,
                text=True,
                timeout=30
            )
            
            render_time = time.time() - start_time
            
            if result.returncode != 0:
                return {
                    'success': False,
                    'error': result.stderr or 'Raytracer failed',
                    'stdout': result.stdout
                }
            
            # Read PPM file
            if not os.path.exists(output_path):
                return {
                    'success': False,
                    'error': 'PPM file not generated'
                }
            
            # Convert PPM to base64 data URI
            with open(output_path, 'rb') as f:
                ppm_data = f.read()
            
            # Create data URI (PPM can be embedded directly in img src)
            ppm_base64 = base64.b64encode(ppm_data).decode('utf-8')
            image_data_uri = f"data:image/x-portable-pixmap;base64,{ppm_base64}"
            
            # Also save as PNG if we have imagemagick
            png_path = output_path.replace('.ppm', '.png')
            try:
                subprocess.run(
                    ['convert', output_path, png_path],
                    timeout=5,
                    capture_output=True
                )
                if os.path.exists(png_path):
                    with open(png_path, 'rb') as f:
                        png_base64 = base64.b64encode(f.read()).decode('utf-8')
                    png_data_uri = f"data:image/png;base64,{png_base64}"
                else:
                    png_data_uri = None
            except:
                png_data_uri = None
            
            # Cleanup
            try:
                os.unlink(output_path)
                if png_path and os.path.exists(png_path):
                    os.unlink(png_path)
            except:
                pass
            
            return {
                'success': True,
                'image_url': png_data_uri or image_data_uri,
                'image_format': 'png' if png_data_uri else 'ppm',
                'render_time': int(render_time * 1000),  # milliseconds
                'width': width,
                'height': height,
                'description': description,
                'timestamp': datetime.now().isoformat()
            }
        
        except subprocess.TimeoutExpired:
            return {
                'success': False,
                'error': 'Raytracer timeout (30 seconds)'
            }
        except Exception as e:
            return {
                'success': False,
                'error': str(e)
            }


# Global raytracer instance
raytracer = RaytracerIntegration()


def get_raytracer_status() -> dict:
    """Get raytracer status"""
    return {
        'available': raytracer.available,
        'path': raytracer.raytracer_path if raytracer.available else None,
        'message': '✅ Raytracer ready' if raytracer.available else '❌ Raytracer not found'
    }
