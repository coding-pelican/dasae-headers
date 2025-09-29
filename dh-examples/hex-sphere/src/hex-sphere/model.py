"""
Hexasphere Module
Core classes and functions for creating geodesic spheres
Save this as 'hexasphere.py' to use with the Earth Visualizer
"""

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

    def to_dict(self):
        """Convert to dictionary for JSON export"""
        return {'x': self.x, 'y': self.y, 'z': self.z}

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
        self.neighbors = []

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

    def get_lat_lon(self, radius):
        """Get latitude and longitude of tile center"""
        point = self.center_point
        phi = math.acos(point.y / radius) if radius != 0 else 0
        theta = (math.atan2(point.x, point.z) + math.pi + math.pi / 2) % (math.pi * 2) - math.pi

        return {
            'lat': 180 * phi / math.pi - 90,
            'lon': 180 * theta / math.pi
        }

    def to_dict(self):
        """Convert to dictionary for JSON export"""
        return {
            'center': self.center_point.to_dict(),
            'boundary': [p.to_dict() for p in self.boundary]
        }

    def __str__(self):
        return str(self.center_point)

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

        # Create tiles and establish neighbors
        for point in self.points.values():
            if point.faces:
                tile = Tile(point, self.hex_size)
                if len(tile.boundary) >= 3:
                    self.tiles.append(tile)

        # Establish neighbor relationships
        for i, tile in enumerate(self.tiles):
            for j, other in enumerate(self.tiles):
                if i != j and self.are_neighbors(tile, other):
                    tile.neighbors.append(other)

    def are_neighbors(self, tile1, tile2):
        """Check if two tiles are neighbors (share faces or vertices)"""
        if tile1 == tile2:
            return False

        # Check if tiles share any faces
        for face1 in tile1.center_point.faces:
            for face2 in tile2.center_point.faces:
                if face1.id == face2.id:
                    return True

        # Alternative: check if center points share any face vertices
        for face in tile1.center_point.faces:
            for point in face.points:
                if point == tile2.center_point:
                    return True

        return False

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

    def to_obj(self):
        """Export to OBJ format string"""
        obj_text = "# Hexasphere OBJ Export\n"
        obj_text += f"# Radius: {self.radius}\n"
        obj_text += f"# Divisions: {self.num_divisions}\n"
        obj_text += f"# Tiles: {len(self.tiles)}\n\n"

        vertex_map = {}
        vertex_count = 0

        # Export vertices
        obj_text += "# Vertices\n"
        for tile in self.tiles:
            for point in tile.boundary:
                key = str(point)
                if key not in vertex_map:
                    vertex_count += 1
                    vertex_map[key] = vertex_count
                    obj_text += f"v {point.x} {point.y} {point.z}\n"

        # Export faces
        obj_text += "\n# Faces\n"
        for tile in self.tiles:
            if len(tile.boundary) >= 3:
                indices = []
                for point in tile.boundary:
                    indices.append(vertex_map[str(point)])
                obj_text += "f " + " ".join(str(i) for i in indices) + "\n"

        return obj_text

    def to_json(self):
        """Export to JSON format string"""
        import json

        data = {
            'radius': self.radius,
            'divisions': self.num_divisions,
            'hex_size': self.hex_size,
            'tiles': [tile.to_dict() for tile in self.tiles]
        }

        return json.dumps(data, indent=2)

# Optional: Include the standalone renderer if running directly
if __name__ == "__main__":
    from demo import TruncatedIcosahedronRenderer
    renderer = TruncatedIcosahedronRenderer()
    renderer.show()
