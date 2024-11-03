import os
import shutil

def backup_settings():
    origin_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'settings.json')
    backup_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'settings.backup.json')
    try:
        # Copy settings.json as settings.backup.json
        shutil.copy(origin_path, backup_path)
        print("Settings backedup successfully.")
    except FileNotFoundError:
        print("Error: settings.json not found.")

if __name__ == "__main__":
    backup_settings()
