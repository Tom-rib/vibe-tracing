"""
Scene Generator - Converts entities into 3D scenes for Three.js
"""

from typing import List, Dict, Any
from datetime import datetime
import uuid
from backend.nlp_engine import Entity, ShapeType


class SceneGenerator:
    """Generates Three.js compatible 3D scenes"""
    
    def __init__(self):
        """Initialize scene generator"""
        self.scene_id = str(uuid.uuid4())[:8]
    
    def generate_scene(self, entities: List[Entity], animate: bool = False,
                      interactive: bool = True) -> Dict[str, Any]:
        """
        Generate complete scene from entities.
        
        Args:
            entities: List of detected entities
            animate: Enable animations
            interactive: Enable interactive controls
            
        Returns:
            Scene dictionary compatible with Three.js
        """
        scene_data = {
            "id": self.scene_id,
            "name": f"Generated Scene {datetime.now().strftime('%Y-%m-%d %H:%M')}",
            "timestamp": datetime.now().isoformat(),
            "scene": {
                "background": [0.1, 0.1, 0.15],
                "fog": {
                    "enabled": True,
                    "color": [0.1, 0.1, 0.15],
                    "near": 1,
                    "far": 100
                },
                "objects": [],
                "lights": [],
                "camera": {
                    "position": [0, 5, 15],
                    "lookAt": [0, 0, 0],
                    "fov": 60
                }
            },
            "options": {
                "animate": animate,
                "interactive": interactive,
                "autoRotate": animate
            }
        }
        
        # Add objects
        for i, entity in enumerate(entities):
            obj = self._entity_to_threejs(entity)
            scene_data["scene"]["objects"].append(obj)
        
        # Add lighting
        scene_data["scene"]["lights"] = self._generate_lighting()
        
        # Adjust camera if many objects
        if len(entities) > 5:
            scene_data["scene"]["camera"]["position"] = [0, 10, 25]
        
        return scene_data
    
    def _entity_to_threejs(self, entity: Entity) -> Dict[str, Any]:
        """Convert entity to Three.js object"""
        
        # Get geometry data
        geometry = self._get_geometry(entity.type)
        
        # Base object
        obj = {
            "id": f"obj_{uuid.uuid4()}",
            "name": entity.name,
            "type": "mesh",
            "geometry": geometry,
            "material": self._get_material(entity.color, entity.material),
            "position": list(entity.position),
            "rotation": list(entity.rotation),
            "scale": [entity.scale] * 3,
            "castShadow": True,
            "receiveShadow": True
        }
        
        # Add animation if specified
        if entity.animation:
            obj["animation"] = self._get_animation(entity.animation)
        
        return obj
    
    def _get_geometry(self, shape_type: ShapeType) -> Dict[str, Any]:
        """Get geometry parameters for shape"""
        
        geometries = {
            ShapeType.CUBE: {
                "type": "BoxGeometry",
                "width": 1,
                "height": 1,
                "depth": 1
            },
            ShapeType.SPHERE: {
                "type": "SphereGeometry",
                "radius": 0.7,
                "widthSegments": 32,
                "heightSegments": 32
            },
            ShapeType.CYLINDER: {
                "type": "CylinderGeometry",
                "radiusTop": 0.5,
                "radiusBottom": 0.5,
                "height": 2,
                "radialSegments": 32
            },
            ShapeType.PYRAMID: {
                "type": "ConeGeometry",
                "radius": 0.7,
                "height": 1.5,
                "radialSegments": 4
            },
            ShapeType.TORUS: {
                "type": "TorusGeometry",
                "radius": 0.7,
                "tube": 0.3,
                "radialSegments": 16,
                "tubularSegments": 100
            },
            ShapeType.CONE: {
                "type": "ConeGeometry",
                "radius": 0.7,
                "height": 1.5,
                "radialSegments": 32
            },
            ShapeType.PLANE: {
                "type": "PlaneGeometry",
                "width": 2,
                "height": 2
            }
        }
        
        return geometries.get(shape_type, geometries[ShapeType.CUBE])
    
    def _get_material(self, color: tuple, material_type: str) -> Dict[str, Any]:
        """Get material properties"""
        
        materials = {
            "matte": {
                "type": "MeshStandardMaterial",
                "color": self._float_to_hex(color),
                "metalness": 0.0,
                "roughness": 0.8,
                "side": "front"
            },
            "metallic": {
                "type": "MeshStandardMaterial",
                "color": self._float_to_hex(color),
                "metalness": 1.0,
                "roughness": 0.2,
                "side": "front"
            },
            "glass": {
                "type": "MeshStandardMaterial",
                "color": self._float_to_hex(color),
                "metalness": 0.0,
                "roughness": 0.1,
                "transparent": True,
                "opacity": 0.7,
                "side": "double"
            }
        }
        
        return materials.get(material_type, materials["matte"])
    
    def _get_animation(self, animation_type: str) -> Dict[str, Any]:
        """Get animation definition"""
        
        animations = {
            "rotation": {
                "type": "rotation",
                "axis": "y",
                "speed": 0.01,
                "direction": 1
            },
            "pulse": {
                "type": "scale",
                "axis": "all",
                "amplitude": 0.2,
                "frequency": 2.0
            },
            "bounce": {
                "type": "position",
                "axis": "y",
                "amplitude": 0.5,
                "frequency": 2.0
            },
            "float": {
                "type": "position",
                "axis": "y",
                "amplitude": 0.3,
                "frequency": 0.5
            },
            "orbit": {
                "type": "orbit",
                "radius": 5,
                "speed": 0.01,
                "axis": "y"
            },
            "fall": {
                "type": "position",
                "axis": "y",
                "direction": -1,
                "speed": 0.05,
                "damping": 0.98
            }
        }
        
        return animations.get(animation_type, animations["rotation"])
    
    def _generate_lighting(self) -> List[Dict[str, Any]]:
        """Generate default lighting setup"""
        return [
            {
                "id": "light_main",
                "type": "DirectionalLight",
                "color": [1, 1, 1],
                "intensity": 1.0,
                "position": [5, 10, 7],
                "castShadow": True,
                "shadow": {
                    "mapSize": 2048,
                    "camera": {
                        "left": -20,
                        "right": 20,
                        "top": 20,
                        "bottom": -20,
                        "near": 0.5,
                        "far": 50
                    }
                }
            },
            {
                "id": "light_fill",
                "type": "DirectionalLight",
                "color": [0.8, 0.8, 1],
                "intensity": 0.5,
                "position": [-5, 3, -7]
            },
            {
                "id": "light_ambient",
                "type": "AmbientLight",
                "color": [1, 1, 1],
                "intensity": 0.3
            }
        ]
    
    def _float_to_hex(self, color: tuple) -> str:
        """Convert float RGB [0-1] to hex color"""
        r, g, b = color
        r_hex = int(round(r * 255))
        g_hex = int(round(g * 255))
        b_hex = int(round(b * 255))
        return f"#{r_hex:02x}{g_hex:02x}{b_hex:02x}"


# Example usage
if __name__ == "__main__":
    from nlp_engine import NLPEngine
    
    nlp = NLPEngine()
    gen = SceneGenerator()
    
    # Test scene generation
    description = "A red cube with a blue sphere rotating"
    entities = nlp.parse_description(description)
    
    print(f"📝 Description: {description}")
    print(f"✅ Detected {len(entities)} entities")
    
    scene = gen.generate_scene(entities, animate=True)
    
    print(f"\n🎨 Generated scene:")
    print(f"   ID: {scene['id']}")
    print(f"   Objects: {len(scene['scene']['objects'])}")
    print(f"   Lights: {len(scene['scene']['lights'])}")
    print(f"   Animations: {scene['options']['animate']}")
    
    # Print first object
    if scene['scene']['objects']:
        obj = scene['scene']['objects'][0]
        print(f"\n📦 First object:")
        print(f"   Name: {obj['name']}")
        print(f"   Type: {obj['geometry']['type']}")
        print(f"   Position: {obj['position']}")
        if 'animation' in obj:
            print(f"   Animation: {obj['animation']['type']}")
