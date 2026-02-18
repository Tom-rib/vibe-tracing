"""
Tests pour ImageGen
Run: pytest tests/
"""

import sys
from pathlib import Path

# Add parent to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from backend.nlp_engine import NLPEngine, ColorName, ShapeType
from backend.scene_generator import SceneGenerator


class TestNLPEngine:
    """Test NLP engine"""
    
    def setup_method(self):
        """Setup for each test"""
        self.nlp = NLPEngine()
    
    def test_parse_simple_cube(self):
        """Test parsing simple cube"""
        result = self.nlp.parse_description("A red cube")
        assert len(result) == 1
        assert result[0].type == ShapeType.CUBE
        assert result[0].color == ColorName.RED.value
    
    def test_parse_simple_sphere(self):
        """Test parsing simple sphere"""
        result = self.nlp.parse_description("A blue sphere")
        assert len(result) == 1
        assert result[0].type == ShapeType.SPHERE
        assert result[0].color == ColorName.BLUE.value
    
    def test_parse_multiple_objects(self):
        """Test parsing multiple objects"""
        result = self.nlp.parse_description("A red cube and a blue sphere")
        assert len(result) == 2
        assert result[0].type == ShapeType.CUBE
        assert result[1].type == ShapeType.SPHERE
    
    def test_parse_with_animation(self):
        """Test parsing with animation"""
        result = self.nlp.parse_description("A rotating sphere")
        assert len(result) == 1
        assert result[0].animation == "rotation"
    
    def test_parse_with_material(self):
        """Test parsing with material"""
        result = self.nlp.parse_description("A metallic cube")
        assert len(result) == 1
        assert result[0].material == "metallic"
    
    def test_french_descriptions(self):
        """Test French descriptions"""
        result = self.nlp.parse_description("Un cube rouge")
        assert len(result) == 1
        assert result[0].type == ShapeType.CUBE
        assert result[0].color == ColorName.RED.value


class TestSceneGenerator:
    """Test scene generator"""
    
    def setup_method(self):
        """Setup for each test"""
        self.nlp = NLPEngine()
        self.gen = SceneGenerator()
    
    def test_generate_scene(self):
        """Test scene generation"""
        entities = self.nlp.parse_description("A blue sphere")
        scene = self.gen.generate_scene(entities)
        
        assert scene["id"] is not None
        assert len(scene["scene"]["objects"]) == 1
        assert len(scene["scene"]["lights"]) > 0
        assert scene["scene"]["camera"] is not None
    
    def test_generate_animated_scene(self):
        """Test animated scene generation"""
        entities = self.nlp.parse_description("A rotating cube")
        scene = self.gen.generate_scene(entities, animate=True)
        
        assert len(scene["scene"]["objects"]) == 1
        obj = scene["scene"]["objects"][0]
        assert "animation" in obj
        assert obj["animation"]["type"] == "rotation"
    
    def test_multiple_objects_scene(self):
        """Test scene with multiple objects"""
        entities = self.nlp.parse_description("A red cube and a blue sphere")
        scene = self.gen.generate_scene(entities)
        
        assert len(scene["scene"]["objects"]) == 2
        
        # Check positions are different
        pos1 = scene["scene"]["objects"][0]["position"]
        pos2 = scene["scene"]["objects"][1]["position"]
        assert pos1 != pos2


class TestIntegration:
    """Integration tests"""
    
    def test_full_pipeline(self):
        """Test full pipeline from description to scene"""
        nlp = NLPEngine()
        gen = SceneGenerator()
        
        description = "A rotating golden pyramid with a blue sphere"
        
        # Parse
        entities = nlp.parse_description(description)
        assert len(entities) == 2
        
        # Generate
        scene = gen.generate_scene(entities, animate=True)
        
        # Validate
        assert scene["id"] is not None
        assert len(scene["scene"]["objects"]) == 2
        assert scene["options"]["animate"] == True
        
        # Check pyramid
        pyramid = scene["scene"]["objects"][0]
        assert pyramid["animation"]["type"] == "rotation"
        assert "golden" in pyramid["name"].lower() or pyramid["material"]["color"] == "#ffd700"
        
        # Check sphere
        sphere = scene["scene"]["objects"][1]
        assert sphere["color"] == ColorName.BLUE.value


# Run tests
if __name__ == "__main__":
    import pytest
    pytest.main([__file__, "-v"])
