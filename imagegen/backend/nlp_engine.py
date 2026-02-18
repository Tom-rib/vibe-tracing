"""
NLP Engine - Parses text descriptions into structured entities
Uses simple keyword matching and rule-based approach (no heavy ML required)
"""

import re
from dataclasses import dataclass
from typing import List, Dict, Tuple
from enum import Enum


class ShapeType(Enum):
    """Supported 3D shapes"""
    CUBE = "cube"
    SPHERE = "sphere"
    CYLINDER = "cylinder"
    PYRAMID = "pyramid"
    TORUS = "torus"
    CONE = "cone"
    PLANE = "plane"


class ColorName(Enum):
    """Named colors"""
    RED = (1, 0, 0)
    BLUE = (0, 0, 1)
    GREEN = (0, 1, 0)
    YELLOW = (1, 1, 0)
    CYAN = (0, 1, 1)
    MAGENTA = (1, 0, 1)
    WHITE = (1, 1, 1)
    BLACK = (0, 0, 0)
    GRAY = (0.5, 0.5, 0.5)
    ORANGE = (1, 0.5, 0)
    PURPLE = (0.5, 0, 0.5)
    PINK = (1, 0.5, 0.5)
    BROWN = (0.6, 0.3, 0)
    GOLD = (1, 0.84, 0)
    SILVER = (0.75, 0.75, 0.75)


@dataclass
class Entity:
    """Represents a detected object in the scene"""
    name: str
    type: ShapeType
    color: Tuple[float, float, float]
    position: Tuple[float, float, float] = (0, 0, 0)
    scale: float = 1.0
    rotation: Tuple[float, float, float] = (0, 0, 0)
    animation: str = None
    material: str = "matte"  # matte, metallic, glass
    
    def to_dict(self) -> dict:
        """Convert to dictionary"""
        return {
            "name": self.name,
            "type": self.type.value,
            "color": list(self.color),
            "position": list(self.position),
            "scale": self.scale,
            "rotation": list(self.rotation),
            "animation": self.animation,
            "material": self.material
        }


