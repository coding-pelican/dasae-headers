import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib.widgets import Slider, Button, TextBox
import math

class Point:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)
        self.faces = []

    def __hash__(self):
        return hash((round(self.x, 3), round(self.y, 3), round(self.z, 3)))

    def __eq__(self, other):
        tolerance = 0.001
        return (abs(self.x - other.x) < tolerance and
                abs(self.y - other.y) < tolerance and
                abs(self.z - other.z) < tolerance)

    def __str__(self):
        return f"{self.x:.3f},{self.y:.3f},{self.z:.3f}"

    def subdivide(self, other, count):
        """Subdivide edge between two points"""
        segments = [self]
        for i in range(1, count):
            t = i / count
            segments.append(Point(
                self.x * (1 - t) + other.x * t,
                self.y * (1 - t) + other.y * t,
                self.z * (1 - t) + other.z * t
            ))
        segments.append(other)
        return segments

    def segment(self, other, percent):
        """Get point at percent distance from other to self"""
        percent = max(0.01, min(1, percent))
        return Point(
            other.x * (1 - percent) + self.x * percent,
            other.y * (1 - percent) + self.y * percent,
            other.z * (1 - percent) + self.z * percent
        )

    def project(self, radius):
        """Project point onto sphere of given radius"""
        mag = math.sqrt(self.x**2 + self.y**2 + self.z**2)
        ratio = radius / mag
        return Point(self.x * ratio, self.y * ratio, self.z * ratio)

    def register_face(self, face):
        """Register a face that contains this point"""
        if face not in self.faces:
            self.faces.append(face)

class Face:
    _id_counter = 0

    def __init__(self, p1, p2, p3):
        self.id = Face._id_counter
        Face._id_counter += 1
        self.points = [p1, p2, p3]

    def get_other_points(self, point):
        """Get the two points that are not the given point"""
        return [p for p in self.points if p != point]

    def get_centroid(self):
        """Get the centroid of the face"""
        return Point(
            sum(p.x for p in self.points) / 3,
            sum(p.y for p in self.points) / 3,
            sum(p.z for p in self.points) / 3
        )

class Tile:
    def __init__(self, center_point, hex_size=0.95):
        self.center_point = center_point
        self.boundary = []

        if not center_point.faces:
            return

        # Get face centroids and sort them around the center point
        face_centroids = [face.get_centroid() for face in center_point.faces]

        # Project to plane perpendicular to center point for sorting
        normal = np.array([center_point.x, center_point.y, center_point.z])
        normal = normal / np.linalg.norm(normal)

        # Create orthonormal basis
        ref = np.array([0, 0, 1]) if abs(normal[2]) < 0.9 else np.array([1, 0, 0])
        v1 = np.cross(normal, ref)
        v1 = v1 / np.linalg.norm(v1)
        v2 = np.cross(normal, v1)

        # Calculate angles and sort
        angles = []
        for centroid in face_centroids:
            vec = np.array([centroid.x, centroid.y, centroid.z])
            angles.append(np.arctan2(np.dot(vec, v2), np.dot(vec, v1)))

        sorted_indices = np.argsort(angles)

        # Create boundary points in sorted order
        for i in sorted_indices:
            self.boundary.append(face_centroids[i].segment(center_point, hex_size))

