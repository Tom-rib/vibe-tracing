#!/usr/bin/env python3
"""
imagegen - Image Generator from Text Descriptions
Generates 3D scenes from natural language descriptions.

Usage:
    imagegen "A red cube"
    imagegen "Three spheres in a row" --output scene.png
    imagegen "A rotating planet" --animate --output planet.html
"""

import argparse
import sys
import json
from pathlib import Path

# Add parent directory to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from backend.nlp_engine import NLPEngine
from backend.scene_generator import SceneGenerator
from backend.client import APIClient


class ImageGenCLI:
    """Command-line interface for image generation"""
    
    def __init__(self):
        self.nlp = NLPEngine()
        self.scene_gen = SceneGenerator()
        self.api_client = APIClient()
    
    def generate_from_description(self, description: str, output: str = None, 
                                 animate: bool = False, format: str = "html",
                                 interactive: bool = True) -> dict:
        """
        Generate scene from text description.
        
        Args:
            description: Text description of desired image
            output: Output file path
            animate: Enable animations
            format: Output format (html, png, svg, json)
            interactive: Enable interactive controls
            
        Returns:
            Scene dictionary
        """
        print(f"📝 Parsing: '{description}'")
        
        # Parse description with NLP
        entities = self.nlp.parse_description(description)
        print(f"🔍 Detected: {len(entities)} entities")
        for entity in entities:
            print(f"   - {entity.type.value}: {entity.name} ({entity.color})")
        
        # Generate scene
        print("🎨 Generating scene...")
        scene = self.scene_gen.generate_scene(
            entities=entities,
            animate=animate,
            interactive=interactive
        )
        
        # Send to backend API
        print("📤 Uploading to server...")
        result = self.api_client.create_scene(scene)
        
        # Export if needed
        if output:
            self._export(result, output, format)
            print(f"✅ Exported to {output}")
        else:
            print(f"✅ Scene created: {result['id']}")
            print(f"📱 Open: http://localhost:5000/view/{result['id']}")
        
        return result
    
    def _export(self, scene_data: dict, output: str, format: str):
        """Export scene to file"""
        output_path = Path(output)
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        if format == "json":
            with open(output_path, 'w') as f:
                json.dump(scene_data, f, indent=2)
        elif format == "html":
            html = self._generate_html(scene_data)
            with open(output_path, 'w') as f:
                f.write(html)
        elif format == "png":
            self.api_client.export_png(scene_data['id'], output)
        else:
            print(f"⚠️  Unknown format: {format}")
    
    def _generate_html(self, scene_data: dict) -> str:
        """Generate standalone HTML file"""
        return f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{scene_data.get('name', 'Generated Scene')}</title>
    <style>
        * {{ margin: 0; padding: 0; }}
        body {{ font-family: Arial, sans-serif; background: #1a1a1a; }}
        #canvas {{ display: block; width: 100vw; height: 100vh; }}
        #info {{ position: absolute; top: 10px; left: 10px; color: #fff; }}
    </style>
</head>
<body>
    <div id="canvas"></div>
    <div id="info">
        <h2>{scene_data.get('name', 'Scene')}</h2>
        <p>Use mouse to rotate/zoom</p>
    </div>
    
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
    <script>
        const sceneData = {json.dumps(scene_data['scene'])};
        // Scene rendering code will be injected here
        console.log('Scene loaded:', sceneData);
    </script>
</body>
</html>
"""


def main():
    """Main entry point"""
    parser = argparse.ArgumentParser(
        description="Generate 3D scenes from text descriptions",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  imagegen "A red cube"
  imagegen "Three spheres rotating" --animate
  imagegen "A castle" --output castle.png
  imagegen "A landscape" --format html --interactive
        """
    )
    
    parser.add_argument(
        "description",
        help="Text description of the scene to generate"
    )
    parser.add_argument(
        "-o", "--output",
        help="Output file path (png, html, json)"
    )
    parser.add_argument(
        "-f", "--format",
        choices=["html", "png", "svg", "json"],
        default="html",
        help="Output format (default: html)"
    )
    parser.add_argument(
        "-a", "--animate",
        action="store_true",
        help="Enable animations"
    )
    parser.add_argument(
        "-i", "--interactive",
        action="store_true",
        default=True,
        help="Enable interactive controls (default: true)"
    )
    parser.add_argument(
        "--no-interactive",
        action="store_false",
        dest="interactive",
        help="Disable interactive controls"
    )
    parser.add_argument(
        "-s", "--size",
        default="800x600",
        help="Output size WIDTHxHEIGHT (default: 800x600)"
    )
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Verbose output"
    )
    
    args = parser.parse_args()
    
    # Create CLI and generate
    cli = ImageGenCLI()
    
    try:
        result = cli.generate_from_description(
            description=args.description,
            output=args.output,
            animate=args.animate,
            format=args.format,
            interactive=args.interactive
        )
        return 0
    except Exception as e:
        print(f"❌ Error: {e}", file=sys.stderr)
        if args.verbose:
            import traceback
            traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