class NLPEngine:
    """Natural Language Processor for scene descriptions"""
    
    # Shape keywords mapping
    SHAPE_KEYWORDS = {
        "cube": ShapeType.CUBE,
        "box": ShapeType.CUBE,
        "square": ShapeType.CUBE,
        "sphere": ShapeType.SPHERE,
        "ball": ShapeType.SPHERE,
        "round": ShapeType.SPHERE,
        "cylinder": ShapeType.CYLINDER,
        "tube": ShapeType.CYLINDER,
        "pyramid": ShapeType.PYRAMID,
        "torus": ShapeType.TORUS,
        "donut": ShapeType.TORUS,
        "cone": ShapeType.CONE,
        "plane": ShapeType.PLANE,
        "flat": ShapeType.PLANE,
    }
    
    # Color keywords mapping
    COLOR_KEYWORDS = {
        "red": ColorName.RED,
        "blue": ColorName.BLUE,
        "green": ColorName.GREEN,
        "yellow": ColorName.YELLOW,
        "cyan": ColorName.CYAN,
        "magenta": ColorName.MAGENTA,
        "white": ColorName.WHITE,
        "black": ColorName.BLACK,
        "gray": ColorName.GRAY,
        "grey": ColorName.GRAY,
        "orange": ColorName.ORANGE,
        "purple": ColorName.PURPLE,
        "pink": ColorName.PINK,
        "brown": ColorName.BROWN,
        "gold": ColorName.GOLD,
        "golden": ColorName.GOLD,
        "silver": ColorName.SILVER,
        "dark": ColorName.BLACK,
        "light": ColorName.WHITE,
    }
    
    # Animation keywords
    ANIMATION_KEYWORDS = {
        "rotate": "rotation",
        "rotating": "rotation",
        "spin": "rotation",
        "spinning": "rotation",
        "pulse": "pulse",
        "pulsing": "pulse",
        "bounce": "bounce",
        "bouncing": "bounce",
        "float": "float",
        "floating": "float",
        "orbit": "orbit",
        "orbiting": "orbit",
        "fall": "fall",
        "falling": "fall",
    }
    
    def __init__(self):
        """Initialize NLP engine"""
        self.position_counter = 0
    
    def parse_description(self, description: str) -> List[Entity]:
        """
        Parse natural language description into entities.
        
        Args:
            description: Text description (e.g., "A red cube with a blue sphere")
            
        Returns:
            List of detected entities
        """
        entities = []
        self.position_counter = 0
        
        # Clean text
        text = description.lower().strip()
        
        # Split by connectors (and, with, next to, etc.)
        object_phrases = self._split_by_connectors(text)
        
        for phrase in object_phrases:
            entity = self._parse_entity(phrase)
            if entity:
                # Auto-position objects
                entity.position = self._get_next_position()
                entities.append(entity)
        
        return entities
    
    def _split_by_connectors(self, text: str) -> List[str]:
        """Split description by logical connectors"""
        connectors = [
            r'\band\b',
            r'\bwith\b',
            r'\bnext\s+to\b',
            r'\bbeside\b',
            r',',
        ]
        
        pattern = '|'.join(connectors)
        phrases = re.split(pattern, text)
        
        return [p.strip() for p in phrases if p.strip()]
    
    def _parse_entity(self, phrase: str) -> Entity:
        """Parse a single entity from a phrase"""
        phrase = phrase.strip()
        
        # Extract shape type
        shape_type = self._extract_shape(phrase)
        if not shape_type:
            return None
        
        # Extract color
        color = self._extract_color(phrase)
        if not color:
            color = self._get_default_color()
        
        # Extract animation
        animation = self._extract_animation(phrase)
        
        # Extract material
        material = self._extract_material(phrase)
        
        # Generate name
        name = self._generate_name(shape_type, color)
        
        return Entity(
            name=name,
            type=shape_type,
            color=color.value,
            animation=animation,
            material=material
        )
    
    def _extract_shape(self, text: str) -> ShapeType:
        """Extract shape type from text"""
        text_lower = text.lower()
        
        for keyword, shape in self.SHAPE_KEYWORDS.items():
            if keyword in text_lower:
                return shape
        
        return None
    
    def _extract_color(self, text: str) -> ColorName:
        """Extract color from text"""
        text_lower = text.lower()
        
        for keyword, color in self.COLOR_KEYWORDS.items():
            if keyword in text_lower:
                return color
        
        return None
    
    def _extract_animation(self, text: str) -> str:
        """Extract animation type from text"""
        text_lower = text.lower()
        
        for keyword, animation in self.ANIMATION_KEYWORDS.items():
            if keyword in text_lower:
                return animation
        
        return None
    
    def _extract_material(self, text: str) -> str:
        """Extract material type from text"""
        text_lower = text.lower()
        
        if any(word in text_lower for word in ["metallic", "metal", "shiny"]):
            return "metallic"
        elif any(word in text_lower for word in ["glass", "transparent", "clear"]):
            return "glass"
        elif any(word in text_lower for word in ["matte", "dull", "rough"]):
            return "matte"
        
        return "matte"
    
    def _get_default_color(self) -> ColorName:
        """Get default color (white)"""
        return ColorName.WHITE
    
    def _generate_name(self, shape: ShapeType, color: ColorName) -> str:
        """Generate descriptive name for entity"""
        return f"{color.name.lower()} {shape.value}"
    
    def _get_next_position(self) -> Tuple[float, float, float]:
        """Get next position in line (for auto-layout)"""
        # Space objects evenly along X axis
        x = (self.position_counter - 1) * 3
        self.position_counter += 1
        return (x, 0, 0)


# Example usage and tests
if __name__ == "__main__":
    nlp = NLPEngine()
    
    # Test descriptions
    test_cases = [
        "A red cube",
        "A blue sphere with a red cube",
        "Three cylinders rotating",
        "A metallic golden pyramid and a glass blue sphere",
        "A spinning purple torus next to a green cube",
    ]
    
    for description in test_cases:
        print(f"\n📝 Input: '{description}'")
        entities = nlp.parse_description(description)
        
        print(f"✅ Found {len(entities)} entities:")
        for entity in entities:
            print(f"   - {entity.name}: {entity.type.value}")
            print(f"     Color: {entity.color}, Material: {entity.material}")
            if entity.animation:
                print(f"     Animation: {entity.animation}")
