"""
Earth Hexasphere Visualizer
A visualization module that uses the hexasphere to render Earth with land/ocean distinction
and spreading animation effects.
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib.widgets import Slider, Button, RadioButtons
from matplotlib.patches import Rectangle
import math
from collections import deque
import random

# Assuming hexasphere module is in the same directory
# You'll need to save the previous code as 'hexasphere.py'
from model import Hexasphere, Point

class EarthVisualizer:
    def __init__(self):
        self.fig = plt.figure(figsize=(16, 10))

        # Main 3D plot
        self.ax = self.fig.add_subplot(121, projection='3d')
        self.ax.set_box_aspect([1, 1, 1])

        # 2D map for reference
        self.ax_map = self.fig.add_subplot(122)

        # Parameters
        self.radius = 30
        self.num_divisions = 10  # Reduced from 15 for better performance
        self.hex_size = 0.95
        self.camera_distance = 65

        # Animation parameters
        self.camera_angle = -math.pi / 1.5
        self.auto_rotate = True
        self.rotation_speed = 0.01

        # Spreading animation
        self.spread_mode = 'none'  # 'wave', 'random', 'none' - default to 'none' to show all
        self.spread_speed = 3
        self.seen_tiles = set()
        self.current_tiles = deque()
        self.tile_opacities = {}

        # Colors
        self.land_colors = [
            '#7cfc00', '#397d02', '#77ee00', '#61b329',
            '#83f52c', '#4cbb17', '#00ee00', '#00aa11'
        ]
        self.ocean_colors = ['#0f2342', '#0f1e38', '#1e3a5f', '#2a4d6b']

        # Create structure
        self.create_earth()

        # Setup controls
        self.setup_controls()

        # Animation
        self.anim = None
        self.frame_count = 0

    def create_earth(self):
        """Create Earth hexasphere with land/ocean distinction"""
        self.structure = Hexasphere(self.radius, self.num_divisions, self.hex_size)

        # Assign land/ocean to each tile based on lat/lon
        self.tile_types = {}
        self.tile_colors = {}

        for i, tile in enumerate(self.structure.tiles):
            if len(tile.boundary) < 3:
                continue

            # Get tile center lat/lon
            lat, lon = self.get_lat_lon(tile.center_point)

            # Determine if land or ocean (simplified continent shapes)
            is_land = self.is_land(lat, lon)
            self.tile_types[i] = 'land' if is_land else 'ocean'

            # Assign random color from appropriate palette
            if is_land:
                self.tile_colors[i] = random.choice(self.land_colors)
            else:
                self.tile_colors[i] = random.choice(self.ocean_colors)

            # Initialize opacity (start with all visible for 'none' mode)
            if self.spread_mode == 'none':
                self.tile_opacities[i] = 1.0
            else:
                self.tile_opacities[i] = 0.0

        # Print debug info
        print(f"Created {len(self.structure.tiles)} tiles")
        print(f"Land tiles: {sum(1 for t in self.tile_types.values() if t == 'land')}")
        print(f"Ocean tiles: {sum(1 for t in self.tile_types.values() if t == 'ocean')}")

        # Initialize spreading animation
        self.initialize_spreading()

    def get_lat_lon(self, point):
        """Convert 3D point to latitude/longitude"""
        # Normalize point
        r = math.sqrt(point.x**2 + point.y**2 + point.z**2)
        if r == 0:
            return 0, 0

        # Calculate spherical coordinates
        lat = math.degrees(math.asin(point.y / r))
        lon = math.degrees(math.atan2(point.x, point.z))

        return lat, lon

    def is_land(self, lat, lon):
        """Simplified land/ocean detection based on lat/lon"""
        # Simplified continent shapes (very approximate)

        # Africa and Europe
        if -35 <= lat <= 70 and -20 <= lon <= 50:
            if lat > 35 or (lat > -10 and lon > 10):
                return True

        # Asia
        if -10 <= lat <= 70 and 50 <= lon <= 150:
            return True

        # North America
        if 15 <= lat <= 70 and -170 <= lon <= -50:
            return True

        # South America
        if -55 <= lat <= 15 and -85 <= lon <= -35:
            return True

        # Australia
        if -45 <= lat <= -10 and 110 <= lon <= 155:
            return True

        # Antarctica (partial)
        if lat <= -60:
            return random.random() > 0.3

        # Greenland
        if 60 <= lat <= 85 and -60 <= lon <= -20:
            return True

        return False

    def initialize_spreading(self):
        """Initialize the spreading animation"""
        self.seen_tiles.clear()
        self.current_tiles.clear()

        # Reset all opacities
        for i in self.tile_opacities:
            self.tile_opacities[i] = 0.0

        if self.spread_mode == 'wave':
            # Start from a few seed points
            num_seeds = min(12, len(self.structure.tiles))
            seed_indices = random.sample(range(len(self.structure.tiles)), num_seeds)

            for idx in seed_indices:
                self.current_tiles.append(idx)
                self.seen_tiles.add(idx)
                self.tile_opacities[idx] = 1.0

        elif self.spread_mode == 'random':
            # All tiles will appear randomly
            pass

        else:  # 'none'
            # Show all tiles immediately
            for i in self.tile_opacities:
                self.tile_opacities[i] = 1.0

    def update_spreading(self):
        """Update the spreading animation"""
        if self.spread_mode == 'wave' and self.current_tiles:
            next_tiles = deque()

            # Process a batch of tiles
            for _ in range(min(self.spread_speed, len(self.current_tiles))):
                if not self.current_tiles:
                    break

                tile_idx = self.current_tiles.popleft()
                tile = self.structure.tiles[tile_idx]

                # Find neighboring tiles
                for other_idx, other_tile in enumerate(self.structure.tiles):
                    if other_idx not in self.seen_tiles:
                        # Check if tiles are neighbors (share a vertex)
                        if self.are_neighbors(tile, other_tile):
                            self.seen_tiles.add(other_idx)
                            next_tiles.append(other_idx)
                            self.tile_opacities[other_idx] = 1.0

            self.current_tiles.extend(next_tiles)

        elif self.spread_mode == 'random':
            # Randomly reveal tiles
            for i in self.tile_opacities:
                if self.tile_opacities[i] < 1.0:
                    if random.random() < 0.05:  # 5% chance per frame
                        self.tile_opacities[i] = 1.0

    def are_neighbors(self, tile1, tile2):
        """Check if two tiles are neighbors (share at least 2 vertices)"""
        # Count shared vertices
        shared_vertices = 0
        threshold = 0.1  # Distance threshold for considering points as the same

        for p1 in tile1.boundary:
            for p2 in tile2.boundary:
                dist = math.sqrt(
                    (p1.x - p2.x)**2 +
                    (p1.y - p2.y)**2 +
                    (p1.z - p2.z)**2
                )
                if dist < threshold:
                    shared_vertices += 1
                    break  # Move to next p1

        # Tiles are neighbors if they share at least 2 vertices (an edge)
        return shared_vertices >= 2

    def setup_controls(self):
        """Setup control widgets"""
        plt.subplots_adjust(bottom=0.25)

        # Animation mode radio buttons
        ax_radio = plt.axes([0.02, 0.15, 0.12, 0.08])
        self.radio_spread = RadioButtons(ax_radio, ('None', 'Wave', 'Random'), active=0)
        self.radio_spread.on_clicked(self.set_spread_mode)

        # Speed slider
        ax_speed = plt.axes([0.15, 0.15, 0.3, 0.03])
        self.slider_speed = Slider(ax_speed, 'Spread Speed', 1, 10, valinit=self.spread_speed, valstep=1)
        self.slider_speed.on_changed(lambda v: setattr(self, 'spread_speed', int(v)))

        # Divisions slider
        ax_div = plt.axes([0.15, 0.10, 0.3, 0.03])
        self.slider_div = Slider(ax_div, 'Divisions', 5, 25, valinit=self.num_divisions, valstep=1)
        self.slider_div.on_changed(self.update_divisions)

        # Camera distance
        ax_cam = plt.axes([0.15, 0.05, 0.3, 0.03])
        self.slider_camera = Slider(ax_cam, 'Camera Dist', 40, 100, valinit=self.camera_distance)
        self.slider_camera.on_changed(lambda v: setattr(self, 'camera_distance', v))

        # Rotation speed
        ax_rot = plt.axes([0.55, 0.15, 0.3, 0.03])
        self.slider_rotation = Slider(ax_rot, 'Rotation Speed', 0, 0.05, valinit=self.rotation_speed)
        self.slider_rotation.on_changed(lambda v: setattr(self, 'rotation_speed', v))

        # Buttons
        ax_rotate = plt.axes([0.55, 0.10, 0.1, 0.04])
        self.btn_rotate = Button(ax_rotate, 'Auto Rotate')
        self.btn_rotate.on_clicked(self.toggle_rotation)

        ax_reset = plt.axes([0.67, 0.10, 0.08, 0.04])
        self.btn_reset = Button(ax_reset, 'Reset')
        self.btn_reset.on_clicked(self.reset_animation)

        ax_restart = plt.axes([0.77, 0.10, 0.08, 0.04])
        self.btn_restart = Button(ax_restart, 'Restart')
        self.btn_restart.on_clicked(self.restart_spreading)

    def set_spread_mode(self, label):
        """Set spreading animation mode"""
        self.spread_mode = label.lower()
        self.initialize_spreading()

    def update_divisions(self, val):
        """Update number of divisions"""
        self.num_divisions = int(val)
        self.create_earth()
        self.draw_earth()

    def toggle_rotation(self, event):
        """Toggle auto rotation"""
        self.auto_rotate = not self.auto_rotate
        self.btn_rotate.label.set_text('Stop' if self.auto_rotate else 'Auto Rotate')

    def reset_animation(self, event):
        """Reset all parameters"""
        self.camera_angle = -math.pi / 1.5
        self.auto_rotate = True
        self.spread_mode = 'none'  # Changed default to 'none'
        self.spread_speed = 3
        self.rotation_speed = 0.01

        self.slider_speed.set_val(3)
        self.slider_rotation.set_val(0.01)

        self.initialize_spreading()

    def restart_spreading(self, event):
        """Restart spreading animation"""
        self.initialize_spreading()

    def draw_earth(self):
        """Draw the Earth hexasphere"""
        self.ax.clear()

        # Set properties
        self.ax.set_xlim([-self.radius*1.5, self.radius*1.5])
        self.ax.set_ylim([-self.radius*1.5, self.radius*1.5])
        self.ax.set_zlim([-self.radius*1.5, self.radius*1.5])
        self.ax.set_xlabel('X')
        self.ax.set_ylabel('Y')
        self.ax.set_zlabel('Z')
        self.ax.set_title(f'Earth Hexasphere (Tiles: {len(self.structure.tiles)})')

        # Calculate camera position
        cam_x = self.camera_distance * math.cos(self.camera_angle)
        cam_y = math.sin(self.camera_angle) * 10
        cam_z = self.camera_distance * math.sin(self.camera_angle)
        self.ax.view_init(elev=cam_y, azim=math.degrees(self.camera_angle))

        # Sort tiles by depth
        tiles_with_depth = []
        for i, tile in enumerate(self.structure.tiles):
            if len(tile.boundary) < 3:
                continue

            # Calculate depth
            center = tile.center_point
            depth = center.x * math.sin(self.camera_angle) + center.z * math.cos(self.camera_angle)
            tiles_with_depth.append((depth, i, tile))

        tiles_with_depth.sort(key=lambda x: x[0])

        # Draw tiles
        for depth, idx, tile in tiles_with_depth:
            verts = [[p.x, p.y, p.z] for p in tile.boundary]

            # Get color and opacity
            color = self.tile_colors.get(idx, '#808080')
            opacity = self.tile_opacities.get(idx, 1.0) * 0.8  # Default to 1.0 if not set

            # Always draw tiles (even with low opacity for debugging)
            if opacity <= 0:
                opacity = 0.1  # Show faint outline even for "hidden" tiles

            poly = [verts]
            self.ax.add_collection3d(
                Poly3DCollection(poly,
                               facecolor=color,
                               edgecolor='black',
                               alpha=opacity,
                               linewidth=0.5)
            )

        # Update 2D map
        self.draw_map()

        # Add fog effect
        self.ax.set_facecolor('#000011')
        self.ax.grid(True, alpha=0.1)
        self.ax.set_box_aspect([1, 1, 1])

    def draw_map(self):
        """Draw 2D world map showing tile distribution"""
        self.ax_map.clear()

        # Create a simple 2D projection
        lats = []
        lons = []
        colors = []
        alphas = []

        for i, tile in enumerate(self.structure.tiles):
            lat, lon = self.get_lat_lon(tile.center_point)
            lats.append(lat)
            lons.append(lon)
            colors.append(self.tile_colors.get(i, '#808080'))
            # Use actual opacity for map
            alphas.append(max(0.1, self.tile_opacities.get(i, 1.0)))

        if lats:
            # Draw each point with its own alpha
            for lat, lon, color, alpha in zip(lats, lons, colors, alphas):
                self.ax_map.scatter(lon, lat, c=[color], s=5, alpha=alpha)

        self.ax_map.set_xlim([-180, 180])
        self.ax_map.set_ylim([-90, 90])
        self.ax_map.set_xlabel('Longitude')
        self.ax_map.set_ylabel('Latitude')
        self.ax_map.set_title(f'Tile Distribution ({len(lats)} tiles)')
        self.ax_map.grid(True, alpha=0.3)
        self.ax_map.set_facecolor('#000022')

    def animate(self, frame):
        """Animation function"""
        self.frame_count += 1

        # Update camera rotation
        if self.auto_rotate:
            self.camera_angle += self.rotation_speed

        # Update spreading animation
        self.update_spreading()

        # Redraw
        self.draw_earth()

        return []

    def start(self):
        """Start the visualization"""
        self.draw_earth()
        self.anim = FuncAnimation(self.fig, self.animate, interval=50,
                                blit=False, cache_frame_data=False)
        plt.show()

# Example usage module
class HexasphereDemo:
    """Demo application showing various uses of the hexasphere"""

    @staticmethod
    def run_earth_visualization():
        """Run the Earth visualization"""
        visualizer = EarthVisualizer()
        visualizer.start()

    @staticmethod
    def run_basic_sphere():
        """Run a basic hexasphere"""
        from hexasphere import TruncatedIcosahedronRenderer
        renderer = TruncatedIcosahedronRenderer()
        renderer.show()

    @staticmethod
    def export_obj(radius=30, divisions=20, hex_size=0.95, filename='hexasphere.obj'):
        """Export hexasphere to OBJ file"""
        sphere = Hexasphere(radius, divisions, hex_size)

        obj_content = "# Hexasphere OBJ Export\n"
        obj_content += f"# Radius: {radius}, Divisions: {divisions}, Hex Size: {hex_size}\n\n"

        # Export vertices
        vertex_map = {}
        vertex_count = 0

        for tile in sphere.tiles:
            for point in tile.boundary:
                key = f"{point.x:.3f},{point.y:.3f},{point.z:.3f}"
                if key not in vertex_map:
                    vertex_count += 1
                    vertex_map[key] = vertex_count
                    obj_content += f"v {point.x:.6f} {point.y:.6f} {point.z:.6f}\n"

        # Export faces
        obj_content += "\n# Faces\n"
        for tile in sphere.tiles:
            if len(tile.boundary) >= 3:
                indices = []
                for point in tile.boundary:
                    key = f"{point.x:.3f},{point.y:.3f},{point.z:.3f}"
                    indices.append(vertex_map[key])

                face_str = "f " + " ".join(str(i) for i in indices)
                obj_content += face_str + "\n"

        # Save to file
        with open(filename, 'w') as f:
            f.write(obj_content)

        print(f"Exported to {filename}")
        print(f"Vertices: {vertex_count}, Faces: {len(sphere.tiles)}")

    @staticmethod
    def export_json(radius=30, divisions=20, hex_size=0.95, filename='hexasphere.json'):
        """Export hexasphere to JSON file"""
        import json

        sphere = Hexasphere(radius, divisions, hex_size)

        data = {
            'radius': radius,
            'divisions': divisions,
            'hex_size': hex_size,
            'tiles': []
        }

        for tile in sphere.tiles:
            tile_data = {
                'center': {
                    'x': tile.center_point.x,
                    'y': tile.center_point.y,
                    'z': tile.center_point.z
                },
                'boundary': [
                    {'x': p.x, 'y': p.y, 'z': p.z}
                    for p in tile.boundary
                ]
            }
            data['tiles'].append(tile_data)

        with open(filename, 'w') as f:
            json.dump(data, f, indent=2)

        print(f"Exported to {filename}")
        print(f"Tiles: {len(sphere.tiles)}")

if __name__ == "__main__":
    print("="*60)
    print("Earth Hexasphere Visualizer")
    print("="*60)
    print("Features:")
    print("  • Land/Ocean distinction with color coding")
    print("  • Spreading animation modes:")
    print("    - None: Show all tiles immediately (default)")
    print("    - Wave: Tiles spread from seed points")
    print("    - Random: Tiles appear randomly")
    print("  • Auto-rotation with adjustable speed")
    print("  • 2D map projection on the right")
    print("  • Export to OBJ/JSON formats")
    print("="*60)
    print("Controls:")
    print("  • Use radio buttons to change spreading mode")
    print("  • Adjust sliders for speed and divisions")
    print("  • 'Restart' to replay spreading animation")
    print("="*60)

    # Run the Earth visualization
    demo = HexasphereDemo()
    demo.run_earth_visualization()

    # Uncomment to export files:
    # demo.export_obj(radius=30, divisions=25, hex_size=0.95)
    # demo.export_json(radius=30, divisions=25, hex_size=0.95)