class Hexasphere:
    def __init__(self, radius=1.0, num_divisions=2, hex_size=0.95):
        self.radius = radius
        self.num_divisions = num_divisions
        self.hex_size = hex_size
        self.tau = 1.61803399  # Golden ratio

        self.points = {}
        self.tiles = []

        self.create_geodesic_sphere()

    def create_initial_vertices(self):
        """Create the 12 vertices of an icosahedron"""
        s = 1000
        t = self.tau * s

        return [
            Point(s, t, 0), Point(-s, t, 0), Point(s, -t, 0), Point(-s, -t, 0),
            Point(0, s, t), Point(0, -s, t), Point(0, s, -t), Point(0, -s, -t),
            Point(t, 0, s), Point(-t, 0, s), Point(t, 0, -s), Point(-t, 0, -s)
        ]

    def create_initial_faces(self, corners):
        """Create the 20 triangular faces of an icosahedron"""
        c = corners
        return [
            Face(c[0], c[1], c[4]), Face(c[1], c[9], c[4]), Face(c[4], c[9], c[5]),
            Face(c[5], c[9], c[3]), Face(c[2], c[3], c[7]), Face(c[3], c[2], c[5]),
            Face(c[7], c[10], c[2]), Face(c[0], c[8], c[10]), Face(c[0], c[4], c[8]),
            Face(c[8], c[2], c[10]), Face(c[8], c[4], c[5]), Face(c[8], c[5], c[2]),
            Face(c[1], c[0], c[6]), Face(c[11], c[1], c[6]), Face(c[3], c[9], c[11]),
            Face(c[6], c[10], c[7]), Face(c[3], c[11], c[7]), Face(c[11], c[6], c[7]),
            Face(c[6], c[0], c[10]), Face(c[9], c[1], c[11])
        ]

    def create_geodesic_sphere(self):
        """Create geodesic sphere"""
        corners = self.create_initial_vertices()
        faces = self.create_initial_faces(corners)

        # Store initial points
        for corner in corners:
            self.points[str(corner)] = corner

        # Subdivide faces if needed
        if self.num_divisions > 1:
            faces = self.subdivide_faces(faces)

        # Project all points onto sphere
        self.project_points_to_sphere()

        # Register faces to points
        for face in faces:
            for i, point in enumerate(face.points):
                projected_key = str(point.project(self.radius))
                if projected_key in self.points:
                    face.points[i] = self.points[projected_key]
                    self.points[projected_key].register_face(face)

        # Create tiles
        for point in self.points.values():
            if point.faces:
                tile = Tile(point, self.hex_size)
                if len(tile.boundary) >= 3:
                    self.tiles.append(tile)

    def subdivide_faces(self, faces):
        """Subdivide triangular faces"""
        new_faces = []

        def get_or_create_point(point):
            key = str(point)
            if key not in self.points:
                self.points[key] = point
            return self.points[key]

        for face in faces:
            p0, p1, p2 = face.points
            left = p0.subdivide(p1, self.num_divisions)
            right = p0.subdivide(p2, self.num_divisions)

            prev_row = [p0]

            for i in range(1, self.num_divisions + 1):
                current_row = []
                left_point = get_or_create_point(left[min(i, len(left)-1)])
                right_point = get_or_create_point(right[min(i, len(right)-1)])

                if i == self.num_divisions:
                    current_row = [left_point]
                    if left_point != right_point:
                        for p in left_point.subdivide(right_point, i)[1:]:
                            current_row.append(get_or_create_point(p))
                else:
                    for p in left_point.subdivide(right_point, i):
                        current_row.append(get_or_create_point(p))

                # Create triangles
                for j in range(len(current_row) - 1):
                    if j < len(prev_row):
                        new_faces.append(Face(prev_row[j], current_row[j], current_row[j + 1]))
                    if j > 0 and j < len(prev_row):
                        new_faces.append(Face(prev_row[j - 1], prev_row[j], current_row[j]))

                prev_row = current_row

        return new_faces

    def project_points_to_sphere(self):
        """Project all points onto the sphere"""
        projected = {}
        for point in self.points.values():
            p = point.project(self.radius)
            key = str(p)
            if key not in projected:
                projected[key] = p
        self.points = projected

