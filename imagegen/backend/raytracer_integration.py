"""
Raytracer Integration - Generate images using the C raytracer
"""

import subprocess
import os
import base64
import json
import shutil
from pathlib import Path
from datetime import datetime
import tempfile
import time


class RaytracerIntegration:
    """Interface avec le raytracer C"""
    
    def __init__(self, raytracer_path: str = None):
        """Initialize raytracer integration."""
        if raytracer_path is None:
            possible_paths = [
                '/mnt/c/Users/Tom/Documents/Github/BTP B2/minivibes/vibe-tracing/raytracer_c/build/bin/raytracer',
                '../raytracer_c/build/bin/raytracer',
                './raytracer_c/build/bin/raytracer',
                '../../raytracer_c/build/bin/raytracer',
                os.path.expanduser('~/projects/raytracer_c/build/bin/raytracer'),
            ]
            
            for path in possible_paths:
                abs_path = os.path.abspath(path)
                if os.path.exists(abs_path):
                    raytracer_path = abs_path
                    print(f"✅ Raytracer found at: {raytracer_path}")
                    break
        
        self.raytracer_path = raytracer_path
        self.available = raytracer_path is not None and os.path.exists(raytracer_path)
        
        # Create output directory if it doesn't exist
        self.output_dir = os.path.join(os.path.dirname(__file__), '..', 'output_images')
        os.makedirs(self.output_dir, exist_ok=True)
        
        if not self.available:
            print(f"⚠️ Raytracer not found")
    
    def _generate_fallback(self, description: str, width: int = 800, height: int = 600) -> dict:
        """Generate fallback gradient image"""
        
        ppm_lines = ["P6", f"{width} {height}", "255"]
        pixels = []
        
        for y in range(height):
            for x in range(width):
                ratio = y / height
                r = int(0 + (200 * ratio))
                g = int(200 * (1 - ratio * 0.5))
                b = int(255 - (100 * ratio))
                pixels.extend([r, g, b])
        
        ppm_data = "\n".join(ppm_lines).encode() + b"\n" + bytes(pixels)
        
        # Save to output directory
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S_%f")
        filename = f"fallback_{timestamp}.ppm"
        filepath = os.path.join(self.output_dir, filename)
        
        with open(filepath, 'wb') as f:
            f.write(ppm_data)
        
        return {
            'success': True,
            'image_url': f'/api/images/{filename}',
            'filename': filename,
            'image_format': 'ppm',
            'render_time': 10,
            'width': width,
            'height': height,
            'description': description,
            'timestamp': datetime.now().isoformat(),
            'note': '⚠️ Fallback preview'
        }
    
    def generate(self, description: str, width: int = 800, height: int = 600) -> dict:
        """Generate image using raytracer."""
        
        if not self.available:
            print("⚠️ Raytracer not available, using fallback")
            return self._generate_fallback(description, width, height)
        
        try:
            raytracer_dir = os.path.dirname(self.raytracer_path)
            output_ppm = os.path.join(raytracer_dir, 'output.ppm')
            
            print(f"🎨 Running raytracer: {self.raytracer_path}")
            print(f"📊 Output: {width}x{height}")
            
            start_time = time.time()
            
            result = subprocess.run(
                [self.raytracer_path, 'output.ppm', str(width), str(height)],
                capture_output=True,
                text=True,
                timeout=30,
                cwd=raytracer_dir
            )
            
            render_time = time.time() - start_time
            
            if result.returncode != 0:
                print(f"❌ Raytracer error: {result.stderr}")
                return {
                    'success': False,
                    'error': result.stderr or 'Raytracer failed'
                }
            
            # Check if file was created
            if not os.path.exists(output_ppm):
                print(f"❌ PPM file not found at {output_ppm}")
                return {
                    'success': False,
                    'error': 'PPM file not generated'
                }
            
            # Copy to output directory with unique name
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S_%f")
            filename = f"render_{timestamp}.ppm"
            dest_path = os.path.join(self.output_dir, filename)
            
            shutil.copy2(output_ppm, dest_path)
            print(f"✅ Image saved to: {dest_path}")
            
            return {
                'success': True,
                'image_url': f'/api/images/{filename}',
                'filename': filename,
                'image_format': 'ppm',
                'render_time': int(render_time * 1000),
                'width': width,
                'height': height,
                'description': description,
                'timestamp': datetime.now().isoformat()
            }
        
        except subprocess.TimeoutExpired:
            print("❌ Raytracer timeout")
            return {'success': False, 'error': 'Raytracer timeout'}
        except Exception as e:
            print(f"❌ Error: {str(e)}")
            return {'success': False, 'error': str(e)}


raytracer = RaytracerIntegration()


def get_raytracer_status() -> dict:
    """Get raytracer status"""
    return {
        'available': raytracer.available,
        'path': raytracer.raytracer_path if raytracer.available else None,
        'message': '✅ Raytracer ready' if raytracer.available else '⚠️ Using fallback',
        'output_dir': raytracer.output_dir
    }