class TruncatedIcosahedronRenderer:
    def __init__(self):
        self.fig = plt.figure(figsize=(14, 10))
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.ax.set_box_aspect([1, 1, 1])

        # Default parameters
        self.radius = 1.0
        self.num_divisions = 2
        self.hex_size = 0.95
        self.camera_distance = 3.0

        # Rotation angles
        self.angle_x = 0
        self.angle_y = 0
        self.angle_z = 0
        self.is_rotating = False

        self.create_structure()
        self.setup_controls()
        self.anim = None

    def create_structure(self):
        """Create the hexasphere structure"""
        self.structure = Hexasphere(self.radius, self.num_divisions, self.hex_size)

    def setup_controls(self):
        """Setup control sliders and buttons"""
        plt.subplots_adjust(bottom=0.35)

        # Division input
        ax_div = plt.axes([0.15, 0.27, 0.2, 0.03])
        self.text_div = TextBox(ax_div, 'Divisions (N):', initial=str(self.num_divisions))
        self.text_div.on_submit(self.update_divisions)

        # Sliders
        sliders_config = [
            ('Tile Size', 0.5, 1.0, self.hex_size, 'hex'),
            ('Camera Distance', 2.0, 10.0, self.camera_distance, 'camera'),
            ('X Rotation', 0, 2*np.pi, 0, 'x'),
            ('Y Rotation', 0, 2*np.pi, 0, 'y'),
            ('Z Rotation', 0, 2*np.pi, 0, 'z')
        ]

        self.sliders = {}
        for i, (label, min_val, max_val, init_val, key) in enumerate(sliders_config):
            ax = plt.axes([0.15, 0.22 - i*0.05, 0.65, 0.03])
            slider = Slider(ax, label, min_val, max_val, valinit=init_val)
            self.sliders[key] = slider

            if key == 'hex':
                slider.on_changed(lambda v: self.update_hex_size())
            elif key == 'camera':
                slider.on_changed(lambda v: self.update_camera_distance())
            else:
                slider.on_changed(lambda v: self.update_rotation())

        # Buttons
        ax_animate = plt.axes([0.85, 0.12, 0.1, 0.04])
        self.animate_button = Button(ax_animate, 'Animate')
        self.animate_button.on_clicked(self.toggle_animation)

        ax_reset = plt.axes([0.85, 0.07, 0.1, 0.04])
        self.reset_button = Button(ax_reset, 'Reset')
        self.reset_button.on_clicked(self.reset_view)

        ax_rebuild = plt.axes([0.85, 0.02, 0.1, 0.04])
        self.rebuild_button = Button(ax_rebuild, 'Rebuild')
        self.rebuild_button.on_clicked(self.rebuild_structure)

    def update_divisions(self, text):
        """Update number of divisions and rebuild"""
        try:
            new_div = max(1, min(10, int(text)))
            self.num_divisions = new_div
            # Keep current hex_size when rebuilding
            self.hex_size = self.sliders['hex'].val
            self.create_structure()
            self.draw_structure()
        except:
            pass

    def update_hex_size(self):
        """Update hex size and rebuild structure"""
        self.hex_size = self.sliders['hex'].val
        self.create_structure()
        self.draw_structure()

    def update_camera_distance(self):
        """Update camera distance"""
        self.camera_distance = self.sliders['camera'].val
        self.draw_structure()

    def rebuild_structure(self, event):
        """Rebuild the structure with current settings"""
        # Keep current settings and rebuild
        self.hex_size = self.sliders['hex'].val
        self.camera_distance = self.sliders['camera'].val

        # Recreate structure with current settings
        self.create_structure()
        self.draw_structure()

    def update_rotation(self):
        """Update rotation"""
        self.angle_x = self.sliders['x'].val
        self.angle_y = self.sliders['y'].val
        self.angle_z = self.sliders['z'].val
        self.draw_structure()

    def rotation_matrix(self, ax, ay, az):
        """Combined rotation matrix"""
        cx, sx = np.cos(ax), np.sin(ax)
        cy, sy = np.cos(ay), np.sin(ay)
        cz, sz = np.cos(az), np.sin(az)

        rx = np.array([[1, 0, 0], [0, cx, -sx], [0, sx, cx]])
        ry = np.array([[cy, 0, sy], [0, 1, 0], [-sy, 0, cy]])
        rz = np.array([[cz, -sz, 0], [sz, cz, 0], [0, 0, 1]])

        return rx @ ry @ rz

    def rotate_point(self, point, rot_matrix):
        """Rotate a point using rotation matrix"""
        p = rot_matrix @ np.array([point.x, point.y, point.z])
        return Point(p[0], p[1], p[2])

    def draw_structure(self):
        """Draw the hexasphere structure"""
        self.ax.clear()

        # Set view limits
        self.camera_distance = self.sliders['camera'].val
        lim = self.radius * self.camera_distance / 2
        self.ax.set_xlim([-lim, lim])
        self.ax.set_ylim([-lim, lim])
        self.ax.set_zlim([-lim, lim])
        self.ax.set_xlabel('X')
        self.ax.set_ylabel('Y')
        self.ax.set_zlabel('Z')

        # Title
        pentagons = sum(1 for t in self.structure.tiles if len(t.boundary) == 5)
        hexagons = sum(1 for t in self.structure.tiles if len(t.boundary) == 6)
        self.ax.set_title(
            f'Geodesic Sphere | Divisions: {self.num_divisions} | '
            f'Tiles: {len(self.structure.tiles)} (P:{pentagons}, H:{hexagons})'
        )

        # Get rotation matrix
        rot_matrix = self.rotation_matrix(self.angle_x, self.angle_y, self.angle_z)

        # Prepare and sort tiles by depth
        tiles_data = []
        for tile in self.structure.tiles:
            center_rot = self.rotate_point(tile.center_point, rot_matrix)
            boundary_rot = [self.rotate_point(p, rot_matrix) for p in tile.boundary]
            tiles_data.append((center_rot.z, tile, boundary_rot))

        tiles_data.sort(key=lambda x: x[0])

        # Draw tiles
        for depth, tile, boundary_rot in tiles_data:
            verts = [[p.x, p.y, p.z] for p in boundary_rot]

            # Color and style based on polygon type
            n_sides = len(tile.boundary)
            color = 'gold' if n_sides == 5 else 'lightblue' if n_sides == 6 else 'lightgray'
            alpha = 0.8 if n_sides == 5 else 0.7

            # Depth-based adjustments
            depth_factor = 1.0 - ((depth + self.radius) / (2 * self.radius) * 0.5)
            alpha *= max(0.3, min(1.0, depth_factor))
            edge_width = 1.5 * (4.0 / self.camera_distance)

            self.ax.add_collection3d(
                Poly3DCollection([verts],
                               facecolor=color,
                               edgecolor='black',
                               alpha=alpha,
                               linewidth=edge_width,
                               zorder=depth)
            )

        self.ax.grid(True, alpha=0.3)
        self.ax.set_box_aspect([1, 1, 1])
        plt.draw()

    def animate(self, frame):
        """Animation function"""
        if self.is_rotating:
            self.angle_x = (self.angle_x + 0.02) % (2 * np.pi)
            self.angle_y = (self.angle_y + 0.03) % (2 * np.pi)
            self.angle_z = (self.angle_z + 0.01) % (2 * np.pi)

            self.sliders['x'].set_val(self.angle_x)
            self.sliders['y'].set_val(self.angle_y)
            self.sliders['z'].set_val(self.angle_z)

            self.draw_structure()
        return []

    def toggle_animation(self, event):
        """Toggle animation"""
        self.is_rotating = not self.is_rotating
        self.animate_button.label.set_text('Stop' if self.is_rotating else 'Animate')

        if self.is_rotating and self.anim is None:
            self.anim = FuncAnimation(self.fig, self.animate, interval=50,
                                    blit=False, cache_frame_data=False)

    def reset_view(self, event):
        """Reset all controls to default values (keep structure)"""
        # Reset rotation angles
        self.angle_x = self.angle_y = self.angle_z = 0
        self.sliders['x'].set_val(0)
        self.sliders['y'].set_val(0)
        self.sliders['z'].set_val(0)

        # Reset camera distance
        self.camera_distance = 3.0
        self.sliders['camera'].set_val(3.0)

        # Reset tile size
        self.hex_size = 0.95
        self.sliders['hex'].set_val(0.95)

        # Reset divisions
        self.num_divisions = 2
        self.text_div.set_val('2')

        # Stop animation
        self.is_rotating = False
        self.animate_button.label.set_text('Animate')

        # Recreate structure with default values and redraw
        self.create_structure()
        self.draw_structure()

    def show(self):
        """Display the renderer"""
        self.draw_structure()
        plt.show()

if __name__ == "__main__":
    print("="*60)
    print("Geodesic Sphere / Truncated Icosahedron Renderer")
    print("="*60)
    print("Controls:")
    print("  • Divisions (N): 1-10 (tessellation level)")
    print("  • Tile Size: 0.5-1.0 (polygon size)")
    print("  • Camera Distance: 2.0-10.0 (zoom)")
    print("  • Rotation: X/Y/Z axis control")
    print("  • Animate: Auto-rotation toggle")
    print("  • Reset: Restore all settings to default")
    print("  • Rebuild: Recreate structure with current settings")
    print("="*60)
    print("Legend: Gold=Pentagon, Blue=Hexagon")
    print("="*60)

    renderer = TruncatedIcosahedronRenderer()
    renderer.show()
